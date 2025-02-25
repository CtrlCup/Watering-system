/* 
  #########################################
  #         readOut |     by Alex         #
  #         v. 1.0                        #
  #########################################
 */

#include <Preferences.h>

#define reset false // Reset all Counter

Preferences preferences;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  Serial.println("\n Read out ESP Values:");
  preferences.begin("waterings", false);
  int waterings = preferences.getInt("waterings", 0);
  int dry_runs = preferences.getInt("dry_runs", 0);
  
  Serial.println("");
  Serial.print("Gesamtanzahl erfolgreicher Bewässerungen: ");
  Serial.println(waterings);
  Serial.print("Fehlgeschlagene Versuche (Wassertank leer): ");
  Serial.println(dry_runs);

  if (reset)
  {
    preferences.clear();
    waterings = preferences.getInt("waterings", 0);
    dry_runs = preferences.getInt("dry_runs", 0);
    
    Serial.println("\nNeue Werte:");
    Serial.print("Gesamtanzahl erfolgreicher Bewässerungen: ");
    Serial.println(waterings);
    Serial.print("Fehlgeschlagene Versuche (Wassertank leer): ");
    Serial.println(dry_runs);
  }

  preferences.end();

}

void loop() {
  // put your main code here, to run repeatedly:
  while(true)
  {
  }
}
