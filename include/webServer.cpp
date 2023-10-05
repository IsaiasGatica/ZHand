// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create a WebSocket object

AsyncWebSocket ws("/ws");

String message = "";
String sliderValue1 = "0";

int SPosicion = 60;

// Replace with your network credentials
const char *ssid = "GWIFI";
const char *password = "independenciaGatica38";

// Json Variable to Hold Slider Values
JSONVar sliderValues;

String getSliderValues()
{
    sliderValues["sliderValue1"] = String(sliderValue1);

    String jsonString = JSON.stringify(sliderValues);
    return jsonString;
}

// Initialize SPIFFS
void initFS()
{
    if (!SPIFFS.begin())
    {
        Serial.println("An error has occurred while mounting SPIFFS");
    }
    else
    {
        Serial.println("SPIFFS mounted successfully");
    }
}

// Initialize WiFi
void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

void notifyClients(String sliderValues)
{
    ws.textAll(sliderValues);
}

// Esta función se penso originalmente para manejar más de un slider
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        message = (char *)data;
        if (message.indexOf("1s") >= 0)
        {
            sliderValue1 = message.substring(2);
            SPosicion = sliderValue1.toInt();
            notifyClients(getSliderValues());
        }

        if (strcmp((char *)data, "getValues") == 0)
        {
            notifyClients(getSliderValues());
        }
    }
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void loadServer()
{
    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html"); });

    server.serveStatic("/", SPIFFS, "/");
}