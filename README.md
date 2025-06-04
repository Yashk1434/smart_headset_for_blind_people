# 🎧 Smart Headset for the Visually Impaired - *The Third Eye*

A compact, wearable assistive device enhancing mobility, safety, and independence for visually impaired individuals using real-time obstacle detection, auditory feedback, emergency SOS, and remote wireless control.

---

## 📸 Project Showcase  
<img src="https://github.com/user-attachments/assets/deae8dd7-1cb7-469d-bc03-cc02b504c5d8" width="500"/>
<img src="https://github.com/user-attachments/assets/32535b62-00e2-461c-b19c-78398e7f8d19" width="500"/>



---

## 📖 Overview

The **Smart Headset for the Visually Impaired** is a low-cost, Arduino Mega-based device that detects obstacles in real time and alerts the user through directional buzzers. It integrates:

- **3 ultrasonic sensors** for obstacle detection
- **Directional buzzers** for sound feedback
- **Touch sensor** for easy control & SOS triggering
- **Bluetooth control** via Android app
- **GPS & GSM modules** for emergency SMS alerts with live location

---

## 🛠️ Features

- 📏 **Multi-Directional Obstacle Detection**
- 🎵 **Directional Audio Feedback**
- 🖐️ **Single-tap Control & SOS Activation**
- 📱 **Bluetooth-based Wireless Control**
- 📍 **Live Location SOS via SMS**
- 🔒 **Secret Keyword-Based Location Requests**
- 🎛️ **Indoor & Outdoor Mode Switching**
- 🔊 **Volume Adjustment via App**
- 📡 **Find My Device Feature**

---

## 🧭 System Architecture  
<img src="https://github.com/user-attachments/assets/05f31248-e680-4f62-ba62-b6f283cb24e9" width="400"/>  
<img src="https://github.com/user-attachments/assets/570c2a0e-c8ec-4f77-a157-36fb71289a3f" width="500"/>

---

## 🔧 Hardware Components

| Component              | Description                                            |
|:----------------------|:------------------------------------------------------|
| Arduino ATmega2560      | Central controller for all connected modules          |
| 3x Ultrasonic Sensors   | Detect obstacles at Left, Right & Front directions     |
| 2x Buzzers              | Provide directional audio alerts                      |
| Touch Sensor (TTP223)   | Toggle detection / Trigger SOS function               |
| NEO-6M GPS Module       | Fetches live GPS location                              |
| SIM800L GSM Module      | Sends location via SMS                                 |
| HC-05 Bluetooth Module  | Enables mobile app communication                       |
| Li-ion Battery (3.7V)   | Powers the device                                       |

---

## 💻 Software Components

- Arduino IDE (Embedded C/C++)
- TinyGPS++ & SoftwareSerial libraries
- Android App (Bluetooth communication & controls)

---

## 📲 Mobile App Features  

- Start/Stop **Find My Device**
- Toggle between **Indoor/Outdoor Mode**
- Adjust **Volume (Low / Medium / High)**
- SOS activation & Bluetooth-based commands

---

## 📈 Working Flow  

**Single Press** → Obstacle Detection Toggle  
**5-Second Hold** → Activate SOS (Send location via SMS)  
**Bluetooth Commands** → Mode switching, volume control, buzzer find function

---

## 📊 Results  

- ✅ Reliable, real-time obstacle detection in 3 directions  
- ✅ Accurate GPS location retrieval and SMS dispatch  
- ✅ Responsive Bluetooth control via Android application  
- ✅ Successfully tested in real-world indoor and outdoor conditions  

---

## 📌 Future Scope

- 🔊 Voice-based guidance feedback  
- 📷 Camera module for object recognition  
- ☁️ Cloud-based location logging & caregiver alerts  
- 🔋 Enhanced battery management system  
- 📱 Dedicated Android/iOS app for expanded controls  

---

## 📚 Report & Documentation  

You can find the full technical documentation in the `Smart Headset for Blind.docx` file included in this repository.

---

## ✨ Demo  
<img src="https://github.com/user-attachments/assets/f1312aee-b9bf-4d96-b214-bfebf1a7355d" width="500"/>  
<img src="https://github.com/user-attachments/assets/d7c980bf-11ca-43f3-a3a9-4d54d79caf77" width="500"/>

---

## 🙌 Authors  

- **Yash Kadu**  
- **Yashas Awachar**  
- **Amrita Bera**  
- **Snehal Borse**  

---

## 📬 Contact  

For queries or collaborations:  
📧 ykadu1434@gmail.com

---
