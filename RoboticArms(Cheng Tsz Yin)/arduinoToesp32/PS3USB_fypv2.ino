/*
 Example sketch for the PS3 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */
#include <mcp2515.h>
MCP2515 mcp2515(7);
#include <SPI.h>
#include <PS3USB.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
struct can_frame Txmsg;
bool printAngle;
uint8_t state = 0;

struct sendMessage{
  char arms[6];
  bool auto1 = 0;
  };
//SoftwareSerial BTSerial(9,10);
const byte structSize = sizeof(sendMessage);
sendMessage UartMess;
String inputString;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  Txmsg.can_id  = 0x036;
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}
void loop() {
  
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
        if(PS3.getAnalogHat(LeftHatX) > 137){
         UartMess.arms[0] = 'A';
          }
        if(PS3.getAnalogHat(LeftHatX) < 117){
         UartMess.arms[0] = 'D';
          }
        if(PS3.getAnalogHat(RightHatY) > 137){
          UartMess.arms[1] = 'A';
          }
        if(PS3.getAnalogHat(RightHatY) < 117){
          UartMess.arms[1] = 'D';
          }
    }

     if(PS3.getAnalogHat(LeftHatX) <= 137 && PS3.getAnalogHat(LeftHatX) >= 117){
         UartMess.arms[0] = 'S';
        }

     if(PS3.getAnalogHat(RightHatY) <= 137 && PS3.getAnalogHat(RightHatY) >= 117){
         UartMess.arms[1] = 'S';
        }
    
    // Analog button values can be read from almost all buttons
    if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
//      Serial.print(F("\r\nL2: "));
//      Serial.print(PS3.getAnalogButton(L2));
      int read3 = PS3.getAnalogButton(L2);
      int read4 = PS3.getAnalogButton(R2);
       if(read3 > 130){
        UartMess.arms[2] = 'A';
        }
       if(read4 > 130){
        UartMess.arms[2] = 'D';
        }
      else if (!PS3.PS3NavigationConnected) {
//        Serial.print(F("\tR2: "));
//        Serial.print(PS3.getAnalogButton(R2));
      }
    }

    if (PS3.getAnalogButton(L2) < 120 && PS3.getAnalogButton(R2)<120) UartMess.arms[2] = 'S';
    
    if (PS3.getButtonClick(PS))
      //Serial.print(F("\r\nPS"));
    if (PS3.getButtonClick(TRIANGLE))
      Serial.print(F("\r\nTriangle"));
     if (PS3.getButtonClick(CROSS))
       Serial.print(F("\r\nCross"));
    if (PS3.getButtonClick(CIRCLE))
      //Serial.print(F("\r\nCircle"));
      UartMess.arms[4] = 'A';
    if (PS3.getButtonClick(SQUARE))
      //Serial.print(F("\r\nSquare"));
      UartMess.arms[4] = 'S';
    if (PS3.getButtonClick(UP)) {
      //Serial.print(F("\r\nUp"));
      PS3.setLedOff();
      PS3.setLedOn(LED4);
    }
    if (PS3.getButtonClick(RIGHT)) {
      //Serial.print(F("\r\nRight"));
      PS3.setLedOff();
      PS3.setLedOn(LED1);
    }
    if (PS3.getButtonClick(DOWN)) {
      //Serial.print(F("\r\nDown"));
      PS3.setLedOff();
      PS3.setLedOn(LED2);
    }
    if (PS3.getButtonClick(LEFT)) {
      //Serial.print(F("\r\nLeft"));
      PS3.setLedOff();
      PS3.setLedOn(LED3);
    }

    if (PS3.getButtonPress(L1) || PS3.getButtonPress(R1)){
      //Serial.print(F("\r\nL1"));
      if(PS3.getButtonPress(L1)){
        UartMess.arms[3] = 'A';
        }
      if(PS3.getButtonPress(R1)){
        UartMess.arms[3] = 'D';
        }
    }
    if((PS3.getButtonPress(L1) || PS3.getButtonPress(R1)) != 1){
         UartMess.arms[3] = 'S';
      }
    if (PS3.getButtonClick(L3))
      //Serial.print(F("\r\nL3"));
    
    if (PS3.getButtonClick(R3))
      //Serial.print(F("\r\nR3"));
    if(PS3.getButtonPress(L1) == 0 && PS3.getButtonPress(R1) == 0){
      UartMess.arms[3] = 'S';
      }

    if (PS3.getButtonClick(SELECT)) {
      //Serial.print(F("\r\nSelect - "));
      PS3.printStatusString();
      UartMess.auto1 = !UartMess.auto1;
    }
    if (PS3.getButtonClick(START)) {
      //Serial.print(F("\r\nStart"));
      printAngle = !printAngle;
    }
    if (printAngle) {
      //Serial.print(F("\r\nPitch: "));
      //Serial.print(PS3.getAngle(Pitch));
      //Serial.print(F("\tRoll: "));
      //Serial.print(PS3.getAngle(Roll));
    }
  }
  else if (PS3.PS3MoveConnected) { // One can only set the color of the bulb, set the rumble, set and get the bluetooth address and calibrate the magnetometer via USB
    if (state == 0) {
      PS3.moveSetRumble(0);
      PS3.moveSetBulb(Off);
    } else if (state == 1) {
      PS3.moveSetRumble(75);
      PS3.moveSetBulb(Red);
    } else if (state == 2) {
      PS3.moveSetRumble(125);
      PS3.moveSetBulb(Green);
    } else if (state == 3) {
      PS3.moveSetRumble(150);
      PS3.moveSetBulb(Blue);
    } else if (state == 4) {
      PS3.moveSetRumble(175);
      PS3.moveSetBulb(Yellow);
    } else if (state == 5) {
      PS3.moveSetRumble(200);
      PS3.moveSetBulb(Lightblue);
    } else if (state == 6) {
      PS3.moveSetRumble(225);
      PS3.moveSetBulb(Purple);
    } else if (state == 7) {
      PS3.moveSetRumble(250);
      PS3.moveSetBulb(White);
    }

    state++;
    if (state > 7)
      state = 0;
    delay(1000);
  }
  
