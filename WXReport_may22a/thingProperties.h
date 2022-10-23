#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>


const char THING_ID[] = "8c6238ec-31f7-4b77-8f14-54bf44a9ff8f";

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)


String qNH;
float uVIndex;
float illuminance;
float dewPoint;
float temperature;

void initProperties(){

  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(qNH, READ, 60 * SECONDS, NULL);
  ArduinoCloud.addProperty(uVIndex, READ, 60 * SECONDS, NULL);
  ArduinoCloud.addProperty(illuminance, READ, 60 * SECONDS, NULL);
  ArduinoCloud.addProperty(dewPoint, READ, 60 * SECONDS, NULL);
  ArduinoCloud.addProperty(temperature, READ, 60 * SECONDS, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
