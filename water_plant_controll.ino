#define LORA_RX 44
#define LORA_TX 43

#define RELAY_PIN 4

#define AOUT_PIN 6
#define THRESHOLD 1488

String inMessage = "";

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);
  delay(1000);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);

  // Serial.println("Send AT...");
  // Serial2.println("AT\r\n");
  // Serial2.println("AT+RESET\r\n");
}

void loop() {
  // read the analog value from soil moisture sensor
  int moisture_value = analogRead(AOUT_PIN);

  if (Serial1.available()) {
    // String inMessage = Serial2.readString();
    // Serial.print("Received: ");
    // Serial.println(inMessage);
    
    char incomingChar = Serial1.read();

    inMessage += String(incomingChar);
    // Serial.println(inMessage);

    if (incomingChar == '\n') {
      
      inMessage.trim();
      
      if(inMessage == "water"){
        // The soil is dry
        // if (moisture_value > THRESHOLD){
          // Turn on water pump motor
          digitalWrite(RELAY_PIN, HIGH);
          // delay(1000);
          // digitalWrite(RELAY_PIN, LOW);
        // }
      }else if(inMessage == "stop water"){
        digitalWrite(RELAY_PIN, LOW);
      }else if(inMessage == "data"){
        Serial1.println("moisture:"+moisture_value);
      }

      inMessage = "";
    }
  }
}