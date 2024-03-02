// โค้ดหุ่นยนต์ติดตามเส้น ESP32 ใน Arduino IDE
// โค้ดภาษา C
// ทำตามที่เขาบอก https://www.arnabkumardas.com/platforms/everything-esp/esp32-qtr-8rc-pid-line-follower-robot-v1/

/** @name QTR_CONTROL
    Public Functions to Control QTR Sensor
*/
void QTR_Init(uint8_t *SensorPin, uint8_t EmitterPin);
void QTR_DeInit();
void QTR_Calibrate(uint16_t *CalibratedMinimum, uint16_t *CalibratedMaximum, uint8_t ReadMode);
void QTR_CalibrateSensor(uint8_t ReadMode);
void QTR_ResetCalibration();
void QTR_ReadSensor(uint16_t *SensorValues, uint8_t ReadMode);
void QTR_ReadRaw(uint16_t *SensorValues);
void QTR_ReadCalibrated(uint16_t *SensorValues, uint8_t ReadMode);
uint16_t QTR_ReadLine(uint16_t *SensorValues, uint8_t ReadMode);
inline void QTR_EmitterOn();
inline void QTR_EmitterOff();
/**@}*/