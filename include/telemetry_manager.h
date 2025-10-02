#ifndef TELEMETRY_MANAGER_H_
#define TELEMETRY_MANAGER_H_

#define MAX_NUMBER_OF_TELEMETRIES 64

#include "telemetry_interface.h"

typedef struct TelemetryManager {
  TelemetryInterface *telemetries[MAX_NUMBER_OF_TELEMETRIES];
  size_t currentNumberOfTelemetries;
} TelemetryManager;

/**
 * @brief Constructor for telemetry manager
 *
 * @param self Uninitialized telemetry manager
 */
void TelemetryManager_create(TelemetryManager *self);

/**
 * @brief Add a new telemetry to the manager
 *
 * @param self Initialized telemetry manager
 * @param newTelemetry Telemetry to be added
 * @return true on success
 * @return false on failure
 */
bool TelemetryManager_addNewTelemetry(TelemetryManager *self, TelemetryInterface *newTelemetry);

#endif  // TELEMETRY_MANAGER_H_
