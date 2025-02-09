#include <Arduino.h>
#include "TIMER_MILLIS.h"
#include "UART.h"

#define MIDI_BAUD 9600
#define MIDI_CHANNEL 1  // MIDI Channel 1



UART_2 uart2; // extern UART_2 uart2;


void sendMIDI(byte command, byte data1, byte data2);



void setup()
{
  //* Init uC configuration
  //setCpuFrequencyMhz(240);      //  Init clock uC
  Serial.begin(MIDI_BAUD);      // Serial Monitor

  delayMicroseconds(1);
  //uart2.init(); // Init UART2
  delayMicroseconds(1);
}

void loop()
{
  //Serial.println("Hello World");
    // if (Serial.available()) {
    //     int note = Serial.parseInt(); // Read number from Serial Monitor
        
    //     if (note >= 1 && note <= 127) {
    //         sendMIDI(0x90, note, 100); // Note On, velocity 100
    //         delay(500);                // Hold note for 500ms
    //         sendMIDI(0x80, note, 0);   // Note Off
    //     } else {
    //     }
    // }
}

void sendMIDI(byte command, byte data1, byte data2) {
    Serial.write(command | (MIDI_CHANNEL - 1)); // Send MIDI command with channel
    Serial.write(data1); // Note number
    Serial.write(data2); // Velocity or other parameter
}
