// Marcelino Pozo
/*
For Problem 2 I have choosen to implement strategy 2.
The advantage of this strategy allow for the guests to avoid any unnecessary attempts to enter the room
when it is occupied.

The disadvantage is that the guest could still possibly form a queue behind the door increasing wait times.

*/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

std::mutex mtx;
std::condition_variable cv;
bool showroomAvailable = true;
std::vector<std::thread> guests;

void guest(int id)
{
    cv.notify_all(); // Notify all guests that they have arrived simultaneously

    std::unique_lock<std::mutex> lock(mtx);
    // Check if the showroom is available
    while (!showroomAvailable) 
    {
        std::cout << "Guest " << id << " waits for the showroom to become available.\n";
        cv.wait(lock);
    }

    // Showroom is available, mark it as busy and enter
    showroomAvailable = false;
    std::cout << "Guest " << id << " enters the showroom.\n";    
    
    // Leave the showroom
    std::cout << "Guest " << id << " exits the showroom.\n";
    showroomAvailable = true;
    cv.notify_one(); // Notify the next guest waiting
}

int main() 
{
    const int numGuests = 5;
    guests.reserve(numGuests);

    // Start guest threads
    for (int i = 0; i < numGuests; ++i) 
    {
        guests.emplace_back(guest, i + 1);
    }

    // Wait for all guests to arrive
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&numGuests]() { return guests.size() == static_cast<std::size_t>(numGuests); });

    // Unlock the mutex to allow guests to proceed
    lock.unlock();
    cv.notify_all();

    // Join guest threads
    for (auto& guestThread : guests) 
    {
        guestThread.join();
    }

    return 0;
}
