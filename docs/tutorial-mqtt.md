# Tutorial MQTT

Este tutorial explica os conceitos básicos do MQTT e como ele é usado neste projeto.

## O que é MQTT?

MQTT (Message Queuing Telemetry Transport) é um protocolo de mensageria leve projetado para IoT. Ele usa um modelo de publicação/assinatura onde:

- **Publisher**: Envia mensagens para um tópico
- **Subscriber**: Recebe mensagens de um tópico
- **Broker**: Gerencia os tópicos e distribui as mensagens

## Tópicos MQTT

Neste projeto usamos dois tópicos:

1. **Comandos** (`etec/hortolandia/rele/cmd`):
   - Usado para enviar comandos ON/OFF
   - Formato da mensagem:
     ```json
     {
         "payload": "ON/OFF",
         "timestamp": 1234567890,
         "roles": ["admin", "professor", "monitor"]
     }
     ```

2. **Roles** (`etec/hortolandia/rele/roles`):
   - Usado para configurar roles permitidas
   - Formato da mensagem:
     ```json
     {
         "roles": ["admin", "professor", "monitor"]
     }
     ```

## Broker MQTT

Usamos o broker público HiveMQ:
- Endereço: `broker.hivemq.com`
- Porta: `1883`
- Sem autenticação (público)

## Bibliotecas

### PubSubClient
```cpp
#include <PubSubClient.h>

// Configuração
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Conexão
WiFiClient espClient;
PubSubClient client(espClient);
client.setServer(mqtt_server, mqtt_port);

// Reconexão
void reconnect() {
    while (!client.connected()) {
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            client.subscribe(mqtt_topic);
        }
    }
}
```

### ArduinoJson
```cpp
#include <ArduinoJson.h>

// Criar mensagem
StaticJsonDocument<512> doc;
doc["payload"] = "ON";
doc["timestamp"] = millis();
JsonArray roles = doc.createNestedArray("roles");
roles.add("admin");

// Converter para string
String payload;
serializeJson(doc, payload);

// Publicar
client.publish(mqtt_topic, payload.c_str());
```

## Exemplos

### Publisher
```cpp
void publishMessage(String message) {
    StaticJsonDocument<512> doc;
    doc["payload"] = message;
    doc["timestamp"] = millis();
    
    JsonArray roles = doc.createNestedArray("roles");
    for (int i = 0; i < MAX_ROLES; i++) {
        if (roles[i].enabled) {
            roles.add(roles[i].name);
        }
    }
    
    String payload;
    serializeJson(doc, payload);
    client.publish(mqtt_topic_cmd, payload.c_str());
}
```

### Subscriber
```cpp
void callback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';
    
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, message);
    
    if (error) return;
    
    // Verifica timestamp
    unsigned long timestamp = doc["timestamp"];
    if (millis() - timestamp > 5000) return;
    
    // Verifica roles
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
        }
        if (!role_allowed) return;
    }
    
    // Processa comando
    String payload_msg = doc["payload"].as<String>();
    if (payload_msg == "ON") {
        digitalWrite(relay_pin, HIGH);
    } else if (payload_msg == "OFF") {
        digitalWrite(relay_pin, LOW);
    }
}
```

## Boas Práticas

1. **Reconexão**: Sempre implemente reconexão automática
2. **Validação**: Verifique o formato das mensagens
3. **Timestamp**: Use timestamp para evitar replay attacks
4. **Roles**: Implemente controle de acesso
5. **Tópicos**: Use tópicos separados para diferentes tipos de mensagem

## Ferramentas

1. **MQTT Explorer**: Cliente MQTT com interface gráfica
2. **Mosquitto**: Broker MQTT local para testes
3. **Arduino IDE**: Para desenvolvimento e upload

## Recursos

- [Documentação MQTT](https://mqtt.org/documentation)
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [ArduinoJson](https://arduinojson.org/)
