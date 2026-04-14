#ifndef __Z_STATE_ESTIMATOR_H__
#define __Z_STATE_ESTIMATOR_H__

#include <Arduino.h>

#include "DataSource.h"

typedef struct {
    float z = 0;                // z position (depth) in global frame [m]
    float pressure_voltage = 0; // pressure sensor voltage [V]
} z_state_t;

/*
 * ZStateEstimator class keeps track of the robot's depth state,
 * incorperating measurements of the system outputs from the Pressure
 * Sensor, as well as the control inputs to the system
 */
class ZStateEstimator : public DataSource
{
public:
  ZStateEstimator(void);

  void init(void);

  z_state_t state;

  void updateState(int pressure_signal);
  String printState(void);

  size_t writeDataBytes(unsigned char * buffer, size_t idx);

  int lastExecutionTime = -1;

private:
  const float depthCal_slope = -1.89;
  const float depthCal_intercept = 5.81;
};

#endif