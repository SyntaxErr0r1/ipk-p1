fs = require('fs');

// user    nice   system  idle      iowait irq   softirq  steal


prevstat = fs.readFileSync("/proc/stat").toString().split("\n")[0];
prevstatSplit = prevstat.split(" ")
prevuser = prevstatSplit[1];
prevnice = prevstatSplit[2];
prevsystem = prevstatSplit[3];
previdle = prevstatSplit[4];
previowait = prevstatSplit[5];
previrq = prevstatSplit[6];
prevsoftirq = prevstatSplit[7];
prevsteal = prevstatSplit[8];

setTimeout( () => {
    stat = fs.readFileSync("/proc/stat").toString().split("\n")[0];
    
    statSplit = stat.split(" ")
    user = statSplit[1];
    nice = statSplit[2];
    system = statSplit[3];
    idle = statSplit[4];
    iowait = statSplit[5];
    irq = statSplit[6];
    softirq = statSplit[7];
    steal = statSplit[8];
    
    PrevIdle = previdle + previowait
    Idle = idle + iowait

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    NonIdle = user + nice + system + irq + softirq + steal

    PrevTotal = PrevIdle + PrevNonIdle
    Total = Idle + NonIdle

    // differentiate: actual value minus the previous one
    totald = Total - PrevTotal
    idled = Idle - PrevIdle

    CPU_Percentage = (totald - idled)/totald
    
    console.log("CPU load:",CPU_Percentage,"%");
}, 100)

