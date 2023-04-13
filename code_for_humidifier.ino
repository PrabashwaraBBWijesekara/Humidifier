

// libraries for OLED display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//--------------------------------------------------------------------------//

//libraries rtc module
#include "Arduino.h"
#include "uRTCLib.h"
#include "RTClib.h"
//--------------------------------------------------------------------------//

//library for humidity sensor
#include "DHT.h"
//-------------------------------------------------------------------------//





//-------------------------------------------------------------------------//
//define required constants
//-------------------------------------------------------------------------//
//define OLED screnn resolution
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //normally this is the address for the 128x64 OLED display
//-----------------------------------------------------------------------------------------------

//define humidity sensor 
#define DHTPIN 2         //pin number  
#define DHTTYPE DHT22    //type of the sensor
//-------------------------------------------------------------------------//

//define keys 
const int ok = 12;
const int up = 11;
const int down = 10;
const int home = 9;
//-------------------------------------------------------------------------//
int okState=digitalRead(ok);
int upState=digitalRead(up);
int downState=digitalRead(down);
int homeState=digitalRead(home);
//------------------------------------------------------------------------//

//define additionals for rtc
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


//------------------------------------------------------------------------//
//creat objects
//------------------------------------------------------------------------//
//humidity sensor object
DHT dht(DHTPIN, DHTTYPE);
//rtc module object
uRTCLib rtc(0x68);
//OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//print mcw logo
const unsigned char myBitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x0e, 0x00, 0x00, 0x80, 0x00, 0x70, 
	0x00, 0x40, 0x00, 0x00, 0x08, 0x00, 0x01, 0xff, 0xff, 0x80, 0x0e, 0x00, 0x01, 0x80, 0x00, 0x70, 
	0x00, 0xc0, 0x00, 0x00, 0x0c, 0x00, 0x03, 0xfc, 0x1f, 0xe0, 0x07, 0x00, 0x01, 0x80, 0x00, 0x70, 
	0x00, 0xe0, 0x00, 0x00, 0x1c, 0x00, 0x0f, 0xe0, 0x01, 0xf8, 0x07, 0x00, 0x01, 0xc0, 0x00, 0x60, 
	0x00, 0xe0, 0x00, 0x00, 0x1c, 0x00, 0x1f, 0x80, 0x00, 0xf8, 0x07, 0x00, 0x03, 0xc0, 0x00, 0xe0, 
	0x00, 0xf0, 0x00, 0x00, 0x3c, 0x00, 0x3e, 0x00, 0x00, 0x3c, 0x07, 0x80, 0x03, 0xe0, 0x00, 0xe0, 
	0x00, 0xf0, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x1e, 0x03, 0x80, 0x03, 0xe0, 0x00, 0xe0, 
	0x00, 0xf8, 0x00, 0x00, 0x7c, 0x00, 0x78, 0x00, 0x00, 0x0c, 0x03, 0x80, 0x07, 0xe0, 0x01, 0xe0, 
	0x00, 0xf8, 0x00, 0x00, 0x7c, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x03, 0x80, 0x07, 0xf0, 0x01, 0xc0, 
	0x00, 0xfc, 0x00, 0x00, 0xfc, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x0f, 0x70, 0x01, 0xc0, 
	0x01, 0xdc, 0x00, 0x00, 0xfc, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x0e, 0x70, 0x03, 0xc0, 
	0x01, 0xde, 0x00, 0x01, 0xce, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x0e, 0x38, 0x03, 0x80, 
	0x01, 0xce, 0x00, 0x01, 0xce, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x1c, 0x38, 0x03, 0x80, 
	0x01, 0xcf, 0x00, 0x03, 0xce, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x3c, 0x03, 0x80, 
	0x01, 0xc7, 0x00, 0x03, 0x8e, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x3c, 0x1c, 0x07, 0x00, 
	0x01, 0xc3, 0x80, 0x07, 0x0e, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x38, 0x1e, 0x07, 0x00, 
	0x03, 0x83, 0x80, 0x07, 0x0e, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x78, 0x0e, 0x07, 0x00, 
	0x03, 0x83, 0xc0, 0x0e, 0x07, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x0e, 0x0f, 0x00, 
	0x03, 0x81, 0xc0, 0x0e, 0x07, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x07, 0x0e, 0x00, 
	0x03, 0x81, 0xe0, 0x1e, 0x07, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x78, 0xf0, 0x07, 0x0e, 0x00, 
	0x03, 0x80, 0xe0, 0x1c, 0x07, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x38, 0xe0, 0x07, 0x1e, 0x00, 
	0x03, 0x80, 0x70, 0x38, 0x07, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x38, 0xe0, 0x03, 0x9c, 0x00, 
	0x03, 0x00, 0x70, 0x38, 0x07, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x39, 0xc0, 0x03, 0x9c, 0x00, 
	0x07, 0x00, 0x78, 0x78, 0x03, 0x81, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xc0, 0x03, 0xdc, 0x00, 
	0x07, 0x00, 0x38, 0x70, 0x03, 0x80, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x01, 0xf8, 0x00, 
	0x07, 0x00, 0x3c, 0xf0, 0x03, 0x80, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x01, 0xf8, 0x00, 
	0x07, 0x00, 0x1c, 0xe0, 0x03, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0xf8, 0x00, 
	0x07, 0x00, 0x0f, 0xc0, 0x03, 0x80, 0x7c, 0x00, 0x00, 0x0c, 0x00, 0x0f, 0x00, 0x00, 0xf8, 0x00, 
	0x07, 0x00, 0x0f, 0xc0, 0x03, 0x80, 0x3e, 0x00, 0x00, 0x1e, 0x00, 0x0f, 0x00, 0x00, 0xf8, 0x00, 
	0x06, 0x00, 0x0f, 0xc0, 0x03, 0x80, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x0f, 0x00, 0x00, 0x70, 0x00, 
	0x06, 0x00, 0x07, 0x80, 0x01, 0x80, 0x0f, 0xc0, 0x01, 0xf8, 0x00, 0x06, 0x00, 0x00, 0x70, 0x00, 
	0x0e, 0x00, 0x07, 0x80, 0x01, 0x80, 0x07, 0xf8, 0x07, 0xf0, 0x00, 0x06, 0x00, 0x00, 0x30, 0x00, 
	0x0e, 0x00, 0x03, 0x00, 0x01, 0xc0, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x06, 0x00, 0x00, 0x20, 0x00, 
	0x0e, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0f, 0xc1, 0xf8, 0x03, 0xe0, 0x00, 0x1f, 0xe0, 0x3c, 0x1f, 0xe0, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x11, 0x02, 0x0c, 0x18, 0x02, 0x10, 0x00, 0x81, 0x03, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x11, 0x00, 0x08, 0x0c, 0x02, 0x10, 0x01, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x19, 0x01, 0x10, 0x04, 0x02, 0x10, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x19, 0x01, 0x10, 0x00, 0x02, 0x10, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x11, 0x00, 0x20, 0x02, 0x02, 0x10, 0x00, 0x00, 0x02, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x21, 0x02, 0x20, 0x02, 0x02, 0x1f, 0xc0, 0x00, 0x02, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x0f, 0x81, 0xf8, 0x20, 0x02, 0x02, 0x10, 0x00, 0x00, 0x02, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x01, 0x08, 0x10, 0x00, 0x02, 0x10, 0x00, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x01, 0x08, 0x10, 0x04, 0x02, 0x10, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x01, 0x04, 0x10, 0x04, 0x02, 0x10, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x01, 0x02, 0x08, 0x08, 0x02, 0x10, 0x01, 0x81, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x01, 0x02, 0x02, 0x20, 0x4c, 0x1f, 0xc0, 0x46, 0x02, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//-----------------------------------------------------------------------------------//




