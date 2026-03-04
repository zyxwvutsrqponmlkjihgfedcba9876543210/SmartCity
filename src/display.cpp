#include "display.h"
int BatteryTimer=6000;
int DistanceTimer=3000;
int Timerstate=BatteryTimer;

void displayStartup() {
    oled.clear();
    oled.setLayout21x8();
    oled.gotoXY(0,0);
    oled.print("Kalibr-");
    oled.gotoXY(0,1);
    oled.print("ering");
}

void displayStatus() {
    static unsigned long lastChangeDisplay = 0;
    static bool showingBattery = true;
    unsigned long nowDisplay = millis();

    // Bytt visning hvert 3 sekund
    if (nowDisplay - lastChangeDisplay >= Timerstate) {
        lastChangeDisplay = nowDisplay;
        showingBattery = !showingBattery;
        if (Timerstate==BatteryTimer) {
            Timerstate=DistanceTimer;
        }
        else if (Timerstate==DistanceTimer) {
            Timerstate=BatteryTimer;
        } 
        oled.clear();
    }
    if (showingBattery) {
            //  Batteri-visning
            oled.gotoXY(0, 0);
            oled.print("Batteri:");

            oled.gotoXY(0, 1);
            oled.print(battery_cap);
            oled.gotoXY(7, 1);
            oled.print("%");

    } else {
            //  Avstands-visning
            oled.gotoXY(0, 0);
            oled.print("Avstand:");
            oled.gotoXY(0, 1);
            oled.print(remaining_distance());
            oled.gotoXY(7, 1);
            oled.print("m");
            }
        
    }

