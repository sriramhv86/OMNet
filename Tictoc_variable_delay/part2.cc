
//Program for part2 of Omnet project

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

SimTime duration, duration2; //Variables to store the time stamp of packets

class Tic : public cSimpleModule
{
  private:
    int counter;  // Counter to limit no of simulation
    cMessage *event;  //pointer to the event which is used to create self message
    cMessage *tictocMsg; // Self message
    int seq;  // message sequence number
    cOutVector durationVector, delayVector; // Variables to store values to graph
  public:
    Tic();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Tic);

Tic::Tic()
{
    event = tictocMsg = nullptr;
}

void Tic::initialize()
{
    // Initialise variables.
    counter = par("limit");
    event = new cMessage("event");
    tictocMsg = nullptr;
    seq = 0;
    // Generate and send initial message.
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
        if (msg == event) {
            duration = simTime() - duration; // Subtracting creation time by current time
            durationVector.record(duration); // Used to plot graph of propagation delay with respect to tic(A)
            EV  << " Received :" << msg->getName() << " and the propagation delay is:" << duration << "ms\n";
            char msgname[20];
            sprintf(msgname, "tic-%d", ++seq);
            msg = new cMessage(msgname);
            send(msg, "out");

        }
        else {

            duration2 = msg->getCreationTime();
            simtime_t delay = par("delayTime");
            //delayVector.record(delay); // Used to plot normal distribution function graph which is used to create variable delay
            scheduleAt(simTime()+delay, event);
        }
    }
}

class Toc : public cSimpleModule
{

  private:
    cMessage *event;
    cMessage *tictocMsg;
    cOutVector duration2Vector;
    int seq=0;  // message sequence number
  public:
     Toc();
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Toc);

Toc::Toc()
{
    event = tictocMsg = nullptr;
}

void Toc::initialize()
{
    event = new cMessage("event");
    tictocMsg = nullptr;
}

void Toc::handleMessage(cMessage *msg)
{
    if(msg==event){
        duration2 = simTime() - duration2;
        //duration2Vector.record(duration2); // Used to plot graph of propagation delay with respect to toc(B)
        EV  << " Received :" << msg->getName() << " and the propagation delay is:" << duration2 << "ms\n";
        char msgname[20];
        sprintf(msgname, "tic-%d", ++seq);
        msg = new cMessage(msgname);
        send(msg, "out");
    }
    else{
        duration = msg->getCreationTime();
        simtime_t delay1 = par("delayTime1");
        //duration2Vector.record(delay1);  // Used to plot normal distribution function graph which is used to create variable delay
        scheduleAt(simTime()+delay1, event);
    }
}