void setup() {

  //define pinmodes of buttons
  pinMode(ok, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(home, INPUT_PULLUP);
  //--------------------------------------------------------------------//

  //start the process of the rtc 
  URTCLIB_WIRE.begin();
  rtc.set(4, 51, 8, 2, 12, 4, 23);
  //--------------------------------------------------------------------//

  //start the process of the display
  display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS);
  
  //------------------------------------------------------------------------------------
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmap, 128, 64, WHITE);
  display.display();
  delay(1000);
  //--------------------------------------------------------------------//

  //start the process of the humidity
  dht.begin();
  //-------------------------------------------------------------------//

  


}

void loop() {

  //Showing the home Screen
  Home();
  //--------------------------------------------------------------------------------//
  
  //display 
  display.display();
  display.invertDisplay(true);
  
  //-------------------------------------------------------------------------------//
  //mainMenuLoop();

  
}
//Home Screen
void Home(){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,32);   
  display.print("Date & Time: ");
  HumTemp(dht);//---------------------------------HumTemp--------------------------//
  date();//---------------------------------------date-----------------------------//
  time();//---------------------------------------time-----------------------------//
  mainMenuLoop();

}

//function for rtc clock
void time(){

  rtc.refresh();
  display.setCursor(0,50);
  display.print(rtc.hour());

  display.setCursor(15,50);
  display.print(":");

  display.setCursor(30,50);
  display.print(rtc.minute());

  display.setCursor(45,50);
  display.print(":");

  display.setCursor(60,50);
  display.print(rtc.second());

  


}
//-----------------------------------------------------------------------------//
void date(){

  rtc.refresh();
  display.setCursor(0,40);   
  display.print(rtc.year());

  display.setCursor(15,40);   
  display.print('/');

  display.setCursor(20,40);   
  display.print(rtc.month());

  display.setCursor(35,40);   
  display.print('/');

  display.setCursor(40,40);   
  display.print(rtc.day());

  display.setCursor(55,40);
  display.print("(");
  display.setCursor(60,40);
  display.print(daysOfTheWeek[rtc.dayOfWeek()-1]);
  display.print(")");
}

