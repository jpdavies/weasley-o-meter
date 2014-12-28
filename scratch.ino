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

//cloud code notes
//use linux command prompt (ssh to jpd.me.uk/fire up linux vm) or install windows version of curl
//curl --get https://api.spark.io/v1/devices -d access_token=9f3f2a818d5eed693b4ada61e59637421d33cb12
//curl https://api.spark.io/v1/devices/53ff6a066667574819392567/sfMtrp -d access_token=9f3f2a818d5eed693b4ada61e59637421d33cb12
//curl https://api.spark.io/v1/devices/53ff6a06666757481992567/sfMtp -d access_token=9f3f2a818d5eed693b4ada61e59637421d33cb12 -d "args=1"

int curPos;
int destPos;

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
    return curPos/127;
}

//Spark Funtion Move To Position
int sfMtp(String args){
    destPos=args.toInt();
    destPos*=PIXEL_XFADE;
    moveToPos(curPos,destPos);
    curPos=destPos;
    return curPos/127;
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

//spin the light round clockwise once
void spinClockwise(){
    for(int i=0; i<(PIXEL_COUNT*PIXEL_XFADE); i++){
        setPos(i);
        delay(2);
    }
}

//spin the light round anticlockwise once
void spinAntiClockwise(){
    for(int i=(PIXEL_COUNT*PIXEL_XFADE); i>0; i--){
        setPos(i);
        delay(2);
    }
}

void loop() {
    //set random position
    //moveToRandPos();

    //spin red v2
    //spinClockwise();
    //spinAntiClockwise();
}
