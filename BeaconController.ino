#include <IRremote.h>
const int IRrecvPin = 11; 
const int IRsendPin = 10;
IRrecv irrecv(IRrecvPin); 
decode_results results; 

IRsend irsend(IRsendPin);
unsigned long ON_HEX_CODE = 0xBA45FF00;
unsigned long OFF_HEX_CODE = 0xB946FF00;
unsigned long RED_HEX_CODE = 0xE916FF00;
unsigned long GREEN_HEX_CODE = 0xE619FF00;
unsigned long BLUE_HEX_CODE = 0xF20DFF00;
unsigned long YELLOW_HEX_CODE = 0xF30CFF00;

const char OFF_COMMANDO = '0';
const char ON_COMMANDO = '1';
const char RED_COMMANDO = '2';
const char GREEN_COMMANDO = '3';
const char BLUE_COMMANDO = '4';
const char YELLOW_COMMANDO = '5';

void setup() {
  Serial.begin(115200); 
  irrecv.enableIRIn(); 
  irrecv.begin(IRrecvPin, ENABLE_LED_FEEDBACK);
  irsend.begin(IRsendPin);
  Serial.println("new code:-----");
}

const int numberOfRepeats = 4;

void loop() {
  char data;
  if(Serial.available()> 0){
    data = Serial.read();
    Serial.println(data);
    switch (data){
      case OFF_COMMANDO:
        irsend.sendNECRaw(OFF_HEX_CODE, numberOfRepeats);    //OFF
        break;
      case ON_COMMANDO:
        irsend.sendNECRaw(ON_HEX_CODE, numberOfRepeats);    //ON
        break;
      case RED_COMMANDO:
        irsend.sendNECRaw(RED_HEX_CODE, numberOfRepeats);    //RED
        break;
      case GREEN_COMMANDO:
        irsend.sendNECRaw(GREEN_HEX_CODE, numberOfRepeats);    //GREEN
        break;
      case BLUE_COMMANDO:
        irsend.sendNECRaw(BLUE_HEX_CODE, numberOfRepeats);    //BLUE
        break;
      case YELLOW_COMMANDO:
        irsend.sendNECRaw(YELLOW_HEX_CODE, numberOfRepeats);    //YELLOW
        break;
      default:
        break;
    }
  }
  Serial.println("loop");
  delay(100);

}