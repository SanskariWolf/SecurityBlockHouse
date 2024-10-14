#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <SHA256.h>  // For hash function

// Wi-Fi credentials
const char* ssid = "ESP32_Blockchain_AP";
const char* password = "blockchain123";

// QoS metrics variables
unsigned long startTime;
unsigned long totalDataSent = 0;
unsigned int packetCount = 0;
unsigned int packetLoss = 0;
const unsigned long QoSInterval = 10000; // Report QoS every 10 seconds

// Blockchain structures
struct Block {
  String data;
  String timestamp;
  String hash;
  String previousHash;
};

#define MAX_BLOCKS 100
Block blockchain[MAX_BLOCKS];
int blockIndex = 0;

// Blockchain Functions
String calculateHash(Block block);
Block createBlock(String data, String prevHash);
void addBlock(String data);
void reportQoS();  // QoS reporting function

// Networking
WiFiServer server(80);
WiFiClient clients[4];

void setup() {
  Serial.begin(115200);

  // Start Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Start the server
  server.begin();

  // Create the Genesis Block (First block of the Blockchain)
  addBlock("Genesis Block");

  // Initialize QoS timer
  startTime = millis();

  // Wait for clients
  Serial.println("Waiting for clients...");
}

void loop() {
  // Accept new clients
  WiFiClient newClient = server.available();
  if (newClient) {
    for (int i = 0; i < 4; i++) {
      if (!clients[i]) {
        clients[i] = newClient;
        Serial.print("Client connected, IP: ");
        Serial.println(clients[i].remoteIP());
        clients[i].println("Connected to ESP32 Blockchain");
        break;
      }
    }
  }

  // Handle each client
  for (int i = 0; i < 4; i++) {
    if (clients[i]) {
      if (!clients[i].connected()) {
        Serial.println("Client disconnected.");
        clients[i].stop();
        clients[i] = WiFiClient();  // Reset the client object
      } else {
        if (clients[i].available()) {
          // Receive data from client
          String data = clients[i].readStringUntil('\n');
          Serial.print("Received from client ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(data);

          // QoS: Increment packet count and data size
          packetCount++;
          totalDataSent += data.length();

          // Add transaction (data) to the Blockchain
          addBlock(data);

          // Broadcast the new block to all clients
          for (int j = 0; j < 4; j++) {
            if (clients[j]) {
              clients[j].println("New block added: " + blockchain[blockIndex - 1].data);
              clients[j].println("Blockchain updated");
            }
          }
        }
      }
    }
  }

  // Periodically report QoS metrics
  if (millis() - startTime >= QoSInterval) {
    reportQoS();
    startTime = millis();
  }
}

// Function to create a new block
Block createBlock(String data, String prevHash) {
  Block newBlock;
  newBlock.data = data;
  newBlock.timestamp = String(millis());
  newBlock.previousHash = prevHash;
  newBlock.hash = calculateHash(newBlock);
  return newBlock;
}

// Function to add a block to the Blockchain
void addBlock(String data) {
  if (blockIndex < MAX_BLOCKS) {
    Block newBlock = createBlock(data, blockchain[blockIndex - 1].hash);
    blockchain[blockIndex] = newBlock;
    blockIndex++;
    Serial.println("Block added to the Blockchain");
    Serial.print("Block data: ");
    Serial.println(newBlock.data);
  }
}

// Simple hash function for a block
String calculateHash(Block block) {
  String input = block.data + block.timestamp + block.previousHash;
  SHA256 sha256;
  sha256.update(input.c_str(), input.length());
  uint8_t* hash = sha256.result();
  String hashString = "";
  for (int i = 0; i < 32; i++) {
    hashString += String(hash[i], HEX);
  }
  return hashString;
}

// Function to calculate and report QoS metrics
void reportQoS() {
  unsigned long currentTime = millis();
  float duration = (currentTime - startTime) / 1000.0;  // Time in seconds
  float throughput = totalDataSent / duration / 1000.0;  // KBps
  Serial.println("===== QoS Report =====");
  Serial.print("Throughput: ");
  Serial.print(throughput);
  Serial.println(" KBps");

  Serial.print("Total Packets Sent: ");
  Serial.println(packetCount);

  Serial.print("Packet Loss: ");
  Serial.print(packetLoss);  // Currently hardcoded, can be calculated by tracking dropped packets
  Serial.println(" packets");

  // Reset metrics for next interval
  totalDataSent = 0;
  packetCount = 0;
  packetLoss = 0;
}