//   Serial.print(F("\r\arms[0]: "));
//   Serial.print(UartMess.arms[0]);
//   Serial.print(F("\tarms[1]: "));
//   Serial.print(UartMess.arms[1]);
//   Serial.print(F("\tarms[0]: "));
//   Serial.print(UartMess.arms[2]);
//   Serial.print(F("\cripper: "));
//   Serial.print(UartMess.cripper);
//   Serial.println("");
//   for (int bufferIndex = 0; bufferIndex < structSize; bufferIndex++)
//  {
//    BTSerial.write(dataToSend.buffer[bufferIndex]);
//    Serial.print(dataToSend.buffer[bufferIndex]);
//  }
//  delay(200); 
  //inputString += "H";
//  for(int i = 0;i<5;i++){
//    inputString += UartMess.arms[i];
//    }
  //inputString += "F";
  //inputString.toCharArray(CharString, inputString.length());
//  char string_array[5];
//  strcpy(string_array, inputString.c_str());
//  Serial.print(string_array);
//  Serial.print(UartMess.auto1);
//  Serial.println("");
  //Serial.write(string_array,sizeof(string_array));
  Txmsg.can_dlc = 8;
  Txmsg.data[0] = UartMess.arms[0];
  Txmsg.data[1] = UartMess.arms[1];
  Txmsg.data[2] = UartMess.arms[2];
  Txmsg.data[3] = UartMess.arms[3];
  Txmsg.data[4] = UartMess.arms[4];
  Txmsg.data[5] = UartMess.auto1;
  mcp2515.sendMessage(&Txmsg);
  for(int i = 0;i<6;i++){
    Serial.print(Txmsg.data[i]);
    }
    Serial.println("");
  //inputString = "";
  //delay(100);
  
}
