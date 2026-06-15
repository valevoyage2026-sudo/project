#include "ThreadPool.h"
#include <cstddef>
#include <mutex>

#include "iostream"



ThreadPool::ThreadPool(size_t ThreadCount)
{
   workers.reserve(ThreadCount);
   for(size_t Thread=0;Thread<ThreadCount;Thread++)
   {
       workers.emplace_back([this]
   {
      while(true)
      {
         std::unique_lock<std::mutex> ulock(m);

         cv.wait(ulock,[this]{
            return !tasks.empty()||stop;
         });

         if(stop && tasks.empty())
         {
             return ;
         }

         auto task=tasks.front();
         tasks.pop();
         ulock.unlock();
         task();

      }

   });

   }
}

ThreadPool::~ThreadPool()
{
  {
      std::lock_guard<std::mutex> guard(m);

      stop=true;


  }
   cv.notify_all();//waking them up as shutdown condition is activated

   for(auto & worker:workers)
   {
       worker.join();//wait for all of them to execute
   }
}


void ThreadPool::Enqueue(std::function<void()> task)
{
  {
     std::lock_guard<std::mutex> guard(m);

     tasks.push(std::move(task));

  }
    cv.notify_one();
}

int main()
{
    ThreadPool pool(4);

    pool.Enqueue([]{
        std::cout << "Task 1\n";
    });

    pool.Enqueue([]{
        std::cout << "Task 2\n";
    });

    pool.Enqueue([]{
        std::cout << "Task 3\n";
    });

    return 0;
}
