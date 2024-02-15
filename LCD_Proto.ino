/*
 Name:		LCD_Proto.ino
 Created:	2/11/2024 8:10:27 PM
 Author:	DSims
*/


#include <LightChrono.h>
#include <Chrono.h>
#include <SPI.h>
#include <WireKinetis.h>
#include <WireIMXRT.h>
#include <Wire.h>
#include <RA8875_w_Touch_T4.h>

#define RA8875_CS 10		//SPI Chip Select
#define RA8875_RESET 255	//any pin or 255 to disable
#define RA8875_INT  16		//touch interrupt
#define MAXTOUCHLIMIT 3     //1...5


RA8875 tft = RA8875(RA8875_CS, RA8875_RESET, 11, 13, 12); //Setup for Teensy 4.1
uint16_t coordinates[MAXTOUCHLIMIT][2];//Holds touch point coordinates
int screenTouched = 0;
bool enableKeypadTouch = true;
int touchCheckReady = 0;
Chrono touchCheckTimer;
bool DemoRunning = false;

void setup() {
	Serial.begin(38400);

	tft.begin(RA8875_800x480);
	tft.fillWindow(RA8875_WHITE);
	tft.setFontScale(2);
	tft.setCursor(400, 160, true);
	tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
	tft.print("7\" LCD with Mounting Case!");
    tft.setCursor(400, 260, true);
    tft.print("-Capacitive Touchscreen-");
    tft.setCursor(400, 360, true);
    tft.print("Touch screen to start Demo");

#if defined(USE_FT5206_TOUCH)
    tft.useCapINT(RA8875_INT);
    tft.setTouchLimit(MAXTOUCHLIMIT);
    tft.enableCapISR(true);// Turn on the capacitive touch screen interrupt
#else
    tft.setTextColor(RA8875_WHITE, RA8875_BLACK);
    tft.print("Error USE_FT5206_TOUCH!");
    delay(10000);
#endif

}

// the loop function runs over and over again until power down or reset
void loop() {
    CheckTouchScreen();
        if (screenTouched) {
            DemoRunning = true;
        }
    if (DemoRunning) {
        testdrawrects();
        testtriangles(false);
        testtriangles(true);
        randomRect(false);
        randomRect(true);
        randomCircles(false);
        randomCircles(true);
        randomLines();
        randomPoints();
        delay(100);
        tft.fillWindow(RA8875_WHITE);
        tft.setFontScale(2);
        tft.setCursor(400, 160, true);
        tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
        tft.print("7\" LCD with Mounting Case!");
        tft.setCursor(400, 260, true);
        tft.print("-Capacitive Touchscreen-");
        tft.setCursor(400, 360, true);
        tft.print("Touch screen to start Demo");
        DemoRunning = false;
    }
    
}

void testdrawrects() {
    tft.fillWindow();
    for (int k = 0; k < 10; k++) {
        for (uint16_t x = 1; x < tft.height(); x += 6) {
            uint16_t c1 = random(0x00FF, 0xFFFF);
            tft.drawRect((uint16_t)((tft.width() / 2) - (x / 2)), (uint16_t)((tft.height() / 2) - (x / 2)), x, x, c1);
        }
    }
}

void randomCircles(bool fill) {
    tft.fillWindow();
    uint8_t k, c;
    for (k = 0; k < 10; k++) {
        for (c = 0; c < 32; c++) {
            //  coordinates
            uint16_t x = random(0, tft.width() - 1);
            uint16_t y = random(0, tft.height() - 1);

            uint16_t r = random(1, tft.height() / 2);

            if (x - r < 0) r = x;
            if (x + r > (tft.width() - 1)) r = (tft.width() - 1) - x;
            if (y - r < 0) r = y;
            if (y + r > (tft.height() - 1)) r = (tft.height() - 1) - y;

            if (fill) {
                tft.fillCircle(x, y, r, random(0x0010, 0xFFFF));
            }
            else {
                tft.drawCircle(x, y, r, random(0x00FF, 0xFFFF));
            }
        }
        if (!fill) tft.fillWindow();
    }
}

