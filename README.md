# 🌟 Projeto de Comunicação Segura entre ESP32 via MQTT  
**Um sistema para iniciantes aprenderem IoT, mensageria e segurança digital!**  

---

## 🧩 **O Que Este Projeto Faz?**  
Imagine dois dispositivos ESP32 conversando entre si pela internet:  
1. **ESP32 "Remetente"**:  
   - Cria uma página web para você controlar um relé (liga/desliga)  
   - Envia comandos via MQTT (como um "carteiro digital")  
   - **Versão segura**: Assina digitalmente as mensagens (como um "selo de autenticidade")  

2. **ESP32 "Destinatário"**:  
   - Recebe os comandos via MQTT  
   - Controla um relé físico de verdade  
   - **Versão segura**: Verifica se a mensagem é confiável antes de executar  

---

## 📚 **O Que Você Vai Aprender**  
✅ Comunicar dispositivos via protocolo MQTT  
✅ Criar interfaces web simples para IoT  
✅ Controlar dispositivos físicos (relés)  
✅ Entender princípios básicos de segurança digital  
✅ Proteger comunicações em redes públicas  

---

## 🗂 **Estrutura do Projeto**  
Organizamos o código em pastas para facilitar o aprendizado:  

```plaintext
Projeto_ESP32_MQTT_Seguro/  
│  
├── 1_Basico/  
│   ├── Remetente/          # ESP32 com página web e envio simples  
│   └── Destinatario/       # ESP32 que recebe comandos básicos  
│  
├── 2_Seguranca_Avancada/  
│   ├── Remetente_Assina/   # Adiciona assinatura digital às mensagens  
│   └── Destinatario_Verifica/  # Verifica as assinaturas recebidas  
│  
├── Diagramas/              # Esquemas de conexão e fluxo de dados  
├── Documentacao/           # Guias complementares e tutoriais  
└── Recursos/               # Bibliotecas e ferramentas recomendadas  
```

---

## 🔌 **Componentes Necessários**  
- 2× ESP32 (com WiFi)  
- 2× Módulo Relé 5V  
- Jumpers e fonte de energia  
- Broker MQTT público (ex: broker.hivemq.com)  

---

## 🛠 **Como Funciona?**  
### Versão Básica (Pasta `1_Basico`):  
1. **Remetente**:  
   - Acesse a página web no ESP32  
   - Clique em um botão para ligar/desligar  
   - O ESP32 envia "ON" ou "OFF" via MQTT  

2. **Destinatário**:  
   - Fica "escutando" o broker MQTT  
   - Quando recebe "ON/OFF", aciona o relé físico  

### Versão Segura (Pasta `2_Seguranca_Avancada`):  
1. **Remetente**:  
   - Gera uma "assinatura única" para cada comando  
   - Envia: Mensagem + Assinatura + Timestamp  

2. **Destinatário**:  
   - Verifica a assinatura usando uma "chave pública"  
   - Só executa se a mensagem for autêntica  

---

## ⚡ **Primeiros Passos**  
1. **Configure o Hardware**:  
   - Conecte os relés aos ESP32s (GPIO23 + GND + VCC)  
2. **Preparação do Software**:  
   - Instale o Arduino IDE ou PlatformIO  
   - Adicione as bibliotecas necessárias (WiFi, MQTT, ECCX08)  
3. **Escolha sua Versão**:  
   - Comece pela pasta `1_Basico` para entender o básico  
   - Depois explore `2_Seguranca_Avancada`  

---

## 🔒 **Por Que a Versão Segura?**  
Protege contra:  
- 🕵️♂️ **Spoofing**: Alguém fingindo ser seu ESP32  
- 🔁 **Replay Attacks**: Reutilização de mensagens antigas  
- ✏️ **Alteração de Comandos**: Mudar "ON" para "OFF" no caminho  

---

## 💡 **Dicas para Iniciantes**  
1. Comece testando sem os relés (use LEDs como simulação)  
2. Use o monitor serial para ver as mensagens MQTT  
3. Teste com broker público antes de criar um local  
4. Anote a chave pública gerada pelo primeiro ESP32!  

---

## 🆘 **Precisa de Ajuda?**  
1. Verifique as conexões WiFi/MQTT  
2. Confira se os tópicos MQTT estão iguais em ambos códigos  
3. Use ferramentas como MQTT Explorer para debug  
4. Pergunte aos colegas ou professores!  

🚀 **Divirta-se explorando o mundo da IoT segura!**