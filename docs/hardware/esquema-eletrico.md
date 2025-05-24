# Esquema Elétrico

Este documento descreve o esquema elétrico do sistema de controle de relé via MQTT.

## Componentes Necessários

### Publisher (ESP32 com Web Server)
- 1x ESP32
- 1x Cabo USB (para programação)
- Conexão WiFi

### Subscriber (ESP32 com Relé)
- 1x ESP32
- 1x Módulo Relé 5V
- 1x Lâmpada 220V
- 1x Fonte de alimentação 5V
- Jumpers
- Cabo USB (para programação)

## Diagrama de Conexão

### Publisher
```
ESP32                    WiFi
+---------------+       +-------+
|               |       |       |
|  GPIO23 (LED) |       |       |
|               |       |       |
|  GPIO2 (LED)  |       |       |
|               |       |       |
+---------------+       +-------+
```

### Subscriber
```
ESP32                    Relé                    Lâmpada
+---------------+       +-------+               +-------+
|               |       |       |               |       |
|  GPIO23 (Relé)|<----->|IN     |               |       |
|               |       |       |               |       |
|               |       |COM    |<------------->|       |
|               |       |NO     |               |       |
|               |       |       |               |       |
+---------------+       +-------+               +-------+
                        |       |
                        |VCC    |<---- 5V
                        |GND    |<---- GND
                        |       |
                        +-------+
```

## Instruções de Conexão

### Publisher
1. Conecte o ESP32 ao computador via USB
2. Não são necessárias conexões adicionais
3. O ESP32 se conectará à rede WiFi

### Subscriber
1. Conecte o ESP32 ao computador via USB
2. Conecte o pino GPIO23 do ESP32 ao pino IN do módulo relé
3. Conecte o pino 5V do ESP32 ao pino VCC do módulo relé
4. Conecte o pino GND do ESP32 ao pino GND do módulo relé
5. Conecte a lâmpada 220V aos pinos COM e NO do módulo relé
6. Conecte a fonte de alimentação 5V ao módulo relé

## Pinagem

### ESP32
- GPIO23: Saída digital para controle do relé
- GPIO2: LED indicador (opcional)
- 5V: Alimentação do módulo relé
- GND: Terra

### Módulo Relé
- IN: Entrada de controle (conecta ao GPIO23)
- VCC: Alimentação 5V
- GND: Terra
- COM: Terminal comum
- NO: Terminal normalmente aberto

## Observações de Segurança

1. **ATENÇÃO**: O módulo relé trabalha com 220V
2. Mantenha as conexões 220V isoladas
3. Use fios com bitola adequada
4. Verifique todas as conexões antes de energizar
5. Não toque nos terminais quando energizado

## Solução de Problemas

### Relé não aciona
1. Verifique a alimentação 5V
2. Confirme a conexão do pino IN
3. Teste o relé com multímetro

### Lâmpada não acende
1. Verifique a tensão 220V
2. Confirme as conexões COM e NO
3. Teste a lâmpada em outra tomada

## Especificações Técnicas

### ESP32
- Tensão de operação: 3.3V
- Corrente máxima por pino: 40mA
- Tensão lógica: 3.3V

### Módulo Relé
- Tensão de operação: 5V
- Corrente de operação: 20mA
- Tensão de comutação: 250VAC
- Corrente de comutação: 10A

### Lâmpada
- Tensão: 220V
- Potência: 60W (máx)
- Corrente: 0.27A (máx) 