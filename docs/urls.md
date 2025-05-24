# URLs do Sistema

## Interface Web (Publisher)

### Página Principal
- URL: `http://IP_DO_ESP32/`
- Descrição: Página inicial com botões para controlar o relé
- Método: GET
- Exemplo: `http://192.168.1.100/`

### Página de Roles
- URL: `http://IP_DO_ESP32/roles`
- Descrição: Página para gerenciar as roles permitidas
- Método: GET
- Exemplo: `http://192.168.1.100/roles`

### API de Controle
- URL: `http://IP_DO_ESP32/toggle`
- Descrição: Endpoint para ligar/desligar o relé
- Método: POST
- Exemplo: `http://192.168.1.100/toggle`

### API de Roles
- URL: `http://IP_DO_ESP32/update-roles`
- Descrição: Endpoint para atualizar as roles permitidas
- Método: POST
- Exemplo: `http://192.168.1.100/update-roles`

## Tópicos MQTT

### Comandos
- Tópico: `etec/hortolandia/rele/cmd`
- Descrição: Tópico para enviar comandos ON/OFF
- Formato: JSON
- Exemplo:
  ```json
  {
      "payload": "ON",
      "timestamp": 1234567890,
      "roles": ["admin", "professor"]
  }
  ```

### Roles
- Tópico: `etec/hortolandia/rele/roles`
- Descrição: Tópico para configurar roles permitidas
- Formato: JSON
- Exemplo:
  ```json
  {
      "roles": ["admin", "professor", "monitor"]
  }
  ```

## Como Descobrir o IP do ESP32

1. Abra o Monitor Serial no Arduino IDE
2. O ESP32 mostrará o IP ao conectar ao WiFi
3. Exemplo de mensagem:
   ```
   WiFi conectado
   Endereço IP: 192.168.1.100
   ```

## Observações

1. O IP do ESP32 pode mudar se:
   - A rede WiFi for reiniciada
   - O ESP32 for reiniciado
   - O roteador for reiniciado

2. Para manter o IP fixo:
   - Configure um IP estático no roteador
   - Ou use o código:
     ```cpp
     IPAddress staticIP(192, 168, 1, 100);
     IPAddress gateway(192, 168, 1, 1);
     IPAddress subnet(255, 255, 255, 0);
     WiFi.config(staticIP, gateway, subnet);
     ```

3. Os tópicos MQTT são fixos e não mudam 