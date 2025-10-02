#include "message_handler.h"

#include <stdlib.h>

#include "zephyr_time.h"

#define MAXIMUM_BUFFER_SIZE 512

//******************************************************************************
// Private functions
//******************************************************************************

/**
 * @brief Generates the CRC code with polynomial 0x1021
 *
 * @param buffer Data over which the CRC will be calculated
 * @param size Size of the buffer
 * @return uint16_t CRC calculated value
 */
static uint16_t _crc16(const uint8_t *buffer, size_t size) {
  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < size; ++i) {
    crc ^= (uint16_t)buffer[i] << 8;
    for (size_t j = 0; j < 8; ++j) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ 0x1021;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

//******************************************************************************
// Private methods
//******************************************************************************
static void MessageHandler_parseHeader(MessageHandler *self, const uint8_t *buffer) {
  size_t memoryOffset = 0;
  memcpy(&self->message.header.timestamp, buffer + memoryOffset,
         sizeof(self->message.header.timestamp));
  memoryOffset += sizeof(self->message.header.timestamp);
  memcpy(&self->message.header.interactionType, buffer + memoryOffset,
         sizeof(self->message.header.interactionType));
  memoryOffset += sizeof(self->message.header.interactionType);
  memcpy(&self->message.header.interactionStage, buffer + memoryOffset,
         sizeof(self->message.header.interactionStage));
  memoryOffset += sizeof(self->message.header.interactionStage);
  memcpy(&self->message.header.transactionId, buffer + memoryOffset,
         sizeof(self->message.header.transactionId));
  memoryOffset += sizeof(self->message.header.transactionId);
  memcpy(&self->message.header.service, buffer + memoryOffset,
         sizeof(self->message.header.service));
  memoryOffset += sizeof(self->message.header.service);
  memcpy(&self->message.header.operation, buffer + memoryOffset,
         sizeof(self->message.header.operation));
  memoryOffset += sizeof(self->message.header.operation);
  memcpy(&self->message.header.areaVersion, buffer + memoryOffset,
         sizeof(self->message.header.areaVersion));
  memoryOffset += sizeof(self->message.header.areaVersion);
  memcpy(&self->message.header.isErrorMessage, buffer + memoryOffset,
         sizeof(self->message.header.isErrorMessage));
  memoryOffset += sizeof(self->message.header.isErrorMessage);
  memcpy(&self->message.header.bodyLength, buffer + memoryOffset,
         sizeof(self->message.header.bodyLength));
  memoryOffset += sizeof(self->message.header.bodyLength);
}

static void MessageHandler_parseBody(MessageHandler *self, const uint8_t *buffer, size_t bufferLength) {
  size_t bodySize = bufferLength - sizeof(MessageHeader_t) - MESSAGE_HANDLER_CRC_SIZE;
  memcpy(self->message.body, buffer + sizeof(MessageHeader_t), bodySize);
}

static void MessageHandler_parseCrc(MessageHandler *self, const uint8_t *buffer, size_t bufferLength) {
  memcpy(self->message.crc, buffer + bufferLength - MESSAGE_HANDLER_CRC_SIZE,
         MESSAGE_HANDLER_CRC_SIZE);
}

/**
 * @brief Receives a raw message in a buffer and parse it into the class internal attributes
 *
 * @param self Initialized message handler
 * @param buffer Raw message to be parsed
 * @param bufferLength Buffer size
 * @return true on success
 * @return false if the message could not be parsed
 */
static bool MessageHandler_parseAndCheckCrc(MessageHandler *self, const uint8_t *buffer, size_t bufferLength) {
  size_t minMessageLength = sizeof(MessageHeader_t) + MESSAGE_HANDLER_CRC_SIZE;
  if (bufferLength < minMessageLength) { return false; }
  MessageHandler_parseHeader(self, buffer);
  size_t expectedMessageLength = sizeof(MessageHeader_t) + self->message.header.bodyLength + MESSAGE_HANDLER_CRC_SIZE;
  if (bufferLength != expectedMessageLength) { return false; }
  MessageHandler_parseBody(self, buffer, bufferLength);
  MessageHandler_parseCrc(self, buffer, bufferLength);
  uint16_t crc = _crc16(buffer, sizeof(MessageHeader_t)+self->message.header.bodyLength);
  if ((self->message.crc[0]==(crc & 0xFF)) && (self->message.crc[1]==(crc >> 8))) {
    return true;
  } else {
    return false;
  }
}

//******************************************************************************
// Public methods
//******************************************************************************
void MessageHandler_create(MessageHandler *self,
                           CommunicationInterface *communicationChannel) {
  self->communicationChannel = communicationChannel;
}

void MessageHandler_initializeHeader(MessageHandler *self,
                                     uint8_t interactionType,
                                     uint8_t interactionStage,
                                     MessageHeaderService_t service,
                                     bool isErrorMessage) {
  self->message.header.interactionType = interactionType;
  self->message.header.interactionStage = interactionStage;
  self->message.header.transactionId = 0;
  self->message.header.service = service;
  self->message.header.areaVersion = MESSAGE_HANDLER_AREA_VERSION;
  self->message.header.isErrorMessage = isErrorMessage;
}

void MessageHandler_send(MessageHandler *self, uint8_t *messageBody,
                         uint16_t messageBodySize) {
  self->message.header.timestamp = (uint64_t)ZephyrTime_getUptimeMilli();
  self->message.header.bodyLength = messageBodySize;
  size_t messageFullSize = sizeof(MessageHeader_t) + messageBodySize + MESSAGE_HANDLER_CRC_SIZE;
  uint8_t buffer[messageFullSize];
  memcpy(buffer, &self->message.header, sizeof(MessageHeader_t));
  memcpy(buffer + sizeof(MessageHeader_t), messageBody, messageBodySize);
  uint16_t crc16 = _crc16(buffer, sizeof(MessageHeader_t)+messageBodySize);
  uint8_t crc[MESSAGE_HANDLER_CRC_SIZE] = {crc16 & 0xFF, crc16 >> 8};
  memcpy(buffer + sizeof(MessageHeader_t) + messageBodySize, crc, MESSAGE_HANDLER_CRC_SIZE);
  self->message.header.transactionId++;
  CommunicationInterface_send(self->communicationChannel, buffer, messageFullSize);
}

bool MessageHandler_receive(MessageHandler *self) {
  uint8_t buffer[MAXIMUM_BUFFER_SIZE];  // Buffer to hold received message
  size_t receivedSize;  // Size of the received message
  uint8_t receiveStatus = CommunicationInterface_receive(
    self->communicationChannel, buffer, MAXIMUM_BUFFER_SIZE, &receivedSize);
  if (receiveStatus == 0) {
    return MessageHandler_parseAndCheckCrc(self, buffer, receivedSize);
  }
  return false;
}

void MessageHandler_setHeader(MessageHandler *self, MessageHeader_t header) {
  self->message.header = header;
}

MessageHeader_t MessageHandler_getHeader(MessageHandler *self) {
  return self->message.header;
}

void MessageHandler_setOperation(MessageHandler *self, uint16_t operation) {
  self->message.header.operation = operation;
}

void MessageHandler_setInteractionStage(MessageHandler *self, uint8_t interactionStage) {
  self->message.header.interactionStage = interactionStage;
}

Message_t *MessageHandler_getMessage(MessageHandler *self) {
  return &self->message;
}

bool MessageHandler_isTelecommand(MessageHandler *self) {
  return self->message.header.service == MESSAGE_HEADER_SERVICE_TELECOMMAND;
}
