#include "servo.h"
#define INTERRUPT 34 
servo ser;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(INTERRUPT,INPUT_PULLUP);
    ser.init();
    ser.bus_servo_set_id(1);
    delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
 if(digitalRead(INTERRUPT)==LOW){
        Serial.println("forward");
        ser.bus_servo_control(0x01, 900, 1000);
        delay(10); 
        }
       else{
        Serial.println("back");
        ser.bus_servo_control(0x01, 3100, 1000);
        delay(10); 
        }
    
    if (ser.RecvFlag)
    {
        uint16_t value = ser.bus_servo_get_value();
        if (value)
        {
            Serial.print("\r\nvalue=");
            Serial.println(value);
        }
        else
        {
            Serial.println("\r\nread error");
        }
        ser.RecvFlag = 0;
    }

    //定时2秒钟运行一次
    if (millis() - ser.time_run >= 2000)
    {
        static int state = 0;
        if (state)
        {
            ser.bus_servo_control(0x01, 900, 1000);
            delay(10);
            state = 0;
        }
        else
        {
            ser.bus_servo_control(0x01, 3100, 1000);
            delay(10);
            state = 1;
        }
        ser.time_run = millis();
        ser.bus_servo_read(1);
    }
}

//串口中断
void serialEvent()
{
    while (Serial.available())
    {
        uint8_t RXTemp = (uint8_t)Serial.read();
        ser.bus_servo_uart_recv(RXTemp);
        // Serial.write(&RXTemp, 1);
    }
}
