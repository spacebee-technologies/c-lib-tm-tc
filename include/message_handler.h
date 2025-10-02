/**
 * @file message_handler.h
 * @brief Handler for message application layer according to CCSDS 521.0-B-2 as
          described in doc/network_specification.md
 */

#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "communication_interface.h"

#define MESSAGE_HANDLER_AREA_VERSION 1
#define MESSAGE_HANDLER_MAX_BODY_SIZE 256
#define MESSAGE_HANDLER_CRC_SIZE 2

/**
 * @brief Structure with the attributes or fields of the message header
 */
typedef struct __attribute__((packed)) MessageHeader {
  uint64_t timestamp;
  uint16_t interactionType;
  uint8_t interactionStage;
  uint64_t transactionId;
  uint16_t service;
  uint16_t operation;
  uint16_t areaVersion;
  uint8_t isErrorMessage;
  uint16_t bodyLength;
} MessageHeader_t;

/**
 * @brief Interation patterns according to standard
 */
typedef enum MessageHeaderInteractionType {
  MESSAGE_HEADER_INTERACTION_TYPE_SEND = 1,
  MESSAGE_HEADER_INTERACTION_TYPE_SUBMIT = 2,
  MESSAGE_HEADER_INTERACTION_TYPE_REQUEST = 3,
  MESSAGE_HEADER_INTERACTION_TYPE_PUBSUB = 6
} MessageHeaderInteractionType_t;

/**
 * @brief Service field where describes if its a telemetry (TM) or a telecommand (TC)
 */
typedef enum MessageHeaderService {
  MESSAGE_HEADER_SERVICE_TELEMETRY,
  MESSAGE_HEADER_SERVICE_TELECOMMAND
} MessageHeaderService_t;

/**
 * @brief Message structure as described in doc/network_specification.md
 */
typedef struct Message {
  MessageHeader_t header;
  uint8_t body[MESSAGE_HANDLER_MAX_BODY_SIZE];
  uint8_t crc[MESSAGE_HANDLER_CRC_SIZE];
} Message_t;

/**
 * @brief Structure representing the message handler class
 */
typedef struct MessageHandler {
  CommunicationInterface *communicationChannel;
  Message_t message;
} MessageHandler;

/**
 * @brief Constructor for message handler
 *
 * @param self Uninitialized message handler structure
 * @param communicationChannel Communication channel to receive and send messages
 */
void MessageHandler_create(MessageHandler *self, CommunicationInterface *communicationChannel);

/**
 * @brief Header initializer
 *
 * @param self Initialized message handler structure
 */
void MessageHandler_initializeHeader(MessageHandler *self,
                                     uint8_t interactionType,
                                     uint8_t interactionStage,
                                     MessageHeaderService_t service,
                                     bool isErrorMessage);

/**
 * @brief Send a message over the communication channel
 *
 * @param self Initialized message handler structure
 * @param messageBody Buffer for the body of the message to be sent
 * @param messageBodySize Size of the message body
 */
void MessageHandler_send(MessageHandler *self, uint8_t *messageBody,
                         uint16_t messageBodySize);

/**
 * @brief Use the communication channel to receive, parse and check a message
 *
 * @param self Initialized message handler
 * @return true on success
 * @return false on unsuccessful reception
 */
bool MessageHandler_receive(MessageHandler *self);

/**
 * @brief Setter for message header
 *
 * @param self Initialized message handler
 */
void MessageHandler_setHeader(MessageHandler *self, MessageHeader_t header);

/**
 * @brief Getter for message header
 *
 * @param self Initialized message handler
 */
MessageHeader_t MessageHandler_getHeader(MessageHandler *self);

/**
 * @brief Setter for message header operation attribute
 *
 * @param self Initialized message handler
 */
void MessageHandler_setOperation(MessageHandler *self, uint16_t operation);

/**
 * @brief Setter for message header interaction stage
 *
 * @param self Initialized message handler
 */
void MessageHandler_setInteractionStage(MessageHandler *self, uint8_t interactionStage);

/**
 * @brief Getter for the message attribute
 *
 * @param self Initialized message handler
 * @return Message_t* The message attribute
 */
Message_t *MessageHandler_getMessage(MessageHandler *self);

/**
 * @brief Returns true if the parsed message is a telecommand
 */
bool MessageHandler_isTelecommand(MessageHandler *self);

#endif  // MESSAGE_HANDLER_H_
