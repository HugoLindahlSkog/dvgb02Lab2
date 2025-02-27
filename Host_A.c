#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sim_Engine.h"

#define MAX_QUEUE_SIZE 100  //queue-size
//queuee for messages
struct msg queue[MAX_QUEUE_SIZE];
int queue_start = 0, queue_end = 0, queue_size = 0;


int A_seqnum = 0;
int wait_ACK = 0;
struct pkt last_packet;

//calculate chechsum for packages
static int calc_checksum(struct pkt packet) {
    int checksum = packet.seqnum + packet.acknum;
    for (int i = 0; i < sizeof(packet.payload); i++) {
        checksum += packet.payload[i];
    }
    return checksum;
}
//add message in queue
void enqueue(struct msg message) {
    if (queue_size < MAX_QUEUE_SIZE) {
        queue[queue_end] = message;
        queue_end = (queue_end + 1) % MAX_QUEUE_SIZE;
        queue_size++;
    }
}

// remove message from queue
struct msg dequeue() {
    struct msg message;
    memset(&message, 0, sizeof(struct msg)); 
    if (queue_size > 0) {
        message = queue[queue_start];
        queue_start = (queue_start + 1) % MAX_QUEUE_SIZE;
        queue_size--;
    }
    return message;
}

//send message from above layer to lower layer
void A_output(struct msg message) {
    if (wait_ACK) {
        enqueue(message);
        return;
    }

    //creating the packet
    struct pkt packet;
    packet.seqnum = A_seqnum;
    packet.acknum = 0;
    memset(packet.payload, 0, sizeof(packet.payload));
    memcpy(packet.payload, message.data, 20);
    packet.checksum = calc_checksum(packet);


    //save packet for timeout
    memcpy(&last_packet, &packet, sizeof(struct pkt));

    //send to layer 3
    tolayer3(0, packet);
    starttimer(0, 15.0); 

    wait_ACK = 1;
}
  //packet from lower layer
void A_input(struct pkt packet) {
    if (packet.checksum != calc_checksum(packet)) {
        return; 
    }

    if (packet.acknum == A_seqnum) {
        stoptimer(0);
        A_seqnum = (A_seqnum + 1) % 2;  
        wait_ACK = 0;  
      //send next msg from queue if its not empty
        if (queue_size > 0) {
            struct msg next_msg = dequeue();
            A_output(next_msg);
        }
    }
}

//timeout 
void A_timerinterrupt() {
    tolayer3(0, last_packet);
    starttimer(0, 15.0);  // Starta timer igen
}

void A_init() {
    A_seqnum = 0;
    wait_ACK = 0;
    queue_size = 0;
    queue_start = 0;
    queue_end = 0;
}
