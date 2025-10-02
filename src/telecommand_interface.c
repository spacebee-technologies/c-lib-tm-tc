#include "telecommand_interface.h"

bool TelecommandInterface_isEnabled(TelecommandInterface *self) {
  return self->isEnabled(self->instance);
}

bool TelecommandInterface_execute(TelecommandInterface *self) {
  return self->execute(self->instance);
}

TelecommandArgumentsInterface *TelecommandInterface_getArguments(TelecommandInterface *self) {
  return self->getArguments(self->instance);
}
