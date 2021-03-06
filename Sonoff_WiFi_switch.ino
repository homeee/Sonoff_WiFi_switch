#define POW
#include "sets.h"             //Содержится в пакете


DNSServer dnsServer;
ESP8266WebServer HTTP(80);
File fsUploadFile;
TickerScheduler ts(10);
WiFiUDP udp;
StringCommand sCmd;                   // Обьект для работы с командами
ESP8266HTTPUpdateServer httpUpdater;
WiFiClient wclient;
PubSubClient client(wclient);
//WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer HTTPWAN(8080);

RCSwitch mySwitch = RCSwitch();
IRrecv *irReceiver;
decode_results results;
IRsend *irSender;
LivoloTx *gLivolo;
WS2812FX ws2812fx = WS2812FX();

#ifdef POW
HLW8012 hlw8012;
#endif

OneWire *oneWire;
DallasTemperature sensors;
DHT dht;
Bounce * buttons = new Bounce[NUM_BUTTONS];

Ticker motion;


void setup() {
  Serial.begin(115200);
  Serial.println();
  start_init();
  //  Serial.println(configSetup);

}

void loop() {
  ts.update();
  HTTP.handleClient();
  dnsServer.processNextRequest();
  handleSSDP();
  handleScenary();
  HTTPWAN.handleClient();
  handleMQTT();
  ws2812fx.service();
  handleRfReceiv();
  handleButtons();
  // webSocket.loop();
}
