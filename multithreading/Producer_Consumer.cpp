#include <iostream>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#define TIME std::chrono::steady_clock::now()

// Simple, clean log macro
#define LOG(x) std::cout << x << std::endl

std::mutex m;
std::queue<int> tasks;
std::condition_variable cv;
bool stop = false;

void work()
{
    // 1. Acquire the lock
  while(true)//this basically says to the thread execute the code untill a false is returned,forcing threads to complete all task
  {
    std::unique_lock<std::mutex> ulock(m);

    // 2. Safely wait until 'ready' is true.
    // This safely releases the lock while waiting and re-acquires it upon waking.
    cv.wait(ulock, []{ return !tasks.empty() ||stop;}); //!task.empty() says this queue is empty(true) hence no work(false)

    if(stop && tasks.empty())//this would not be required ,but as we used shutdown(stop)..check docs
    {
        return;
    }

    // 3. This section is safely synchronized
    int task=tasks.front();
    tasks.pop();

    ulock.unlock();//unlock so next thread can acces the mutex,while the thread is processing

    LOG("Proccessesing"<<task<<"\n");
  }
}


int main()
{
    auto start = TIME;

    // Spin up the worker threads
    std::thread t2(work);
    std::thread t3(work);

    // Simulate some main thread work

    // 4. Safely update the shared state under the lock scope
    {
        std::lock_guard<std::mutex> lock(m);
        tasks.push(1);
        tasks.push(2);
        tasks.push(3);
        tasks.push(4);

        stop=true;//by making it true here we make sure that shutdown is initialized only when all tasks are pushed,prevents early killing of threads
    } //basically prevents others threads from reading the queue while main is updating it.

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
