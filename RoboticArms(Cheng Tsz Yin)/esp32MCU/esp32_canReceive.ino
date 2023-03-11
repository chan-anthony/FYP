#include <Arduino.h>
#include <ESP32CAN.h>
#include <CAN_config.h>
#include "servo.h"

servo ser;
CAN_device_t CAN_cfg;                // CAN Config
const int rx_queue_size = 10;       // Receive Queue size
struct controlBut{
  char yaw[4];
  char grip;
  char aim; 
  };
controlBut pannel;

int pos = 2000;
int pos2 = 2000;
int pos3 = 2000;
int pos4 = 2000;
int pos5 = 2000;
int open2  = 200;
int close2 = 3000;

void setup() {
  Serial.begin(115200);
  Serial.println("Mirror Demo - ESP32-Arduino-CAN");
  CAN_cfg.speed = CAN_SPEED_1000KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_25;
  CAN_cfg.rx_pin_id = GPIO_NUM_26;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();
  ser.init();
  delay(10);
}

void loop() {
  CAN_frame_t rx_frame;
  //receive next CAN frame from queue
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {

    if (rx_frame.FIR.B.FF == CAN_frame_std) {
      //printf("New standard frame");
    }
    else {
      //printf("New extended frame");
    }

    if (rx_frame.FIR.B.RTR == CAN_RTR) {
      //printf(" RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
    }
    else {
      //printf(" from 0x%08X, DLC %d, Data ", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
//      for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
//        printf("%02X ", rx_frame.data.u8[i]);
//      }
      pannel.yaw[0] = rx_frame.data.u8[0];
      pannel.yaw[1] = rx_frame.data.u8[1];
      pannel.yaw[2] = rx_frame.data.u8[2];
      pannel.yaw[3] = rx_frame.data.u8[3];
      pannel.grip = rx_frame.data.u8[4];
      pannel.aim = rx_frame.data.u8[5];
    }
  printCanRecev();
   if(pannel.yaw[0] == 'A'){
      pos += 50;
      //delay(50);
     //Serial.println(pos);
     if(pos>4000){
      pos = 4000;
      }
      ser.bus_servo_control(0x01, pos, 0);
      }
     if(pannel.yaw[0] == 'D'){
      pos -= 50;
      //delay(50);
      //Serial.println(pos);
      if(pos<0){
      pos = 0;
      }
      ser.bus_servo_control(0x01, pos, 0);
      }
    if(pannel.yaw[1] == 'A'){
      pos2 += 50;
      //delay(50);
     //Serial.println(pos2);
     if(pos2>4000){
      pos2 = 4000;
      }
      ser.bus_servo_control(0x02, pos2, 0);
      }
     if(pannel.yaw[1] == 'D'){
      pos2 -= 50;
      //delay(50);
      //Serial.println(pos2);
      if(pos2<0){
      pos2 = 0;
      }
      ser.bus_servo_control(0x02, pos2, 0);
      }
     
      if(pannel.yaw[2] == 'A'){
      pos3 += 50;
      //delay(50);
     //Serial.println(pos3);
     if(pos3>4000){
      pos3 = 4000;
      }
      ser.bus_servo_control(0x03, pos3, 0);
      }
     if(pannel.yaw[2] == 'D'){
      pos3 -= 50;
      //delay(50);
      //Serial.println(pos3);
      if(pos3<0){
      pos3 = 0;
      }
      ser.bus_servo_control(0x03, pos3, 0);
      }
      if(pannel.yaw[3] == 'A'){
      pos4 += 50;
      //delay(50);
     //Serial.println(pos4);
     if(pos4>4000){
      pos4 = 4000;
      }
      ser.bus_servo_control(0x04, pos4, 0);
      }
     if(pannel.yaw[3] == 'D'){
      pos4 -= 50;
      //delay(50);
      //Serial.println(pos4);
      if(pos4<0){
      pos4 = 0;
      }
      ser.bus_servo_control(0x04, pos4, 0);
      }
      if(pannel.yaw[4] == 'O'){
      pos5 += 50;
      //delay(50);
     //Serial.println(pos5);
     if(pos5>4000){
      pos5 = 4000;
      }
      ser.bus_servo_control(0x05, pos5, 0);
      }
     if(pannel.yaw[4] == 'F'){
      pos5 -= 50;
      //delay(50);
      //Serial.println(pos5);
      if(pos5<0){
      pos5 = 0;
      }
      ser.bus_servo_control(0x05, pos5, 0);
      }
      if(pannel.yaw[5] == 'O'){
      ser.bus_servo_control(0x06, close2, 0);
      }
     if(pannel.yaw[5] == 'F'){
      ser.bus_servo_control(0x06, open2, 0);
      }
  }

}

void printCanRecev(){
 printf("%c\r",char(pannel.yaw[0]));
    printf("%c\r",char(pannel.yaw[1]));
    printf("%c\r",char(pannel.yaw[2]));
    printf("%c\r",char(pannel.yaw[3]));
    printf("%c\r",char(pannel.grip));
    printf("%c\r",char(pannel.aim));
    printf("\n");
  }
