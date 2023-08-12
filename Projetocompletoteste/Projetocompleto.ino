#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SD.h> // Biblioteca para o cartão SD

// Definições do sensor DHT11
#define DHT_PIN 2
#define DHT_TYPE DHT11

// Definições do display LCD
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Definições da bomba de irrigação
#define PUMP_PIN 8
#define PUMP_INTERVAL 300000  // Intervalo de salvamento ajustado para 5 minutos
#define PUMP_DURATION 5000

unsigned long previousSaveTime = 0; // Variável para rastrear o tempo da última operação de salvamento


// Definições do cartão SD
const int chipSelect = 10; // Pino CS do cartão SD

//Leds

int L1 = 9;
int L2 = 7;
int L3 = 6;
int L4 = 5;
int L5 = 4;
int L6 = 3;  //6 LED pin
int buttonPin = 0;  //the number of the pushbutton pin

int de=50;  // delay time

int p=0;    // variable for pattem

int buttonState = 0;         // variable for reading the pushbutton status



// Criação dos objetos DHT, LiquidCrystal_I2C e File
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
File dataFile; // Objeto para manipular o arquivo de dados

String obterDataHora() {
  // Obtém a data e hora atual do sistema
  unsigned long currentTime = millis();
  unsigned long seconds = currentTime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;

  // Calcula o dia, mês, ano, hora, minuto e segundo
  int year = 2023;
  int month = 6;
  int day = 20;
  int hour = hours % 24;
  int minute = minutes % 60;
  int second = seconds % 60;

  // Formata a data e hora como uma string
  String formattedDateTime = String(day) + "/" + String(month) + "/" + String(year) + " " + String(hour) + ":" + String(minute) + ":" + String(second);
  return formattedDateTime;
}

void salvarDadosSD(float temperature, float humidity) {
  String dataHora = obterDataHora();
  String dataString = dataHora + "," + String(temperature) + "," + String(humidity);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.flush();
    Serial.println("Dados salvos no cartão SD: " + dataString);
  } else {
    Serial.println("Erro ao salvar os dados no cartão SD.");
  }
}
void setup() {
  Wire.begin(); // Inicializa a comunicação I2C
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.backlight(); // Habilita a iluminação de fundo do display
  dht.begin(); // Inicializa o sensor DHT11
  pinMode(PUMP_PIN, OUTPUT); // Configura o pino da bomba de irrigação como saída

  lcd.setCursor(0, 0);
  lcd.print("TEMP:");
  lcd.setCursor(0, 1);
  lcd.print("HUMID:");

  // Inicializa o cartão SD
  if (SD.begin(chipSelect)) {
    Serial.println("Cartão SD inicializado com sucesso.");
  } else {
    Serial.println("Falha ao inicializar o cartão SD.");
  }

  // Abre o arquivo para escrita
  dataFile = SD.open("dados.txt", FILE_WRITE);
  if (dataFile) {
    Serial.println("Arquivo aberto para escrita.");
  } else {
    Serial.println("Erro ao abrir o arquivo para escrita.");
  }

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(L5, OUTPUT);
  pinMode(L6, OUTPUT);
  pinMode(buttonPin, INPUT);

}



