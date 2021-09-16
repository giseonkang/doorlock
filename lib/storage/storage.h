#pragma once

#include <Arduino.h>

void writeRom(int address, String value);
String readRom(int address);