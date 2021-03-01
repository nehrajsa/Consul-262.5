#include <Keyboard.h>;

void setup()
{
  Serial1.begin(9600, SERIAL_8N2);
  
  TCCR1B = 0x18; // 0001 1000, Disable Timer Clock 
  TCCR1A = 0x50; // 0101 0000
  
  ICR1 = 52-1; // Low Phase Shift Resolution > 1 degree step-size 
  OCR1A = (int) (ICR1 * 0,04);
  OCR1B = (int) (ICR1 * 0,50);
  TCNT1=0x0;
  
  pinMode(9, OUTPUT);  // OC1a
  pinMode(10, OUTPUT); // OC1b
    
  TCCR1B |= 1; // Prescale=1, Enable Timer Clock
}

void sendKey(byte key, byte modifier)
{
  Keyboard.press(modifier);
  Keyboard.write(key);
  Keyboard.release(modifier);
}

int currentLed = 0;

void shiftLeds() {
  if(currentLed >= 8) {
    currentLed = 0;
  }
  
  currentLed += 1;
  
  int newLed = ((int) pow(2, currentLed) + 1) >> 1;
  Serial1.write(newLed);
}

bool altDown = false;

void loop()
{
  int sr = Serial1.read();
  if(sr != -1) {
    int sra = ~sr & 0xff;

    switch(sra)
    {
      case 0xC0: shiftLeds(); break;
      
      case 0xC1: Keyboard.write(KEY_UP_ARROW); break;
      case 0xC2: Keyboard.write(KEY_DOWN_ARROW); break;
      case 0xC4: Keyboard.write(KEY_LEFT_ARROW); break;
      case 0xC3: Keyboard.write(KEY_RIGHT_ARROW); break;
      case 0x1B: Keyboard.write(KEY_ESC); break;

      case 0xF7: Keyboard.write(0x37); break; // 7
      case 0xF8: Keyboard.write(0x38); break; // 8
      case 0xF9: Keyboard.write(0x39); break; // 9
      case 0xED: Keyboard.write(0x2D); break; // -

      case 0x09: Keyboard.write(KEY_TAB); break;

      case 0x11: sendKey(0x71, KEY_LEFT_CTRL); break; // left ctrl + q
      case 0x17: sendKey(0x77, KEY_LEFT_CTRL); break; // left ctrl + w
      case 0x05: sendKey(0x65, KEY_LEFT_CTRL); break; // left ctrl + e
      case 0x12: sendKey(0x72, KEY_LEFT_CTRL); break; // left ctrl + r
      case 0x14: sendKey(0x74, KEY_LEFT_CTRL); break; // left ctrl + t
      case 0x19: sendKey(0x79, KEY_LEFT_CTRL); break; // left ctrl + y
      case 0x15: sendKey(0x75, KEY_LEFT_CTRL); break; // left ctrl + u
      case 0x0F: sendKey(0x6F, KEY_LEFT_CTRL); break; // left ctrl + o
      case 0x10: sendKey(0x70, KEY_LEFT_CTRL); break; // left ctrl + p
      case 0x7F: Keyboard.write(KEY_DELETE); break;
      case 0xF4: Keyboard.write(0x34); break; // 4
      case 0xF5: Keyboard.write(0x35); break; // 5
      case 0xF6: Keyboard.write(0x36); break; // 6
      case 0xEC: Keyboard.write(0x2C); break; // ,

      case 0x01: sendKey(0x61, KEY_LEFT_CTRL); break; // left ctrl + a
      case 0x13: sendKey(0x73, KEY_LEFT_CTRL); break; // left ctrl + s
      case 0x04: sendKey(0x64, KEY_LEFT_CTRL); break; // left ctrl + d
      case 0x06: sendKey(0x66, KEY_LEFT_CTRL); break; // left ctrl + f
      case 0x07: sendKey(0x67, KEY_LEFT_CTRL); break; // left ctrl + g
      case 0x0A: sendKey(0x6A, KEY_LEFT_CTRL); break; // left ctrl + j
      case 0x0B: sendKey(0x6B, KEY_LEFT_CTRL); break; // left ctrl + k
      case 0x0C: sendKey(0x6C, KEY_LEFT_CTRL); break; // left ctrl + l
      case 0x1D: sendKey(0x5D, KEY_LEFT_CTRL); break; // left ctrl + ]

      case 0x0D: Keyboard.write(KEY_RETURN); break;
      case 0xF1: Keyboard.write(0x31); break; // 1
      case 0xF2: Keyboard.write(0x32); break; // 2
      case 0xF3: Keyboard.write(0x33); break; // 3
      case 0xF0: Keyboard.write(0x30); break; // 0
      case 0xEE: Keyboard.write(0x2E); break; // .
      
      case 0x1A: sendKey(0x7A, KEY_LEFT_CTRL); break; // left ctrl + z
      case 0x18: sendKey(0x78, KEY_LEFT_CTRL); break; // left ctrl + x
      case 0x03: sendKey(0x63, KEY_LEFT_CTRL); break; // left ctrl + c
      case 0x16: sendKey(0x76, KEY_LEFT_CTRL); break; // left ctrl + v
      case 0x02: sendKey(0x62, KEY_LEFT_CTRL); break; // left ctrl + b
      case 0x0E: sendKey(0x6E, KEY_LEFT_CTRL); break; // left ctrl + n
      case 0x1F: sendKey(0x2F, KEY_LEFT_CTRL); break; // left ctrl + /
      case 0x1C: sendKey(0x5C, KEY_LEFT_CTRL); break; // left ctrl + \

      case 0xCF: sendKey(0xCD, KEY_LEFT_CTRL); break; // left ctrl + enter
      case 0x00: sendKey(0x20, KEY_LEFT_CTRL); break; // left ctrl + enter

      case 0xCD: Keyboard.write(KEY_RETURN); break; // enter
      case 0x08: Keyboard.write(KEY_BACKSPACE); break; // back space
      case 0xC5:  
        altDown = !altDown;
        if(altDown) 
          Keyboard.press(KEY_LEFT_ALT);
        else 
          Keyboard.release(KEY_LEFT_ALT);
         
        break;
      
      default: Keyboard.write(sra); break;
    }      
  }   
}
