#include "telemetry_manager.h"

//******************************************************************************
// Public methods
//******************************************************************************
void TelemetryManager_create(TelemetryManager *self) {
  self->currentNumberOfTelemetries = 0;
}

bool TelemetryManager_addNewTelemetry(TelemetryManager *self, TelemetryInterface *newTelemetry) {
  if (self->currentNumberOfTelemetries == MAX_NUMBER_OF_TELEMETRIES) { return false; }
  self->telemetries[self->currentNumberOfTelemetries++] = newTelemetry;
  return true;
}