//function for measure temp and humidity
void HumTemp(DHT dht){

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();


  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    //display.write(F("Failed to read from DHT sensor!"));
    return;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);   
  display.print("Humidity: ");
  display.setCursor(0,16);   
  display.print("Temperature: ");
 
    
  
  display.setCursor(60,0);   
  display.print(humidity);
  display.setCursor(95,0);
  display.print("%");
  

  
  
  display.setCursor(70,16);
  display.print(temperature);
  display.print(" C");

}

void mainMenu(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Set time or date");
    display.println("");
    display.setTextColor(BLACK, WHITE);
    display.println("1  time");
    display.setTextColor(WHITE, BLACK);
    display.println("2  date");
    display.display();
  }

void setTimeOk(){
    //Serial.println("setTimeOk pressed");
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(WHITE, BLACK);
    display.println("set time");
    setTime(); //---------------------------------setTime--------------------------//
    display.display();
}
void setDateOk(){
    //Serial.println("setDateOk pressed");
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(WHITE, BLACK);
    display.println("set date");
    setDate();//---------------------------------setDate-------------------------//
    display.display(); 
}

void menuGoUp(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE, BLACK);
  display.println("Set time or date");
  display.println("");
  display.setTextColor(BLACK, WHITE);
  display.println("1  time");
  display.setTextColor(WHITE, BLACK);
  display.println("2  date");
  display.display();
  okState=digitalRead(ok);
  upState=digitalRead(up);
}

void menuGoDown(){
  display.clearDisplay();
  delay(50);
  display.setCursor(0,0);
  display.setTextColor(WHITE, BLACK);
  display.println("Set time or date");
  display.println("");
  display.setTextColor(WHITE, BLACK);
  display.println("1  time");
  display.setTextColor(BLACK, WHITE);
  display.println("2  date");
  display.display();
  okState=digitalRead(ok);
  upState=digitalRead(up);
}

void mainMenuLoop(){
  okState=digitalRead(ok);
  if(okState==LOW){
    okState=HIGH;
    delay(100);
    Serial.println("ok pressed");
    Serial.println(okState);
    while(1){
    mainMenu();//----------------------------------------mainMenu--------------//
    okState=digitalRead(ok);
    homeState=digitalRead(home);
    if(okState==LOW){
      Serial.println("ok pressed");
      okState=HIGH;
      delay(100);
      while(1){
      setTimeOk();//-------------------------------------setTimeOk--------------//
      }  
    }
  else if(homeState==LOW){
    homeState=HIGH;
    delay(100);
    Serial.println("home pressed");
    while(1){
      Home();
      display.display();
      display.invertDisplay(true);
    }
  }
    menuGoUpLoop();//------------------------------------menuGoUpLoop-----------//
    menuGoDownLoop();//------------------------------------menuGoDownLoop-----------/
    
  }
  }
}

