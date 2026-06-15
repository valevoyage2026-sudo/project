#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#define TIME std::chrono::steady_clock::now()

// Simple, clean log macro
#define LOG(x) std::cout << x << std::endl

std::mutex m;
std::condition_variable cv;
bool ready = false; // Guarded by mutex m

void work()
{
    // 1. Acquire the lock
    std::unique_lock<std::mutex> lock(m);

    // 2. Safely wait until 'ready' is true.
    // This safely releases the lock while waiting and re-acquires it upon waking.
    cv.wait(lock, []{ return ready; });

    // 3. This section is safely synchronized
    LOG(std::this_thread::get_id() << " Accessed");
}

int main()
{
    auto start = TIME;

    // Spin up the worker threads
    std::thread t2(work);
    std::thread t3(work);

    // Simulate some main thread work
    for(int i = 0; i < 10; i++)
    {
        LOG("Main thread working: " << i);
    }

    // 4. Safely update the shared state under the lock scope
    {
        std::lock_guard<std::mutex> lock(m);
        ready = true;
    } //basically prevents others threads from reading "ready" while main is updating it.

    // 5. Notify the sleeping threads
    cv.notify_all();

    // 6. Join threads to clean up resources
    t2.join();
    t3.join();

    auto end = TIME;
    auto TotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    LOG("Total time: " << TotalTime << "ms");
    return 0;
}
