#include <Wire.h>
//#include <avr/wdt.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

//#define DISPLAY_ADDRESS1 0x21 //This is the change address
#define DISPLAY_ADDRESS2 0x22 //This is the change address
#define DISPLAY_ADDRESS3 0x23 //This is the change address

int cycles = 0;
int value = 0;

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
}

void drawchar(int value, int cycles) {
  display.clearDisplay();

  display.setTextSize(3.5);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(1, 1);     // Start at top-left corner

  //display.print('value:');
  display.println(value);

  //display.print('cycles: ');
  display.print(cycles);

  display.display();

}

void i2cSendValue(char addr, int value, int cycles)
{
  Wire.beginTransmission(addr); // transmit to device #1

  Wire.write(0x7A); // Brightness control command
  Wire.write(25); // Set brightness level: 0% to 100%

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

void loop()
{
  //7-seg setup
  Wire.begin();
  //Wire.beginTransmission(DISPLAY_ADDRESS1);
  //Wire.write('v');
  //Wire.endTransmission();
  Wire.beginTransmission(DISPLAY_ADDRESS2);
  Wire.write('v');
  Wire.endTransmission();
  Wire.beginTransmission(DISPLAY_ADDRESS3);
  Wire.write('v');
  Wire.endTransmission();
  while (1)
    {
      drawchar(cycles % 500, cycles);
      i2cSendValue(DISPLAY_ADDRESS2, abs(cycles)-int(abs(cycles)/10000)*10000, cycles); // Send the four characters to the display
      i2cSendValue(DISPLAY_ADDRESS3, int(abs(cycles)/10000), cycles); // Send the four characters to the display
      //i2cSendValue(DISPLAY_ADDRESS3, random(9999), cycles); // Send the four characters to the display
      cycles++;
      Serial.println(cycles);

      delay(2);
    }
}
