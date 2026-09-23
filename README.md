🤖 OSCAR 1.0 — Autonomous Sumo Robot

OSCAR 1.0 is a 4-wheel-drive autonomous Sumo Robot designed to detect, chase, and push opponents out of the Dohyo while avoiding the ring boundary.

⚙️ Hardware

Component| Quantity| Purpose
Arduino Nano| 1| Main controller
HC-SR04| 3| Opponent detection
Line Sensors| 3| Edge detection
JGA25-370 Motors| 4| Movement & pushing
L298N| 1| Motor control
Battery + Buck Converter| 1| Power supply

«No rear ultrasonic sensor and no servo motor are used in the final design.»

🧠 How It Works

1. Check the edge
Rear line sensors are checked first. If the boundary is detected, OSCAR immediately avoids it.

2. Detect the opponent
Three ultrasonic sensors scan Front / Left / Right.

3. React

Situation| Action
Edge detected| Avoid
Opponent ≤ 35 cm| Attack
Opponent ≤ 100 cm| Track
No opponent| Search

🎯 Control Priority

EDGE AVOIDANCE
       ↓
OPPONENT DETECTION
       ↓
ATTACK / TRACK
       ↓
SEARCH

The robot makes decisions approximately every 20 ms and has a 5-second startup delay.

🔧 Mechanical Design

The chassis focuses on:

Traction + Stability + Pushing Force

It features a 4-wheel-drive system, curved front pushing structure, and protective lower body.

🧪 Development

The project went through multiple iterations involving:

- Wiring & soldering
- Sensor failures
- Motor driver problems
- Mechanical issues
- Wheel problems
- Extensive testing & debugging

«“After 1,000 uploads, rewiring every single pin, and deeply questioning our life choices... IT FINALLY WORKS! 🔥”»

🚀 OSCAR 1.0

SENSORS
   ↓
DECISION MAKING
   ↓
MOTOR CONTROL
   ↓
MOVEMENT
   ↓
PUSH

🤖 Built to Detect. Built to React. Built to Push.
