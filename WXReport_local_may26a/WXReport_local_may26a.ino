#include "arduino_secrets.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFi.h>
#include <Arduino_MKRENV.h>
#include <RTCZero.h>

const char ssid[]     = SECRET_SSID;    // Network SSID (name)
const char pass[]     = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

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
float prevMinutePressure[] = {NULL, NULL};
float pressDiff;
float totalPRise;
float totalPFall;
boolean PRESRR = true;
boolean PRESFR = false;
String qNH;
WiFiServer server(80);
RTCZero rtc;
unsigned long epoch;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while(!Serial);
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }
  // Connect to WiFi
  while(WiFi.begin(ssid, pass) !=  WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    delay(5000);
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
  // Start server
  server.begin();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Sensor read out
  temperature = float(ENV.readTemperature());
  humidity = float(ENV.readHumidity());
  dewPoint = dPoint(temperature, humidity);
  pressure = float(ENV.readPressure(MILLIBAR));
  // check if pressure is rising or falling rapidly
  if(prevMinutePressure[0] != NULL && prevMinutePressure[1] != rtc.getMinutes() && rtc.getSeconds() < 2) {
    pressDiff = pressure - prevMinutePressure[0];
    prevMinutePressure[0] = pressure;
    prevMinutePressure[1] = rtc.getMinutes();
    if(pressDiff >= 0.03386) { // if pressure rise rapidly at this minute
      PRESFR = false;
      totalPFall = 0;
      totalPRise = totalPrise + pressDiff;
      if(totalPRise >= 0.6772) {
        // if pressure rised rapidly for more than 0.6772hPa
        PRESRR = true;
      } else {
        // if pressure hasn't rised much, continue to count total amount
        PRESRR = false;
      }
    } else if(pressDiff <= -0.03386) { // if pressure fall rapidly at this minute
      PRESRR = false;
      totalPRise = 0;
      totalPFall = totalPrise + pressDiff;
      if(totalPFall <= -0.6772) {
        // if pressure fell rapidly for more than 0.6772hPa
        PRESFR = true;
      } else {
        // if pressure hasn't fallen much, continue count total amount
        PRESFR = false;
      }
    } else { // if pressure difference doesn't meet criteria
      PRESRR = false;
      PRESFR = false;
      totalPRise = 0;
      totalPFall = 0;
    }
  }
    Serial.println("2");
  illuminance = float(ENV.readIlluminance());
  uVIndex = float(ENV.readUVIndex());
  uva = float(ENV.readUVA());
  uvb = float(ENV.readUVB());
  qnh_raw = (pressure - 0.3) * pow(1 + 0.00002567294033 * fieldElev / pow(pressure - 0.3, 0.190284), 1 / 0.190284);
  alt = 2.95300 * qnh_raw;
  if (qnh_raw < 1000) {
     qNH = "Q0" + String(round(qnh_raw)) + "/A" + String(round(alt));
  } else {
     qNH = "Q" + String(round(qnh_raw)) + "/A" + String(round(alt)); 
  }
  WiFiClient client = server.available();
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
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("ZZZZ ");
          client.print(rtc.getDay());
          client.print(rtc.getHours());
          client.print(rtc.getMinutes());
          client.print("Z ");
          client.print(int(temperature));
          client.print("/");
          client.print(int(dewPoint));
          client.println(" "+qNH+"<br />");
          client.print("RMK STNPRES");
          client.print(pressure);
          if(PRESRR == true) {
            client.print(" PRESRR");
          } else if(PRESFR == true) {
            client.print(" PRESFR");
          }
          client.print(" ILLUM");
          client.print(illuminance);
          client.println("<br />");
          client.print("UVA ");
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
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}
// Calculate dew point
float dPoint(float temperature, float relativeHumidity) {
  //float a = 6.112;
  float b = 17.67;
  float c = 243.5;
  float g = log(relativeHumidity / 100) + (b * temperature / (c + temperature));
  return c * g / (b - g);
  }
