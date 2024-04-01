#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

// Fallback network
const char* fallbackSSID = "Netpresso_Fallback";
const char* fallbackPassword = "Netpresso";
const char* fallbackHostname = "netpresso";

// Handle pins on EPS8266
const int powerPin = 14;  // D5
const int espressoPin = 12;  // D6
const int americanoPin = 13;  // D7

struct WiFiCredentials {
    char ssid[32];
    char password[64];
};

ESP8266WebServer server(80);

void saveCredentialsToEEPROM(WiFiCredentials &creds) {
    EEPROM.begin(sizeof(WiFiCredentials));
    EEPROM.put(0, creds);
    EEPROM.commit();
}

void loadCredentialsFromEEPROM(WiFiCredentials &creds) {
    EEPROM.begin(sizeof(WiFiCredentials));
    EEPROM.get(0, creds);
}

void clearEEPROM() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

const char* beginning = 
"<!DOCTYPE html><html lang='en'><head>"
"<style>"
"body { font-family: Galvji, sans-serif; }"
"input {border-radius: 0.1em; border: solid 1px black; padding: 0.4em; }"
".box { margin: 4em auto 1em auto; padding: 1em; width: 25em; border: gray solid 1px; }"
".container { display: grid; grid-template-columns: 1fr 2fr; grid-template-rows: 1fr 1fr 1fr; gap: 0.25em 0.5em; grid-template-areas: '. .' '. .' '. .';}"
"button { padding: 0.5em; background: black; border: solid 1px black; border-radius: 0.1em; color: white; width: 100%;}"
"</style>"
"<title>Netpresso</title></head><body>";

// Handle a fallback access point, in cases where WiFi config failes
void handleConfigPage() {
    String content = String(beginning)
    + "<form class='box' action='/saveconfig' method='post'><h2>Netpresso</h2>"
    "<p>Please connect your Netpresso to your network.</p><div class='container'>"
    "<label for='ssid'>SSID:</label><input type='text' id='ssid' name='ssid'>"
    "<label for='password'>Password:</label><input type='password' id='password' name='password'>"
    "</div><button type='submit'>Go online</button></form></div></body></html>";
    char response[content.length()];
    content.toCharArray(response, content.length());

    server.send(200, "text/html", response);
}

void handleSaveConfig() {
    WiFiCredentials storedCredentials; 
    strcpy(storedCredentials.ssid, server.arg("ssid").c_str());
    strcpy(storedCredentials.password, server.arg("password").c_str());

    saveCredentialsToEEPROM(storedCredentials);

    String content = String(beginning)
    + "<div class='box'><h2>Netpresso</h2>"
    + "<p>Rebooting, will connect to " + storedCredentials.ssid + " on startup.</p><a href='/'>Check status</a></div></body></html>";
    char response[content.length()];
    content.toCharArray(response, content.length());

    server.send(200, "text/html", response);
    delay(2000);
    ESP.restart();
}

void handleAccessPoint() {
    server.on("/", HTTP_GET, handleConfigPage);
    server.on("/saveconfig", HTTP_POST, handleSaveConfig);
    server.begin();

    WiFi.mode(WIFI_AP);
    Serial.println("Setting up fallback access point");

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    WiFi.softAP(fallbackSSID, fallbackPassword);

    if (!MDNS.begin(fallbackHostname)) {
        Serial.println("Error setting up MDNS responder!");
    }

    while (WiFi.status() != WL_CONNECTED) {
      server.handleClient();
      MDNS.update();

      delay(200);
    }
}

// Debugging wifi status
void printWifiStatus() {
  int wifiStatus = WiFi.status();

  Serial.print("Wifi: ");

  if (wifiStatus == WL_IDLE_STATUS) {
    Serial.println("Idle");
  }
  if (wifiStatus == WL_NO_SSID_AVAIL) {
    Serial.println("SSID not available");
  }
  if (wifiStatus == WL_CONNECTED) {
    Serial.println("Connected");
  }
  if (wifiStatus == WL_CONNECT_FAILED) {
    Serial.println("Failed");
  }
  if (wifiStatus == WL_WRONG_PASSWORD) {
    Serial.println("Wrong password");
  }
  if (wifiStatus == WL_DISCONNECTED) {
    Serial.println("Disconnected");
  }
}