void loop() {
  // Leitura da temperatura e umidade
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Verifica se a leitura é válida
  if (!isnan(temperature)) {
    // Atualiza as informações no display LCD
    lcd.setCursor(7, 0);
    lcd.print(temperature, 1);
    lcd.print("C");
  }

  lcd.setCursor(10, 1);
  lcd.print(humidity, 1);
  lcd.print(" %");


  // Verifica se é hora de acionar a bomba de irrigação
  unsigned long currentMillis = millis();
  static unsigned long previousPumpTime = 0;
  static bool pumpActive = false;


  if (currentMillis - previousPumpTime >= PUMP_INTERVAL) {
    previousPumpTime = currentMillis;
    pumpActive = true;
    digitalWrite(PUMP_PIN, HIGH); // Ativa a bomba de irrigação
    lcd.setCursor(0, 1);
    lcd.print("*REGA");
  }

// Verifica se é hora de salvar os dados no cartão SD
  if (currentMillis - previousSaveTime >= PUMP_INTERVAL) {
    previousSaveTime = currentMillis;
    salvarDadosSD(temperature, humidity);
  }

    // Salva os dados no cartão SD
  salvarDadosSD(temperature, humidity);

  // Desativa a bomba de irrigação após a duração desejada
  if (pumpActive && currentMillis - previousPumpTime >= PUMP_DURATION) {
    pumpActive = false;
    digitalWrite(PUMP_PIN, LOW); // Desativa a bomba de irrigação
    lcd.setCursor(0, 1);
    lcd.print("HUMID:");
  }

  {
 buttonState = digitalRead(buttonPin);

 if (buttonState == HIGH)
 
    {
      p++;
      delay(2000);
    } 
 
  if(p==1)
  {
 digitalWrite(L1,1); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de);  
  
 digitalWrite(L1,0); 
 digitalWrite(L2,1); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 
  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,1); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,1); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,1); 
 digitalWrite(L6,0); 
 
  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,1); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  }
  
  
  
  
  if(p==2)
  {
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 
 
  delay(de); 
  
  digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,1); 
 
  delay(de);
  
  digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,1); 
 digitalWrite(L6,0); 

  delay(de); 
  
  digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,1); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de);  
 
  digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,1); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 
 
  delay(de);  
 
 
  digitalWrite(L1,0); 
 digitalWrite(L2,1); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 
 
  delay(de); 
  
   digitalWrite(L1,1); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de);  
  
  }
  
    if(p==3)
  {
 digitalWrite(L1,1); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de);  
  
 digitalWrite(L1,0); 
 digitalWrite(L2,1); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,1); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,1); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,1); 
 digitalWrite(L6,0); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,1); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  
   
  digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,1); 

  delay(de);
  
  digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,1); 
 digitalWrite(L6,0); 

  delay(de); 
  
  digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,1); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de);  
 
  digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,1); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de);  
 
 
  digitalWrite(L1,0); 
 digitalWrite(L2,1); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  }
  
  
  
  
    if(p==4)
  {
 digitalWrite(L1,1); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de);  
  
 digitalWrite(L1,0); 
 digitalWrite(L2,1); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,1); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,1); 
 digitalWrite(L4,0); 
 digitalWrite(L5,1); 
 digitalWrite(L6,0); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,1); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
 
  }
  
  
      if(p==5)
  {
 

 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,1); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de); 
  
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,1); 
 digitalWrite(L4,0); 
 digitalWrite(L5,1); 
 digitalWrite(L6,0); 

  delay(de); 
  
  
  
 digitalWrite(L1,0); 
 digitalWrite(L2,1); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,1); 

  delay(de); 
    
 digitalWrite(L1,1); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  delay(de);  
  }
 
 if(p==6)
{
 digitalWrite(L1,1); 
 delay(de); 
 digitalWrite(L2,1); 
 delay(de); 
 digitalWrite(L3,1);
 delay(de);  
 digitalWrite(L4,1);
 delay(de);  
 digitalWrite(L5,1);
 delay(de);  
 digitalWrite(L6,1);
 

 delay(de); 
 digitalWrite(L6,0);
 delay(de); 
 digitalWrite(L5,0);
 delay(de); 
 digitalWrite(L4,0);
 delay(de); 
 digitalWrite(L3,0);
 delay(de); 
 digitalWrite(L2,0);
 delay(de); 
 digitalWrite(L1,0);
 delay(de); 
 

}  
 
 if(p==7)
{
 digitalWrite(L1,0); 
 digitalWrite(L2,0); 
 digitalWrite(L3,0); 
 digitalWrite(L4,0); 
 digitalWrite(L5,0); 
 digitalWrite(L6,0); 

  p=0;
  }


  
  delay(300); // Pequena pausa entre as iterações para melhorar a responsividade
}
}
