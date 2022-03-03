#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#define NUMGUESTS 10

int num_guests = NUMGUESTS;

// Mutex to allow only one guest in at a time
std::mutex in_labyrinth;

// Condition variable to let the Minotaur know to find another guest
std::condition_variable exit_labyrinth;

void guest(int id) {
    // Variable to track the first guest that will eat the cupcakes
    int eater = 0;

    // bool to track if the cupcake has been refreshed or not
    bool cupcake = true;

    // Flag to find the first guest
    bool first_flag = true;

    // Array of bools to track if any threads have been through the labyrinth
    bool been_there[NUMGUESTS];

    // intialialize all to false 
    for(int i = 0; i < NUMGUESTS; i++){
        been_there[i] = false;
    }

    // While their are guests who have not entered the labyrinth
    while (num_guests > 0){
        std::unique_lock<std::mutex> labyrinth_lock(in_labyrinth);
        while((id != num_guests % NUMGUESTS) && (num_guests > 0)){
            // Exploring the castle and waiting for the minotaur to call them in
            exit_labyrinth.wait(labyrinth_lock); // Someone in Labrynith already
        }
        if( num_guests > 0){

            if(first_flag){
                // First person in becomes the eater
                eater = id;
                // Set to false after the first person
                first_flag = false; 
            }

            // If the person is the eater
            if(id == eater){
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
                // If it is their first time entering the maze refresh cupcake
                if(been_there[id]){
                    been_there[id] = true;
                    if(!cupcake){
                        cupcake = true;
                    }
                }
                
                // Else do nothing
                else{

                }
            }
            printf("Person %d went through the labyrinth.\n", id);

            labyrinth_lock.unlock(); // Unlock the labryrinth
            exit_labyrinth.notify_all(); // Minotaur tells another guest to enter labyrinth
        }
    }
}

int main(){

    // Minotaur invites 5 guests
    std::thread guestThreads[NUMGUESTS];
    
    for (int i = 0; i < NUMGUESTS; i++) {
        guestThreads[i] = std::thread(guest, i);
    }

    for (auto& gt : guestThreads) {
        gt.join();
    }

}