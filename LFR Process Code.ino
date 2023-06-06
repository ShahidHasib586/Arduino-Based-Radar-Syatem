#include <QTRSensors.h>
#define NUM_SENSORS             10  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN   QTR_NO_EMITTER_PIN  // Since there is no emitter(ir LED) control option in this Sensor

// sensors 0 through 9 are connected to analog inputs A0 through A9, respectively (right to left)
QTRSensorsAnalog qtra( (unsigned char[]) {
  A9, A8, A7, A6, A5, A4, A3, A2, A1, A0
}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];   //array to store Sensor readings


void setup()
{
  delay(500);
  Serial.begin(9600); // set the data rate in bits per second for serial data transmission

  calibrateSensors();  // read the description above for details on calibrating

  delay(1000);
}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 9000 (right to left)
  // To get raw sensor values, call:
  //  qtra.read(sensorValues); instead of unsigned int position = qtra.readLine(sensorValues);
  unsigned int position = qtra.readLine(sensorValues, QTR_EMITTERS_ON, true);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.print('\t');
  Serial.println(position); // comment this line out if you are using raw values

  delay(100);
}


void calibrateSensors() {
  Serial.print("Calibrating. ");
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
    Serial.print('.');
  }
  Serial.println();
  Serial.println("Calibration complete ");
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
  // print the calibration maximum values measured
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  // print the calibration maximum values measured
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtra.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
}