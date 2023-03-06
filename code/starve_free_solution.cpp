// ....... initialization .......
// entry_mutex =1;
// shared_resources_lock = 1;
// read_count = 0;
// readers_mutex = 1;

reader() {
    read_count = 0;                       // initialize read_count to 0
    do {
        wait(entry_mutex, pid);      // acquire lock over entry mutex
        wait(readers_mutex, pid);    // acquire lock over readers mutex
        signal(entry_mutex);    // release lock over entry mutex
        read_count++;           // increment readers count
        if (read_count == 1) {  // initial reader

            wait(shared_resources_lock, pid);  // acquire lock over shared resources
        }

        signal(readers_mutex);  // releases lock over readers mutex

        //
        //  Critical Section  -- do reading over shared resources
        //

        wait(readers_mutex, pid);                // acquires lock over readers mutex
        read_count--;                       // decrements readers count
        if (read_count == 0) {              // last reader
            signal(shared_resources_lock);  // releases lock over shared resources
        }

        signal(readers_mutex);  // releases lock over readers mutex

    } while (true);
}

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