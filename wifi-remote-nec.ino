#include <IRremote.h>  
#include <WiFi.h>
#include <WebServer.h>

#define IR_SEND_PIN D4  

// Wi-Fi credentials
const char* ssid = "rik's wifi";
const char* password = "rikpassword";

WebServer server(80);  


void handleRoot() {
  String html = R"rawliteral(
    <html><head><title>ESP32 LG IR Remote</title></head>
    <body>
      <h2>LG TV Remote Control</h2>
      <button onclick="sendIR('power')">Power</button>
      <button onclick="sendIR('mute')">Mute</button>
      <button onclick="sendIR('volume_up')">Volume +</button>
      <button onclick="sendIR('volume_down')">Volume -</button>
      <button onclick="sendIR('channel_up')">Channel +</button>
      <button onclick="sendIR('channel_down')">Channel -</button>
      <script>
        function sendIR(cmd) {
          fetch('/send?cmd=' + cmd);
        }
      </script>
    </body></html>
  )rawliteral";
  server.send(200, "text/html", html);
}


void handleSend() {
  String cmd = server.arg("cmd");

  if (cmd == "power") {
    IrSender.sendNECRaw(0xF708FB04, 32);  
    Serial.println("Sent: Power");
  } else if (cmd == "mute") {
    IrSender.sendNECRaw(0xF609FB04, 32);  
    Serial.println("Sent: Mute");
  } else if (cmd == "volume_up") {
    IrSender.sendNECRaw(0xFD02FB04, 32);  
    Serial.println("Sent: Volume Up");
  } else if (cmd == "volume_down") {
    IrSender.sendNECRaw(0xFC03FB04, 32); 
    Serial.println("Sent: Volume Down");
  } else if (cmd == "channel_up") {
    IrSender.sendNECRaw(0xFF00FB04, 32);  
    Serial.println("Sent: Channel Up");
  } else if (cmd == "channel_down") {
    IrSender.sendNECRaw(0xFE01FB04, 32);  
    Serial.println("Sent: Channel Down");
  } else {
    Serial.println("Unknown command");
  }

  server.send(200, "text/plain", "IR command sent");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

 
  IrSender.begin(IR_SEND_PIN);

 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  
  server.on("/", handleRoot);     
  server.on("/send", handleSend);  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  
}
