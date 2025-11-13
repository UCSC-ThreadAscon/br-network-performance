#pragma once

#include <stdint.h>

#define OBSERVE_SERVER_IP_ADDRESS CONFIG_COAP_OBSERVE_SERVER_IP

#define NO_EXPERIMENT (CONFIG_EXPERIMENT == 0)
#define EXPERIMENT_THROUGHPUT_CONFIRMABLE (CONFIG_EXPERIMENT == 1)
#define EXPERIMENT_PACKET_LOSS_CONFIRMABLE (CONFIG_EXPERIMENT == 2)
#define EXPERIMENT_THROUGHPUT_OBSERVE (CONFIG_EXPERIMENT == 3)
#define EXPERIMENT_PACKET_LOSS_OBSERVE (CONFIG_EXPERIMENT == 4)
#define EXPERIMENT_RTT_CPU_CONFIRMABLE (CONFIG_EXPERIMENT == 5)

#define MAX_PACKETS 1000
#define PAYLOAD_SIZE_BYTES 4

typedef enum Experiment
{
  ThroughputConfirmable,
  PacketLossConfirmable,
  RttCpuConfirmable
}
Experiment;

#define THROUGHPUT_CONFIRMABLE_URI "throughput-confirmable"
#define PACKET_LOSS_CONFIRMABLE_URI "packet-loss-confirmable"
#define RTT_CPU_CONFIRMABLE_URI "rtt-cpu-confirmable"

#define OBSERVE_SERVER_URI "temperature"

/**
 * CoAP Observe payloads simulate sensor data from a thermometer.
 *
 * According to the World Meteorological Organization:
 * https://wmo.asu.edu/content/world-highest-temperature
 * 
 * The highest temperature recorded was 134° Fahrenheit. As a result,
 * the temperature will be stored as an unsigned 8 bit integer.
 */
typedef uint8_t Fahrenheit;

#define OBSERVE_MAX_PACKETS 1000
#define OBSERVE_PAYLOAD_SIZE_BYTES sizeof(Fahrenheit)