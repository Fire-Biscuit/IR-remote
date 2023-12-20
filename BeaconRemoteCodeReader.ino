#include <IRremote.h>

const int IRrecvPin = 11; 
IRrecv irrecv(IRrecvPin); 
decode_results results; 

void setup() {
  Serial.begin(115200); 
  irrecv.enableIRIn();
  irrecv.begin(IRrecvPin, ENABLE_LED_FEEDBACK);
  Serial.println("new code:-----");
}

void loop() {
  Serial.println("loop");

  if (irrecv.decode()) {
    //Serial.println(results.value, HEX);
    Serial.println(irrecv.decodedIRData.decodedRawData,DEC);
    Serial.println(irrecv.decodedIRData.decodedRawData,HEX);
    irrecv.printIRResultShort(&Serial); 
    irrecv.printIRSendUsage(&Serial);
    Serial.println("-------------------");
    irrecv.resume();
  }  

  delay(1500);
}
/*
results:
ON = 3125149440
BA45FF00

OFF = 3108437760
B946FF00

RED = 3910598400
E916FF00

GREEN = 3860463360
E619FF00

BLUE = 4061003520
F20DFF00

ORANGE = 4077715200
F30CFF00

*/
