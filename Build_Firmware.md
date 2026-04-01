# Build and Firmware

This document provides step-by-step instructions to **build, configure, and upload the ARGB controller firmware** to your **Arduino Pro Mini** board.

---

## 🛠️ Requirements

- Arduino IDE (version 1.8.x or newer)  
- **Arduino Pro Mini (5V / 16 MHz)**  
- USB-to-Serial adapter (FTDI, CH340, or similar) for programming  
- USB cable for adapter connection  
- 5V 3-pin ARGB LED strips (data + 5V + GND)  
- External 5V power supply if LEDs > ~10  

---

## 📦 Dependencies

- [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel)  
  Install via Arduino IDE Library Manager.

---

## 🛠️ Configuration

Before building, edit the main sketch to configure your pins, LED count, brightness, and custom color:

```cpp
const uint8_t DATA_PIN_1 = 6;    // First LED strip data pin
const uint8_t DATA_PIN_2 = 7;    // Second LED strip data pin
const uint8_t BUTTON_PIN = 10;   // Button input pin

const uint8_t NUM_LEDS_1 = 20;   // Number of LEDs on first strip
const uint8_t NUM_LEDS_2 = 6;    // Number of LEDs on second strip

const uint8_t BRIGHTNESS = 100;  // LED brightness (0-255)

const uint8_t CUSTOM_COLOR[3] = {150, 75,  0};  // Optional custom color
```

>  ⚠️ Make sure the pinout matches the Pro Mini layout. `DATA_PIN_1` and `DATA_PIN_2` must be connected to digital pins capable of driving NeoPixels.

---

## 🔨 Build & Upload (Arduino Pro Mini)

1. Connect your **Arduino Pro Mini** to a USB-to-Serial adapter. Ensure proper power and data connections.  
2. Open the sketch in **Arduino IDE**.  
3. Select the correct board, processor, and programmer corresponding to your setup.  
4. Compile/verify the sketch to check for errors.  
5. Upload the firmware to the board.  

After upload, the firmware will automatically:
- Run a color diagnostic
- Restore the last saved color mode from EEPROM
- Allow mode switching via the button

---

## 🔌 Wiring / Connection (Arduino Pro Mini)

See [Wiring Diagram](./Wiring_Diagram.md) for full wiring instructions and pinout details.

---

## ✅ Notes
- Adjust BRIGHTNESS to reduce power consumption
- You can define a custom color in CUSTOM_COLOR
- Make sure Adafruit NeoPixel library is installed before compiling
- For longer LED strips, consider capacitor on VCC and resistor on data line for stability
