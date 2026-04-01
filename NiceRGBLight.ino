#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

// =================== Settings ===================
const uint8_t DATA_PIN_1   = 6;    // First Data pin (20 LEDs)
const uint8_t DATA_PIN_2   = 7;    // Second Data pin (6 LEDs)
const uint8_t BUTTON_PIN   = 10;   // Button pin

const uint8_t NUM_LEDS_1   = 20;   // Number of LEDs in first strip
const uint8_t NUM_LEDS_2   = 6;    // Number of LEDs in second strip
const uint8_t BRIGHTNESS   = 100;  // LED brightness

const uint16_t DEBOUNCE_MS   = 50;  // Button debounce time
const uint16_t RAINBOW_SPEED = 10;  // Rainbow speed (ms between steps)

// Static colors
const uint8_t COLOR_RED[3]        = {255, 0,   0};    // Bright red
const uint8_t COLOR_GREEN[3]      = {0,   255, 0};    // Bright green
const uint8_t COLOR_BLUE[3]       = {0,   0,   255};  // Bright blue
const uint8_t COLOR_CYAN[3]       = {0,   255, 255};  // Cyan, cool
const uint8_t COLOR_MAGENTA[3]    = {128, 0,   128};  // Magenta
const uint8_t COLOR_YELLOW[3]     = {255, 255, 0};    // Yellow
const uint8_t COLOR_ORANGE[3]     = {255, 165, 0};    // Warm orange
const uint8_t COLOR_PINK[3]       = {255, 105, 180};  // Bright pink
const uint8_t COLOR_COLD_WHITE[3] = {255, 255, 255};  // Cold white
const uint8_t COLOR_WARM_WHITE[3] = {255, 180, 140};  // Warm white
const uint8_t COLOR_LIGHT_BLUE[3] = {173, 216, 230};  // Soft light blue
const uint8_t COLOR_LIGHT_GREEN[3]= {144, 238, 144};  // Soft light green
const uint8_t COLOR_LAVENDER[3]   = {230, 230, 250};  // Lavender, light purple
const uint8_t COLOR_PEACH[3]      = {255, 218, 185};  // Light peach
const uint8_t COLOR_DEEP_PURPLE[3]= {102, 0,   153};  // Deep purple
const uint8_t COLOR_TEAL[3]       = {0,   128, 128};  // Dark cyan
const uint8_t CUSTOM_COLOR[3]     = {150, 75,  0};    // Brown
uint8_t clr_cnt = 17;
// =====================================================

// Create NeoPixel objects
Adafruit_NeoPixel fan1(NUM_LEDS_1, DATA_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel fan2(NUM_LEDS_2, DATA_PIN_2, NEO_GRB + NEO_KHZ800);

// =================== Variables =====================
uint8_t mode = 0;                    // Color mode
int lastButtonState = HIGH;          // For debounce
int buttonStableState = HIGH;        // Stable state after debounce
unsigned long lastDebounceTime = 0;
unsigned long lastRainbowUpdate = 0;
uint16_t rainbowIndex = 0;           // Rainbow index
// =====================================================

void setup() {
  fan1.begin();
  fan1.setBrightness(BRIGHTNESS);
  fan1.show();

  fan2.begin();
  fan2.setBrightness(BRIGHTNESS);
  fan2.show();

  // Run color diagnostic on startup
  diagnosticColors();

  // Restore last saved color mode
  mode = EEPROM.read(0);
  if (mode > clr_cnt) mode = 0;

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button connected to GND
}

void loop() {
  handleButton();     // Handle button input
  updateColor(mode);  // Update color on both strips
}

// =================== Functions =====================
void handleButton() {
  int reading = digitalRead(BUTTON_PIN);

  // Debounce
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != buttonStableState) {
      buttonStableState = reading;

      // Falling edge HIGH → LOW
      if (buttonStableState == LOW) {
        mode++;
        if (mode >= clr_cnt) mode = 0;
        EEPROM.update(0, mode);
      }
    }
  }

  lastButtonState = reading;
}

