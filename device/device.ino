#include "HX711.h"
#include <Adafruit_ADS1X15.h>

bool CalibrationMode = true;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

//LVDT calibration parameters for interpolation
const double y0 = 0.0; //measured cm of x0
const double y1 = 2.3; //measured cm of x1
const double x0 = 1000; //raw adc value of y0
const double x1 = 5000; //raw adc value of y1

//LoadCell calibration parameters for interpolation
const double y0_ = 0.0; //measured kg of x0
const double y1_ = 2.3; //measured kg of x1
const double x0_ = 1000; //raw adc value of y0
const double x1_ = 5000; //raw adc value of y1

HX711 scale;
Adafruit_ADS1115 ads;

double LVDT_measurement() {
  int16_t adcValue = ads.readADC_SingleEnded(0);
  double measurement = y0 + (y1 - y0) * (adcValue - x0) / (x1 - x0);
  return measurement;
}

double LoadCell_measurement() {
  if(scale.is_ready()) {
    long reading = scale.read();
    double measurement = y0_ + (y1_ - y0_) * (reading - x0_) / (x1_ - x0_);
    return measurement;
  }
  else {
    return 0;
  }
}

void setup() {
  Serial.println("Uniaxial Compression Device");
  Serial.println("Civil Engineering Laboratory");

  
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Initializing ADS1115...");
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115!");
    while (1);
  }
}

void loop() {
  if (CalibrationMode) {
    if (scale.is_ready()) {
    long reading = scale.read();
    int16_t adcValue = ads.readADC_SingleEnded(0);
    Serial.println("W:\t"+String(reading)+"\tD:\t"+String(adcValue));
    } else {
      Serial.println("HX711 not found.");
    }
  }
  else {
    double loadcell = LoadCell_measurement();
    double lvdt = LVDT_measurement();
    Serial.println("LoadCell:\t"+String(loadcell)+"\tDisplacement:\t"+String(lvdt));
  }

  delay(500);
}
