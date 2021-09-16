#include <Button.h>

Button::Button(int pin): pin(pin){
    pinMode(pin, INPUT_PULLUP);
    callback = NULL;

}

void Button::setCallback(button_callback_t callback){
    this->callback = callback;
}

int Button::read(){
    return !digitalRead(pin);
}

void Button::check(){
    bool o_sw,n_sw;

    o_sw = read();
    delay(10);
    n_sw = read();

    if(o_sw ==0 && n_sw ==1){
        if(callback != NULL){
            callback();
        }
    }
}

void Button::attachInterrupt(button_callback_t callback, int mode){
    ::attachInterrupt(digitalPinToInterrupt(pin), callback, mode);
    t1 = millis();
}

boolean Button::debounce(){
    unsigned long t2 = millis();
    if((t2-t1) < 200) return false;

    t1=t2;
    return true;
}