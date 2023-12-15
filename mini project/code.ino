#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#define REPORTING_PERIOD_MS 1000
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

PulseOximeter pox;

float BPM, SpO2;
uint32_t tsLastReport = 0;


char auth[] = "3aZk_EPSKybgsNBSYF2R6ET3qTJbF9LH"; 
char ssid[] = "Realme";                          
char pass[] = "hi22me03";                        


void onBeatDetected()
{
    Serial.println("Beat Detected!");

}

void setup()
{
    Serial.begin(115200);

    Blynk.begin(auth, ssid, pass, IPAddress(117, 236, 190, 213), 8080);

    Wire.begin(); 

    display.begin();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(0, 0);

    display.println("Initializing pulse oximeter..");
    display.display();
    Serial.print("Initializing pulse oximeter..");

   

    if (!pox.begin())
    {
        Serial.println("FAILED");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(1);
        display.setCursor(0, 0);
        display.println("FAILED");
        display.display();
        for (;;);
    }
    else
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(1);
        display.setCursor(0, 0);
        display.println("SUCCESS");
        display.display();
        Serial.println("SUCCESS");
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    pox.update();
    Blynk.run();

    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print("SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");

        Blynk.virtualWrite(V1, BPM);
        Blynk.virtualWrite(V2, SpO2);


        display.clearDisplay();

        display.setTextSize(1);
        display.setTextColor(1);
        display.setCursor(0, 8);
        display.print("BPM: ");
        display.print(BPM);

        display.setTextSize(1);
        display.setTextColor(1);
        display.setCursor(0, 24);
        display.print("SpO2: ");
        display.print(SpO2);


        display.display();
        tsLastReport = millis();
    }
}
