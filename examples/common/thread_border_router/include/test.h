#pragma once

typedef enum Test {
  Throughput,
  PacketLoss,
  Delay
} Test;

#define THROUGHPUT_URI "throughput"
#define PACKET_LOSS_URI "packetloss"
#define DELAY_URI "delay"