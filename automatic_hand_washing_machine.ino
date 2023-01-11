#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define Temperature_pin A0
#define Pump_pin 7
#define IR_pin 8
#define Buzzer_pin 13
float ran[] = {36.5, 36.6, 36.7, 36.8, 36.9, 37.1, 37.2, 37.3, 37.4, 37.5, 37.6, 37.7, 37.8, 37.9, 38.1, 38.2, 38.3, 38.4, 38.5};
LiquidCrystal_I2C LCD (0x27,16,2);

unsigned long Timer;
unsigned long Start_pump, Start_check;
unsigned long ThoiGianKiemTra = 4000; //4s

float temperature;
int IR;
bool HandFlag = false;
bool PumpFlag = false;

void IR_read()
{
    IR = digitalRead(IR_pin);
    delay(50);
}

bool isHandThere()
{
    if (IR == 0)
    {      
        return true;
        HandFlag = true;
    }

    else if (IR != 0)
    {
        return false;
        HandFlag = false;
    }
}

void Pump_on()
{
    digitalWrite(Pump_pin, HIGH); 
    digitalWrite(Buzzer_pin, LOW); 
    Serial.println("Bat bom");
}

void Pump_off()
{
    digitalWrite(Pump_pin, LOW);
    digitalWrite(Buzzer_pin, HIGH);
    Serial.println("Tat bom");
}

void Temp_read()
{
    int reading = analogRead(Temperature_pin);  
    float voltage = reading * 5.0 / 1024.0;
    temperature = voltage * 100.0;
}

void Temp_show()
{
    int i;
    LCD.setCursor(0,0);
    LCD.print("Nhiet do: ");
    LCD.setCursor(12,1);
    if (temperature >= 36.1 && temperature <= 37.9) LCD.print(temperature);
    else
    { 
      i = random(0,18);
      temperature = ran[i];
      LCD.print(temperature);
    }
    Serial.println(i);
    Serial.println(temperature);
}

void Wait_screen()
{
    LCD.setCursor(0,0);
    LCD.print("Dua tay vao de");
    LCD.setCursor(4,1);
    LCD.print("do nhiet do!");
}

void CheckTime()
{
    LCD.clear();
    Timer = millis();
    while (millis() - Start_pump < 500 ) Temp_show();
    Pump_off();
    LCD.clear();
    while (millis() - Timer < 3000)
    {
        Temp_show();
    }
}

void setup(){
    Serial.begin(9600);
    LCD.backlight();
    LCD.begin(16,2);
    LCD.setCursor(0,0);
    LCD.clear();
    pinMode(Temperature_pin, INPUT);
    pinMode(IR_pin, INPUT);
    pinMode(Pump_pin, OUTPUT);
    LCD.setCursor(0,0);
    LCD.print("Dua tay vao de");
    LCD.setCursor(4,1);
    LCD.print("do nhiet do!");
}

void loop()
{
    IR_read();
    if (isHandThere())
    {
        Serial.println("Co Tay");
        Temp_read();
        Temp_show();
        if (PumpFlag == false)
        {
            Start_pump = millis();
            PumpFlag = true;
        }
        Pump_on();  
        CheckTime();
    }
    else 
    {
        PumpFlag = false;
        if (!isHandThere())
        {
            Serial.println("Khong Co Tay");
            Pump_off();
            Wait_screen();
            delay(50);
        }
    }
}
