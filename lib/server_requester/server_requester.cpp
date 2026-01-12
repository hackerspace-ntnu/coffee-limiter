#include <WiFi.h>
#include <server_requester.h>

// API constants
static const char* API_SERVER = "hackerspace-ntnu.no";
static const char* API_BASE_PATH = "/api/coffee/check?card=";

int wifiStatus = WL_IDLE_STATUS;
WiFiClient client;

static DisplayCallback displayCallback = nullptr;

void setDisplayCallback(DisplayCallback callback) {
  displayCallback = callback;
}

static void updateDisplay(char* message) {
  if (displayCallback != nullptr) {
    displayCallback(message);
  }
}

bool setupWiFi(char ssid[], char pass[])
{
  // Check for shield
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    return false;
  }

  // Connect to WiFi
  while (wifiStatus != WL_CONNECTED)
  {
    Serial.print("Connecting to: ");
    Serial.println(ssid);
    updateDisplay((char*)"Connecting to WiFi...");
    wifiStatus = WiFi.begin(ssid, pass);
    delay(5000);
  }

  Serial.println("Connected to WiFi!");
  updateDisplay((char*)"WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  return true;
}

bool httpGet(const char *server, const char *path)
{
  // Get-request to the server.
  if (client.connect(server, 80))
  {
    client.print("GET ");
    client.print(path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    return true;
  }
  return false;
}

bool getCardData(uint32_t cardid, char* outBuffer, size_t bufferSize)
{
  // Setup API path with cardid
  char path[64];
  sprintf(path, "%s%lu", API_BASE_PATH, cardid);

  // Contact server
  updateDisplay((char*)"Contacting server...");
  if (!httpGet(API_SERVER, path))
  {
    Serial.println("Connection failed");
    updateDisplay((char*)"Connection failed!");
    return false;
  }

  Serial.println("Request sent!");
  updateDisplay((char*)"Waiting for response...");

  // Wait for response with timeout
  unsigned long timeout = millis() + 5000;
  while (client.connected() && millis() < timeout)
  {
    if (client.available())
    {
      // Read HTTP Status
      char statusLine[32];
      size_t len = client.readBytesUntil('\n', statusLine, sizeof(statusLine) - 1);
      statusLine[len] = '\0';

      // Extract status code
      int statusCode = 0;
      char* statusStart = strchr(statusLine, ' ');
      if (statusStart) {
        statusCode = atoi(statusStart + 1);
      }

      Serial.print("HTTP Status: ");
      Serial.println(statusCode);

      // Check for error status codes
      if (statusCode < 200 || statusCode >= 300) {
        Serial.print("HTTP Error: ");
        Serial.println(statusLine);
        client.stop();
        return false;
      }

      // Skip remaining header
      if (client.find("\r\n\r\n"))
      {
        // Read body into buffer
        size_t bytesRead = 0;
        while (client.available() && bytesRead < bufferSize - 1)
        {
          outBuffer[bytesRead++] = client.read();
        }
        outBuffer[bytesRead] = '\0'; // Null-terminate

        Serial.print("Response body: ");
        Serial.println(outBuffer);

        client.stop();
        return true;
      }
    }
  }

  Serial.println("Request timeout");
  client.stop();
  return false;
}