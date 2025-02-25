/* 
  #########################################
  #      Bewesserungssystem by Alex       #
  #      v. 1.1                           #
  #########################################
 */
 #include <Preferences.h>

 #define SOIL_SENSOR_PIN 0
 #define waterPump 1
 #define buzzer 2
 #define led 8
 #define waterON 2200
 #define waterOFF 1700
 
 // Nasse Erde 1660    Wasser ist unten im Topf angekommen
 // Außerhalb vom Blumentopf => 3050
 
 Preferences preferences;
 
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
     while (!Serial) {
       delay(100);
     }
     Serial.println("");
     Serial.println("ESP32 booting...");
     pinMode(SOIL_SENSOR_PIN, INPUT);
     pinMode(led, OUTPUT);
     pinMode(waterPump, OUTPUT);
     pinMode(buzzer, OUTPUT);
 
     int sensorValue = analogRead(SOIL_SENSOR_PIN);
     float voltage = sensorValue * (3.3 / 4095.0);
     bool waterEmpty = false;
     int waterings = 0; 
     int dry_runs = 0; 
 
     preferences.begin("waterings", false);
 
     waterings = preferences.getInt("waterings", 0);
     dry_runs = preferences.getInt("dry_runs", 0);
 
     Serial.println("");
     Serial.print("Gesamtanzahl erfolgreicher Bewässerungen: ");
     Serial.println(waterings);
     Serial.print("Fehlgeschlagene Versuche (Wassertank leer): ");
     Serial.println(dry_runs);
     
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
         waterings++;
     } else if (sensorValue > waterOFF) {
         digitalWrite(led, LOW);
         digitalWrite(waterPump, HIGH);
     }
 
     // Zusätzliche Wartezeit
     delay(10000); // 5 Sekunden warten, um serielle Ausgaben zu sehen
     sensorValue = analogRead(SOIL_SENSOR_PIN);
     if (sensorValue < waterOFF)
     {
       break;
       waterEmpty = false;
     }
     else
     {
       if (i == 2) waterEmpty = true;
     }
     
     if (waterEmpty) dry_runs++;
     digitalWrite(led, HIGH);
     digitalWrite(waterPump, LOW);
   }
 
   preferences.putInt("waterings", waterings);
   preferences.putInt("dry_runs", dry_runs);
   
   preferences.end();
 
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
     esp_sleep_enable_timer_wakeup(5 * 1000000); // 1 Stunde
     esp_deep_sleep_start();
   }
   else
   {
     blinkLED();
     Serial.println("Gehe in den Tiefschlaf");
     esp_sleep_enable_timer_wakeup(10 * 1000000); // 15 Sekunden
     esp_deep_sleep_start();
   }
 }
 
 void loop() {
     // Loop bleibt leer
 }
 