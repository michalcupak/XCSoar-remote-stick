#include <FastLED.h>

// uses libraries from
// https://github.com/r89m/PushButton 
// https://github.com/r89m/Button
// https://github.com/thomasfredericks/Bounce2

#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>
#include <BleKeyboard.h>


// Version
#define VERSION "2.3"

// Init BLE
BleKeyboard bleKeyboard("Remote Stick 1", "Michal Cupak", 100);


// Pins
const int ledPin = 27;
const int Button_home_pin = 39;    // M5 HOME button

const int Joy_up_pin = 19;    // some pin
const int Joy_down_pin = 33;    // some pin
const int Joy_left_pin = 23;    // some pin
const int Joy_right_pin = 26;    // some pin
const int Button_ok_pin = 32;    // some pin
const int Button_fn_pin = 22;    // some pin
const int Button_menu_pin = 21;    // some pin
const int Button_cancel_pin = 25;    // some pin


// Button's keys
const char Joy_up_press_key = KEY_UP_ARROW;
const char Joy_down_press_key = KEY_DOWN_ARROW;
const char Joy_left_press_key = KEY_LEFT_ARROW;
const char Joy_right_press_key = KEY_RIGHT_ARROW;
const char Button_ok_press_key = KEY_RETURN;
const char Button_ok_hold_key = 'Z';
const char Button_fn_press_key = 'M';
const char Button_fn_hold_key = 'C';
const char Button_menu_press_key = KEY_F1;
const char Button_menu_hold_key = 'A';
const char Button_cancel_press_key = KEY_ESC;
const char Button_cancel_hold_key = 'D';


// PushButton's instances
PushButton Joy_up = PushButton(Joy_up_pin, ENABLE_INTERNAL_PULLUP);
PushButton Joy_down = PushButton(Joy_down_pin, ENABLE_INTERNAL_PULLUP);
PushButton Joy_left = PushButton(Joy_left_pin, ENABLE_INTERNAL_PULLUP);
PushButton Joy_right = PushButton(Joy_right_pin, ENABLE_INTERNAL_PULLUP);
PushButton Button_ok = PushButton(Button_ok_pin, ENABLE_INTERNAL_PULLUP);
PushButton Button_fn = PushButton(Button_fn_pin, ENABLE_INTERNAL_PULLUP);
PushButton Button_menu = PushButton(Button_menu_pin, ENABLE_INTERNAL_PULLUP);
PushButton Button_cancel = PushButton(Button_cancel_pin, ENABLE_INTERNAL_PULLUP);


// define timing for buttons etc.
const int joy_rebounce_interval = 3;
const int joy_key_rebounce_threshold = 20;
const int joy_key_first_pressed_threshold = 100;
const int button_hold_threshold = 500;
const int joy_hold_threshold = 1;


// Variables
int bt_first_connected = false;
boolean first_pressed = 1;
int joy_key_counter = 0;


// RGB led
#define NUM_LEDS 1    // How many leds in your strip?
CRGB leds[NUM_LEDS];  // Define the array of leds


void setup() {

  Serial.begin(115200);
  Serial.println("Version: " VERSION);
  

  // pinMode(ledPin, OUTPUT);
  FastLED.addLeds<SK6812, ledPin, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(100);
  leds[0] = CRGB::White;
  FastLED.show();


    
    Serial.println("Starting BLE work!");
    bleKeyboard.begin();
    
    Joy_up.onRelease(onJoyReleased);
    Joy_down.onRelease(onJoyReleased);
    Joy_left.onRelease(onJoyReleased);
    Joy_right.onRelease(onJoyReleased);
  
    Joy_up.onHoldRepeat(joy_hold_threshold, joy_rebounce_interval, onJoy);
    Joy_down.onHoldRepeat(joy_hold_threshold, joy_rebounce_interval, onJoy);
    Joy_left.onHoldRepeat(joy_hold_threshold, joy_rebounce_interval, onJoy);
    Joy_right.onHoldRepeat(joy_hold_threshold, joy_rebounce_interval, onJoy);
    
    Button_ok.onRelease(0, button_hold_threshold-1, onButtonReleased);
    Button_fn.onRelease(0, button_hold_threshold-1, onButtonReleased);
    Button_menu.onRelease(0, button_hold_threshold-1, onButtonReleased);
    Button_cancel.onRelease(0, button_hold_threshold-1, onButtonReleased);
    
    Button_ok.onHold(button_hold_threshold, onButtonHeld);
    Button_fn.onHold(button_hold_threshold, onButtonHeld);
    Button_menu.onHold(button_hold_threshold, onButtonHeld);
    Button_cancel.onHold(button_hold_threshold, onButtonHeld);
  
    joy_key_counter = 0;
  
  
}



void loop() {

    if(bleKeyboard.isConnected()) {
      leds[0] = CRGB::Blue;
      FastLED.show(); 
      if (not bt_first_connected) { 
        leds[0] = CRGB::Blue;
        FastLED.show(); 
        bt_first_connected = true;
        Serial.println("BT connected");
      }
  
      Joy_up.update();
      Joy_down.update();
      Joy_left.update();
      Joy_right.update();
      Button_ok.update();
      Button_fn.update();
      Button_menu.update();
      Button_cancel.update();
      
    } else {
      leds[0] = CRGB::White;
      FastLED.show();
    }
  
}


void keyboardPress(char key) {
  bleKeyboard.press(key);
}

void onButtonReleased(Button& btn, uint16_t duration){
  if (btn.is(Button_ok)) keyboardPress(Button_ok_press_key);
  if (btn.is(Button_fn)) keyboardPress(Button_fn_press_key);
  if (btn.is(Button_menu)) keyboardPress(Button_menu_press_key);
  if (btn.is(Button_cancel)) keyboardPress(Button_cancel_press_key);
  
  bleKeyboard.releaseAll();
}

void onButtonHeld(Button& btn, uint16_t duration){
  if (btn.is(Button_ok)) keyboardPress(Button_ok_hold_key);
  if (btn.is(Button_fn)) keyboardPress(Button_fn_hold_key);
  if (btn.is(Button_menu)) keyboardPress(Button_menu_hold_key);
  if (btn.is(Button_cancel)) keyboardPress(Button_cancel_hold_key);
  
  bleKeyboard.releaseAll();
}

void onJoy(Button& btn, uint16_t duration, uint16_t repeat_count){
  if(btn.isPressed() && joy_key_counter == 5) {
    if(btn.is(Joy_up)) keyboardPress(Joy_up_press_key);
    if(btn.is(Joy_down)) keyboardPress(Joy_down_press_key);
    if(btn.is(Joy_left)) keyboardPress(Joy_left_press_key);
    if(btn.is(Joy_right)) keyboardPress(Joy_right_press_key);
    bleKeyboard.releaseAll(); 
  } 
  joy_key_counter = joy_key_counter +1; 
  if (first_pressed && joy_key_counter > joy_key_first_pressed_threshold){
    joy_key_counter = 0;
    first_pressed = 0;
  }
  if (!first_pressed && joy_key_counter > joy_key_rebounce_threshold) joy_key_counter = 0;  
}

void onJoyReleased(Button& btn, uint16_t duration){
  joy_key_counter = 0;
  first_pressed = 1;
}


