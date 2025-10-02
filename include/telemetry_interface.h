/**
 * @file telecommand_interface.h
 * @author Spacebeetech
 * @brief Interface to handle telemetries
 * @date 2023-08-12
 */

#ifndef TELEMETRY_INTERFACE_H_
#define TELEMETRY_INTERFACE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct TelemetryInterface {
  void *instance;
  bool (*isEnabled)(void *self);
  void (*setEnabled)(void *self, bool state);
  bool (*execute)(void *self);
  bool (*getParameters)(void *self, uint8_t *parameters, size_t *parametersSize);
  uint16_t (*getOperation)(void *self);
} TelemetryInterface;

bool TelemetryInterface_isEnabled(TelemetryInterface *self);

bool TelemetryInterface_execute(TelemetryInterface *self);

bool TelemetryInterface_getParameters(TelemetryInterface *self, uint8_t *parameters, size_t *parametersSize);

uint16_t TelemetryInterface_getOperation(TelemetryInterface *self);

void TelemetryInterface_setEnabled(TelemetryInterface *self, bool state);

#endif  // TELEMETRY_INTERFACE_H_
