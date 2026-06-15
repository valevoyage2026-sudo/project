#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include "vector"
#include "functional"
#include "queue"
#include "thread"
class ThreadPool
{
    private:
        std::mutex m;
        std::condition_variable cv;
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        bool stop=false;

    public:
        ThreadPool(size_t Thread);

        ~ThreadPool();

        void Enqueue(std::function<void()> task);

};
