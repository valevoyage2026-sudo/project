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




int main()
{
    ThreadPool pool(4);

    auto future1 =
        pool.Enqueue([]{
            return 42;
        });

    auto future2 =
        pool.Enqueue([]{
            return std::string("Hello");
        });

    std::cout << future1.get() << '\n';
    std::cout << future2.get() << '\n';




}
