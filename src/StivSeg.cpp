#include "Arduino.h"
#include "StivSeg.h"

/*TERMINOLOGY
 *seg = a segment on the display
 *dig = a digit on the display
 *RHDP = right hand decimal point
 */

StivSeg::StivSeg(int dp[], int sp[], int latchPin, int clockPin, int dataPin, bool isCath, bool _useShiftRegister,int refreshRate)
{
    numDigits = sizeof(dp) / sizeof(int);
    isComCathode = isCath;
    useShiftRegister = _useShiftRegister;
    hz = 1000 / refreshRate;

    //stores digit pins
    for (int i = 0; i < numDigits; i++)
    {
        digitPins[i] = dp[i];
        pinMode(digitPins[i], OUTPUT);
    }

    if (useShiftRegister)
    {
        latch = latchPin;
        clock = clockPin;
        data = dataPin;

        //set shift register pins
        pinMode(latch, OUTPUT);
        pinMode(clock, OUTPUT);
        pinMode(data, OUTPUT);
    }
    else
    {
        //stores segment pins
        for (int i = 0; i <= 7; i++)
        {
            segPins[i] = sp[i];
            pinMode(segPins[i], OUTPUT);
        }
    }

    //determines common type and preps pin states
    if (!isComCathode)
    {
        for (int i = 0; i < numDigits; i++)
        {
            digitalWrite(digitPins[i], HIGH);
        }
        if (useShiftRegister)
        {
            //Shift register is locked for high latch.
            digitalWrite(latch, HIGH);
        }
        else
        {
            for (int i = 0; i <= 7; i++)
            {
                digitalWrite(segPins[i], HIGH);
            }
        }
    }
}

StivSeg::writeInt(int n)
{
    int nray[numDigits];

    //splits each digit of input and saves to a temporary array
    for (int i = 0; i < numDigits; i++)
    {
        nray[i] = n % 10;
        n /= 10;
    }

    //writes to display here, decides how to treat display based off common type
    if (isComCathode)
    {
        //cycles through each dig
        for (int dig = 0; dig < numDigits; dig++)
        {
            //cathode pulled low to allow seg illumination
            digitalWrite(digitPins[dig], LOW);

            if (useShiftRegister)
            {
                digitalWrite(latch, LOW);
                shiftOut(data, clock, MSBFIRST, nray[dig]);
                digitalWrite(latch, HIGH);
            }
            else
            {
                //cycles through each seg and sets state in accordance with array SYMBOL[][]
                for (int seg = 0; seg <= 6; seg++)
                {
                    digitalWrite(segPins[seg], SYMBOL[nray[dig]][seg]);
                }
            }
            delay(hz);
            //cathode pulled high to disallow illumination
            digitalWrite(digitPins[dig], HIGH);
        }
    }
    else
    {
        //cycles through each dig in physical display
        for (int dig = 0; dig < numDigits; dig++)
        {
            //anode pulled high to allow seg illumination
            digitalWrite(digitPins[dig], HIGH);

            if (useShiftRegister)
            {
                digitalWrite(latch, LOW);
                shiftOut(data, clock, MSBFIRST, nray[dig]);
                digitalWrite(latch, HIGH);
            }
            else
            {
                //cycles through each seg and sets state in accordance with the boolean opposite of array SYMBOL[][]
                for (int seg = 0; seg <= 6; seg++)
                {
                    digitalWrite(segPins[seg], !SYMBOL[nray[dig]][seg]);
                }
            }
            delay(hz);
            //anode pulled low to disallow illumination
            digitalWrite(digitPins[dig], LOW);
        }
    }
}

StivSeg::writeFloat(float n)
{
    int nray[numDigits];
    int pointPos = numDigits + 1;

    //determines which physical display digit will "own" the decimal point (based off RHDP)
    while (n > 1.0)
    {
        n /= 10.0;
        pointPos--;
    }
    //converts input to integer
    n *= (float)pow(10.0, (float)numDigits);

    for (int i = 0; i < numDigits; i++)
    {
        nray[i] = (int)n % 10;
        n /= 10;
    }

    if (isComCathode)
    {
        for (int dig = 0; dig < numDigits; dig++)
        {
            digitalWrite(digitPins[dig], LOW);

            for (int seg = 0; seg <= 6; seg++)
            {
                digitalWrite(segPins[seg], SYMBOL[nray[dig]][seg]);
            }

            //sets RHDP
            if (pointPos == dig + 1)
            {
                digitalWrite(segPins[7], HIGH);
            }
            else
            {
                digitalWrite(segPins[7], LOW);
            }
            delay(hz);
            digitalWrite(digitPins[dig], HIGH);
        }
    }
    else
    {
        for (int dig = 0; dig < numDigits; dig++)
        {
            digitalWrite(digitPins[dig], HIGH);

            for (int seg = 0; seg <= 6; seg++)
            {
                digitalWrite(segPins[seg], !SYMBOL[nray[dig]][seg]);
            }

            //sets RHDP
            if (pointPos == dig + 1)
            {
                digitalWrite(segPins[7], LOW);
            }
            else
            {
                digitalWrite(segPins[7], HIGH);
            }
            delay(hz);
            digitalWrite(digitPins[dig], LOW);
        }
    }
}

StivSeg::clear()
{
    //sets all dig pins and seg pins to low
    for (int dig = 0; dig < numDigits; dig++)
    {
        if (isComCathode)
            digitalWrite(digitPins[dig], HIGH);
        else
            digitalWrite(digitPins[dig], LOW);

        if (useShiftRegister)
        {
            digitalWrite(latch, LOW);
            shiftOut(data, clock, MSBFIRST, digit_map[15]); //Set blank
            digitalWrite(latch, HIGH);
        }
        else
        {
            for (int seg = 0; seg <= 7; seg++)
            {
                digitalWrite(segPins[seg], LOW);
            }
        }
    }
}