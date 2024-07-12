#include "handler.h"

void packetLossRequestHandler(void* aContext,
                              otMessage *aMessage,
                              const otMessageInfo *aMessageInfo)
{
  static uint32_t received = 0;
  received += 1;

  uint32_t packetsLost = PACKET_LOSS_MAX_PACKETS - received;
  double packetLoss = ((double) packetsLost) / PACKET_LOSS_MAX_PACKETS;

  /** These print statements are not debugging statement, as these
   *  print statement may be for the last packet. If so, these statements
   *  report the packet loss for the experiment.
   */
  PrintDelimiter();
  printRequest(aMessage, aMessageInfo);
  otLogNotePlat("Received: %" PRIu32 " packets.", received);
  otLogNotePlat("Packet Loss Ratio: %.3f", packetLoss);
  PrintDelimiter();

  /** Calling sendCoapResponse() will not affect the Non-Confirmable tests,
   *  since the function will only ACK if the request is a GET or Confirmable.
   */
  sendCoapResponse(aMessage, aMessageInfo);
  return;
}