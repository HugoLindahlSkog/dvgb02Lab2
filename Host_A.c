#include "Sim_Engine.h"
#include <string.h>
#include <stdio.h> 


int A_seqnum = 0;
int wait_ACK = 0;
struct pkt last_packet;


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


/* Called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message) {
  
  printf("DEBUG: A_output called with payload: %s\n", message.data);
//debug

if (wait_ACK)
{
  //wait for ack before sending new packet
  printf("Väntar på ACK, kan ej skicka nytt packet\n");
  return;
}


  //Create packet
  struct pkt packet;
  //Define seqnumber and acknumber
  packet.seqnum = A_seqnum; //set seqnum to 0 or 1
  packet.acknum = 0; 



  //Copy message
  memset(packet.payload, 0, sizeof(packet.payload)); 
  memcpy(packet.payload, message.data, 20);
  
  packet.checksum = calc_checksum(packet);

  memcpy(&last_packet, &packet, sizeof(struct pkt)); //save last packet
  //Send packet to layer 3 (Host B)

  printf("A_output: Attempting to send packet %d, payload: %s\n", 
    packet.seqnum, message.data); //debug

    printf("DEBUG: A sending packet %d with payload: %s\n", packet.seqnum, packet.payload);
//debug

  tolayer3(0, packet);

  starttimer(0, 15.0);


  //printf to see if it works
  printf(" sending packet  %d, %d, %s\n", packet.seqnum, packet.checksum, packet.payload);
  wait_ACK = 1; //wait for ACK
  
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
 
 if (packet.checksum != calc_checksum(packet))
 {
  printf("Mottaget ACK är korrupt, ignorerar\n");
  return;
 }

 printf("A_input: Received ACK %d, expected ACK %d\n", packet.acknum, A_seqnum);
 //debug

  if(packet.acknum == A_seqnum)
  {
    stoptimer(0); //if acknum is same as seqnum, stop timer
    printf("Recieved ack for packet %d\n", packet.seqnum);

    A_seqnum = (A_seqnum + 1) % 2; //change seqnum
    wait_ACK = 0; 
  } else {
    printf("Felaktigt ACK (%d), förväntade %d. ignorerar.\n", packet.acknum, A_seqnum);
  }
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  printf("A_timerinterupt: Sending packet again...\n");

  printf("A_timerinterrupt: Resending packet %d, payload: %s\n", 
    last_packet.seqnum, last_packet.payload); //debug

  tolayer3(0, last_packet);

  starttimer(0, 15.0);
}  

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  

}
