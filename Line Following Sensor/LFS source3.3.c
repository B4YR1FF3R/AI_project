// โค้ดหุ่นยนต์ติดตามเส้น ESP32 ใน Arduino IDE
// โค้ดภาษา C
// ทำตามที่เขาบอก https://www.arnabkumardas.com/platforms/everything-esp/esp32-qtr-8rc-pid-line-follower-robot-v1/


// main.c
#define LFR_MAX_MOTOR_SPEED 125    /**< Sets the Maximum PWM Duty Cycle for Line Follower Robot 0=0% 255=100% */


// PID Line Follower
// main.c : int main()
uint16_t LFR_SensorValue[QTR_SENSOR_COUNT];       /**< Array to Save Raw IR Sensor values of QTR-8RC */
uint16_t LFR_Position = 0;         /**< Variable to Save the QTR-8RC Sensor Position */
int16_t  LFR_Proportional = 0;     /**< Variable to Save the Proportional Output of PID Control Algorithm */
int16_t  LFR_LastProportional = 0; /**< Variable to Save the Previous Proportional Output of PID Control Algorithm */
int16_t  LFR_Derivative = 0;       /**< Variable to Save the Derivative Output of PID Control Algorithm */
int64_t  LFR_Integral = 0;         /**< Variable to Save the Integral Output of PID Control Algorithm */
int16_t  LFR_ControlOutput = 0;    /**< Variable to Save the Final Control Output of PID Control Algorithm */

uint8_t  Kd = 14;
uint8_t  Kp = 2;
uint16_t Ki = 10000;

uint16_t Speed = LFR_MAX_MOTOR_SPEED;


// ระบุอุปกรณ์ต่อพ่วงที่ใช้ทั้งหมดของ ESP32
// main.c : void LFR_Initialize()
void LFR_Initialize()
{
  uint8_t QTR_Pins[] = {QTR_1_PIN, QTR_2_PIN, QTR_3_PIN, QTR_4_PIN, QTR_5_PIN, QTR_6_PIN, QTR_7_PIN, QTR_8_PIN};
  QTR_Init(QTR_Pins, QTR_EMITTER_PIN);    /**< Initializes the QTR-8RC Sensor */
  Motor_Init();                           /**< Initializes the Motors */
  delay(2000);                            /**< Pause ; Useful to Align the Robot Manually if Outside the Line */
}


// main.c : void LFR_Calibrate()
// ปรับเทียบเซ็นเซอร์เส้น QTR-8RC ให้หุ่นยนต์หมุนในตำแหน่งเดียวกันและปรับเทียบเซ็นเซอร์ด้วยค่าเซ็นเซอร์ที่เป็นไปได้ทั้งหมดที่สุ่มตัวอย่างในช่วงเวลานี้
void LFR_Calibrate()
{
  Motor_SetSpeed(90, -90);         /**< Rotates the Robot 90, -90 300RPM */
  for (uint8_t i = 0; i < 40; i++) /**< Calibrate the QTR-8RC Sensor */
  {
    QTR_CalibrateSensor(QTR_EMITTERS_ON);
    delay(20);
  }
  Motor_SetSpeed(0, 0);            /**< Stops the Robot */
  delay(500);
  Motor_SetSpeed(-90, 90);         /**< Rotates the Robot */
  for (uint8_t i = 0; i < 80; i++) /**< Calibrate the QTR-8RC Sensor */
  {
    QTR_CalibrateSensor(QTR_EMITTERS_ON);
    delay(20);
  }
  Motor_SetSpeed(0, 0);            /**< Stops the Robot */
  delay(500);
  Motor_SetSpeed(90, -90);         /**< Rotates the Robot */
  for (uint8_t i = 0; i < 40; i++) /**< Calibrate the QTR-8RC Sensor */
  {
    QTR_CalibrateSensor(QTR_EMITTERS_ON);
    delay(20);
  }
  Motor_SetSpeed(0, 0);         /**< Stops the Robot */
  delay(2000);                  /**< Pause ; Useful to Realign the Robot Manually if Outside the Line */
}


// ฟังก์ชั่นนี้รับผิดชอบอัลกอริทึม PID ค่าที่เพิ่มขึ้น (Kp=1/2, Ki=1/10000 และ Kd=14) ทำให้ Line Follower Robot ทำงานนอกกรอบหากโครงสร้างที่คล้ายกันเสร็จสิ้น จะทราบว่าค่าสัมประสิทธิ์ PID ขึ้นอยู่กับคุณสมบัติทางกายภาพ เช่น ขนาดของหุ่นยนต์ น้ำหนัก สัดส่วน แรงดันไฟฟ้าของแบตเตอรี่ แรงเสียดทาน ฯลฯ แนะนำให้ปรับแต่งหุ่นยนต์เสมอหลังจากเปลี่ยนฮาร์ดแวร์
// main.c : int main()
void loop()
{
  LFR_Position = QTR_ReadLine(LFR_SensorValue, QTR_EMITTERS_ON);    /**< Reads the QTR-8RC Line Sensor to Get the Line Position */

  LFR_Proportional = LFR_Position - QTR_LINE_MID_VALUE;             /**< Computes the Proportional Output of PID Control Algorithm */
  LFR_Derivative = LFR_Proportional - LFR_LastProportional;         /**< Computes the Derivative Output of PID Control Algorithm */
  LFR_Integral += LFR_Proportional;                                 /**< Computes the Integral Output of PID Control Algorithm */
  LFR_LastProportional = LFR_Proportional;                          /**< Saves the Old Proportional Output of PID Control Algorithm */

  LFR_ControlOutput = LFR_Proportional / Kp + LFR_Integral / Ki + LFR_Derivative * Kd; /**< Computes the Final Control Output of PID Control Algorithm 300RPM*/

  if (LFR_ControlOutput > Speed)
  {
    LFR_ControlOutput = Speed;    /**< Keeps The Motor Speed in Limit */
  }
  if (LFR_ControlOutput < -Speed)
  {
    LFR_ControlOutput = -Speed;    /**< Keeps The Motor Speed in Limit */
  }

  if (LFR_ControlOutput < 0)
  {
    Motor_SetSpeed(Speed + LFR_ControlOutput, Speed);    /**< Drives the Motor According to the Control Output */
  }
  else
  {
    Motor_SetSpeed(Speed, Speed - LFR_ControlOutput);    /**< Drives the Motor According to the Control Output */
  }
}