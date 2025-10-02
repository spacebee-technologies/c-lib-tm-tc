#include "telecommands_manager.h"

#include <stdbool.h>

//******************************************************************************
// Public methods
//******************************************************************************
void TelecommandsManager_create(TelecommandsManager *self) {
  self->currentNumberOfTelecommands = 0;
}

bool TelecommandsManager_addNewTelecommand(TelecommandsManager *self,
                                           TelecommandInterface *newTelecommand) {
  if (self->currentNumberOfTelecommands == MAX_NUMBER_OF_TELECOMMANDS) { return false; }
  self->telecommands[self->currentNumberOfTelecommands++] = newTelecommand;
  return true;
}

bool TelecommandsManager_isKnownOperation(TelecommandsManager *self, Message_t *message) {
  for (int i = 0; i < self->currentNumberOfTelecommands; i++) {
    TelecommandInterface *testTelecommand = self->telecommands[i];
    TelecommandArgumentsInterface *arguments = TelecommandInterface_getArguments(testTelecommand);
    if (TelecommandArgumentsInterface_getOperation(arguments) == message->header.operation) {
      return true;
    }
  }
  return false;
}

TelecommandInterface *TelecommandsManager_getTelecommandFromMessage(
    TelecommandsManager *self, Message_t *message) {
  for (int i = 0; i < self->currentNumberOfTelecommands; i++) {
    TelecommandInterface *testTelecommand = self->telecommands[i];
    TelecommandArgumentsInterface *arguments = TelecommandInterface_getArguments(testTelecommand);
    if (TelecommandArgumentsInterface_getOperation(arguments) == message->header.operation) {
      return testTelecommand;
    }
  }
  return self->telecommands[0];
}