void menuGoUpLoop(){
  upState=digitalRead(up);
  if(upState==LOW){
      Serial.println("up pressed");
      upState=HIGH;
      delay(100);
      while(1){
        menuGoUp();//--------------------------------------menuGoUp--------------//
        okState=digitalRead(ok);
        downState=digitalRead(down);
        homeState=digitalRead(home);
        if(okState==LOW){
          Serial.println("ok pressed");
          okState=HIGH;
          delay(100);
          while(1){
          setTimeOk();//------------------------------------setTimeOk-------------//   
          }
        }
        else if(downState==LOW){
          Serial.println("down pressed");
          downState=HIGH;
          while(1){
            menuGoDownLoop();//----------------------------------menuGoUp---------------//
         }
        }
        else if(homeState==LOW){
          homeState=HIGH;
          delay(100);
          Serial.println("home pressed");
          while(1){
            Home();
            display.display();
            display.invertDisplay(true);
          }
        }
      }
  }
}
void menuGoDownLoop(){
  downState=digitalRead(down);
  if(downState==LOW){
      downState=HIGH;
      Serial.println("down pressed");
      while(1){
      menuGoDown();//------------------------------------menuGoDown------------//
      okState=digitalRead(ok);
      upState=digitalRead(up);
      homeState=digitalRead(home);
      if(okState==LOW){
        Serial.println("ok pressed");
        okState=HIGH;
        delay(100);
        while(1){
        setDateOk();//------------------------------------setDateOk-------------//   
        }
      }
      else if(upState==LOW){
        Serial.println("up pressed");
        upState=HIGH;
        while(1){
          menuGoUpLoop();//----------------------------------menuGoUp---------------//
        }
      }

      else if(homeState==LOW){
        homeState=HIGH;
        delay(100);
        Serial.println("home pressed");
        while(1){
          Home();
          display.display();
          display.invertDisplay(true);
        }
      }

      
    }

  }

}

