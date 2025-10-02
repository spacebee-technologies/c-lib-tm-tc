#include "telecommand_arguments_interface.h"

void TelecommandArgumentsInterface_loadInputArguments(TelecommandArgumentsInterface *self, uint8_t *arguments, size_t argumentsSize) {
  return self->loadInputArguments(self->instance, arguments, argumentsSize);
}

void TelecommandArgumentsInterface_getReturnArguments(TelecommandArgumentsInterface *self, uint8_t *arguments, size_t *argumentsSize) {
  return self->getReturnArguments(self->instance, arguments, argumentsSize);
}

uint16_t TelecommandArgumentsInterface_getOperation(TelecommandArgumentsInterface *self) {
  return self->getOperation(self->instance);
}
