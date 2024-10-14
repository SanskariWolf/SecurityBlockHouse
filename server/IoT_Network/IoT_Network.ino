#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

// Wi-Fi credentials
const char* ssid = "ESP32_AccessPoint";
const char* password = "password123";

// Create a server object
WiFiServer server(80);

// QoS metrics variables
unsigned long startTime;
unsigned long totalDataSent = 0;
unsigned int packetCount = 0;
unsigned int packetLoss = 0;
const unsigned long QoSInterval = 10000; // Report QoS every 10 seconds

// Store connected clients
WiFiClient clients[4];

void setup() {
  Serial.begin(115200);

  // Set up ESP32 as an Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Start the TCP server
  server.begin();

  // Initialize QoS timer
  startTime = millis();
}

void loop() {
  // Check for new clients
  WiFiClient newClient = server.available();
  if (newClient) {
    // Add the client if there is space
    for (int i = 0; i < 4; i++) {
      if (!clients[i]) {
        clients[i] = newClient;
        Serial.print("Client connected, IP: ");
        Serial.println(clients[i].remoteIP());
        break;
      }
    }
  }

  // Handle connected clients
  for (int i = 0; i < 4; i++) {
    if (clients[i]) {
      // Check if client has disconnected
      if (!clients[i].connected()) {
        Serial.println("Client disconnected.");
        clients[i].stop();
        clients[i] = WiFiClient(); // Reset the client object
      } else {
        // Handle incoming data from clients
        if (clients[i].available()) {
          String data = clients[i].readStringUntil('\n');
          Serial.print("Received from client ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(data);

          // Send data back as acknowledgment
          clients[i].print("Message received: " + data);
          clients[i].flush();
          packetCount++; // Increment packet count
          totalDataSent += data.length(); // Track the total data sent
        }
      }
    }
  }

  // Calculate and report QoS metrics periodically
  if (millis() - startTime >= QoSInterval) {
    reportQoS();
    startTime = millis();
  }
}

// Function to calculate and report QoS metrics
void reportQoS() {
  unsigned long currentTime = millis();
  float duration = (currentTime - startTime) / 1000.0; // Time in seconds
  float throughput = totalDataSent / duration / 1000.0; // KBps
  Serial.println("===== QoS Report =====");
  Serial.print("Throughput: ");
  Serial.print(throughput);
  Serial.println(" KBps");

  Serial.print("Total Packets Sent: ");
  Serial.println(packetCount);

  Serial.print("Packet Loss: ");
  Serial.print(packetLoss);
  Serial.println(" packets");

  // Reset metrics
  totalDataSent = 0;
  packetCount = 0;
  packetLoss = 0;
}