//--------------------------------------set the time---------------------------------//
int state=1;
void setTime(){
  rtc.refresh();
  display.setTextSize(2);
  if(state==1){
    display.setTextColor(BLACK,WHITE);
    display.setCursor(15,25);
    display.print(rtc.hour());
    display.setTextColor(WHITE,BLACK);}
  else{
    display.setCursor(15,25);
    display.print(rtc.hour());
  }
  
  display.setCursor(35,25);
  display.print(":");

  if(state==2){
    display.setCursor(45,25);
    display.setTextColor(BLACK,WHITE);
    display.print(rtc.minute());
    display.setTextColor(WHITE,BLACK);
  }
  else{
  display.setCursor(55,25);
  display.print(rtc.minute());
  }

  display.setCursor(80,25);
  display.print(":");

  display.setCursor(90,25);
  display.print(rtc.second());
  display.setTextSize(1);


  Serial.println(state);
  if(state==1){
  upState=digitalRead(up);
  okState=digitalRead(ok);
  downState=digitalRead(down);
  homeState=digitalRead(home);
  delay(100);
  if(upState==LOW){
    Serial.println("up pressed!");
    upState=HIGH;
    delay(100);
    int curhour=rtc.hour();
    int minute=rtc.minute();
    int second=rtc.minute();
    int day=rtc.day();
    int month=rtc.month();
    int year=rtc.year();

    rtc.set(second, minute, curhour+1, 0, day, month, year);
  }

  else if(downState==LOW){
    Serial.println("down Pressed");
    downState=HIGH;
    delay(100);
    int curhour=rtc.hour();
    int minute=rtc.minute();
    int second=rtc.minute();
    int day=rtc.day();
    int month=rtc.month();
    int year=rtc.year();
    rtc.set(second, minute, curhour-1, 0, day, month, year);
  }

  else if(okState==LOW){
    Serial.println("ok pressed");
    okState=HIGH;
    state=2;
  }
  else if(homeState==LOW){
    homeState=HIGH;
    delay(100);
    Serial.println("home pressed");
    while(1){
      Home();
      display.display();
      display.invertDisplay(true);
    }
  }

  
  }
  
  if(state==2){
  upState=digitalRead(up);
  okState=digitalRead(ok);
  downState=digitalRead(down);
  homeState=digitalRead(home);
  delay(100);
    if(upState==LOW){
    Serial.println("up pressed!");
    upState=HIGH;
    delay(100);
    int curmin=rtc.minute();
    int hour=rtc.hour();
    int second=rtc.minute();
    int day=rtc.day();
    int month=rtc.month();
    int year=rtc.year();

    rtc.set(second, curmin+1, hour, 0, day, month, year);
  }

  else if(downState==LOW){
    Serial.println("down Pressed");
    downState=HIGH;
    int curmin=rtc.minute();
    int hour=rtc.hour();
    int second=rtc.minute();
    int day=rtc.day();
    int month=rtc.month();
    int year=rtc.year();

    rtc.set(second, curmin-1, hour, 0, day, month, year);
  }
  else if(okState==LOW){
    Serial.println("ok pressed");
    okState=HIGH;
    state=3;

  }
   else if(homeState==LOW){
    homeState=HIGH;
    delay(100);
    Serial.println("home pressed");
    while(1){
      Home();
      display.display();
      display.invertDisplay(true);
    }
  }
}
if(state==3){
  state=1;
}
}
//------------------------------set_the_date------------------------------//
int stage=1;
void setDate(){
  rtc.refresh();
  display.setTextSize(2);
  display.setCursor(15,25);
  if(stage==1){
    display.setTextColor(BLACK,WHITE);   
    display.print(rtc.year());
    display.setTextColor(WHITE,BLACK);
  }
  else{
    display.print(rtc.year()); 
  }
  display.print('/');

  if(stage==2){
    display.setTextColor(BLACK,WHITE);   
    display.print(rtc.month());
    display.setTextColor(WHITE,BLACK);
  }
  else{
    display.print(rtc.month());
    }
  
  display.print('/');

  if(stage==3){
    display.setTextColor(BLACK,WHITE);   
    display.print(rtc.day());
    display.setTextColor(WHITE,BLACK);
  }

  else{
    display.print(rtc.day());
    }
  display.setTextSize(1);
  
  if(stage==1){
    upState=digitalRead(up);
    okState=digitalRead(ok);
    downState=digitalRead(down);
    homeState=digitalRead(home);
    delay(100);
    if(upState==LOW){
      Serial.println("up pressed!");
      upState=HIGH;
      delay(100);
      int hour=rtc.hour();
      int minute=rtc.minute();
      int second=rtc.minute();
      int day=rtc.day();
      int month=rtc.month();
      int curyear=rtc.year();

      rtc.set(second, minute, hour, 0, day, month, curyear+1);
    }

    else if(downState==LOW){
      Serial.println("down Pressed");
      downState=HIGH;
      delay(100);
      int hour=rtc.hour();
      int minute=rtc.minute();
      int second=rtc.minute();
      int day=rtc.day();
      int month=rtc.month();
      int curyear=rtc.year();
      rtc.set(second, minute, hour, 0, day, month, curyear-1);
  }

    else if(okState==LOW){
      Serial.println("ok pressed");
      okState=HIGH;
      delay(100);
      stage=2;

    }
    else if(homeState==LOW){
      homeState=HIGH;
      delay(100);
      Serial.println("home pressed");
      while(1){
        Home();
        display.display();
        display.invertDisplay(true);
      }
    }
  }

  if(stage==2){
    upState=digitalRead(up);
    okState=digitalRead(ok);
    downState=digitalRead(down);
    homeState=digitalRead(home);
    delay(100);
    if(upState==LOW){
      Serial.println("up pressed!");
      upState=HIGH;
      delay(100);
      int hour=rtc.hour();
      int minute=rtc.minute();
      int second=rtc.minute();
      int day=rtc.day();
      int curmonth=rtc.month();
      int year=rtc.year();

      rtc.set(second, minute, hour, 0, day, curmonth+1, year);
    }

    else if(downState==LOW){
      Serial.println("down Pressed");
      downState=HIGH;
      delay(100);
      int hour=rtc.hour();
      int minute=rtc.minute();
      int second=rtc.minute();
      int day=rtc.day();
      int curmonth=rtc.month();
      int year=rtc.year();
      rtc.set(second, minute, hour, 0, day, curmonth-1, year);
  }

    else if(okState==LOW){
      Serial.println("ok pressed");
      okState=HIGH;
      delay(100);
      stage=3;

    }
    else if(homeState==LOW){
      homeState=HIGH;
      delay(100);
      Serial.println("home pressed");
      while(1){
        Home();
        display.display();
        display.invertDisplay(true);
      }
    }
  }

  if(stage==3){
    upState=digitalRead(up);
    okState=digitalRead(ok);
    downState=digitalRead(down);
    homeState=digitalRead(home);
    delay(100);
    if(upState==LOW){
      Serial.println("up pressed!");
      upState=HIGH;
      delay(100);
      int hour=rtc.hour();
      int minute=rtc.minute();
      int second=rtc.minute();
      int curday=rtc.day();
      int month=rtc.month();
      int year=rtc.year();

      rtc.set(second, minute, hour, 0, curday+1, month, year);
    }

    else if(downState==LOW){
      Serial.println("down Pressed");
      downState=HIGH;
      delay(100);
      int hour=rtc.hour();
      int minute=rtc.minute();
      int second=rtc.minute();
      int curday=rtc.day();
      int month=rtc.month();
      int year=rtc.year();
      rtc.set(second, minute, hour, 0, curday-1, month, year);
  }

    else if(okState==LOW){
      Serial.println("ok pressed");
      okState=HIGH;
      delay(100);
      stage=1;

    }
    else if(homeState==LOW){
      homeState=HIGH;
      delay(100);
      Serial.println("home pressed");
      while(1){
        Home();
        display.display();
        display.invertDisplay(true);
      }
    }
  }

}