// Update color on both strips based on mode
void updateColor(uint8_t _mode) {
  switch(_mode) {
    case 0:  rainbowCycleSmooth(); break; // Rainbow
    case 1:  setStaticColor(COLOR_RED[0],         COLOR_RED[1],         COLOR_RED[2]); break;
    case 2:  setStaticColor(COLOR_GREEN[0],       COLOR_GREEN[1],       COLOR_GREEN[2]); break;
    case 3:  setStaticColor(COLOR_BLUE[0],        COLOR_BLUE[1],        COLOR_BLUE[2]); break;
    case 4:  setStaticColor(COLOR_MAGENTA[0],     COLOR_MAGENTA[1],     COLOR_MAGENTA[2]); break;
    case 5:  setStaticColor(COLOR_YELLOW[0],      COLOR_YELLOW[1],      COLOR_YELLOW[2]); break;
    case 6:  setStaticColor(COLOR_ORANGE[0],      COLOR_ORANGE[1],      COLOR_ORANGE[2]); break;
    case 7:  setStaticColor(COLOR_PINK[0],        COLOR_PINK[1],        COLOR_PINK[2]); break;
    case 8:  setStaticColor(COLOR_COLD_WHITE[0],  COLOR_COLD_WHITE[1],  COLOR_COLD_WHITE[2]); break;
    case 9:  setStaticColor(COLOR_WARM_WHITE[0],  COLOR_WARM_WHITE[1],  COLOR_WARM_WHITE[2]); break;
    case 10: setStaticColor(COLOR_LIGHT_BLUE[0],  COLOR_LIGHT_BLUE[1],  COLOR_LIGHT_BLUE[2]); break;
    case 11: setStaticColor(COLOR_LIGHT_GREEN[0], COLOR_LIGHT_GREEN[1], COLOR_LIGHT_GREEN[2]); break;
    case 12: setStaticColor(COLOR_LAVENDER[0],    COLOR_LAVENDER[1],    COLOR_LAVENDER[2]); break;
    case 13: setStaticColor(COLOR_PEACH[0],       COLOR_PEACH[1],       COLOR_PEACH[2]); break;
    case 14: setStaticColor(COLOR_DEEP_PURPLE[0], COLOR_DEEP_PURPLE[1], COLOR_DEEP_PURPLE[2]); break;
    case 15: setStaticColor(COLOR_TEAL[0],        COLOR_TEAL[1],        COLOR_TEAL[2]); break;
    case 16: setStaticColor(CUSTOM_COLOR[0],      CUSTOM_COLOR[1],      CUSTOM_COLOR[2]); break;
  }
}

// Set static color on both strips
void setStaticColor(uint8_t r, uint8_t g, uint8_t b) {
  for(uint8_t i = 0; i < NUM_LEDS_1; i++) fan1.setPixelColor(i, fan1.Color(r,g,b));
  for(uint8_t i = 0; i < NUM_LEDS_2; i++) fan2.setPixelColor(i, fan2.Color(r,g,b));
  fan1.show();
  fan2.show();
}

// Smooth rainbow animation on both strips
void rainbowCycleSmooth() {
  if (millis() - lastRainbowUpdate < RAINBOW_SPEED) return;
  lastRainbowUpdate = millis();

  for(uint8_t i = 0; i < NUM_LEDS_1; i++)
    fan1.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS_1) + rainbowIndex) & 255));

  for(uint8_t i = 0; i < NUM_LEDS_2; i++)
    fan2.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS_2) + rainbowIndex) & 255));

  fan1.show();
  fan2.show();
  rainbowIndex++;
}

// Rainbow color generator
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) return fan1.Color(255 - WheelPos*3, 0, WheelPos*3);
  else if(WheelPos < 170) { WheelPos -= 85; return fan1.Color(0, WheelPos*3, 255 - WheelPos*3); }
  else { WheelPos -= 170; return fan1.Color(WheelPos*3, 255 - WheelPos*3, 0); }
}

// Color diagnostic: quickly run through all color maps
void diagnosticColors() {
  const uint16_t delay_ms = 250; // Delay between colors
  for (uint8_t i = 0; i < clr_cnt; i++) {
    updateColor(i);
    delay(delay_ms);
  }
}