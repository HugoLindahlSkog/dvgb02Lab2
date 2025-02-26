#include "Sim_Engine.h"
#include <string.h>
#include <stdio.h> 

//helpfunction to calculate checksum, seqnum + acknum
static int calc_checksum(struct pkt packet)
{
  int checksum = packet.seqnum + packet.acknum;

  for(int i = 0; i < sizeof(packet.payload); i++)
  {
    checksum += packet.payload[i];
 
  }
  return checksum;
}

struct pkt last_packet;

/* Called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message) {
  
  //Create packet
  struct pkt packet;
  //Define seqnumber and acknumber
  packet.seqnum = 0;
  packet.acknum = 0;



  //Copy message
  memcpy(packet.payload, message.data, sizeof(message.data));
  
  packet.checksum = calc_checksum(packet);

  memcpy(&last_packet, &packet, sizeof(struct pkt));
  //Send packet to layer 3 (Host B)
  tolayer3(0, packet);

  starttimer(0, 15.0);


  //printf to see if it works
  printf(" sending packet  %d, %d, %s\n", packet.seqnum, packet.checksum, packet.payload);

  
  

}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
 
 
  if(packet.acknum == 1)
  {

    printf(" Recieved ack for packet %d\n", packet.seqnum);
  }
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  printf("A_timerinterupt: Sending packet again...\n");
  tolayer3(0, last_packet);

  starttimer(0, 15.0);
}  

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  

}
