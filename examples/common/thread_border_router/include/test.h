#pragma once

#define NO_EXPERIMENT (CONFIG_EXPERIMENT == 0)
#define EXPERIMENT_THROUGHPUT_CONFIRMABLE (CONFIG_EXPERIMENT == 1)
#define EXPERIMENT_PACKET_LOSS_CONFIRMABLE (CONFIG_EXPERIMENT == 2)

#define MAX_PACKETS 1000
#define PAYLOAD_SIZE_BYTES 4

typedef enum Test
{
  ThroughputConfirmable,
  PacketLossConfirmable,
}
Test;

#define THROUGHPUT_CONFIRMABLE_URI "throughput-confirmable"
#define PACKET_LOSS_CONFIRMABLE_URI "packet-loss-confirmable"