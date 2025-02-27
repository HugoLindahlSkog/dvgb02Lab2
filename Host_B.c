#include "Sim_Engine.h"
#include <stdio.h>
#include <string.h>


int B_seqnum = 0;
struct pkt ack_packet;

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
 
if (packet.checksum != calc_checksum(packet))
{
  printf("B_input: Paket är korrupt, ignorerar.\n");
  return;
}

  printf("B_input: Received packet %d, checksum: %d, payload: %s\n", 
    packet.seqnum, packet.checksum, packet.payload); //debug

    printf("DEBUG: B received raw payload: %s\n", packet.payload);
//debug

if(packet.seqnum == B_seqnum) //if seqnum is expected seqnum
{
  tolayer5(1, packet.payload); //send to layer 5

  ack_packet.acknum = B_seqnum; //set ack num to expected seqnum
  ack_packet.seqnum = 0;  //set seqnum to standard
  ack_packet.checksum = calc_checksum(ack_packet);

  tolayer3(1, ack_packet); //send to layer 3

  B_seqnum = (B_seqnum + 1) % 2; //chnage to next seqnum
} else {

  printf("Duplicerat packet, skickar om senaste ACK\n");

  ack_packet.acknum = (B_seqnum + 1) % 2; //send latest ACK
  ack_packet.checksum = calc_checksum(ack_packet);
  tolayer3(1, ack_packet);
}


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
