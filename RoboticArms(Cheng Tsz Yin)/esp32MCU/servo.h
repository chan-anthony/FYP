#ifndef servo_h
#define servo_h
#include "Arduino.h"
class servo {
  public:
//接收数据相关变量
uint8_t Rx_Data[8] = {0};
uint8_t Rx_index = 0;
uint8_t Rx_Flag = 0;
uint8_t RecvFlag = 0;
// 舵机定时运行变量
uint64_t time_run = 0;
  void init();
  void bus_servo_control(int id, int value, int time);
  void bus_servo_set_id(uint8_t id);
  void bus_servo_read(uint8_t id);
  uint16_t bus_servo_get_value(void);
  void bus_servo_uart_recv(uint8_t Rx_Temp);
  };
#endif
