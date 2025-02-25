/* 
  #########################################
  #      Bewesserungssystem by Alex       #
  #      v. 1.0                           #
  #########################################
 */

 #define SOIL_SENSOR_PIN 0
 #define led 8
 #define waterPump 1
 #define waterSensor 3
 #define waterON 2200
 #define waterOFF 1700
 #define buzzer 2
 
 // Nasse Erde 1660    Wasser ist unten im Topf angekommen
 // Außerhalb vom Blumentopf => 3050
 
 
 void blinkLED()
 {
   for (int i = 0; i < 3; i++)
   {
     digitalWrite(led, HIGH);
     delay(100);
     digitalWrite(led, LOW);
     delay(100);
   }  
 }
 
 void setup() {
     Serial.begin(115200);
     Serial.println("ESP32 booting...");
     pinMode(SOIL_SENSOR_PIN, INPUT);
     pinMode(led, OUTPUT);
     pinMode(waterPump, OUTPUT);
     pinMode(waterSensor, INPUT);
     pinMode(buzzer, OUTPUT);
 
     int sensorValue = analogRead(SOIL_SENSOR_PIN);
     float voltage = sensorValue * (3.3 / 4095.0);
     bool waterEmpty = false;
     //int waterSensorINT = analogRead(waterSensor);
 
     Serial.print("Sensor Value: ");
     Serial.print(sensorValue);
     Serial.print(" | Voltage: ");
     Serial.println(voltage);
 
   for (int i = 0; i < 3; i++)
   {
     sensorValue = analogRead(SOIL_SENSOR_PIN);
     if (sensorValue < waterON) {
         digitalWrite(led, HIGH);
         digitalWrite(waterPump, LOW);
     } else if (sensorValue > waterOFF) {
         digitalWrite(led, LOW);
         digitalWrite(waterPump, HIGH);
     }
 
     // Zusätzliche Wartezeit
     delay(5000); // 5 Sekunden warten, um serielle Ausgaben zu sehen
     sensorValue = analogRead(SOIL_SENSOR_PIN);
     if ((sensorValue < waterOFF))
     {
       break;
       waterEmpty = false;
     }
     else
     {
       if (i == 2) waterEmpty = true;
     }
     
     digitalWrite(led, HIGH);
     digitalWrite(waterPump, LOW);
   }
 
   if (waterEmpty)
   {
     for (int i = 0; i < 3; i++)
     {
       digitalWrite(buzzer, HIGH);
       delay(300);
       digitalWrite(buzzer, LOW);
       delay(200);
     }
     waterEmpty = false;
     blinkLED();
     Serial.println("Wasserbehälter leer");
     esp_sleep_enable_timer_wakeup(1 * 1000000); // 1 Stunde
     esp_deep_sleep_start();
   }
   else
   {
     blinkLED();
     Serial.println("Gehe in den Tiefschlaf");
     esp_sleep_enable_timer_wakeup(4 * 1000000); // 15 Sekunden
     esp_deep_sleep_start();
   }
 }
 
 void loop() {
     // Loop bleibt leer
 }
 