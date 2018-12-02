#ifndef StivSeg_h
#define StivSeg_h

#include "Arduino.h"

class StivSeg
{
  public:
    StivSeg(int dp[], int sp[], int latchPin, int clockPin, int dataPin, bool isCath, bool _useShiftRegister,int refreshRate);
    writeInt(int n);
    writeFloat(float n);
    writeShiftInt(int n);
    writeShiftFloat(float n);
    clear();

  private:
    int numDigits;
    int digitPins[4];
    int segPins[8];
    int latch;
    int clock;
    int data;
    int hz;
    bool isComCathode;
    bool useShiftRegister;
    const byte digit_map[16] =
    {
        B00111111, // 0
        B00000110, // 1
        B01011011, // 2
        B01001111, // 3
        B01100110, // 4
        B01101101, // 5
        B01111101, // 6
        B00000111, // 7
        B01111111, // 8
        B01101111, // 9
        B01110111, // A
        B01111100, // b
        B00111001, // C
        B01011110, // d
        B01111001, // E
        B01110001, // F
        B00000000  // BLANK
    };
    bool SYMBOL[11][7] = {
        //A     B      C     D     E     F     G
        {true, true, true, true, true, true, false},     //0
        {false, true, true, false, false, false, false}, //1
        {true, true, false, true, true, false, true},    //2
        {true, true, true, true, false, false, true},    //3
        {false, true, true, false, false, true, true},   //4
        {true, false, true, true, false, true, true},    //5
        {true, false, true, true, true, true, true},     //6
        {true, true, true, false, false, false, false},  //7
        {true, true, true, true, true, true, true},      //8
        {true, true, true, false, false, true, true},    //9
        //{true, true, true, false, true, true, true},    //A
        //{false, false, true, true, true, true, true},   //b
        //{true, false, false, true, true, true, false},  //C
        //{false, true, true, true, true, false, true},   //d
        //{true, false, false, true, true, true, true},   //E
        //{true, false, false, false, true, true, true},  //F
        {false, false, false, false, false, false, false} //blank
    };
};

#endif