void connectToWiFi() {
    Serial.println("Connecting to WiFi");

    WiFiCredentials storedCredentials;
    loadCredentialsFromEEPROM(storedCredentials);

    int numNetworks = WiFi.scanNetworks();

    printWifiStatus();
    String ssid;
    if (numNetworks == 0) {
        Serial.println("No networks found");
    } else {
        Serial.print("Found ");
        Serial.print(numNetworks);
        Serial.println(" networks:");
        
        for (int i = 0; i < numNetworks; ++i) {
            ssid = WiFi.SSID(i);
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(ssid);  // Get SSID of the network
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));  // Get signal strength
            Serial.println(" dBm)");

            if (strcmp(ssid.c_str(), storedCredentials.ssid) == 0) {
              printWifiStatus();

              WiFi.begin(storedCredentials.ssid, storedCredentials.password);
              Serial.print("Connecting to SSID: ");
              Serial.println(storedCredentials.ssid);
              // Serial.print("Using pwd: ");
              // Serial.println(storedCredentials.password);
              
              // Let the wifi connect
              delay(3000);
              break;            
            }
        }
    }
}

void coffeeSetup() {
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);

  pinMode(americanoPin, OUTPUT);
  digitalWrite(americanoPin, LOW);

  pinMode(espressoPin, OUTPUT);
  digitalWrite(espressoPin, LOW);
}

void pressPowerButton() {
  // Turn the pin on
  digitalWrite(powerPin, HIGH);
  server.send(200, "application/json", "{ \"status\": \"OK\" }");
  delay(600); // Wait for 0.6 seconds  
  // Turn the pin off
  digitalWrite(powerPin, LOW);
}

void pressAmericanoButton() {
  // Turn the pin on
  digitalWrite(americanoPin, HIGH);
  server.send(200, "application/json", "{ \"status\": \"OK\" }");
  delay(600); // Wait for 0.6 seconds  
  // Turn the pin off
  digitalWrite(americanoPin, LOW);
}

void pressEspressoButton() {
  // Turn the pin on
  digitalWrite(espressoPin, HIGH);
  server.send(200, "application/json", "{ \"status\": \"OK\" }");
  delay(600); // Wait for 0.6 seconds  
  // Turn the pin off
  digitalWrite(espressoPin, LOW);
}

void handleResetAction() {
  String content = String(beginning) 
  + "<div class='box'><h2>Netpresso</h2>"
  + "<p>Will reset within 2 seconds.</p></div>"
  + "</body></html>";
  char response[content.length()];
  content.toCharArray(response, content.length());
  
  server.send(200, "text/html", response);
  clearEEPROM();
  ESP.restart();
}

void handleStatusPage() {
    WiFiCredentials storedCredentials;
    loadCredentialsFromEEPROM(storedCredentials);

    String content = String(beginning) 
    + "<form class='box' method='POST' action='/reset'><h2>Netpresso</h2>"
    + "<p>Connected to " + storedCredentials.ssid + ".</p><button type='submit'>Go offline</button></form>"
    + "</body></html>";
    char response[content.length()];
    content.toCharArray(response, content.length());
    
    server.send(200, "text/html", response);
}

void handleCoffeActions() {
    Serial.println("Setting up coffe endpoints");
    server.on("/", HTTP_GET, handleStatusPage);
    server.on("/power", HTTP_GET, pressPowerButton);
    server.on("/americano", HTTP_GET, pressAmericanoButton);
    server.on("/espresso", HTTP_GET, pressEspressoButton);
    server.on("/reset", HTTP_POST, handleResetAction);
    server.begin();  
}

void setup() {
    Serial.begin(115200);
    Serial.println("Welcome to Netpresso");
    randomSeed(analogRead(0));


    connectToWiFi();

    int attempt = 1;

    printWifiStatus();
    while (WiFi.status() != WL_CONNECTED) {
      printWifiStatus();
      if (WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_WRONG_PASSWORD || attempt > 3) {
        handleAccessPoint();
      } else {
        delay(5000);
        attempt++;
        connectToWiFi();
      }
    }
    // Logg lokal IP
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());

    coffeeSetup();
    handleCoffeActions();
}

void loop() {
    // Handle client requests for both modes
    server.handleClient();
    delay(100);
}
