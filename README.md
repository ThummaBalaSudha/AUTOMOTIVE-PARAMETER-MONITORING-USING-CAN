🚗 Automotive Parameter Monitoring using CAN

I recently worked on a project focused on real-time vehicle parameter monitoring using CAN (Controller Area Network) communication, enabling reliable data exchange and efficient monitoring in automotive embedded systems.

📌 Problem
Modern vehicles require continuous monitoring of parameters such as fuel level, engine temperature, and indicator status. Traditional point-to-point wiring increases complexity and reduces reliability in communication between different modules.

💡 Solution
I developed an Automotive Parameter Monitoring System using CAN protocol with LPC2129 microcontrollers, where multiple nodes communicate sensor and control data in real time through a CAN bus network.

⚙️ How it Works
• Main node continuously monitors engine temperature using DS18B20 sensor
• Fuel node reads fuel gauge data through on-chip ADC and calculates fuel percentage
• Indicator node receives control signals from the main node using CAN communication
• Real-time sensor data and system status are displayed on LCD
• External interrupts are used for indicator control operations
• CAN transceivers ensure reliable communication between all nodes
• System performs continuous monitoring and data exchange in real time

🛠️ Tech Stack
LPC2129 Microcontroller | Embedded C | CAN Protocol | MCP2551 CAN Transceiver | DS18B20 Temperature Sensor | ADC | LCD | Keil uVision | Flash Magic

🔍 Key Features
• Real-time automotive parameter monitoring
• Multi-node CAN communication
• Fuel level monitoring using ADC
• Engine temperature sensing and display
• Interrupt-based indicator control
• Reliable embedded networking system

🎯 Outcome
Successfully developed a working prototype capable of monitoring and transmitting vehicle parameters in real time using CAN communication, improving reliability and efficiency in automotive embedded systems.
