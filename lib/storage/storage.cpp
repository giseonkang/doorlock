#include <EEPROM.h>
#include <Arduino.h>




void writeRom(int address, String value){
    int length = value.length();

    EEPROM.write(address, length);

    for(int ix =1; ix <= length; ix++){
        EEPROM.write(address+ix,value.charAt(ix-1));
        delay(100);
    }
}


String readRom(int address){
    String value = "";
    int length = EEPROM.read(address);

    for(int ix =1; ix <= length; ix++){
        value += (char)EEPROM.read(address+ix);
        delay(100);
    }
    return value;
}