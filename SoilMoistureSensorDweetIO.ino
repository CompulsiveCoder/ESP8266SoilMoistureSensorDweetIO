#include <ESP8266WiFi.h>

#define moisturePin A0
#define sensorPowerPin 4 // This is marked D2 on the ESP8266 development board
#define ledPin 16

const char* ssid = "-SSID-";
const char* password = "-PASSWORD-";

String dweetId = "28157f6e-9118-4a44-aa4d-05ffe3c18286";

long second = 1000;
long minute = second * 60;
long hour = minute * 60;

long publishInterval = 5 * second;
//long publishInterval = 30 * minute;
//long publishInterval = 12 * hour;

int delayAfterTurningSensorOn = 3000; // TODO: Check if this can be reduced without affecting the readings

long lastPublishTime = 0;

int currentReading = 0;


void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("Starting ESP8266 soil moisture sensor (posting to dweet.io)");

  pinMode(sensorPowerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(sensorPowerPin, LOW);

  setupWiFi();

  Serial.println();  
  String url = "https://dweet.io/get/dweets/for/" + dweetId;
  Serial.println("View data at:");
  Serial.println(url);  
  Serial.println();  
  
  // If the interval is short then leave the sensor on all the time
  if (publishInterval < delayAfterTurningSensorOn)
  {
    sensorOn();
  }
}

void setupWiFi()
{
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Trying to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (lastPublishTime + publishInterval < millis()
      || lastPublishTime == 0)
  {
    Serial.println();
    
    takeReading();

    publishReading();

    lastPublishTime = millis();

    Serial.println();
  }

  delay(10);
}


void takeReading()
{
  //Serial.println("Taking reading");

  sensorOn();

  int reading = analogRead(moisturePin);

  Serial.print("Sensor reading: ");
  Serial.println(reading);
  
  currentReading = reading;

  sensorOff();

  Serial.println();
}

void publishReading()
{
  Serial.println();
  Serial.println("Publishing reading");
  
  digitalWrite(ledPin, HIGH);

  char host[] = "dweet.io";
  int port = 80;
  String path = "/dweet/for/" + dweetId + "?moistureSensorReading=";
  path += currentReading;
  
  // Attempt to connect to website
  if ( !getRequest(host, port, path) )
    Serial.println("GET request failed");
  else
  {
    Serial.println();
    Serial.println("Finished publishing.");
  }
  
  digitalWrite(ledPin, LOW);
}

void sensorOn()
{
  if (publishInterval > delayAfterTurningSensorOn)
  {
    Serial.println("Turning sensor on");

    digitalWrite(sensorPowerPin, HIGH);

    // Wait for the sensor to settle down
    delay(delayAfterTurningSensorOn);
  }
}

void sensorOff()
{
  if (publishInterval > delayAfterTurningSensorOn)
  {
    Serial.println("Turning sensor off");

    digitalWrite(sensorPowerPin, LOW);
  }
}

bool getRequest(char host[], int port, String path)
{
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return false;
  }

  // We now create a URI for the request
  String url = path;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  return true;
}
