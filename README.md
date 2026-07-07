# 🚗 AUTOMOTIVE PARAMETER MONITORING USING CAN

## 📌 Project Overview

This project implements an Automotive Parameter Monitoring System using **LPC2129 microcontrollers** and the **Controller Area Network (CAN)** protocol. The system continuously monitors vehicle parameters such as engine temperature, fuel level, and indicator status, while enabling reliable real-time communication between multiple embedded nodes over a CAN bus.

The project demonstrates how CAN communication simplifies wiring, improves reliability, and enables efficient data exchange in automotive embedded systems.

---

## 🎯 Objective

To design a multi-node automotive embedded system that monitors vehicle parameters in real time and exchanges data reliably using the CAN (Controller Area Network) protocol.

---

## 🧰 Hardware Requirements

* LPC2129 Microcontroller (3 Nodes)
* MCP2551 CAN Transceiver
* DS18B20 Temperature Sensor
* Fuel Sensor / Potentiometer (ADC Input)
* 20×4 LCD Display
* Push Buttons / Switches
* LEDs (Indicator Simulation)
* CAN Bus Connections

---

## 💻 Software Requirements

* Embedded C
* Keil uVision
* Flash Magic

---

## ⚙️ Working Principle

1. Initializes GPIO, ADC, LCD, CAN controller, and interrupts.
2. The Main Node continuously reads engine temperature using the DS18B20 sensor.
3. The Fuel Node reads the fuel level using the on-chip ADC and calculates the fuel percentage.
4. Sensor data is transmitted between nodes using the CAN protocol.
5. The Indicator Node receives control messages and updates the indicator status.
6. Real-time temperature, fuel level, and system status are displayed on the LCD.
7. CAN transceivers ensure reliable communication between all embedded nodes.

---

## 📡 CAN Communication

**Main Node**

* Reads engine temperature
* Transmits temperature data via CAN
* Controls indicator operations

**Fuel Node**

* Reads fuel level through ADC
* Calculates fuel percentage
* Sends fuel data over CAN

**Indicator Node**

* Receives CAN messages
* Controls indicator LEDs
* Updates indicator status in real time

---

## 🔔 Features

* Real-time vehicle parameter monitoring
* Multi-node CAN communication
* Engine temperature monitoring using DS18B20
* Fuel level monitoring using ADC
* LCD display for live system status
* Interrupt-based indicator control
* Reliable embedded networking using CAN

---

## 🚀 Applications

* Automotive Electronic Control Units (ECUs)
* Vehicle Monitoring Systems
* Automotive Communication Networks
* Industrial CAN-based Monitoring Systems
* Embedded Networking Applications
  
---

## 📷 Hardware Setup
<img width="1280" height="957" alt="Hardware setup" src="https://github.com/user-attachments/assets/e1b31e7a-2a10-4a55-a01a-b2a44c34a232" />

## 👤 Author

**Bala Sudha**
