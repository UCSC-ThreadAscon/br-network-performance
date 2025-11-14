#include "independent_variables.h"

void printCipherSuite()
{
#if (CIPHER_SUITE == AES)
  otLogNotePlat("Cipher Suite: AES");
#elif (CIPHER_SUITE == NONE)
  otLogNotePlat("Cipher Suite: No Encryption");
#elif (CIPHER_SUITE == LIBASCON_128A)
  otLogNotePlat("Cipher Suite: ASCON-128a (LibAscon)");
#elif (CIPHER_SUITE == LIBASCON_128)
  otLogNotePlat("Cipher Suite: ASCON-128 (LibAscon)");
#elif (CIPHER_SUITE == ASCON_AEAD_128)
  otLogNotePlat("Cipher Suite: AsconAead128");
#elif (CIPHER_SUITE == CHA_CHA_POLY)
  otLogNotePlat("Cipher Suite: ChaChaPoly");
#endif
  return;
}

void printNetPerfTest()
{
#if THROUGHPUT_CONFIRMABLE
  otLogNotePlat("Current Test: Throughput Confirmable");
#elif PACKET_LOSS_CONFIRMABLE
  otLogNotePlat("Current Test: Packet Loss Confirmable");
#elif THROUGHPUT_OBSERVE
  otLogNotePlat("Current Test: Throughput Observe (Non-Confirmable)");
#elif PACKET_LOSS_OBSERVE
  otLogNotePlat("Current Test: Throughput Observe (Non-Confirmable)");
#elif DELAY_CLIENT
  otLogNotePlat("Current Test: Delay Client (Confirmable)");
#elif DELAY_SERVER
  otLogNotePlat("Current Test: Delay Server (Confirmable)");
#endif
  return;
}

void printTxPower()
{
  int8_t txPower = 0;
  if (getTxPower(&txPower) != OT_ERROR_NONE)
  {
    otLogCritPlat("Failed to get TX Power.");
  }
  return;
}

void printNetworkKey()
{
  otNetworkKey key;
  EmptyMemory(&key, sizeof(otNetworkKey));

  otThreadGetNetworkKey(esp_openthread_get_instance(), &key);
  otDumpNotePlat("Thread Network Key:", key.m8, OT_NETWORK_KEY_SIZE);
  return;
}