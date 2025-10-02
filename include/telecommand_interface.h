#ifndef TELECOMMAND_INTERFACE_H_
#define TELECOMMAND_INTERFACE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "telecommand_arguments_interface.h"

typedef struct TelecommandInterface {
  void *instance;
  bool (*isEnabled)(void *self);
  bool (*execute)(void *self);
  TelecommandArgumentsInterface *(*getArguments)(void *self);
} TelecommandInterface;

/**
 * @brief True if the telecommand is enabled
 *
 * @param self A TelecommandInterface instance
 */
bool TelecommandInterface_isEnabled(TelecommandInterface *self);

/**
 * @brief Execute the telecommand, shall be called after TelecommandInterface_loadInputArguments
 *
 * @param self A TelecommandInterface instance
 * @return true if successful execution
 * @return false if execution failure
 */
bool TelecommandInterface_execute(TelecommandInterface *self);

/**
 * @brief Get the corresponding arguments for the given telecommand
 *
 * @param self A TelecommandInterface instance
 * @return TelecommandArgumentsInterface* Input/output arguments interface
 */
TelecommandArgumentsInterface *TelecommandInterface_getArguments(TelecommandInterface *self);

#endif  // TELECOMMAND_INTERFACE_H_
