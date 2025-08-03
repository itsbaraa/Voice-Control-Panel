# Voice Servo Controller

A web-based voice control system for servo motors using ESP32, Arduino, and speech recognition. Control servos with voice commands "Forward" and "Backward".

<img width="700" height="800" alt="image" src="https://github.com/user-attachments/assets/aa45a39f-17a8-44c1-89ce-2a753c2b1a90" />


## Features

- Browser-based speech recognition using Web Speech API
- Real-time servo control via WiFi
- MySQL database for command persistence
- Responsive web interface with dark theme
- Single servo control optimized for voice commands

## System Architecture

Voice Input → Web Browser → PHP API → MySQL Database → ESP32 → Arduino → Servo Motor

## Hardware Requirements

- ESP32 Development Board
- Arduino Uno/Nano
- Servo Motor
- Jumper wires

## Software Requirements

- Web server with PHP support
- MySQL database
- Arduino IDE
- Modern web browser (Chrome, Edge, Safari)

## Wiring

ESP32 Pin 25 (TX) → Arduino Pin 2 (RX)
ESP32 Pin 27 (RX) → Arduino Pin 3 (TX)
Arduino Pin 13 → Servo Signal Pin
GND connections between all components

## Installation

### Database Setup

```sql
CREATE DATABASE servo_control;
USE servo_control;
CREATE TABLE directions (
    id INT AUTO_INCREMENT PRIMARY KEY,
    direction ENUM('forward', 'backward') NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### Configuration

1. Edit `db_config.php` with your MySQL credentials
2. Update WiFi credentials in `ESP32.ino`
3. Set server IP address in `ESP32.ino`
4. Upload Arduino and ESP32 code
5. Deploy web files to your server

## Usage

1. Open web interface in browser
2. Click microphone button
3. Say "Forward" or "Backward"
4. Servo moves to corresponding position (Forward=180°, Backward=0°)

## File Structure

- `index.html` - Main web interface
- `style.css` - Interface styling
- `save_direction.php` - Command storage API
- `direction.php` - Command retrieval API
- `db_config.php` - Database configuration
- `Arduino.ino` - Servo control code
- `ESP32.ino` - WiFi communication code

## API Endpoints

**POST /save_direction.php**
Body: `direction=forward|backward`
Response: `{"success": true, "direction": "forward"}`

**GET /direction.php**
Response: `{"success": true, "direction": "forward", "angle": 180}`

## Showcase
