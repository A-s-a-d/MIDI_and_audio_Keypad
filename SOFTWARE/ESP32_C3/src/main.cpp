#include <Arduino.h>
#include "PINOUT.h"
#include <Ticker.h>

int readButton(uint8_t channel);
void readButtons(); // all buttons
void selectMuxChannel(uint8_t channel);


Ticker ButtonsTicker(readButtons, 1000);;


void setup() {
  Serial.begin(115200); 

  // Initialize the multiplexer select pins
  pinMode(MUX_SEL_A, OUTPUT);
  pinMode(MUX_SEL_B, OUTPUT);
  pinMode(MUX_SEL_C, OUTPUT);

  // Initialize the button output pin
pinMode(BUTTONS_OUT, INPUT);

  // Set up the ticker to call readButtons every 1000 milliseconds
  ButtonsTicker.start();

  Serial.println("Setup complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  ButtonsTicker.update();
}

void selectMuxChannel(uint8_t channel) {
  Serial.print("Selecting MUX channel: ");
  Serial.println(channel);
  digitalWrite(MUX_SEL_A, channel & 0x01);
  digitalWrite(MUX_SEL_B, (channel >> 1) & 0x01);
  digitalWrite(MUX_SEL_C, (channel >> 2) & 0x01);
}


int readButton(uint8_t channel) {
  selectMuxChannel(channel);
  delayMicroseconds(300);  // Augmenté pour éviter un flottement
  int buttonState = digitalRead(BUTTONS_OUT);
  return buttonState;
}


void readButtons() {
  Serial.println("Reading buttons...");
  for (uint8_t i = 0; i < 8; i++) {
    int buttonState = readButton(i);
    Serial.print("Button ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(buttonState);
Serial.print("  ");
  }
Serial.println();
Serial.println();
}
