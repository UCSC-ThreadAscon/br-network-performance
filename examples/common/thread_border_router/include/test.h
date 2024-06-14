#pragma once

typedef enum Test {
  Confirmable,
  NonConfirmable,
  PacketLoss,
  Delay
} Test;

#define THROUGHPUT_CONFIRMABLE_URI "confirmable"
#define THROUGHPUT_NONCONFIRMABLE_URI "nonconfirmable"
#define PACKET_LOSS_URI "packetloss"
#define DELAY_URI "delay"