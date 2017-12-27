//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

double dewPoint(double celsius, double humidity)
{
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

        // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

        // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP/0.61078);   // temp var
  return (241.88 * T) / (17.558 - T);
}

#include <dht.h>

#define DHT11_PIN 7
#define GREEN LED_BUILTIN
#define YELLOW 12
#define RED 11

dht DHT;
double lastTemp;
double allTimeHighTemperature;
double allTimeLowTemperature;
 
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  
  pinMode(GREEN, OUTPUT); // green
  pinMode(YELLOW, OUTPUT); // yellow
  pinMode(RED, OUTPUT); // red
  
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, HIGH);
  delay(500);
  
  lastTemp = 999;
  allTimeHighTemperature = -50;
  allTimeLowTemperature = 999;
}

void clearLeds() {
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  DHT.read11(DHT11_PIN);
  
  double t = DHT.temperature;
  double h = DHT.humidity;
  double d = dewPoint(t, h);

  if (lastTemp != 999) {
    if (lastTemp == t) {
      clearLeds();
      digitalWrite(YELLOW, HIGH);
      Serial.println("Status: YELLOW");
    } else if (t < lastTemp) {
      clearLeds();
      digitalWrite(GREEN, HIGH);
      Serial.println("Status: GREEN");
    } else if (t > lastTemp) {
      clearLeds();
      digitalWrite(RED, HIGH);
      Serial.println("Status: RED");
    } 
  }

  lastTemp = t;

  if (lastTemp > allTimeHighTemperature) {
    allTimeHighTemperature = lastTemp;
  }

  if (lastTemp < allTimeLowTemperature) {
    allTimeLowTemperature = lastTemp;
  }
  
  Serial.print("Temperature: ");
  Serial.print(Fahrenheit(t));
  Serial.println("°");
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");
  
  Serial.print("Dew Point: ");
  Serial.print(d);
  Serial.println("°");

  Serial.print("All time low: ");
  Serial.println(Fahrenheit(allTimeLowTemperature));

  Serial.print("All time high: ");
  Serial.println(Fahrenheit(allTimeHighTemperature));
  
  Serial.println();
  delay(1000);
}
