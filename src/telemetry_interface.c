#include "telemetry_interface.h"

bool TelemetryInterface_isEnabled(TelemetryInterface *self) {
  return self->isEnabled(self->instance);
}

bool TelemetryInterface_execute(TelemetryInterface *self) {
  return self->execute(self->instance);
}

bool TelemetryInterface_getParameters(TelemetryInterface *self, uint8_t *parameters, size_t *parametersSize) {
  return self->getParameters(self->instance, parameters, parametersSize);
}

uint16_t TelemetryInterface_getOperation(TelemetryInterface *self) {
  return self->getOperation(self->instance);
}

void TelemetryInterface_setEnabled(TelemetryInterface *self, bool state) {
  return self->setEnabled(self->instance, state);
}
