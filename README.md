# 📡 Send SMS with SIM800L and Arduino Uno

This project demonstrates how to send SMS using a **SIM800L GSM module** connected to an **Arduino Uno**.

![SIM800L with Arduino](https://projects.arduinocontent.cc/7d78f296-1c07-445f-96e1-eb0a231f8f1d.jpg)

## 🚀 Features
✔️ Checks **signal strength** before sending SMS  
✔️ **Re-connects** if the network is unavailable  
✔️ **Sets SMS Center Number (SMSC)** automatically  
✔️ Enables **SMS delivery reports**  
✔️ **Prints debug logs** to help troubleshoot  

---

## 🛠️ Hardware Requirements
- **Arduino Uno**
- **SIM800L GSM Module**
- **SIM Card (with SMS credit)**
- **External Power Supply (4.2V, 2A)**
- **Jumper Wires**
- **Antenna for SIM800L**
- **Resistors (1KΩ & 2KΩ for voltage divider)**

---

## 🔌 Wiring Diagram
### **Wiring (SIM800L ↔ Arduino)**
| **SIM800L Pin** | **Arduino Uno Pin** |
|---------------|------------------|
| **VCC (4.2V)** | **External 4.2V Power Supply (2A)** |
| **GND** | **GND** |
| **TX** | **Pin 10 (RX)** |
| **RX** | **Pin 11 (TX) via a voltage divider** |
| **RST (Optional)** | **Arduino Reset Pin** |

### **Wiring Image**
![SIM800L Wiring Diagram](https://projects.arduinocontent.cc/5b073429-78e0-40b3-8c65-bf3a46ca811e.jpg)

📌 **Important Notes**
- **Use a 4.2V, 2A power supply** (Arduino 5V is **not enough**).
- If using a **5V Arduino**, step down TX voltage using a **voltage divider** (1KΩ & 2KΩ resistors).
- Ensure the **antenna is connected** properly.
- The **SIM card must be activated** with SMS balance.


---

### **📌 What This README Includes**
✅ **Detailed setup guide**  
✅ **Wiring diagram**  
✅ **Complete Arduino Code**  
✅ **Troubleshooting & Debugging Guide**  
✅ **Helpful images & resources**  

---

### **🚀 Next Steps**
- **Add this README to your GitHub repo**.
- **Upload wiring images to GitHub and replace links**.
- **Test and improve SMS sending features**.

Let me know if you need more improvements! 🚀

---

## 📝 Code
Upload this code to your **Arduino Uno**:
```cpp

#include <SoftwareSerial.h>

// Define SoftwareSerial pins for SIM800L
SoftwareSerial sim800l(10, 11); // RX, TX

