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
    
    // get count from cmd args
    int count = atoi(argv[1]);

    // create synch primitives (semaphores, condition variables, mutexes)
    mutex m;
    condition_variable cv;
    
    // create threads with parameters
    for(int i = 0; i < count; i++) { // loops for each line printed
        // create and run threads
        thread t1(query, i, &m, &cv);
        thread t2(response, &m, &cv);

        // join threads
        t1.join();
        t2.join();
    }    

    return 0;
}