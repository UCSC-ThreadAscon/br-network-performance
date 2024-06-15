#pragma once

typedef enum PacketType {
  Confirmable,
  NonConfirmable
} PacketType;

#define CONFIRMABLE_URI "confirmable"
#define NONCONFIRMABLE_URI "nonconfirmable"
#define DELAY_URI "delay"