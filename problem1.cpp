// Marcelino Pozo
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex mtx;
bool all_guests_visited = false;
int num_guests_visited = 0;
const int N;

void minotaur() 
{
    // Wait for guests to arrive
    std::this_thread::sleep_for(std::chrono::seconds(1)); 
    std::cout << "The Minotaur welcomes the guests!\n";
    
    // Wait for all guests to visit the labyrinth
    while (!all_guests_visited) 
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "All guests have visited the labyrinth. The party is over!\n";
}

void guest(int id) 
{
    mtx.lock();
    std::cout << "Guest " << id << " enters the labyrinth.\n";
    mtx.unlock();
    
    // Decide whether to eat the cupcake or not
    bool eat_cupcake = (id % 2 == 0); // Even guests eat cupcakes
    if (eat_cupcake) 
    {
        mtx.lock();
        std::cout << "Guest " << id << " eats the cupcake.\n";
        mtx.unlock();
    } 
    else 
    {
        mtx.lock();
        std::cout << "Guest " << id << " leaves the cupcake.\n";
        mtx.unlock();
    }
    
    mtx.lock();
    num_guests_visited++;
    if (num_guests_visited == 10) 
    {
        all_guests_visited = true;
    }

    mtx.unlock();
}

int main() 
{
    // Create and start the minotaur thread    
    std::thread minotaur_thread(minotaur);
    // Create the list of guest_threads
    std::vector<std::thread> guest_threads;
    
    // Start the guest threads using the guest function with i as the ID
    for (int i = 0; i < N; ++i) 
    {
        guest_threads.push_back(std::thread(guest, i));
    }
    
    // Blocks main until completing until minotaur is done
    minotaur_thread.join();

    // Make sure main doesnt complete until each of the guests are finished
    for (auto& t : guest_threads) 
    {
        t.join();
    }
    
    return 0;
}
