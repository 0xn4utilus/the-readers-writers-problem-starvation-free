
reader() {
    do {
        wait(mutex);                      // acquire lock over mutex
        read_count++;                     // increment readers count
        if (read_count == 1) {            // initial reader
            wait(shared_resources_lock);  // acquire lock over shared resources
        }

        signal(mutex);                    // releases lock over mutex

        //
        //  Critical Section
        //

        wait(mutex);                        // acquires lock over mutex
        read_count--;                       // decrements readers count
        if (read_count == 0) {              // last reader
            signal(shared_resources_lock);  // releases lock over shared resources
        }

        signal(mutex);                      // releases lock over mutex

    } while (true);
}

writer() {
    do {
        wait(mutex);  // acquires lock over mutex

        //
        //       Critical Section
        //

        signal(mutex);  // releases lock over mutex
    } while (true);
}