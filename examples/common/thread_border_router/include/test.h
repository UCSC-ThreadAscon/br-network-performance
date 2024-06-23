#pragma once

#define NO_EXPERIMENT (CONFIG_EXPERIMENT == 0)
#define EXPERIMENT_THROUGHPUT_CONFIRMABLE (CONFIG_EXPERIMENT == 1)
#define EXPERIMENT_THROUGHPUT_NONCONFIRMABLE (CONFIG_EXPERIMENT == 2)
#define EXPERIMENT_PACKET_LOSS_CONFIRMABLE (CONFIG_EXPERIMENT == 3)
#define EXPERIMENT_PACKET_LOSS_NONCONFIRMABLE (CONFIG_EXPERIMENT == 4)

#define SAMPLE_SIZE_PACKETS 1000
#define PAYLOAD_SIZE_BYTES 4

typedef enum Test {
  ThroughputConfirmable,
  ThroughputNonConfirmable,
  PacketLossConfirmable,
  PacketLossNonConfirmable
} Test;

#define THROUGHPUT_CONFIRMABLE_URI "throughput-confirmable"
#define THROUGHPUT_NONCONFIRMABLE_URI "throughput-nonconfirmable"
#define PACKET_LOSS_CONFIRMABLE_URI "packet-loss-confirmable"
#define PACKET_LOSS_NONCONFIRMABLE_URI "packet-loss-nonconfirmable"