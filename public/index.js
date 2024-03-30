// Import the functions you need from the SDKs you need
import { initializeApp } from "https://www.gstatic.com/firebasejs/10.10.0/firebase-app.js";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/10.10.0/firebase-database.js"

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyDTiSPMtYjCvtxyTZh2EApYBUlPGVpd6e0",
  authDomain: "tempmonitor-267d5.firebaseapp.com",
  databaseURL: "https://tempmonitor-267d5-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "tempmonitor-267d5",
  storageBucket: "tempmonitor-267d5.appspot.com",
  messagingSenderId: "701721884081",
  appId: "1:701721884081:web:e1d81ca9674e80a6a17990"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app)

const dataRef = ref(database, 'tempmonitor')

const updateElement = (tempElem, humiElem, data) => {
  const { humi, temp } = data

  tempElem.textContent = temp
  humiElem.textContent = humi
}

onValue(dataRef, (snapshot) => {
  const data = snapshot.val()

  updateElement(document.getElementById('temperature'), document.getElementById('humidity'), data)
})