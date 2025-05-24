# Guia de Configuração

Este guia explica como configurar e usar o sistema de controle de relé via MQTT com ESP32.

## Visão Geral

O sistema consiste em dois componentes principais:

1. **Publisher (ESP32 com Web Server)**
   - Interface web para controle
   - Gerenciamento de roles
   - Envio de comandos MQTT

2. **Subscriber (ESP32 com Relé)**
   - Recebe comandos MQTT
   - Controla relé
   - Verifica roles e timestamp

## Hardware

### Publisher
- ESP32
- Conexão WiFi

### Subscriber
- ESP32
- Módulo Relé
- Conexão WiFi

## Software

### Bibliotecas Necessárias
- PubSubClient
- ArduinoJson
- WebServer

### Configuração do Publisher

1. Abra o arquivo `publisher-web.ino`
2. Configure as credenciais WiFi:
   ```cpp
   const char* ssid = "SUA_REDE";
   const char* password = "SUA_SENHA";
   ```
3. Configure o servidor MQTT:
   ```cpp
   const char* mqtt_server = "broker.hivemq.com";
   const int mqtt_port = 1883;
   ```
4. Faça upload do código

### Configuração do Subscriber

1. Abra o arquivo `subscriber-relay.ino`
2. Configure as credenciais WiFi
3. Configure o servidor MQTT
4. Conecte o relé ao pino definido
5. Faça upload do código

## Tópicos MQTT

O sistema usa dois tópicos:

1. **Comandos** (`etec/hortolandia/rele/cmd`):
   - Envia comandos ON/OFF
   - Inclui timestamp e roles

2. **Roles** (`etec/hortolandia/rele/roles`):
   - Configura roles permitidas
   - Sobrescreve configuração anterior

## Sistema de Roles

### Roles Predefinidas
- admin
- professor
- aluno
- visitante
- monitor

### Gerenciamento de Roles
1. Acesse a interface web (http://IP_DO_ESP32)
2. Vá para a página de roles
3. Ative/desative roles conforme necessário
4. As alterações são salvas automaticamente

### Comportamento do Subscriber
- Se nenhuma role for configurada: aceita todos os comandos
- Se roles forem configuradas: aceita apenas comandos com roles permitidas
- Nova configuração de roles sobrescreve a anterior

## Segurança

### Timestamp
- Mensagens são válidas por 5 segundos
- Evita replay attacks

### Roles
- Controle de acesso baseado em roles
- Configuração dinâmica via MQTT

## Solução de Problemas

### Publisher
1. **Interface web não acessível**
   - Verifique conexão WiFi
   - Confirme IP do ESP32

2. **Comandos não enviados**
   - Verifique conexão MQTT
   - Confirme tópicos corretos

### Subscriber
1. **Relé não responde**
   - Verifique conexão MQTT
   - Confirme roles configuradas
   - Verifique timestamp

2. **Roles não atualizadas**
   - Verifique mensagem no tópico de roles
   - Confirme formato JSON correto

## Limitações

1. Broker MQTT público sem autenticação
2. Mensagens não criptografadas
3. Timestamp baseado em millis() do ESP32

## Próximos Passos

1. Implementar autenticação MQTT
2. Adicionar criptografia
3. Sincronizar timestamp com NTP
