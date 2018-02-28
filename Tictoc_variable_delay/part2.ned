
import ned.DatarateChannel;

simple Tic
{
    parameters:
   		volatile double delayTime @unit(s);
   		int limit = default(2);
    gates:
        input in;
        output out;
}

simple Toc
{
    parameters:
        volatile double delayTime1 @unit(s);
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
        tic.out --> {delay=100ms; datarate=10Mbps;} --> toc.in;
        tic.in <-- {delay=100ms; datarate=10Mbps;} <-- toc.out;
}

