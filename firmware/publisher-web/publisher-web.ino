#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// Configurações WiFi
const char* ssid = "SUA_REDE";
const char* password = "SUA_SENHA";

// Configurações MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "etec/hortolandia/rele";

// Objetos
WiFiClient espClient;
PubSubClient client(espClient);
WebServer server(80);

// Estrutura para roles
struct Role {
    String name;
    bool enabled;
};

// Lista de roles
const int MAX_ROLES = 5;
Role roles[MAX_ROLES] = {
    {"admin", true},
    {"professor", true},
    {"aluno", true},
    {"visitante", false},
    {"monitor", true}
};

// HTML da página principal
const char* html_main = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Controle de Relé</title>
    <style>
        body { font-family: Arial; text-align: center; margin: 20px; }
        .button {
            background-color: #4CAF50;
            border: none;
            color: white;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 4px;
        }
        .button-off {
            background-color: #f44336;
        }
        .status {
            margin-top: 20px;
            padding: 10px;
            border-radius: 4px;
        }
        .nav {
            margin: 20px;
        }
        .nav a {
            color: #2196F3;
            text-decoration: none;
            margin: 0 10px;
        }
    </style>
</head>
<body>
    <h1>Controle de Relé</h1>
    <div class="nav">
        <a href="/">Controle</a>
        <a href="/roles">Gerenciar Roles</a>
    </div>
    <button class="button" onclick="toggleRelay('ON')">Ligar</button>
    <button class="button button-off" onclick="toggleRelay('OFF')">Desligar</button>
    <div id="status" class="status"></div>
    <script>
        function toggleRelay(state) {
            fetch('/toggle?state=' + state)
                .then(response => response.text())
                .then(data => {
                    document.getElementById('status').innerHTML = data;
                });
        }
    </script>
</body>
</html>
)";

// HTML da página de roles
const char* html_roles = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Gerenciar Roles</title>
    <style>
        body { font-family: Arial; text-align: center; margin: 20px; }
        .role-list {
            max-width: 400px;
            margin: 20px auto;
            text-align: left;
        }
        .role-item {
            margin: 10px 0;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
        .nav {
            margin: 20px;
        }
        .nav a {
            color: #2196F3;
            text-decoration: none;
            margin: 0 10px;
        }
        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }
        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
            border-radius: 34px;
        }
        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }
        input:checked + .slider {
            background-color: #2196F3;
        }
        input:checked + .slider:before {
            transform: translateX(26px);
        }
    </style>
</head>
<body>
    <h1>Gerenciar Roles</h1>
    <div class="nav">
        <a href="/">Controle</a>
        <a href="/roles">Gerenciar Roles</a>
    </div>
    <div class="role-list">
        <div class="role-item">
            <span>Admin</span>
            <label class="switch">
                <input type="checkbox" id="admin" onchange="updateRole('admin', this.checked)">
                <span class="slider"></span>
            </label>
        </div>
        <div class="role-item">
            <span>Professor</span>
            <label class="switch">
                <input type="checkbox" id="professor" onchange="updateRole('professor', this.checked)">
                <span class="slider"></span>
            </label>
        </div>
        <div class="role-item">
            <span>Aluno</span>
            <label class="switch">
                <input type="checkbox" id="aluno" onchange="updateRole('aluno', this.checked)">
                <span class="slider"></span>
            </label>
        </div>
        <div class="role-item">
            <span>Visitante</span>
            <label class="switch">
                <input type="checkbox" id="visitante" onchange="updateRole('visitante', this.checked)">
                <span class="slider"></span>
            </label>
        </div>
        <div class="role-item">
            <span>Monitor</span>
            <label class="switch">
                <input type="checkbox" id="monitor" onchange="updateRole('monitor', this.checked)">
                <span class="slider"></span>
            </label>
        </div>
    </div>
    <script>
        // Carrega estado inicial dos roles
        window.onload = function() {
            fetch('/getRoles')
                .then(response => response.json())
                .then(roles => {
                    roles.forEach(role => {
                        document.getElementById(role.name).checked = role.enabled;
                    });
                });
        }

        function updateRole(name, enabled) {
            fetch('/updateRole?name=' + name + '&enabled=' + enabled)
                .then(response => response.text())
                .then(data => {
                    console.log(data);
                });
        }
    </script>
</body>
</html>
)";

void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);
    loadRoles();
    setupWiFi();
    setupMQTT();
    setupWebServer();
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

void setupWebServer() {
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", html_main);
    });

    server.on("/roles", HTTP_GET, []() {
        server.send(200, "text/html", html_roles);
    });

    server.on("/toggle", HTTP_GET, []() {
        String state = server.arg("state");
        if (state == "ON" || state == "OFF") {
            publishMessage(state);
            server.send(200, "text/plain", "Comando enviado: " + state);
        } else {
            server.send(400, "text/plain", "Estado inválido");
        }
    });

    server.on("/getRoles", HTTP_GET, []() {
        String json = "[";
        for (int i = 0; i < MAX_ROLES; i++) {
            if (i > 0) json += ",";
            json += "{\"name\":\"" + roles[i].name + "\",\"enabled\":" + String(roles[i].enabled) + "}";
        }
        json += "]";
        server.send(200, "application/json", json);
    });

    server.on("/updateRole", HTTP_GET, []() {
        String name = server.arg("name");
        bool enabled = server.arg("enabled") == "true";
        
        for (int i = 0; i < MAX_ROLES; i++) {
            if (roles[i].name == name) {
                roles[i].enabled = enabled;
                saveRoles();
                server.send(200, "text/plain", "Role atualizada");
                return;
            }
        }
        server.send(400, "text/plain", "Role não encontrada");
    });

    server.begin();
}

void callback(char* topic, byte* payload, unsigned int length) {
    // Não usado no publisher
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Tentando conexão MQTT...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            Serial.println("conectado");
        } else {
            Serial.print("falha, rc=");
            Serial.print(client.state());
            Serial.println(" tentando novamente em 5s");
            delay(5000);
        }
    }
}

void publishMessage(String message) {
    if (!client.connected()) {
        reconnect();
    }

    // Adiciona timestamp e roles ativas
    unsigned long timestamp = millis();
    
    // Cria payload com timestamp e roles
    StaticJsonDocument<512> doc;
    doc["payload"] = message;
    doc["timestamp"] = timestamp;
    
    // Adiciona roles ativas
    JsonArray rolesArray = doc.createNestedArray("roles");
    for (int i = 0; i < MAX_ROLES; i++) {
        if (roles[i].enabled) {
            rolesArray.add(roles[i].name);
        }
    }
    
    // Converte para string
    String payload;
    serializeJson(doc, payload);
    
    // Publica no tópico
    if (client.connected()) {
        client.publish(mqtt_topic, payload.c_str());
        Serial.println("Mensagem publicada: " + payload);
    }
}

void loadRoles() {
    for (int i = 0; i < MAX_ROLES; i++) {
        roles[i].enabled = EEPROM.read(i) == 1;
    }
}

void saveRoles() {
    for (int i = 0; i < MAX_ROLES; i++) {
        EEPROM.write(i, roles[i].enabled ? 1 : 0);
    }
    EEPROM.commit();
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    server.handleClient();
}
