//
//  Weather update server in C
//  Binds PUB socket to tcp://*:5556
//  Publishes random weather updates
//
#include <zmq.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define within(num) (int) ((float) num * random () / (RAND_MAX + 1.0))

int main () {
    void *context;          //  ØMQ context for our process
    void *socket;           //  Socket to talk to clients

    //  Initialize random number generator
    srandom ((unsigned) time (NULL));

    //  Prepare our context and socket
    context = zmq_init (1);
    socket = zmq_socket (context, ZMQ_PUB);
    zmq_bind (socket, "tcp://*:5556");
    zmq_bind (socket, "ipc://weather");

    while (1) {
        zmq_msg_t message;
        int zipcode, temperature, relhumidity;

        //  Get values that will fool the boss
        zipcode     = within (100000);
        temperature = within (215) - 80;
        relhumidity = within (50) + 10;
        
        //  Send message to all subscribers
        zmq_msg_init_size (&message, 20);
        sprintf ((char *) zmq_msg_data (&message),
            "%05d %d %d", zipcode, temperature, relhumidity);
        zmq_send (socket, &message, 0);
        zmq_msg_close (&message);
    }
    return 0;
}
