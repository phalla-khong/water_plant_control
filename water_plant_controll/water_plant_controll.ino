#define LORA_RX 44
#define LORA_TX 43
#define RELAY_PIN 4
#define MOISTURE_PIN 6
#define LED_PIN 7

// percentage of wet
#define THRESHOLD 30

String inMessage = "";

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);
  // delay(1000);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  pinMode(MOISTURE_PIN, INPUT);
  // set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);

  pinMode(LED_PIN, OUTPUT);

  Serial.println("Finish Setup");
  // Serial2.println("AT\r\n");
  // Serial2.println("AT+RESET\r\n");
}

void loop() {
  // read the analog value from soil moisture sensor
  int moisture_value = analogRead(MOISTURE_PIN);
  // Serial.println("Moisture: "+moisture_value);

  // Convert raw value to percentage (approximate)
  // 0% (dry) is usually higher raw value, 100% (wet) is lower raw value
  // Formula: moisture_percent = 100 - (rawValue / 40.95)
  // 4095 / 100 = 40.95
  int moisturePercent = (100 - (moisture_value / (2895 / 100))); // More precise conversion
  Serial.println("Moisture: "+ String(moisturePercent) +"%/"+String(moisture_value));

  if (Serial1.available()) {
    // String inMessage = Serial2.readString();
    // Serial.print("Received: ");
    // Serial.println(inMessage);
    
    char incomingChar = Serial1.read();

    inMessage += String(incomingChar);
    // Serial.println(inMessage);

    if (incomingChar == '\n') {
      
      inMessage.trim();
      Serial.println(inMessage);

      if(inMessage == "water"){
        // The soil is dry
        if (moisturePercent < THRESHOLD){
          digitalWrite(LED_PIN, HIGH);
          // Turn on water pump motor
          digitalWrite(RELAY_PIN, HIGH);
        }
      }else if(inMessage == "stop water"){
        digitalWrite(LED_PIN, LOW);
        // Turn off water pump motor
        digitalWrite(RELAY_PIN, LOW);
      }else if(inMessage == "query"){
        Serial1.println("moisture:"+String(moisturePercent));
      }

      inMessage = "";
    }
  }

  delay(100);
}