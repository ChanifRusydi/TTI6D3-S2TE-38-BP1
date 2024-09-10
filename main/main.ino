/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  Example based on the Universal Arduino Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/FlashLED/FlashLED.ino
*/
// #include <key.h>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Biznet atas";
const char* password = "Biznetatas";

// Initialize Telegram BOT
#define BOTtoken "7268202981:AAEzGZTqVnKyi1yqeoxD2Pl8CVPcjDu_LfE"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "5863961583"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/state") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}




// #include <string.h>
// // Device libraries (Arduino ESP32/ESP8266 Cores)
// #include <Arduino.h>
// #ifdef ESP8266
// #include <ESP8266WiFi.h>
// #else // jika menggunakan ESP32
// #include <WiFi.h>
// #endif
// // Custom libraries
// #include <utlgbotlib.h>

// #include "DHT.h"
// #define DHTPIN 15
// #define DHTTYPE DHT11
// DHT dht(DHTPIN, DHTTYPE);
// float h, t;
// #define WIFI_SSID "Biznet atas"
// #define WIFI_PASS "Biznetatas"

// #define MAX_CONN_FAIL 50
// #define MAX_LENGTH_WIFI_SSID 31
// #define MAX_LENGTH_WIFI_PASS 63

// #define TLG_TOKEN "5863961583:AAFFZHH7qgWyQ-D-sSxgvSBZhPYb0zAWPnQ" // token anakkendali_bot

// #define DEBUG_LEVEL_UTLGBOT 0

// #define PIN_RELAY 5 // ESP32 RELAYnya di pin gpio5

// const char TEXT_START[] =
//   "Hello, im a Bot running in an ESP microcontroller that let you turn on/off a RELAY/light.\n"
//   "\n"
//   "Check /help command to see how to use me.";

// char buff[100];
// boolean state_t, state_h;

// const char TEXT_HELP[] =
//   "Available Commands:\n"
//   "\n"
//   "/start - Show start text.\n"
//   "/help - Show actual text.\n"
//   "/relayon - Turn on the RELAY.\n"
//   "/relayoff - Turn off the RELAY.\n"
//   "/relaystatus - Show actual RELAY status.\n"
//   "/statussuhu - Show actual temperature status.\n"
//   "/statuskelembaban - Show actual Humidity status.\n";

// void wifi_init_stat(void);
// bool wifi_handle_connection(void);


// // Create Bot object
// uTLGBot Bot(TLG_TOKEN);

// // RELAY status
// uint8_t relay_status;

// void setup(void)
// {
//   Bot.set_debug(DEBUG_LEVEL_UTLGBOT);
//   Serial.begin(9600);
//   digitalWrite(PIN_RELAY, HIGH);
//   pinMode(PIN_RELAY, OUTPUT);
//   relay_status = 1;

//   wifi_init_stat();

//   Serial.println("Waiting for WiFi connection.");
//   while (!wifi_handle_connection())
//   {
//     Serial.print(".");
//     delay(500);
//   }
//   dht.begin();
//   Bot.getMe();
// }

// void loop()
// {
//   if (!wifi_handle_connection())
//   {
//     // Wait 100ms and check again
//     delay(100);
//     return;
//   }

//   // Check for Bot received messages
//   while (Bot.getUpdates())
//   {
//     Serial.println("Received message:");
//     Serial.println(Bot.received_msg.text);
//     Serial.println(Bot.received_msg.chat.id);

//     if (strncmp(Bot.received_msg.text, "/start", strlen("/start")) == 0)
//     {
//       Bot.sendMessage(Bot.received_msg.chat.id, TEXT_START);
//     }

//     else if (strncmp(Bot.received_msg.text, "/help", strlen("/help")) == 0)
//     {
//       Bot.sendMessage(Bot.received_msg.chat.id, TEXT_HELP);
//     }

//     else if (strncmp(Bot.received_msg.text, "/relayon", strlen("/relayon")) == 0)
//     {
//       relay_status = 0;
//       Serial.println("Command /relayon received.");
//       Serial.println("Turning on the Relay.");

//       Bot.sendMessage(Bot.received_msg.chat.id, "Relay turned on.");
//     }

