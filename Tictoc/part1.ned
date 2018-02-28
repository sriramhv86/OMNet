import ned.DatarateChannel;

simple Tic
{
    
    parameters:
   		volatile double delayTime @unit(s); // volatile variable is used to create processing delay, here it's not used
   		int limit = default(2);
    gates:
        input in;
        output out;
}

simple Toc
{
    parameters:
        volatile double delayTime1 @unit(s); // volatile variable is used to create processing delay, here it's not used
    gates:
        input in;
        output out;
}

network Tictoc
{         
    submodules:
        tic: Tic;
        toc: Toc;
    connections:
        tic.out --> {delay=100ms; datarate=100Mbps;} --> toc.in; // This delay is changed for 15 different simulations, datarate is kept constant
        tic.in <-- {delay=250ms; datarate=100Mbps;} <-- toc.out;
}

