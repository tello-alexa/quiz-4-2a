#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
using namespace std;

bool responded = false;

void query(int num, mutex *m, condition_variable *cv) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
    lock_guard<mutex> guard(*m);
    cout << "[" << num << "] SYN ... "; 
    responded = true;
    cv->notify_one();  
}

void response(mutex *m, condition_variable *cv) {
    // Should print "ACK"
    unique_lock<mutex> mlock(*m); // lock
    cv->wait(mlock, [] { return responded ? true : false; }); // wait
    cout << "ACK" << endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }

    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */
    // get count from cmd args
    int count = atoi(argv[1]);

    // create synch primitives (semaphores, condition variables, mutexes)
    mutex m;
    condition_variable cv;
    
    // create threads with parameters
    /*vector<thread> threads;
    for(int i = 0; i < count; i++) {
        thread(query, i);
        thread(response, i);
    }

    for(size_t j = 0; j < threads.size(); ++j) {
        threads.at(j).join();
    }*/

    for(int i = 0; i < count; i++) {
        thread t1(query, i, &m, &cv);
        thread t2(response, &m, &cv);

        t1.join();
        t2.join();
    }
    

    return 0;
}