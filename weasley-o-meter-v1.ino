/*
 * Weasley-o-Meter Location/State Indicator for Spark Core and Internet Button
 * Jon Davies 2014
 *
 * Set a specific position of the indicator with the following Curl command:
 * curl https://api.spark.io/v1/devices/{device_id}/sfMtp -d access_token={token} -d "args={position}"
 * Set a random position of the indicator with the following Curl command:
 * curl https://api.spark.io/v1/devices/{device_id}/sfMtrp -d access_token={token}
 * 
 * Where...
 * {device_id} = The Device ID of the Spark Core (Browse to https://www.spark.io/build/ and select the "Cores" icon)
 * {token}     = Your Access Token (Browse to https://www.spark.io/build/ and select the "Settings" icon)
 * {position}  = Zero-based indicator/pixel position from 0 to 10 (total of 11 positions).
 */

// This #include statement was automatically added by the Spark IDE.
#include "neopixel/neopixel.h"
// These #includes added by Jon :)
#include "application.h"
#include "math.h"

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_COUNT 11
#define PIXEL_PIN A7 // Pin used on the Internet Button
#define PIXEL_TYPE WS2812B
//Weasley-o-Meter specific constant(s)
#define PIXEL_XFADE 127

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
//               note: if not specified, D2 is selected for you.
// Parameter 3 = pixel type [ WS2812, WS2812B, WS2811, TM1803 ]
//               note: if not specified, WS2812B is selected for you.
//               note: RGB order is automatically applied to WS2811,
//                     WS2812/WS2812B/TM1803 is GRB order.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int curPos; // Current position of pixel
int destPos; // Destination position of pixel

void setup() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    curPos=0;
    destPos=0;
    Spark.function("sfMtrp", sfMtrp);
    Spark.function("sfMtp", sfMtp);
}

//Spark Function Move To Random Position
int sfMtrp(String args){
    moveToRandPos();
    return curPos/PIXEL_XFADE;
}

//Spark Funtion Move To Position
int sfMtp(String args){
    destPos=args.toInt();
    destPos*=PIXEL_XFADE;
    moveToPos(curPos,destPos);
    curPos=destPos;
    return curPos/PIXEL_XFADE;
}

//set position ([PIXEL_COUNT positions] x [PIXEL_XFADE light levels])
void setPos(int pos){
    float curPos=((float)pos)/((float)PIXEL_XFADE);
    int pos1=floorf(curPos);
    int pos2=ceilf(curPos);
    if(pos1==(PIXEL_COUNT-1) && pos2==PIXEL_COUNT){
        strip.setPixelColor(pos1, (PIXEL_XFADE-1)-(pos%PIXEL_XFADE), 0, 0);
        strip.setPixelColor(0, (pos-1)%PIXEL_XFADE, 0, 0);
    }else if(pos1!=pos2){
        strip.setPixelColor(pos1, (PIXEL_XFADE-1)-(pos%PIXEL_XFADE), 0, 0);
        strip.setPixelColor(pos2, (pos-1)%PIXEL_XFADE, 0, 0);
    }else{
        strip.setPixelColor(pos2, PIXEL_XFADE, 0, 0);
    }
    strip.show();
}

//move light from curPos to destPos by smooth 'scroll' transition
void moveToPos(int curPos, int destPos){
    //simple version
    if(curPos<destPos){//clockwise
        for(int i=curPos; i<=destPos; i++){
            setPos(i);
            delay(2);
        }
    }
    if(curPos>destPos){//anticlockwise
        for(int i=curPos; i>=destPos; i--){
            setPos(i);
            delay(2);
        }
    }
}

//move light to random position
int moveToRandPos(){
    destPos=random(PIXEL_COUNT);
    destPos*=PIXEL_XFADE;
    moveToPos(curPos, destPos);
    curPos=destPos;
}

void loop() {
}
