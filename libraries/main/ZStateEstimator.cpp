#include "ZStateEstimator.h"
#include <math.h>
#include "Printer.h"
extern Printer printer;

ZStateEstimator::ZStateEstimator(void)
  : DataSource("z,pressure_voltage","float,float") // from DataSource
{}

void ZStateEstimator::init(void) {
  state.z = 0;
  state.pressure_voltage = 0;
}

void ZStateEstimator::updateState(int pressure_signal) {
  state.pressure_voltage = (double)pressure_signal;
  state.pressure_voltage *= (3.3 / 1023.0);  // convert from Teensy units to Volts

  state.z = depthCal_slope * state.pressure_voltage + depthCal_intercept; // convert from Volts to depth [m]

  String calibrationMessage = "Pressure Sensor Voltage: " + String(state.pressure_voltage, 3);
  printer.printMessage(calibrationMessage, 20);
}

String ZStateEstimator::printState(void) {
  String currentState = "";
  int decimals = 3;
  currentState += "Z_State: z: ";
  currentState += String(state.z, decimals);
  currentState += "[m], V: ";
  currentState += String(state.pressure_voltage, decimals);
  currentState += "[V]";
  return currentState;
}

size_t ZStateEstimator::writeDataBytes(unsigned char * buffer, size_t idx) {
    float * data_slot = (float *) &buffer[idx];
    data_slot[0] = state.z;
    data_slot[1] = state.pressure_voltage;
    return idx + 2 * sizeof(float);
}