void randomRect(bool fill) {
    tft.fillWindow();
    uint16_t k, c;
    for (k = 0; k < 40; k++) {
        for (c = 0; c < 32; c++) {
            uint16_t cx, cy, x, y, w, h;
            //  center
            cx = random(0, tft.width() - 1);
            cy = random(0, tft.height() - 1);
            //  size
            w = random(1, tft.width() / 3);
            h = random(1, tft.height() / 3);
            //  upper-left
            x = cx - w / 2;
            y = cy - h / 2;
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            //  adjust size
            if (x + w >= tft.width()) w = tft.width() - 1 - x;
            if (y + h >= tft.height()) h = tft.height() - 1 - y;
            if (fill) {
                tft.fillRect(x, y, w, h, random(0x0010, 0xFFFF));
            }
            else {
                tft.drawRect(x, y, w, h, random(0x0010, 0xFFFF));
            }

        }
        tft.fillWindow();
    }
}


void randomLines() {
    tft.fillWindow();
    uint16_t k, c;
    for (k = 0; k < 120; k++) {
        for (c = 0; c < 32; c++) {
            uint16_t x1 = random(0, tft.width() - 2);
            uint16_t y1 = random(0, tft.height() - 2);
            uint16_t x2 = random(0, tft.width() - 1);
            uint16_t y2 = random(0, tft.height() - 1);
            tft.drawLine(x1, y1, x2, y2, random(0x0010, 0xFFFF));
        }
        tft.fillWindow();
    }
}


void randomPoints() {
    tft.fillWindow();
    uint16_t k, c;
    for (k = 0; k < 40; k++) {
        for (c = 0; c < 1000; c++) {
            uint16_t x = random(0, tft.width() - 1);
            uint16_t y = random(0, tft.height() - 1);
            tft.drawPixel(x, y, random(0x0010, 0xFFFF));
        }
        tft.fillWindow();
    }
}

void testtriangles(bool fill) {
    tft.fillWindow();
    uint16_t p1x, p1y, p2x, p2y, p3x, p3y;
    uint16_t colour;
    for (uint16_t k = 0; k < 40; k++) {
        for (uint16_t t = 0; t <= 30; t += 1) {
            p1x = random(0, tft.width() - 1);   //get a random number 0-319
            p1y = random(0, tft.height() - 1);  //get a random number 0-239
            p2x = random(0, tft.width() - 1);   //get a random number 0-319
            p2y = random(0, tft.height() - 1);  //get a random number 0-239
            p3x = random(0, tft.width() - 1);   //get a random number 0-319
            p3y = random(0, tft.height() - 1);  //get a random number 0-239
            colour = random(0, 65536);       //get a random number 0-65535
            //draw the triangle
            if (fill) {
                tft.fillTriangle(p1x, p1y, p2x, p2y, p3x, p3y, colour);
            }
            else {
                tft.drawTriangle(p1x, p1y, p2x, p2y, p3x, p3y, colour);
            }
        }
        tft.clearScreen();
    }
}

uint16_t halveColor(uint16_t rgb) {
    return (((rgb & 0b1111100000000000) >> 12) << 11 | ((rgb & 0b0000011111100000) >> 6) << 5 | ((rgb & 0b0000000000011111) >> 1));
}



void CheckTouchScreen() {

    screenTouched = 0;
    if (touchCheckReady) {		//Timer on touchCheck Ready limits Touch Interrupt Check

        if (tft.touched(true)) {     //touched(true) detaches isr

            tft.updateTS();//Get data from FT5206 Registers


            if (tft.getTScoordinates(coordinates) == 1) { //Only Use Data with One Touch Point
                tft.getTScoordinates(coordinates);//done

                if (tft.getTouchState() == 2)   // 2 = Contact State
                {
                    screenTouched = 1;

                    if (false) {  // For Debug
                        tft.fillCircle(coordinates[0][0], coordinates[0][1], 6, RA8875_RED);
                    }

                }
            }

            tft.enableCapISR();//rearm ISR
        }
        touchCheckReady = false;
    }

    if (touchCheckTimer.hasPassed(10,true)) {
        touchCheckReady = true;
    }

}