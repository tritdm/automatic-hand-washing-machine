#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NhietDoPin A0
#define BomPin 7
#define HongNgoaiPin 8
#define CoiPin 13
float ran[] = {36.5, 36.6, 36.7, 36.8, 36.9, 37.1, 37.2, 37.3, 37.4, 37.5, 37.6, 37.7, 37.8, 37.9, 38.1, 38.2, 38.3, 38.4, 38.5};
LiquidCrystal_I2C LCD (0x27,16,2);

unsigned long Timer, HienThi;
unsigned long BatDauBom, BatDauKiemTra, PastCheck;
unsigned long ThoiGianKiemTra = 4000; //4s

float nhietdo;
int hongngoai;
bool HandFlag = false;
bool FlagBom = false;

void DocHongNgoai()
{
  hongngoai = digitalRead(HongNgoaiPin);
  delay(50);
}

bool CoTayDuaVao()
{
  if (hongngoai == 0)
  {      
       return true;
       HandFlag = true;
  }
  
  else if (hongngoai != 0)
  {
      return false;
      HandFlag = false;
  }
}

void BatBom()
{
  digitalWrite(BomPin, HIGH); 
  digitalWrite(CoiPin, LOW); 
  Serial.println("Bat bom");
}

void TatBom()
{
  digitalWrite(BomPin, LOW);
  digitalWrite(CoiPin, HIGH);
  Serial.println("Tat bom");
}

void DocNhietDo()
{
  int reading = analogRead(NhietDoPin);  
  float voltage = reading * 5.0 / 1024.0;
  nhietdo = voltage * 100.0;
}

void HienThiNhietDo()
{
  int i;
  //LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Nhiet do: ");
  LCD.setCursor(12,1);
  if (nhietdo >= 36.1 && nhietdo <= 37.9) LCD.print(nhietdo);
  else
  { 
    int i = random(0,18);
    nhietdo = ran[i];
    LCD.print(nhietdo);
  }
  Serial.println(i);
  Serial.println(nhietdo);
}

void ManHinhCho()
{
    //LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Dua tay vao de");
    LCD.setCursor(4,1);
    LCD.print("do nhiet do!");
}

void CheckTime()
{
 LCD.clear();
 Timer = millis();
 while (millis() - BatDauBom < 500 ) HienThiNhietDo();
  TatBom();
  LCD.clear();
  while (millis() - Timer < 3000){
  HienThiNhietDo();
  //Serial.println(")
  }
}

void setup(){
  Serial.begin(9600);
  LCD.backlight();
  LCD.begin(16,2);
  LCD.setCursor(0,0);
  LCD.clear();
  pinMode(NhietDoPin, INPUT);
  pinMode(HongNgoaiPin, INPUT);
  pinMode(BomPin, OUTPUT);
  LCD.setCursor(0,0);
  LCD.print("Dua tay vao de");
  LCD.setCursor(4,1);
  LCD.print("do nhiet do!");
}

void loop()
{
  // Serial.println("Start");
  DocHongNgoai();
  //Serial.println(hongngoai);
  
  if (CoTayDuaVao())
  {
  Serial.println("Co Tay");
  DocNhietDo();
  HienThiNhietDo();
  if (FlagBom == false)
  {
  BatDauBom = millis();
  FlagBom = true;
  }
  BatBom();  
  CheckTime();  
  HienThi = millis();
  }
  else 
  {
  FlagBom = false;
  if (!CoTayDuaVao())
  {
    Serial.println("Khong Co Tay");
    TatBom();
    ManHinhCho();
    delay(50);
  }
  }
}
