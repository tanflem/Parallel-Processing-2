#include <iostream>
#include <unistd.h>
#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <chrono>

#define NUMGUESTS 100

// Mutex to allow only one guest in at a time
std::mutex in_labyrinth;

// Condition variable to let the Minotaur know to find another guest
std::condition_variable exit_labyrinth;

// Variable to track the first guests id and Flag to find the first guest
std::atomic<int> eater;
std::atomic<bool> first_flag = true;

// bool to track if the cupcake has been refreshed or not
std::atomic<bool> cupcake = true;

std::atomic<int> num_guests(NUMGUESTS);

void guest(int chosen) {
    // Array of bools to track if any threads have been through the labyrinth
    bool been_there[NUMGUESTS];

    // intialialize all to false 
    for(int i = 0; i < NUMGUESTS; i++){
        been_there[i] = false;
    }

    // Minotaur random selection
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, NUMGUESTS);
    auto minotaur = std::bind ( distribution, generator);

    // While their are guests who have not entered the labyrinth
    while (num_guests > 0){
        // Lock to keep more than one guest from getting into the maze
        std::unique_lock<std::mutex> labyrinth_lock(in_labyrinth);
        
        while((chosen == minotaur()) && (num_guests > 0)){
            // Exploring the castle and waiting for the minotaur to call them in
            exit_labyrinth.wait(labyrinth_lock);
        }

        if( num_guests > 0){

            if(first_flag){
                // First person in becomes the eater
                eater = chosen;
                // Set to false after the first person
                first_flag = false; 
            }

            // If the person is the eater
            if(chosen == eater){
                // And there is a cupcake present
                if(cupcake){
                    // Eat the cupcake
                    cupcake = false;
                    
                    // And decrement the number of guests left to enter
                    num_guests--;
                }
            }
            // Else the person is not the eater
            else{               
                // If the cupcake is not refreshed and 
                if(!cupcake){
                    // And it is their first time in the maze. 
                    if(been_there[chosen] == false){
                        // Remember fro yourself that you have been there
                        been_there[chosen] = true;
                        // Refresh cupcake
                        cupcake = true;
                    }
                }
            }
            // Unlock the labyrinth and let the minotaur notify another guest
            labyrinth_lock.unlock(); 
            exit_labyrinth.notify_all();
        }
    }
}

int main(){

    clock_t start, end;


    start = clock();
    std::thread guestThreads[NUMGUESTS];

    for(int i = 0; i < NUMGUESTS; i++){
        guestThreads[i] = std::thread(guest, i);
    }

    for (auto& gt : guestThreads) {
        gt.join();
    }

    end = clock();

    float time = ((float) end - start) / CLOCKS_PER_SEC;

    std::cout << "ALL GUESTS HAVE COMPLETED THE LABYRINTH IN " << time << " seconds\n";
}

