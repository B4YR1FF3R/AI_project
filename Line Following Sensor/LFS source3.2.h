// โค้ดหุ่นยนต์ติดตามเส้น ESP32 ใน Arduino IDE
// โค้ดภาษา C
// ทำตามที่เขาบอก https://www.arnabkumardas.com/platforms/everything-esp/esp32-qtr-8rc-pid-line-follower-robot-v1/

/** @name MOTOR_CONTROL
    Public Functions to Control Motor
*/
/**@{*/
void Motor_Init(void);
void Motor_DeInit(void);
void Motor_SetSpeed(int16_t M1Speed, int16_t M2Speed);
void Motor_SetSpeedM1(int16_t M1Speed);
void Motor_SetSpeedM2(int16_t M2Speed);
/**@}*/