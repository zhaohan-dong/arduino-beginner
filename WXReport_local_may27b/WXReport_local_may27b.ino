#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFi.h>
#include <Arduino_MKRENV.h>
#include <RTCZero.h>
#include <SD.h>
#include "arduino_secrets.h" 

double fieldElev = 30; // field elevation in ft
float temperature;
float humidity;
float dewPoint;
float pressure;
float illuminance;
float uVIndex;
float uva;
float uvb;
float qnh_raw;
float alt;
float prevMinutePressure[] = {0, 0};
float pressDiff;
float totalPRise;
float totalPFall;
boolean PRESRR = false;
boolean PRESFR = false;
String qNH;
int year;
int month;
int day;
int hour;
int minute;
RTCZero rtc;
unsigned long epoch;
File myFile;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // Initialize real time from internet
  rtc.begin();
  int numberOfTries = 0, maxTries = 10;
  do {
    delay(4000);
    epoch = WiFi.getTime();
    numberOfTries++;
  }
  while ((epoch == 0) && (numberOfTries < maxTries));
  if (numberOfTries == maxTries) {
    Serial.println("NTP unreachable!!");
    while (1);
  } else {
    rtc.setEpoch(epoch);
    Serial.print("Epoch received: ");
    Serial.println(epoch);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();

  SPI.begin();
  SD.begin(4);
  delay(100);
  myFile = SD.open("log.csv");
  if (!myFile) {
    myFile = SD.open("log.csv", FILE_WRITE);
    myFile.println("timestamp, elev(ft), temperature, dew_point, qnh, humidity, pressure, illuminance, UVA, UVB, UVindex");
  }
  myFile.close();
  delay(100);
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  year = rtc.getYear();
  month = rtc.getMonth();
  day = rtc.getDay();
  hour = rtc.getHours();
  minute = rtc.getMinutes();
  if(rtc.getSeconds() < 2){
    temperature = ENV.readTemperature();
    humidity = ENV.readHumidity();
    dewPoint = dPoint(temperature, humidity);
    pressure = ENV.readPressure(MILLIBAR);
    illuminance = ENV.readIlluminance();
    uVIndex = ENV.readUVIndex();
    uva = ENV.readUVA();
    uvb = ENV.readUVB();
    qnh_raw = (pressure - 0.3) * pow(1 + 0.00002567294033 * fieldElev / pow(pressure - 0.3, 0.190284), 1 / 0.190284);
    alt = 2.95300 * qnh_raw;
    if (qnh_raw < 1000) {
      qNH = "Q0" + String(round(qnh_raw)) + "/A" + String(round(alt));
    } else {
      qNH = "Q" + String(round(qnh_raw)) + "/A" + String(round(alt)); 
    }
    
    myFile = SD.open("log.csv", FILE_WRITE);
    if(myFile) {
      myFile.print(year);
      myFile.print("-");
      if(month < 10) {
        myFile.print("0");
      }
      myFile.print(month);
      myFile.print("-");
      if(day < 10) {
        myFile.print("0");
      }
      myFile.print(day);
      myFile.print("-");
      if(hour < 10) {
        myFile.print("0");
      }
      myFile.print(hour);
      if(minute < 10) {
        myFile.print("0");
      }
      myFile.print(minute);
      myFile.print("00Z,");
      myFile.print(fieldElev);
      myFile.print(",");
      myFile.print(temperature);
      myFile.print(",");
      myFile.print(dewPoint);
      myFile.print(",");
      myFile.print(qNH);
      myFile.print(",");
      myFile.print(humidity);
      myFile.print(",");
      myFile.print(pressure);
      myFile.print(",");
      myFile.print(illuminance);
      myFile.print(",");
      myFile.print(uva);
      myFile.print(",");
      myFile.print(uvb);
      myFile.print(",");
      myFile.println(uVIndex);
      myFile.close();
    }
  }
  
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 15");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("ZZZZ ");
          if(day<10) {
            client.print("0");
          }
          client.print(day);
          if(hour<10) {
            client.print("0");
          }
          client.print(hour);
          if(minute<10) {
            client.print("0");
          }
          client.print(minute);
          client.print("Z ");
          client.print(round(temperature));
          client.print("/");
          client.print(round(dewPoint));
          client.print(" ");
          client.print(qNH);
          client.println("<br />");
          client.print("RMK STNPRES");
          client.print(pressure);
          client.print(" ILLUM");
          client.print(illuminance);
          client.println("<br />");
          client.print(" UVA");
          client.print(uva);
          client.print(" UVB");
          client.print(uvb);
          client.print(" UVI");
          client.print(uVIndex);
          client.println("<br />");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

float dPoint(float temperature, float relativeHumidity) {
  //float a = 6.112;
  float b = 17.67;
  float c = 243.5;
  float g = log(relativeHumidity / 100) + (b * temperature / (c + temperature));
  return c * g / (b - g);
  }
