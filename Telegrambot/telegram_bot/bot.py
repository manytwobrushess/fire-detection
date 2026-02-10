import logging
from datetime import datetime, timedelta
from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, ContextTypes
import firebase_admin
from firebase_admin import credentials, db
import config

# ======================
# Initialize Firebase
# ======================
try:
    cred = credentials.Certificate(config.SERVICE_ACCOUNT_FILE)
    firebase_admin.initialize_app(cred, {"databaseURL": config.DATABASE_URL})
    # Reference to the 'counter' folder inside 'test'
    ref = db.reference("test/counter")
    print("✅ Firebase Connected Successfully")
except Exception as e:
    print(f"❌ Firebase Error: {e}")

# Global variables to track state
last_status = "OK"
last_warning_time = "No warnings recorded yet."
is_offline_alerted = False 

# ======================
# Helper Functions
# ======================
def get_issue_description(status_code):
    s = str(status_code).upper()
    if s == "DANGER": return "🔥 FIRE DETECTED!"
    if s == "ALERT": return "💨 GAS DETECTED!"
    if s == "WARNING": return "🌡️ HIGH TEMPERATURE!"
    return f"⚠️ UNKNOWN ISSUE ({status_code})"

# ======================
# Automatic Alert Logic (5-Second Cooldown)
# ======================
async def check_and_alert(context: ContextTypes.DEFAULT_TYPE):
    global last_status, last_warning_time, is_offline_alerted
    
    try:
        # Get only the very last entry
        snapshot = ref.order_by_key().limit_to_last(1).get()
        if not snapshot:
            return

        timestamp_str = list(snapshot.keys())[0]
        data = snapshot[timestamp_str]
        
        # --- OFFLINE CHECK (1 HOUR) ---
        try:
            # Matches format: 2026-01-17_18-56-37
            last_data_time = datetime.strptime(timestamp_str, "%Y-%m-%d_%H-%M-%S")
            time_diff = datetime.now() - last_data_time

            if time_diff > timedelta(hours=1):
                if not is_offline_alerted:
                    await context.bot.send_message(
                        chat_id=config.ALERT_CHAT_ID, 
                        text="⚠️ **System Alert:** Board is **OFFLINE**.\nNo data received for over 1 hour.",
                        parse_mode='Markdown'
                    )
                    is_offline_alerted = True
                return # Skip status processing for old data
            else:
                is_offline_alerted = False
        except Exception as e:
            # If time parsing fails, we log it but keep checking status
            print(f"DEBUG: Time parse error on {timestamp_str}: {e}")

        # --- STATUS ALERT LOGIC ---
        current_status = str(data.get("status", "OK")).upper()

        if current_status in ["DANGER", "ALERT", "WARNING"]:
            if last_status != current_status:
                last_warning_time = timestamp_str
                issue = get_issue_description(current_status)
                
                message = (
                    f"🚨🚨 **SYSTEM {current_status}** 🚨🚨\n\n"
                    f"⚠️ **ISSUE:** {issue}\n"
                    f"⏰ **TIME:** `{timestamp_str}`\n"
                    f"🌡️ **TEMP:** {data.get('temperature')}°C\n"
                    f"💨 **GAS:** {data.get('gas')}\n"
                    "━━━━━━━━━━━━━━━━━━\n"
                    "‼️ Please investigate immediately!"
                )
                await context.bot.send_message(chat_id=config.ALERT_CHAT_ID, text=message, parse_mode='Markdown')
                last_status = current_status
        
        elif current_status == "OK" and last_status != "OK":
            await context.bot.send_message(chat_id=config.ALERT_CHAT_ID, text="✅ **Status:** Normal (OK)")
            last_status = "OK"

    except Exception as e:
        print(f"❌ Loop Error: {e}")

# ======================
# Commands
# ======================
async def status(update: Update, context: ContextTypes.DEFAULT_TYPE):
    snapshot = ref.order_by_key().limit_to_last(1).get()
    if not snapshot:
        await update.message.reply_text("❌ No data found in Firebase.")
        return
    
    time_key = list(snapshot.keys())[0]
    data = snapshot[time_key]
    
    await update.message.reply_text(
        f"📅 **Last Update:** `{time_key}`\n"
        f"🌡️ **Temp:** {data.get('temperature')}°C\n"
        f"💨 **Gas:** {data.get('gas')}\n"
        f"📢 **Status:** {data.get('status')}",
        parse_mode='Markdown'
    )

async def last_warning(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(f"⚠️ **Last Incident Timestamp:**\n`{last_warning_time}`", parse_mode='Markdown')

# ======================
# Main Execution
# ======================
def main():
    app = ApplicationBuilder().token(config.TELEGRAM_TOKEN).build()

    app.add_handler(CommandHandler("status", status))
    app.add_handler(CommandHandler("lastwarning", last_warning))

    if app.job_queue:
        # CHANGED: interval=5 for the 5-second cooldown
        app.job_queue.run_repeating(check_and_alert, interval=5, first=2)

    print("🚀 Bot running | 5s Check Interval | 1hr Offline Detection")
    app.run_polling()

if __name__ == "__main__":
    main()