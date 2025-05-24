# ESP32-Trusted-MQTT

Sistema de controle de relé via MQTT com ESP32, implementando controle de acesso baseado em roles.

## Descrição

Este projeto implementa um sistema de controle de relé usando ESP32 e MQTT, com as seguintes características:

- Interface web para controle do relé
- Sistema de roles para controle de acesso
- Proteção contra replay attacks usando timestamp
- Persistência de configurações na EEPROM
- Filas MQTT separadas para comandos e roles

## Estrutura do Projeto

```
.
├── firmware/
│   └── basic-version/
│       ├── publisher-web/
│       │   └── publisher-web.ino
│       └── subscriber-relay/
│           └── subscriber-relay.ino
└── docs/
    ├── guia-configuracao.md
    └── tutorial-mqtt.md
```

## Funcionalidades

### Publisher (ESP32 com Web Server)
- Interface web para controle do relé
- Gerenciamento de roles via web
- Envio de comandos MQTT com timestamp
- Persistência de configurações na EEPROM

### Subscriber (ESP32 com Relé)
- Recebe comandos MQTT
- Verifica roles e timestamp
- Controla relé
- Configuração dinâmica de roles

## Requisitos

### Hardware
- 2x ESP32
- Módulo Relé
- Conexão WiFi

### Software
- Arduino IDE
- Bibliotecas:
  - PubSubClient
  - ArduinoJson
  - WebServer

## Instalação

1. Clone o repositório
2. Instale as bibliotecas necessárias
3. Configure as credenciais WiFi e MQTT
4. Faça upload do código para os ESP32s

## Uso

### Publisher
1. Acesse a interface web (http://IP_DO_ESP32)
2. Use os botões para controlar o relé
3. Gerencie as roles na página de configuração

### Subscriber
1. Conecte o relé ao pino definido
2. O ESP32 se conectará automaticamente ao MQTT
3. Processará comandos baseado nas roles configuradas

## Tópicos MQTT

1. **Comandos** (`etec/hortolandia/rele/cmd`):
   ```json
   {
       "payload": "ON/OFF",
       "timestamp": 1234567890,
       "roles": ["admin", "professor", "monitor"]
   }
   ```

2. **Roles** (`etec/hortolandia/rele/roles`):
   ```json
   {
       "roles": ["admin", "professor", "monitor"]
   }
   ```

## Sistema de Roles

### Roles Predefinidas
- admin
- professor
- aluno
- visitante
- monitor

### Comportamento
- Sem roles configuradas: aceita todos os comandos
- Com roles configuradas: aceita apenas comandos com roles permitidas
- Nova configuração de roles sobrescreve a anterior

## Segurança

### Timestamp
- Mensagens são válidas por 5 segundos
- Evita replay attacks

### Roles
- Controle de acesso baseado em roles
- Configuração dinâmica via MQTT

## Limitações

1. Broker MQTT público sem autenticação
2. Mensagens não criptografadas
3. Timestamp baseado em millis() do ESP32

## Próximos Passos

1. Implementar autenticação MQTT
2. Adicionar criptografia
3. Sincronizar timestamp com NTP

## Contribuição

1. Faça um fork do projeto
2. Crie uma branch para sua feature
3. Faça commit das mudanças
4. Faça push para a branch
5. Abra um Pull Request

## Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

## Contato

Para dúvidas ou sugestões, abra uma issue no GitHub.