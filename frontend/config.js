// config.js
const firebaseConfig = {
  apiKey: "AIzaSyAknpKiL4-ShxkL0KrLXapd7rU9rLe0HCo",
  //authDomain: "replace it",
  databaseURL: "https://ffiredetection-1673c-default-rtdb.asia-southeast1.firebasedatabase.app",
  //projectId: "replaced it",
};

// Initialize Firebase right here
if (!firebase.apps.length) {
    firebase.initializeApp(firebaseConfig);
}