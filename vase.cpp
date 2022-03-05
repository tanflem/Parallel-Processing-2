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
#define NUMSTARES 1000

// Mutex to allow only one guest in at a time
std::mutex in_vase_room;

// Condition variable representing the sign on the door
std::condition_variable sign_on_door;

// Atomic variable to track when the total number of stares at the vase reaches a certain number
std::atomic<int> stared_at_vase(0);

// Guest random selection
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, NUMGUESTS - 1);
auto dice = std::bind ( distribution, generator);


std::atomic<int> lucky_guest;
void guest(int viewer) {

    // While the total number of stares has not reached the specified number
    while (stared_at_vase < NUMSTARES){
        // Lock to keep more than one guest from getting into the vase room
        std::unique_lock<std::mutex> vase_room_lock(in_vase_room);

        while(viewer != lucky_guest && stared_at_vase < NUMSTARES){
            // Set the sign on the door to unaviliable

            // This is where the other guests can explore the castle

            sign_on_door.wait(vase_room_lock);
        }

        if(stared_at_vase < NUMSTARES){
            // Stare at the beautiful vase
            stared_at_vase++;

            // A new guest finds the door
            lucky_guest = dice();

            // previous guest flips the sign to open
            vase_room_lock.unlock(); 
            sign_on_door.notify_all();
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

    std::cout << "ALL GUESTS HAVE STARED AT THE VASE "<< NUMSTARES << " TIMES IN " << time << " SECONDS!\n";
}