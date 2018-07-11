#include <Arduino.h>
#include <math.h>

#define LEITURAS 2000

enum PINS{
  POTENCIOMETRO = A0,
  MOSFET =11,
  LED =13
};

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void setup() {
    // put your setup code here, to run once:
    // Serial.begin(115200);
    pinMode(PINS::LED,OUTPUT);
    pinMode(PINS::MOSFET,OUTPUT);
    pinMode(PINS::POTENCIOMETRO,INPUT);
    // setPwmFrequency(PINS::MOSFET, 1);
    analogWrite(PINS::MOSFET, 130);
}

void loop() {
    // put your main code here, to run repeatedly:
    uint32_t read=0;
    for(int i=0;i<LEITURAS;i++){
        read+=analogRead(PINS::POTENCIOMETRO);
    }
    uint32_t media = read/LEITURAS;
    // int linear = log10(media)/log10(1000) * 1023;
    int saida = map(media   ,0,1023,0,255);
    if(saida>255)saida=255;
    if(saida<0)saida=0;
    analogWrite(PINS::MOSFET, saida);
    // static int ultimo=0;
    // if(ultimo!=saida){
    //   ultimo=saida;
    //   Serial.println(ultimo);
    // }
delay(1);
}
