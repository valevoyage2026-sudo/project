#include "ThreadPool.h"
#include <chrono>
#include <cstddef>
#include <fstream>
#include <future>
#include <mutex>
#include <nlohmann/detail/meta/type_traits.hpp>
#include <nlohmann/detail/value_t.hpp>
#include <nlohmann/json_fwd.hpp>
#include <numeric>

#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "math.h"

#include "numeric"

ThreadPool::ThreadPool(size_t ThreadCount,size_t MaxTasks)
{
   this->MaxTasks=MaxTasks;
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
         QueueNotFull.notify_one();//single producer
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


nlohmann::json Parser(const std::string& file)
{

  std::ifstream in(file);
  nlohmann::json data;

  in>>data;
  return data;
}

void Processing(std::vector<std::future<nlohmann::json>>& vec)
{

  for(auto &future:vec)
  {
      nlohmann::json data=future.get();

    for(auto &[key,value]:data.items())
  {


      if(value.is_array()){std::cout<<key<<":array"<<"\n";
         auto vec=value.get<std::vector<int>>();
          std::cout<<key<<":Sum:"<<std::accumulate(vec.begin(),vec.end(),0)<<"\n";}
      if(value.is_number())
      {std::cout<<key<<":number"<<"\n";

      }
      if(value.is_string()){std::cout<<key<<":string"<<"\n";}
      if(value.is_boolean()){std::cout<<key<<":boolean"<<"\n";}
      if(value.is_object()){std::cout<<key<<":object"<<"\n";}


  }

}
}

int main()
{
auto start = std::chrono::steady_clock::now();
    ThreadPool pool(4,100);

std::vector<std::future<nlohmann::json>>Futures;
Futures.reserve(4);

Futures.emplace_back(pool.Enqueue([]{
    return Parser("a.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("b.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("c.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("d.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("e.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("f.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("g.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("h.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("i.json");

}));
Futures.emplace_back(pool.Enqueue([]{
    return Parser("j.json");

}));





Processing(Futures);

auto end = std::chrono::steady_clock::now();

auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
std::cout<<duration.count();

}
