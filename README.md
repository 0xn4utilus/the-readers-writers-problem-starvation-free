# The Readers Writers Problem - Starvation Free

## What is `The Readers Writers Problem` ?

The Readers Writers Problem is a classic synchronization problem in computer science that occurs when multiple threads or processes need to access a shared resource, such as a file or a database, simultaneously.

`Readers` - read data from shared resources without modifying it

`Writers` - modify data on shared resources

## Starvation

Starvation happens if a process is indefinitely delayed. This can emerge once a process needs a further resource for execution that isn’t assigned.

These resources can be:

* CPU time
* Memory
* Disk space
* Network bandwidth
* I/O access to network or disk

## Classical Solution

### Initialization

```cpp
//....... initialization .......
shared_resources_lock = 1;
read_count = 0;
mutex = 1;
```

### Classical Implementation - `Writer`

```cpp
writer() {
    do {
        wait(shared_resources_lock);  // acquires lock over shared resources mutex

        //
        //       Critical Section  -  do writing over shared resources
        //

        signal(shared_resources_lock);  // releases lock over shared resources mutex
    } while (true);
}
```

### Classical Implementation - `Reader`

```cpp
reader() {
    read_count = 0;                       // initialize read_count to 0
    do {
        wait(mutex);                      // acquire lock over mutex
        read_count++;                     // increment readers count
        if (read_count == 1) {            // initial reader
            wait(shared_resources_lock);  // acquire lock over shared resources
        }

        signal(mutex);                    // releases lock over mutex

        //
        //  Critical Section  -  do reading from shared resources
        //

        wait(mutex);                        // acquires lock over mutex
        read_count--;                       // decrements readers count
        if (read_count == 0) {              // last reader
            signal(shared_resources_lock);  // releases lock over shared resources
        }

        signal(mutex);                      // releases lock over mutex

    } while (true);
}
```

## Problems in Classical Solution

In the classical sloution writers can starve, which means they may be blocked indefinitely and never get access to the shared resource. This can happen if multiple readers continually arrive and acquire the shared resource, preventing the writer from ever acquiring the lock.

## Starve Free Solution

### Initialization

```cpp
//....... initialization .......
entry_mutex =1;
shared_resources_lock = 1;
read_count = 0;
readers_mutex = 1;
```

### Semaphore using waiting_queue

```cpp
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

```

The `entry_mutex` semaphore which is used to gives equal chance to both reader and writer. It solves the problem of starvation such that when reader/writer tries to acquire `shared_resources_mutex` it has to first acquire `entry_mutex` and only the process which tried to acquire `shared_resources_mutex` first will be given lock over `entry_mutex` which is the front element of `waiting_queue`. In this way no process will suffer from starvation.

### Starvefree Implementation - `Writer`

```cpp
writer() {
    do {
        wait(entery_mutex, pid);           // acquire lock over entry mutex
        wait(shared_resources_lock, pid);  // acquires lock over shared resources
        signal(entry_mutex);          // releases lock over entry mutex

        //
        //       Critical Section   -  do writing over shared resources
        //

        signal(shared_resources_lock);  // releases lock over shared resources
    } while (true);
}
```

### Starvefree Implementation - `Reader`

```cpp
reader() {
    read_count = 0;                        // initialize read_count to 0
    do {
        wait(entry_mutex, pid);            // acquire lock over entry mutex
        wait(readers_mutex, pid);          // acquire lock over readers mutex
        signal(entry_mutex);               // release lock over entry mutex
        read_count++;                      // increment readers count
        if (read_count == 1) {             // initial reader

            wait(shared_resources_lock, pid);   // acquire lock over shared resources
        }

        signal(readers_mutex);             // releases lock over readers mutex

        //
        //  Critical Section  -- do reading over shared resources
        //

        wait(readers_mutex, pid);                // acquires lock over readers mutex
        read_count--;                       // decrements readers count
        if (read_count == 0) {              // last reader
            signal(shared_resources_lock);  // releases lock over shared resources
        }

        signal(readers_mutex);              // releases lock over readers mutex

    } while (true);
}
```

## Submission Details

Gyanendra Kumar Banjare

21114040

B.Tech CSE 2y