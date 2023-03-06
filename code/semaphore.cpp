class Semaphore {
   public:
    int value = 1;

    Queue<processes> waiting_processes;        // queue of processes containing process information

    Semaphore(int x) {                        // constructor
        this.value = x;
    }
}

wait(Semaphore s, process pid) {
    s.value --;                               // decrement value

    if(s.value < 0){
        s.waiting_queue.push(pid);
        sleep(pid);                           // let the process sleep
    }

}

signal(Semaphore s) {
    s.value++;                                // increment value
    if(s.value <= 0){
        process pid = s.waiting_queue.pop();  // get the front pid from waiting_queue and remove it from queue
        wakeup(pid);
    }

}
