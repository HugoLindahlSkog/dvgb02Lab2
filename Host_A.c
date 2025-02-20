#include "Sim_Engine.h"
#include <string.h>
#include <stdio.h> 


/* Called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message) {

  struct pkt testpacket;

  testpacket.seqnum = 0;
  testpacket.acknum = 0;



  
  memcpy(testpacket.payload, message.data, sizeof(message.data));
  
  tolayer3(0, testpacket);

  printf("sending packet  %d, %s", testpacket.seqnum, testpacket.payload);


  

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
