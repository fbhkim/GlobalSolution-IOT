#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define vaga1_red 13
#define vaga1_green 10
#define vaga2_red 9
#define vaga2_green 8
#define echo1 15
#define trigger1 14
#define echo2 17
#define trigger2 16
#define tmp A4
#define sprinkles 6
#define alarme 7
#define sensor_gas A5

float distancia1 = 0;
float distancia2 = 0;
float tempo1;
float tempo2;
float velocidade = 0.172316;
int vagas_ocupadas = 0;
int vagas_livres = 2;
int ultrasonic1 = 0;
int ultrasonic2 = 0;
int temperatura = 0;
int gas_valor = 0;
int i;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(vaga1_red, OUTPUT);
  pinMode(vaga1_green, OUTPUT);
  pinMode(vaga2_red, OUTPUT);
  pinMode(vaga2_green, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger1, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(tmp, INPUT);
  pinMode(sprinkles, OUTPUT);
  pinMode(sensor_gas, INPUT);
}

void loop()
{
  digitalWrite(trigger1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);
  tempo1 = pulseIn(echo1, HIGH);
  distancia1 = tempo1 * velocidade;
  delay(500);
  Serial.print("Distancia 1: ");
  Serial.println(distancia1);
  digitalWrite(trigger2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);
  tempo2 = pulseIn(echo2, HIGH);
  distancia2 = tempo2 * velocidade;
  delay(500);
  Serial.print("Distancia 2: ");
  Serial.println(distancia2);
  vagas_ocupadas = ultrasonic1 + ultrasonic2;  
  vagas_livres = 2 - vagas_ocupadas;  
  temperatura = map(analogRead(tmp), 20, 358, -40, 125);
  gas_valor = map(analogRead(sensor_gas), 487, 880, 0, 100);
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  Serial.print("Sensor gas: ");
  Serial.println(gas_valor);
  if(temperatura >= 48 || gas_valor >= 30)
   {
    lcd.clear();
    lcd.setCursor(0,0);  
    lcd.print("Entrada proibida");
    lcd.setCursor(3,1);  
    lcd.print("EMERGENCIA");  
    digitalWrite(sprinkles, HIGH);
    digitalWrite(vaga1_red, HIGH);
    delay(100);
    digitalWrite(vaga1_red, LOW);
    digitalWrite(vaga1_green, LOW);
    ultrasonic1 = 1;
    digitalWrite(vaga2_red, HIGH);
    delay(100);
    digitalWrite(vaga2_red, LOW);
    digitalWrite(vaga2_green, LOW);
    ultrasonic2 = 1;
    for(i = 2000; i < 3000; i = i + 100)
    {
      tone(alarme, i);
      delay(50);
    }
    for(i = 3000; i > 2000; i = i - 100)
    {
      tone(alarme, i);
      delay(50);
    }
  }
  else
    {
    if(vagas_livres >= 1)
    {
    lcd.clear();
    lcd.setCursor(3,0);  
    lcd.print("BEM-VINDO!");  
    delay(1000);
    lcd.setCursor(0,0);  
    lcd.print("Vagas livres = ");  
    lcd.print(vagas_livres);  
    lcd.setCursor(0,1);  
    lcd.print("Vagas ocup. = ");  
    lcd.print(vagas_ocupadas);
    delay(1000);
    noTone(alarme);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(3,0);  
      lcd.print("SEM VAGAS");
      noTone(alarme);
    }
    if(distancia1 >= 2.4 && distancia1 <= 3310.0)
      {
      digitalWrite(vaga1_red, HIGH);
      digitalWrite(vaga1_green, LOW);
      ultrasonic1 = 1;
      noTone(alarme);
      }
    else
    {
      digitalWrite(vaga1_red, LOW);
      digitalWrite(vaga1_green, HIGH);
      ultrasonic1 = 0;
      noTone(alarme);
    }
    if(distancia2 >= 2.4 && distancia2 <= 3310.0)
    {
      digitalWrite(vaga2_red, HIGH);
      digitalWrite(vaga2_green, LOW);
      ultrasonic2 = 1;
      noTone(alarme);
    }
    else
    {
      digitalWrite(vaga2_red, LOW);
      digitalWrite(vaga2_green, HIGH);
      ultrasonic2 = 0;
      noTone(alarme);
    }
  }
}