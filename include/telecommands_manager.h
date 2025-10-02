#ifndef TELECOMMANDS_MANAGER_H_
#define TELECOMMANDS_MANAGER_H_

#include "telecommand_interface.h"
#include "message_handler.h"

#define MAX_NUMBER_OF_TELECOMMANDS 64

typedef struct TelecommandsManager {
  TelecommandInterface *telecommands[MAX_NUMBER_OF_TELECOMMANDS];
  size_t currentNumberOfTelecommands;
} TelecommandsManager;

/**
 * @brief Initialize a new instance of a TelecommandsManager
 *
 * @param self Uninitialized instance of TelecommandsManager
 */
void TelecommandsManager_create(TelecommandsManager *self);

/**
 * @brief Register a new telecommand
 *
 * @param self A TelecommandsManager instance
 * @param newTelecommand A new telecommand to be registered
 * @return true if success
 * @return false if failure to add new telecommand
 */
bool TelecommandsManager_addNewTelecommand(TelecommandsManager *self, TelecommandInterface *newTelecommand);

/**
 * @brief Check if a given message corresponds to a known telecommand operation number
 *
 * @param self A TelecommandsManager instance
 * @param message A message with a given operation number
 * @return true if the message has a known operation number
 * @return false if the message has an unknown operation number
 */
bool TelecommandsManager_isKnownOperation(TelecommandsManager *self, Message_t *message);

/**
 * @brief Get a telecommand for the given message. Shall be called after checking
 *        the operation number is known with TelecommandsManager_isKnownOperation
 *
 * @param self A TelecommandsManager instance
 * @param message A message with a given operation number
 * @return TelecommandInterface* being the actual telecommand for the given operation number
 */
TelecommandInterface *TelecommandsManager_getTelecommandFromMessage(TelecommandsManager *self, Message_t *message);

#endif  // TELECOMMANDS_MANAGER_H_
