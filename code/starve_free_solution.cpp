reader() {
    read_count = 0;                       // initialize read_count to 0
    do {
        wait(entry_mutex);      // acquire lock over entry mutex
        wait(readers_mutex);    // acquire lock over readers mutex
        signal(entry_mutex);    // release lock over entry mutex
        read_count++;           // increment readers count
        if (read_count == 1) {  // initial reader

            wait(shared_resources_lock);  // acquire lock over shared resources
        }

        signal(readers_mutex);  // releases lock over readers mutex

        //
        //  Critical Section  -- do reading over shared resources
        //

        wait(readers_mutex);                // acquires lock over readers mutex
        read_count--;                       // decrements readers count
        if (read_count == 0) {              // last reader
            signal(shared_resources_lock);  // releases lock over shared resources
        }

        signal(readers_mutex);  // releases lock over readers mutex

    } while (true);
}

writer() {
    do {
        wait(entery_mutex);           // acquire lock over entry mutex
        wait(shared_resources_lock);  // acquires lock over shared resources
        signal(entry_mutex);          // releases lock over entry mutex

        //
        //       Critical Section   -  do writing over shared resources
        //

        signal(shared_resources_lock);  // releases lock over shared resources
    } while (true);
}