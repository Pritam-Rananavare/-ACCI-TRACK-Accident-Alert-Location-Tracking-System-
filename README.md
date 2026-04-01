# -ACCI-TRACK-Accident-Alert-Location-Tracking-System-
I’m excited to share a project I recently developed that applies embedded systems to a critical real-world problem: road safety and emergency response times.

Project: Acci-Track (Accident Alert & Location Tracking System) 

The Problem: Delayed emergency response in road accidents significantly increases fatality risks, especially in remote areas where incidents can go unnoticed. Traditional reporting relies on bystanders, which isn't always reliable. 

The Solution: I co-developed an automated, cost-effective hardware prototype that detects vehicle collisions in real-time and instantly notifies emergency contacts without requiring any human intervention.

Technical Architecture & My Role: 
Core Controller: Programmed an ESP32 microcontroller in Embedded C (via Arduino IDE) to serve as the brain of the operation, handling sensor fusion and logic. 
Motion Sensing: Interfaced an MPU6050 (Accelerometer + Gyroscope) via I2C to continuously monitor vehicle motion, utilizing algorithmic thresholds to detect the severe impacts or tilts indicative of a crash.
Location & Communication: Integrated a NEO-6M GPS module to pinpoint exact coordinates and a SIM800L GSM module to blast an immediate SMS alert containing a Google Maps link to emergency responders.
IoT Dashboard: Linked the system to the Blynk app over Wi-Fi for live tracking and data visualization.

The Outcome: We successfully built and tested a compact, Li-ion powered device that can be integrated into bikes, cars, and even helmets. By bridging the gap between hardware sensors and cellular communication, Acci-Track proves that affordable IoT solutions can drastically cut down response times and potentially save lives. 

