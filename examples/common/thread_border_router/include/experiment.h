#pragma once

#define NO_EXPERIMENT (CONFIG_EXPERIMENT == 0)
#define EXPERIMENT_THROUGHPUT_CONFIRMABLE (CONFIG_EXPERIMENT == 1)
#define EXPERIMENT_PACKET_LOSS_CONFIRMABLE (CONFIG_EXPERIMENT == 2)
#define EXPERIMENT_THROUGHPUT_UDP (CONFIG_EXPERIMENT == 3)
#define EXPERIMENT_PACKET_LOSS_UDP (CONFIG_EXPERIMENT == 4)

#define MAX_PACKETS 1000
#define PAYLOAD_SIZE_BYTES 4

typedef enum Experiment
{
  ThroughputConfirmable,
  PacketLossConfirmable,
}
Experiment;

#define THROUGHPUT_CONFIRMABLE_URI "throughput-confirmable"
#define PACKET_LOSS_CONFIRMABLE_URI "packet-loss-confirmable"