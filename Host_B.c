#include "Sim_Engine.h"
#include <stdio.h>
#include <string.h>

 static int calc_checksum(struct pkt packet)
{
  int checksum = packet.seqnum + packet.acknum;

  for(int i = 0; i < sizeof(packet.payload); i++)
  {
    checksum += packet.payload[i];

  }
  return checksum;
}
/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet) {
 
 int rec_checksum = calc_checksum(packet);
 
 if(packet.checksum != rec_checksum)
 {
  printf("corrupt packet\n");
  return;
 }
  struct pkt ack_packet;

  printf("packet delieverd %d, %s", packet.seqnum, packet.payload);

  ack_packet.seqnum = packet.seqnum; //same seqnum
  ack_packet.acknum = 1; //Ack is 1
  ack_packet.checksum = calc_checksum(packet);

  tolayer3(1, ack_packet);

}

/* Called when B's timer goes off */
void B_timerinterrupt() {
  /* TODO */
}  

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init() {
  /* TODO */
}
