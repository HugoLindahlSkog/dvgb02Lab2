#include "Sim_Engine.h"
#include <string.h>
#include <stdio.h> 


/* Called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message) {
  
  //Create packet
  struct pkt packet;
  //Define seqnumber and acknumber
  packet.seqnum = 0;
  packet.acknum = 0;



  //Copy message
  memcpy(packet.payload, message.data, sizeof(message.data));
  //Send packet to layer 3 (Host B)
  tolayer3(0, packet);

  //printf to see if it works
  printf("sending packet  %d, %s", packet.seqnum, packet.payload);


  

}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
  /* TODO */
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  /* TODO */
}  

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  

}
