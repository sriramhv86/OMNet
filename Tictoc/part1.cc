
// Program for part1 of Omnet project

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

SimTime duration, duration2; //Variables to store the time stamp of packets

class Tic : public cSimpleModule
{
  private:
    int counter;  // Counter to limit no of simulation
    int seq;  // message sequence number

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Tic);

void Tic::initialize()
{
    // Initialise variables.
    counter = par("limit");
    seq = 0;
    // Generate and send initial message.
    simtime_t delay = par("delayTime");
    EV << "Sending initial message\n";
    char msgname[20];
    sprintf(msgname, "toc-%d", ++seq);  // Initial message
    cMessage *msg = new cMessage(msgname);
    send(msg, "out");
}

void Tic::handleMessage(cMessage *msg)
{
    counter--;
    if(counter==0){
        EV << getName() << "'s counter reached zero, deleting message\n";
        delete msg;
    }
    else{
            //duration2 = msg->getCreationTime();
            duration = simTime() - duration; // Subtracting creation time by current time
            EV  << " Received :" << msg->getName() << " and the propagation delay is:" << duration << "ms\n";
            char msgname[20];
            sprintf(msgname, "tic-%d", ++seq);
            msg = new cMessage(msgname);
            send(msg, "out");

    }
}

class Toc : public cSimpleModule
{
  private:
    int seq=0;  // message sequence number

  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Toc);


void Toc::handleMessage(cMessage *msg)
{
        duration = msg->getCreationTime();
        //duration2 = simTime() - duration2;
        //EV  << " Received :" << msg->getName() << " and the propagation delay is:" << duration2 << "ms\n";
        char msgname[20];
        sprintf(msgname, "tic-%d", ++seq);
        msg = new cMessage(msgname);
        send(msg, "out");
}

