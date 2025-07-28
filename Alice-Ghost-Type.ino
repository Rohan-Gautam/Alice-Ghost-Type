#include <WiFi.h>
#include <WebServer.h>
#include <BleKeyboard.h>

// Hotspot credentials
const char* ssid = "ESP32_Hotspot";
const char* password = "12345678"; // Minimum 8 characters for WPA2

// Static IP configuration for the hotspot
IPAddress local_IP(192, 69, 69, 69);
IPAddress gateway(192, 69, 69, 69); // Gateway same as local IP for AP
IPAddress subnet(255, 255, 255, 0);

// Web server on port 80
WebServer server(80);

// Bluetooth keyboard instance
BleKeyboard bleKeyboard("ESP32 Keyboard", "ESP32", 100);

// HTML for the web interface
const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Text Sender</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      padding: 20px;
      background: #0a0a1a;
      color: #fff;
    }
    h1 {
      background: rgba(50, 0, 100, 0.2);
      backdrop-filter: blur(10px);
      -webkit-backdrop-filter: blur(10px);
      padding: 20px;
      border: 2px solid rgba(255, 255, 255, 0.3);
      border-radius: 10px;
      margin: 20px auto;
      width: fit-content;
      box-shadow: 0 0 20px rgba(128, 0, 255, 0.3);
    }
    .container {
      width: 90%;
      background: rgba(50, 0, 100, 0.2);
      backdrop-filter: blur(10px);
      -webkit-backdrop-filter: blur(10px);
      padding: 20px;
      border: 2px solid rgba(255, 255, 255, 0.3);
      border-radius: 10px;
      display: inline-block;
      box-shadow: 0 0 20px rgba(128, 0, 255, 0.3);
    }
    textarea {
      width: 90%;
      height: 400px;
      margin: 10px;
      background: transparent;
      border: 2px solid rgba(255, 255, 255, 0.3);
      border-radius: 10px;
      color: #fff;
      padding: 10px;
      font-family: monospace;
      resize: none;
    }
    textarea::placeholder {
      color: rgba(255, 255, 255, 0.5);
    }
    button {
      padding: 10px 20px;
      font-size: 16px;
      background: transparent;
      border: 2px solid rgba(255, 255, 255, 0.3);
      border-radius: 10px;
      color: #fff;
      cursor: pointer;
      transition: all 0.3s ease;
    }
    button:hover {
      background: rgba(128, 0, 255, 0.3);
      box-shadow: 0 0 10px rgba(128, 0, 255, 0.5);
    }
  </style>
</head>
<body>
<h1>ESP32 Text Sender</h1>
<div class="container">
  <textarea id="codeInput" placeholder="Paste your code here..."></textarea><br>
  <button onclick="sendCode()">Send Code</button>
</div>
<script>
  function sendCode() {
    const code = document.getElementById('codeInput').value;
    fetch('/send', {
      method: 'POST',
      headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
      body: 'code=' + encodeURIComponent(code)
    }).then(response => response.text())
            .then(data => alert(data))
            .catch(error => alert('Error: ' + error));
  }
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", html);
}

bool sendKeyWithRetry(char c, uint8_t maxRetries = 5) {
  for (uint8_t attempt = 0; attempt < maxRetries; attempt++) {
    if (bleKeyboard.isConnected()) {
      Serial.print("Sending key: ");
      Serial.println(c);
      bleKeyboard.print(c);
      delay(30); // Increased delay for reliable transmission
      // Verify connection remains active after sending
      if (bleKeyboard.isConnected()) {
        return true; // Assume success if still connected
      }
      Serial.println("Connection lost during send, retrying...");
    } else {
      Serial.println("Bluetooth not connected, attempting reconnect...");
      bleKeyboard.begin(); // Attempt to restart BLE
      delay(200); // Wait for reconnection
    }
  }
  Serial.print("Failed to send key: ");
  Serial.println(c);
  return false;
}

void handleSendCode() {
  if (server.hasArg("code")) {
    String code = server.arg("code");
    if (bleKeyboard.isConnected()) {
      bool success = true;
      Serial.println("Starting to send code...");
      for (int i = 0; i < code.length(); i++) {
        if (!sendKeyWithRetry(code[i])) {
          success = false;
          Serial.println("Code sending aborted due to failure");
          break;
        }
      }
      if (success) {
        server.send(200, "text/plain", "Code sent successfully");
        Serial.println("Code sent successfully");
      } else {
        server.send(500, "text/plain", "Failed to send code due to Bluetooth issues");
        Serial.println("Failed to send code");
      }
    } else {
      server.send(500, "text/plain", "Bluetooth not connected");
      Serial.println("Bluetooth not connected");
    }
  } else {
    server.send(400, "text/plain", "No code provided");
    Serial.println("No code provided");
  }
}

void setup() {
  Serial.begin(115200);

  // Configure static IP for the access point
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("AP IP configuration failed");
  }

  // Start WiFi hotspot on channel 1 to minimize Bluetooth interference
  WiFi.softAP(ssid, password, 1); // Channel 1
  delay(100); // Allow AP to initialize
  Serial.println("Hotspot started");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Start web server
  server.on("/", handleRoot);
  server.on("/send", HTTP_POST, handleSendCode);
  server.begin();
  Serial.println("Web server started");

  // Start Bluetooth keyboard with optimized parameters
  bleKeyboard.setDelay(30); // Set BLE delay to 30ms for stability
  bleKeyboard.begin();
}

void loop() {
  server.handleClient();
  delay(2); // Allow the server to handle requests
}
