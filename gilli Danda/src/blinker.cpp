#include "blinker.h"

void setHigh(int pin){
    digitalWrite(pin, HIGH);
}
void setLow(int pin){
    digitalWrite(pin, LOW);
}