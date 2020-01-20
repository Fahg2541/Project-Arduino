#include <ESP8266WiFi.h>              // Include main library
#include <BH1750FVI.h>
#include <Wire.h>
#include <BH1750.h>

const char* ssid     = "fah";    // Set your SSID
const char* password = "0987654321";  // Set your password
const char* thing = "vr_Saowarat_01";   // Set Thing name
const char* host = "dweet.io";        // Set host 
int count = 0;
int sensorValue = 0;

BH1750 lightMeter;
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes); // สร้างออปเจกเซนเซอร์ BH1750

const int led1 = D0;
const int pingPin = 13;
int inPin = 12;
int pinTone = D8;

void setup() 
{
  Serial.begin(9600);               // Print setting message
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  pinMode(led1, OUTPUT);
  digitalWrite(led1,LOW);
  Serial.println("ArduinoAll TEST BH1750");
  LightSensor.begin(); // สั่งให้เซนเซอร์ เริ่มทำางน
  analogWrite(pinTone,255);
  delay(100);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");                // Print WiFi status
  }
  Serial.println("");
  Serial.println("WiFi connected");   // Print connect status
  Serial.println("IP address: ");     
  Serial.println(WiFi.localIP());     // Print IP address

  Wire.begin();
  lightMeter.begin();
  
}
void loop() 
{
  //sensorValue = analogRead(A0);       // Read analog input
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");
  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(inPin, INPUT);
  duration = pulseIn(inPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
//  delay(500); 
  delay(1000);
  ++count;                            // Count value
  WiFiClient client;
  const int httpPort = 80;            // Set HTTP port
  if (!client.connect(host, httpPort))// Test connection 
  {
    Serial.println("connection failed"); // Print connection fail message
    return;
  }

if (lux > 10){
  digitalWrite(led1,LOW);
  if(cm <= 50){
    analogWrite(pinTone,255);
    delay(100);
    analogWrite(pinTone,0);
    delay(100);
  }else{
    analogWrite(pinTone,0);
    delay(100);
  }
}else{
  digitalWrite(led1,HIGH);
  if(cm <= 60){
    analogWrite(pinTone,255);
    delay(100);
    analogWrite(pinTone,0);
    delay(100);
  }else{
    analogWrite(pinTone,0);
    delay(100);
  }
}

//if(cm <= 35 && lux < 10){
//  digitalWrite(led1,HIGH);
//    analogWrite(pinTone,25);
//    delay(100);
//    analogWrite(pinTone,0);
//    delay(100);
//}else if(cm >= 36  && cm <= 250 && lux < 10) {
//   digitalWrite(led1,HIGH);
//   analogWrite(pinTone,25);
//    delay(100);
//    analogWrite(pinTone,0);
//    delay(100);
//}else{
//    analogWrite(pinTone,0);
//    digitalWrite(led1,LOW);
//  }
 
delay(100);
  
  String url =  String("/dweet/for/") + thing + "?"; // Set message
  url += "count=";
  url += count;
  url += "&light=";
  url += lux;
  url += "&centimet=";
  url += cm;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: keep-alive\r\n" +
               "Cache-Control: max-age=0\r\n\r\n");
  Serial.println("URL : ");         
  Serial.println(url);              // Print URL 
  delay(100);
  
  while (client.available()) 
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection"); // Print closing status
}
long microsecondsToCentimeters(long microseconds)
{

return microseconds / 29 / 2;
}

