#include <Wire.h>

#define DISPLAY_ADDRESS1 0x21 //This is the change address
#define DISPLAY_ADDRESS2 0x22 //This is the change address
#define DISPLAY_ADDRESS3 0x23 //This is the change address

int cycles = 0;
int value = 0;

void setup() {
  Serial.begin(9600);
}

void loop()
{
  //7-seg setup
  Wire.begin();
  Wire.beginTransmission(DISPLAY_ADDRESS1);
  Wire.write('v');
  Wire.endTransmission();
  Wire.beginTransmission(DISPLAY_ADDRESS2);
  Wire.write('v');
  Wire.endTransmission();
  Wire.beginTransmission(DISPLAY_ADDRESS3);
  Wire.write('v');
  Wire.endTransmission();
  while (1)
    {
      i2cSendValue(DISPLAY_ADDRESS1, random(9999), cycles); // Send the four characters to the display
      i2cSendValue(DISPLAY_ADDRESS2, random(9999), cycles); // Send the four characters to the display
      i2cSendValue(DISPLAY_ADDRESS3, random(9999), cycles); // Send the four characters to the display
      cycles++;
      Serial.println(cycles);

      delay(2);
    }
}

void i2cSendValue(char addr, int value, int cycles)
{
  Wire.beginTransmission(addr); // transmit to device #1

  Wire.write(0x7A); // Brightness control command
  Wire.write(50); // Set brightness level: 0% to 100%

  if (cycles % 2 == 0){
    // Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
    Wire.write(0x77); // Decimal control command
    Wire.write(0b00000000); // Turn on far-right decimal
    // Wire.endTransmission(); //Stop I2C transmission
  }
  if (cycles % 2 == 1){
    // Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1
    Wire.write(0x77); // Decimal control command
    Wire.write(0b00100000); // Turn on far-right decimal
    // Wire.endTransmission(); //Stop I2C transmission
  }

  Wire.write(value / 1000); //Send the left most digit
  value %= 1000; //Now remove the left most digit from the number we want to display
  Wire.write(value / 100);
  value %= 100;
  Wire.write(value / 10);
  value %= 10;
  Wire.write(value); //Send the right most digit

  Wire.endTransmission(); //Stop I2C transmission
}
