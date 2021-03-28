#include <Wire.h>
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);    // I2C
#include "Wire.h"
#include "RTClib.h"
RTC_DS1307 RTC;


//Таймер часов

DateTime today;

// Время полива
int water_hours = 22;
int water_minutes = 53;

// set water relays
int relays[4] = {6,8,9,10};

// set water pump
int pump = 4;

static unsigned long currentMillis_send = 0;
static unsigned long  Lasttime_send = 0;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat",};
unsigned long nowtime;
unsigned long endtime;
unsigned long nowtimeNext;
unsigned long nowtime1;
unsigned long endtime1;
unsigned long nowtimeNext1;
unsigned long nowtime2;
unsigned long endtime2;
unsigned long nowtimeNext2;
unsigned long nowtime3;
unsigned long endtime3;
unsigned long nowtimeNext3;


void setup()
{
  delay(2000); 
  Wire.begin();
  RTC.begin();
  Serial.begin(9600);
  
  // declare relay as output
  for(int i = 0; i < 4; i++) {
    pinMode(relays[i], OUTPUT);
  } 
   
  // declare pump as output
  pinMode(pump, OUTPUT);
}

void loop()
{
  //Получаем знач. из RTC в объект
  today = RTC.now();

  //Выводим в консоль
  Serial.print(today.hour(), DEC);
  Serial.print(':');
  Serial.println(today.minute(), DEC);
  
  //Если пришло время полива - включаем помпу
  if(today.hour() == water_hours && today.minute() == water_minutes)
      water_flower();
      
  u8g.firstPage();
  do
  {
    drawtime();  
  } while ( u8g.nextPage() );

  delay(100);
}

void water_flower()
{
    Serial.print("Started watering");
    for(int i = 0; i < 2; i++) {
      
      digitalWrite(relays[i], HIGH);
      delay(50);
      digitalWrite(pump, HIGH);
      delay(20000);

      digitalWrite(pump, LOW);
      delay(50);
      digitalWrite(relays[i], LOW);  
        
     } 
    Serial.print("Finished watering");
    //wait till 1 minute reaches
    delay(60000);
}

void drawtime(void)
{
  int x = 5;
  float i = 25.00;
  float j = 54;
  DateTime now = RTC.now();
  //Serial.print(now.year(), DEC);
  if (! RTC.isrunning())
  {
    u8g.setFont(u8g_font_6x10);
    u8g.setPrintPos(5, 20);
    u8g.print("RTC is NOT running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  else
  {
    u8g.setFont(u8g_font_7x13);
    u8g.setPrintPos(x, 11);
    u8g.print(now.year(), DEC);
    u8g.setPrintPos(x + 80, 11);
    u8g.print(daysOfTheWeek[now.dayOfTheWeek()]);
    u8g.setPrintPos(x + 28, 11);
    u8g.print("/");
    u8g.setPrintPos(x + 33, 11);
    u8g.print(now.month(), DEC);
    if (now.month() < 10)
      x -= 7;
    u8g.setPrintPos(x + 47, 11);
    u8g.print("/");
    u8g.setPrintPos(x + 53, 11);
    u8g.print(now.day(), DEC);
    u8g.setFont(u8g_font_8x13);
    int x = 35;
    u8g.setPrintPos(x, 33);
    u8g.print(now.hour(), DEC);
    if (now.hour() < 10)
      x -= 7;
    u8g.setPrintPos(x + 15, 33);
    u8g.print(":");
    u8g.setPrintPos(x + 21, 33);
    u8g.print(now.minute(), DEC);
    if (now.minute() < 10)
      x -= 7;
    u8g.setPrintPos(x + 36, 33);
    u8g.print(":");
    u8g.setPrintPos(x + 42, 33);
    u8g.print(now.second(), DEC);
  }
}