//     else if (strncmp(Bot.received_msg.text, "/relayoff", strlen("/relayoff")) == 0)
//     {
//       relay_status = 1;
//       // Show command reception through Serial
//       Serial.println("Command /relayoff received.");
//       Serial.println("Turning off the RELAY.");

//       // Send a Telegram message to notify that the RELAY has been turned off
//       Bot.sendMessage(Bot.received_msg.chat.id, "RELAY turned off.");
//     }

//     // If /RELAYstatus command was received
//     else if (strncmp(Bot.received_msg.text, "/relaystatus", strlen("/relaystatus")) == 0)
//     {
//       // Send a Telegram message to notify actual RELAY status
//       if (relay_status)
//         Bot.sendMessage(Bot.received_msg.chat.id, "The RELAY is off.");
//       else
//         Bot.sendMessage(Bot.received_msg.chat.id, "The RELAY is on.");
//     }
//     else if (strncmp(Bot.received_msg.text, "/statussuhu", strlen("/statussuhu")) == 0)
//     {
//       t = dht.readTemperature();
//       if (isnan(h) || isnan(t)) {
//         Serial.println(F("Failed to read from DHT sensor!"));
//         return;
//       }
//       String msg = "Status Suhu :";
//       msg += t;
//       msg += " °C\n";

//       msg.toCharArray(buff, 100);
//       Bot.sendMessage(Bot.received_msg.chat.id, buff);
//     }
//     else if (strncmp(Bot.received_msg.text, "/statuskelembaban", strlen("/statuskelembaban")) == 0)
//     {
//       h = dht.readHumidity();
//       if (isnan(h) || isnan(t)) {
//         Serial.println(F("Failed to read from DHT sensor!"));
//         return;
//       }
//       String msg = "Status Kelembaban :";
//       msg += h;
//       msg += " %Rh\n";

//       msg.toCharArray(buff, 100);
//       Bot.sendMessage(Bot.received_msg.chat.id, buff);
//     }

//     digitalWrite(PIN_RELAY, relay_status);
//     // Feed the Watchdog
//     yield();
//   }

//   h = dht.readHumidity();
//   t = dht.readTemperature();
//   if (isnan(h) || isnan(t)) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }

//   if (t > 37 && state_t == 0) {
//     state_t = 1;
//     String msg = "Status Suhu :";
//     msg += t;
//     msg += " °C\n";

//     msg.toCharArray(buff, 100);
//     Bot.sendMessage("556538872", buff);
//   }
//   else if (t <= 37) {
//     state_t = 0;

//   }

//   if (h < 60 && state_h == 0) {
//     state_h = 1;
//     String msg = "Status Kelembaban :";
//     msg += h;
//     msg += " %Rh\n";
//     msg.toCharArray(buff, 100);
//     Bot.sendMessage("556538872", buff);
//   }
//   else if (h >= 60) {
//     state_h = 0;
//   }
//   Serial.print(F("Humidity: "));
//   Serial.print(h);
//   Serial.print(F("%  Temperature: "));
//   Serial.print(t);
//   Serial.println(F("°C "));
//   delay(1000);
// }


// // Init WiFi interface
// void wifi_init_stat(void)
// {
//   Serial.println("Initializing TCP-IP adapter...");
//   Serial.print("Wifi connecting to SSID: ");
//   Serial.println(WIFI_SSID);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(WIFI_SSID, WIFI_PASS);

//   Serial.println("TCP-IP adapter successfuly initialized.");
// }


// bool wifi_handle_connection(void)
// {
//   static bool wifi_connected = false;

//   // Device is not connected
//   if (WiFi.status() != WL_CONNECTED)
//   {
//     // Was connected
//     if (wifi_connected)
//     {
//       Serial.println("WiFi disconnected.");
//       wifi_connected = false;
//     }

//     return false;
//   }
//   // Device connected
//   else
//   {
//     // Wasn't connected
//     if (!wifi_connected)
//     {
//       Serial.println("");
//       Serial.println("WiFi connected");
//       Serial.print("IP address: ");
//       Serial.println(WiFi.localIP());

//       wifi_connected = true;
//     }

//     return true;
//   }
// }