#pragma once

#define NO_EXPERIMENT (CONFIG_EXPERIMENT == 0)
#define EXPERIMENT_THROUGHPUT_CONFIRMABLE (CONFIG_EXPERIMENT == 1)
#define EXPERIMENT_THROUGHPUT_NONCONFIRMABLE (CONFIG_EXPERIMENT == 2)

#define SAMPLE_SIZE_PACKETS 1000
#define PAYLOAD_SIZE_BYTES 4

typedef enum PacketType {
  Confirmable,
  NonConfirmable
} PacketType;

#define CONFIRMABLE_URI "confirmable"
#define NONCONFIRMABLE_URI "nonconfirmable"
#define DELAY_URI "delay"