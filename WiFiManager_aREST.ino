/* 
  This a simple example of the aREST UI Library for the ESP8266.
  See the README file for more details.
 
  Written in 2014 by Marco Schwartz under a GPL license. 
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

// Create aREST instance
aREST_UI rest = aREST_UI();

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

#define RELAYPIN 12
#define LEDPIN 13
#define EEPROM_STATE_ADDRESS 128

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  wifiManager.setTimeout(180);
  
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect("WIFIMANAGER")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 
 
  // Create button to control pin 5
  rest.button(RELAYPIN, "RELAY");
  rest.button(LEDPIN, "LED");
  
   
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");

  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  
}

void loop() {

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}

