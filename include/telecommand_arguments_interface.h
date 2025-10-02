#ifndef TELECOMMAND_ARGUMENTS_INTERFACE_H_
#define TELECOMMAND_ARGUMENTS_INTERFACE_H_

#include <stdint.h>
#include <stddef.h>

typedef struct TelecommandArgumentsInterface {
  void *instance;
  void (*loadInputArguments)(void *self, uint8_t *arguments, size_t argumentsSize);
  void (*getReturnArguments)(void *self, uint8_t *arguments, size_t *argumentsSize);
  uint16_t (*getOperation)(void *self);
} TelecommandArgumentsInterface;

/**
 * @brief Load input arguments into the telecommand
 *
 * @param self A TelecommandArgumentsInterface instance
 * @param arguments Received arguments as a stream of bytes
 * @param argumentsSize Length of the arguments stream
 */
void TelecommandArgumentsInterface_loadInputArguments(TelecommandArgumentsInterface *self, uint8_t *arguments, size_t argumentsSize);

/**
 * @brief Get the output of the telecommand execution (if any)
 *
 * @param self A TelecommandArgumentsInterface instance
 * @param arguments Reference of a buffer where the output will be returned
 * @param argumentsSize Pointer to return the size of the output arguments
 */
void TelecommandArgumentsInterface_getReturnArguments(TelecommandArgumentsInterface *self, uint8_t *arguments, size_t *argumentsSize);

/**
 * @brief Get the unique operation number of each command
 *
 * @param self A TelecommandArgumentsInterface instance
 */
uint16_t TelecommandArgumentsInterface_getOperation(TelecommandArgumentsInterface *self);

#endif  // TELECOMMAND_ARGUMENTS_INTERFACE_H_
