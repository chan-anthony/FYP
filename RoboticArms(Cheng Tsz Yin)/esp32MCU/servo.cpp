#include "servo.h"
#define RX_MAX_BUF 8
#include <HardwareSerial.h>
HardwareSerial Serialport(2);

void servo::init(){
  Serialport.begin(115200,SERIAL_8N1,17,16);
  }

void servo::bus_servo_control(int id, int value, int time){
  uint8_t head1 = 0xff;
    uint8_t head2 = 0xff;
    uint8_t s_id = id & 0xff;
    uint8_t len = 0x07;
    uint8_t cmd = 0x03;
    uint8_t addr = 0x2a;

    if (value > 4000)
        value = 4000;
    else if (value < 96)
        value = 96;

    uint8_t pos_H = (value >> 8) & 0xff;
    uint8_t pos_L = value & 0xff;

    uint8_t time_H = (time >> 8) & 0xff;
    uint8_t time_L = time & 0xff;

    uint8_t checknum = (~(s_id + len + cmd + addr +
                          pos_H + pos_L + time_H + time_L)) & 0xff;
    uint8_t data[] = {head1, head2, s_id, len, cmd,
                      addr, pos_H, pos_L, time_H, time_L, checknum};
    
    Serialport.write(data, 11);
  }

  /* 写入目标ID(1~250) */
void servo::bus_servo_set_id(uint8_t id)
{
    if ((id >= 1) && (id <= 250))
    {
        uint8_t head1 = 0xff;
        uint8_t head2 = 0xff;
        uint8_t s_id = 0xfe; /* 发送广播的ID */
        uint8_t len = 0x04;
        uint8_t cmd = 0x03;
        uint8_t addr = 0x05;
        uint8_t set_id = id; /* 实际写入的ID */

        uint8_t checknum = (~(s_id + len + cmd + addr + set_id)) & 0xff;
        uint8_t data[] = {head1, head2, s_id, len, cmd, addr, set_id, checknum};

        Serialport.write(data, 8);
    }
}

/* 发送读取舵机位置命令 */
void servo::bus_servo_read(uint8_t id)
{
    uint8_t head1 = 0xff;
    uint8_t head2 = 0xff;
    uint8_t s_id = id & 0xff;
    uint8_t len = 0x04;
    uint8_t cmd = 0x02;
    uint8_t param_H = 0x38;
    uint8_t param_L = 0x02;

    uint8_t checknum = (~(s_id + len + cmd + param_H + param_L)) & 0xff;
    uint8_t data[] = {head1, head2, s_id, len, cmd, param_H, param_L, checknum};

    Serialport.write(data, 8);
}

//转化接收到的值为位置数
uint16_t servo::bus_servo_get_value(void)
{
    uint8_t checknum = (~(Rx_Data[2] + Rx_Data[3] + Rx_Data[4] + Rx_Data[5] + Rx_Data[6])) & 0xff;
    if(checknum == Rx_Data[7])
    {
        uint8_t s_id = Rx_Data[2];
        uint16_t value_H = 0;
        uint16_t value_L = 0;

        value_H = Rx_Data[5];
        value_L = Rx_Data[6];
        uint16_t value = (value_H << 8) + value_L;
        return value;
    }
    return 0;
}

//处理串口数据，如果符合协议则设置RecvFlag = 1
void servo::bus_servo_uart_recv(uint8_t Rx_Temp)
{
    switch(Rx_Flag)
    {
        case 0:
            if(Rx_Temp == 0xff)
            {
                Rx_Data[0] = 0xff;
                Rx_Flag = 1;
            }
            else if (Rx_Temp == 0xf5)
            {
                Rx_Data[0] = 0xff;
                Rx_Data[1] = 0xf5;
                Rx_Flag = 2;
                Rx_index = 2;
            }
            break;

        case 1:
            if(Rx_Temp == 0xf5)
            {
                Rx_Data[1] = 0xf5;
                Rx_Flag = 2;
                Rx_index = 2;
            } else
            {
                Rx_Flag = 0;
                Rx_Data[0] = 0x0;
            }
            break;

        case 2:
            Rx_Data[Rx_index] = Rx_Temp;
            Rx_index++;

            if(Rx_index >= RX_MAX_BUF)
            {
                Rx_Flag = 0;
                RecvFlag = 1;
            }
            break;

        default:
            break;
    }
}
