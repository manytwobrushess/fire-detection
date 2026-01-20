// config.js
const firebaseConfig = {
  apiKey: "AIzaSyAknpKiL4-ShxkL0KrLXapd7rU9rLe0HCo",
  authDomain: "ffiredetection-1673c.firebaseapp.com",
  databaseURL: "https://ffiredetection-1673c-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "ffiredetection-1673c",
};

// Initialize Firebase right here
if (!firebase.apps.length) {
    firebase.initializeApp(firebaseConfig);
}