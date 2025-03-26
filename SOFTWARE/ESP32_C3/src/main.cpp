#include <Arduino.h>
#include "PINOUT.h"
#include <Ticker.h>

#define MIDI_CHANNEL 1   // MIDI channel (1-16)

// Define MIDI notes for each button
const uint8_t midiNotes[7] = {60, 62, 64, 65, 67, 69, 71}; // C, D, E, F, G, A, B

void readButtons(); 
void selectMuxChannel(uint8_t channel);
void sendMidiNoteOn(uint8_t note);
void sendMidiNoteOff(uint8_t note);

Ticker ButtonsTicker(readButtons, 1); // Run every 1ms

bool lastButtonStates[7] = {0}; // Store button states
uint32_t lastDebounceTime[7] = {0}; // Store debounce timestamps
const uint16_t debounceDelay = 10;  // 10ms debounce time

void setup() {
  Serial.begin(115200);  
  Serial1.begin(115200, SERIAL_8N1, -1, UART_TX); // Initialize MIDI UART data is 8 bit 1 stop bit lsb first not inverted

  // Configure multiplexer select pins
  pinMode(MUX_SEL_A, OUTPUT);
  pinMode(MUX_SEL_B, OUTPUT);
  pinMode(MUX_SEL_C, OUTPUT);

  // Configure input pin
  pinMode(BUTTONS_OUT, INPUT);

  // Start ticker
  ButtonsTicker.start();

  Serial.println("Setup complete");
}

void loop() {

  for(int i = 0; i < 7; i++)
  {
    sendMidiNoteOn(midiNotes[i]);
    delay(250);
    sendMidiNoteOff(midiNotes[i]);
    delay(250);
  }
  // ButtonsTicker.update();
}

// Select a channel on the multiplexer
void selectMuxChannel(uint8_t channel) {
  digitalWrite(MUX_SEL_A, channel & 0x01);
  digitalWrite(MUX_SEL_B, (channel >> 1) & 0x01);
  digitalWrite(MUX_SEL_C, (channel >> 2) & 0x01);
}

// Read button state with debounce logic
int readButton(uint8_t channel) {
  selectMuxChannel(channel);
  return digitalRead(BUTTONS_OUT);
}

// Scan all buttons and send MIDI signals
void readButtons() {
  uint32_t currentMillis = millis();

  for (uint8_t i = 0; i < 7; i++) {
    int buttonState = readButton(i);

    if (buttonState != lastButtonStates[i]) {
      if (currentMillis - lastDebounceTime[i] > debounceDelay) {
        lastDebounceTime[i] = currentMillis;

        if (buttonState == HIGH) {
          sendMidiNoteOn(midiNotes[i]);
          Serial.print("Note ON: ");
        } else {
          sendMidiNoteOff(midiNotes[i]);
          Serial.print("Note OFF: ");
        }
        Serial.println(midiNotes[i]);

        lastButtonStates[i] = buttonState;
      }
    }
  }
}

// Send MIDI Note ON message
void sendMidiNoteOn(uint8_t note) {
  Serial1.write(0x90 | (MIDI_CHANNEL - 1)); // Note On
  Serial1.write(note);
  Serial1.write(127); // Max velocity
}

// Send MIDI Note OFF message
void sendMidiNoteOff(uint8_t note) {
  Serial1.write(0x80 | (MIDI_CHANNEL - 1)); // Note Off
  Serial1.write(note);
  Serial1.write(0); // Velocity 0
}
