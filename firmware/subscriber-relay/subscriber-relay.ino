#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configurações WiFi
const char* ssid = "SUA_REDE";
const char* password = "SUA_SENHA";

// Configurações MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic_cmd = "etec/hortolandia/rele/cmd";
const char* mqtt_topic_roles = "etec/hortolandia/rele/roles";

// Configurações do relé
const int relay_pin = 23;  // Pino do relé

// Roles permitidas
const int MAX_ROLES = 10;
String allowed_roles[MAX_ROLES];
int num_allowed_roles = 0;
bool roles_configured = false;

// Objetos
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(115200);
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, LOW);
    
    setupWiFi();
    setupMQTT();
}

void setupWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado ao WiFi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void setupMQTT() {
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
    // Converte payload para string
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';
    
    // Verifica qual tópico recebeu a mensagem
    if (strcmp(topic, mqtt_topic_roles) == 0) {
        handleRolesMessage(message);
    } else if (strcmp(topic, mqtt_topic_cmd) == 0) {
        handleCommandMessage(message);
    }
}

void handleRolesMessage(const char* message) {
    // Parse do JSON
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, message);
    
    if (error) {
        Serial.println("Erro ao parsear JSON de roles");
        return;
    }
    
    // Limpa roles anteriores
    num_allowed_roles = 0;
    
    // Processa nova lista de roles
    JsonArray roles = doc["roles"];
    for (JsonVariant role : roles) {
        if (num_allowed_roles < MAX_ROLES) {
            allowed_roles[num_allowed_roles++] = role.as<String>();
        }
    }
    
    roles_configured = true;
    Serial.println("Roles atualizadas:");
    for (int i = 0; i < num_allowed_roles; i++) {
        Serial.println("- " + allowed_roles[i]);
    }
}

void handleCommandMessage(const char* message) {
    // Parse do JSON
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, message);
    
    if (error) {
        Serial.println("Erro ao parsear JSON do comando");
        return;
    }
    
    // Verifica timestamp (5 segundos de validade)
    unsigned long timestamp = doc["timestamp"];
    if (millis() - timestamp > 5000) {
        Serial.println("Mensagem expirada");
        return;
    }
    
    // Se roles foram configuradas, verifica permissões
    if (roles_configured) {
        bool role_allowed = false;
        JsonArray roles = doc["roles"];
        
        for (JsonVariant role : roles) {
            String role_name = role.as<String>();
            for (int i = 0; i < num_allowed_roles; i++) {
                if (role_name == allowed_roles[i]) {
                    role_allowed = true;
                    break;
                }
            }
            if (role_allowed) break;
        }
        
        if (!role_allowed) {
            Serial.println("Role não autorizada");
            return;
        }
    }
    
    // Processa o comando
    String payload_msg = doc["payload"].as<String>();
    if (payload_msg == "ON") {
        digitalWrite(relay_pin, HIGH);
        Serial.println("Relé LIGADO");
    } else if (payload_msg == "OFF") {
        digitalWrite(relay_pin, LOW);
        Serial.println("Relé DESLIGADO");
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Tentando conexão MQTT...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            Serial.println("conectado");
            client.subscribe(mqtt_topic_cmd);
            client.subscribe(mqtt_topic_roles);
        } else {
            Serial.print("falha, rc=");
            Serial.print(client.state());
            Serial.println(" tentando novamente em 5s");
            delay(5000);
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
