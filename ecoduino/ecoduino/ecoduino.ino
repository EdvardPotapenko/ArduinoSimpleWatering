#include <AutoWatering.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>





//Объекты таймера, автополива и ЖК
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x20, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
AutoWatering flower;


//Таймер часов

DateTime today;

// Время полива
int water_hours = 15;
int water_minutes = 0;

void setup()
{
  //инициализируем RTC
  rtc.begin();
  
  // Если он уже не установлен - ставим дату и время
  if ( !rtc.isrunning()) {
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  //Инициализируем ЖК дисплей
  lcd.init();
  lcd.backlight();
  //Ставим скорость передачи сириал порта
  Serial.begin(9600); 


  //Иниц. полив растений.
  flower.Initialization();  

  //Включаем пин для звука
  pinMode(9,OUTPUT);
  digitalWrite(9,LOW);
  today = rtc.now();
  Update_Time(today);
  delay(60000);
}


void loop()
{
  //Таймер для обновления часов на дисплее

  //Получаем знач. из RTC в объект
  today = rtc.now();

  //Если прошла одна минута - обновить ЖК

  //Выводим в консоль
  Serial.print(today.hour(), DEC);
  Serial.print(':');
  Serial.println(today.minute(), DEC);
  

  //Обновляем ЖК
  Update_Time(today);
  
  //Если пришло время полива - включаем помпу
  if(today.hour() == water_hours && today.minute() == water_minutes)
    {
      timerOn();
      digitalWrite(9,HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Watering...");
      delay(15000);
      Update_Time(today);
      digitalWrite(9,LOW);
      flower.pumpOff();
      Serial.println("end of procedure");
      Serial.println();
    }
    delay(60000);
}


unsigned int wateredTimes = 0;

//Включение помпы
void timerOn()
{
  tone(10,10,1000);
  digitalWrite(6,HIGH);
  digitalWrite(5,HIGH);
  Serial.println("Pump is timed on!");
  wateredTimes++;
}

//Функция обновления ЖК
void Update_Time(DateTime today_lcd)
{

  lcd.clear();
  lcd.home();

  lcd.print("N");
  
  if(today_lcd.hour()< 10)
  {
  lcd.print('0');
  lcd.print(today_lcd.hour(), DEC);
  }
  else
  {
  lcd.print(today_lcd.hour(), DEC);
  }
  lcd.print(':');
  
  if(today_lcd.minute()< 10)
  {
  lcd.print('0');
  lcd.print(today_lcd.minute(), DEC);
  }
  else{
  lcd.print(today_lcd.minute(), DEC);
  }

  write_Watered();
  write_Schedule();
}



void write_Schedule()
{
  lcd.setCursor(7,0);
  lcd.print("S");
  lcd.setCursor(8,0);
  
  if(water_hours< 10)
  {
  lcd.print('0');
  lcd.print(water_hours);
  }
  else
  {
  lcd.print(water_hours);
  }
  lcd.print(':');
  
  if(water_minutes< 10)
  {
  lcd.print('0');
  lcd.print(water_minutes);
  }
  else{
  lcd.print(water_minutes);
  }
}

void write_Watered()
{
  lcd.setCursor(0,1);
  lcd.print("Wat.Times:");
  lcd.print(wateredTimes);
}
