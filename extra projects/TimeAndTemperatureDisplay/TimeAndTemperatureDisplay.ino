#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"
#include "uRTCLib.h"
#include <dht.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define outPin 3
#define buttonPin 2

int buttonVal = 0;
int prevVal = 0;

bool screen = true; 
String s = "AM"; 
int diff = 0; 

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// uRTCLib rtc;
uRTCLib rtc(0x68);
dht DHT;     

char daysOfTheWeek[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Declaration for SSD1306 display connected using software SPI:
//#define OLED_MOSI   9
//#define OLED_CLK   10
//#define OLED_DC    11
//#define OLED_CS    12
//#define OLED_RESET 13
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup()
{
  Serial.begin(9600);
  delay(3000);

  pinMode(buttonPin, INPUT); 

  URTCLIB_WIRE.begin();

  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);

  //rtc.set(15, 54, 17, 3, 11, 7, 23);
  
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Uncomment this if you are using SPI
  //if(!display.begin(SSD1306_SWITCHCAPVCC)) {
  //  Serial.println(F("SSD1306 allocation failed"));
  //  for(;;); // Don't proceed, loop forever
  //}

  // Clear the buffer.
  display.clearDisplay();
  
}

void loop() {

  rtc.refresh();
  display.clearDisplay();

  buttonVal = digitalRead(buttonPin); 

  if(buttonVal == HIGH && prevVal == LOW) {
      screen = !screen; 
  }


  if(screen) {
    // Display Text
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(3,5);
    display.fillRoundRect(0, 0, 110, 15, 3, WHITE);

    display.print(daysOfTheWeek[rtc.dayOfWeek()-1]);
    display.print(", ");
    display.print(months[rtc.month()-1]);
    display.print(" ");
    display.print(rtc.day());
    display.print(", 20");
    display.println(rtc.year());

    display.setTextSize(3);
    display.setCursor(15,32);
    display.setTextColor(WHITE);

    if(rtc.hour() > 12) {
      diff = 12;
      s = "PM"; 

    } else if (rtc.hour() == 12) {
      diff = 0;
      s = "PM"; 
    } else {
      diff = 0;
      s = "AM";  
    }

    if(rtc.hour() - diff >= 10) {
        display.setCursor(0,32);
      } else {
        display.setCursor(17,32);
      }
    


    display.print(rtc.hour() - diff);
    display.print(":");

    if(rtc.minute() < 10) {
      display.print(0); 
    }

    display.print(rtc.minute());
    display.print(" "); 

    display.drawRoundRect(90, 28, 30, 22, 6, WHITE);
    display.setTextSize(2);
    display.setCursor(94, 32);
 
    display.print(s);


  } else {
    int readData = DHT.read11(outPin);

    float t = DHT.temperature;        // Read temperature
    float h = DHT.humidity;           // Read humidity

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    display.fillCircle(35, 35, 20, WHITE);
    display.fillCircle(106, 35, 20, WHITE);

    display.print("T     H");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(12,0);

    display.print("emp         umidity");

    display.setTextSize(2);
    display.setCursor(18, 30); 
    display.setTextColor(BLACK);

    display.print(t, 0); 
    display.print("C");

    display.setCursor(88, 30); 
    display.print(h, 0); 
    display.print("%"); 
  }

  display.display(); 

  prevVal = buttonVal;


}
