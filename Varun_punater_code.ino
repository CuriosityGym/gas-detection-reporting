#include <SimpleDHT.h>
#include <SD.h>
#include <SPI.h>
#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
File myFile;
int pinCS = 10;
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);
const int MethaneVal=A1;
int c=0;
int x;
int y;
int m_val;
double ppm;
void setup() {
  // put your setup code here, to run once:
pinMode(MethaneVal,INPUT); 
pinMode(7, OUTPUT);

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  //delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  //Serial.begin(115200);
    pinMode(pinCS, OUTPUT);
  
  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  rtc.begin();

  myFile = SD.open("VP_test8.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing to file...");
    // Write to file
   myFile.print("Date");
   myFile.print(",");
   myFile.print("Time");
   myFile.print(",");
   myFile.print("Temperature in Celcius");
   myFile.print(",");
   myFile.print("Humidity in Percentage");
   myFile.print(",");
   myFile.print("Methane ppm");
   myFile.print(",");
   myFile.println();
   // myFile.println("Testing text 1, 2 ,3...");
   myFile.close(); // close the file
   Serial.println("Done.");
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DHT11 failed, err=");
    //Serial.println(err);
    delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");

  m_val = analogRead(A1);
  ppm = 10.938*exp(1.7742*(m_val*3.3/4095));


  if (ppm>999) {
    digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(7, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  }

    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    //delay(1000);

        myFile = SD.open("VP_test8.txt", FILE_WRITE);
    if (myFile) {
          myFile.print(now.year(), DEC);
          myFile.print('/');
          myFile.print(now.month(), DEC);
          myFile.print('/');
          myFile.print(now.day(), DEC);
          myFile.print(",");
          //myFile.print(" (");
          //myFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
          //myFile.print(") ");
          myFile.print(now.hour(), DEC);
          myFile.print(':');
          myFile.print(now.minute(), DEC);
          myFile.print(':');
          myFile.print(now.second(), DEC);
          myFile.print(",");
          myFile.print((int)temperature); 
          myFile.print(","); 
          myFile.print((int)humidity); 
          myFile.print(",");
          myFile.print(ppm);
          myFile.print(",");
          myFile.println();
          myFile.close();
          delay(60000);
    }
}
