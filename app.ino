#include "keymap.h"
#include <MiniCom.h>
#include <Keypad.h>
#include <Led.h>
#include <SimpleTimer.h>
#include <Servo.h>
#include <storage.h>
#include <Button.h>

const int BASE_ADDRESS = 100;

Led beep(13);
MiniCom com;
int timerId = -1; //타임 아웃용 타이머 아이디
Button btn(2);

Servo lock;
Keypad keypad = Keypad( makeKeymap(keys),rowPins,colPins,ROWS,COLS);

bool bInput = false;
String input = "";
String password = "";
String inputstar = "";
bool bSetPassword = false;

void setup(){
    com.init();
    com.print(0, "[[Door Lock]]");
    password = readRom(BASE_ADDRESS);
    lock.attach(3);
    lock.write(0);
    com.offLcd();
    btn.setCallback(setPassword);
}

void reset() {
    // com.print(1,"");
    input = "";
    bInput = false;
    com.offLcd();
}
void process(char key) {
    tick();
    SimpleTimer& timer = com.getTimer();
    if(key == '*' && bInput ==false){
        com.onLcd();
        input = "";
        inputstar = "";
        bInput = true;
        timerId = timer.setTimeout(5000, reset);
        // com.print(1, "");
    }
    else if (key =='#'){
        timer.deleteTimer(timerId);
        bInput = false;
        check();
        delay(1000);
        com.offLcd();
    }else if(bInput){
        timer.restartTimer(timerId);
        input += key;
        inputstar += "*";
        com.print(1, inputstar.c_str());
    }

}

void loop(){
    btn.check();
    com.run();
    char key = keypad.getKey();
    
    if (key){
        process(key);
    }
}

void check() {
    if(input == password) {
        lock.write(90);
        com.print(1,"open");
        delay(5000);
        lock.write(0);
        com.print(1, "");
    }else{ //비밀번호 불일치
        delay(1000);
        for(int i =0; i<4; i++){
            tick();
            delay(100);
        }
        com.print(1,"wrong password");
        delay(3000);
        com.print(1, "");
    }
}
void tick() {
    beep.on();
    delay(100);
    beep.off();
}
void setPassword(){
    bSetPassword = !bSetPassword;
    if(bSetPassword) {
        
        input ="";
        inputstar="";
        bInput = true;
        com.onLcd();
        
    }else{
        //  ROM에 비밀번호 저장
        bInput = false;
        password = input;
        writeRom(BASE_ADDRESS, password);
        com.print(1,"");
        com.offLcd();
    }
}