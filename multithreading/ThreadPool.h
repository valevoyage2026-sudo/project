#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include "vector"
#include "functional"
#include "queue"
#include "thread"
#include "iostream"

class ThreadPool
{
    private:
        std::mutex m;
        std::condition_variable cv;
        std::vector<std::thread> workers;
        size_t MaxTasks;
        std::condition_variable QueueNotFull;


        std::queue<std::function<void()>> tasks;

        bool stop=false;

    public:
        ThreadPool(size_t ThreadCount,size_t MaxTasks);

        ThreadPool(const ThreadPool& ThreadPool)=delete;
        ThreadPool& operator =(const ThreadPool& ThreadPool)=delete;//avoid copy of threda and mutex and locks
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;//disable moving

        ~ThreadPool();
       template<typename T>
       auto Enqueue(T&& func)-> std::future<std::invoke_result_t<T>>;//&& takes referenec to temporary variables,function like lambda

};

template<typename T>
auto ThreadPool::Enqueue(T&& func)-> std::future<std::invoke_result_t<T>>
{

 std::unique_lock<std::mutex> Qulock(m);

 QueueNotFull.wait(Qulock,[this]{//check ilf producer can take more tasks(QueueNotful)

     return tasks.size()<MaxTasks;
 });

   using ReturnType=std::invoke_result_t<T>;

   auto task=std::make_shared<

       std::packaged_task<ReturnType()>
       >(

       std::forward<T>(func)//forward preserves the function type(temp or permanent)while pasing,as we want it to be a temporary function.using directly
       //declaring func will make the function value permanent(lvalue)
       // func

       );//creting a package task to store it in heap and automate future and promise

  auto future=task->get_future();


  if(stop)
  {
      throw std::runtime_error("Thread Pool Shuting down");
  }

     tasks.emplace(
         [task]{

             (*task)();//as the packed task is stored as address we need to deref it
             //which gets executed in constructor.
         });




    cv.notify_one();//wake up worker in constructor

    return future;
}
struct ProcessResult
{
  std:: string Filename;
  std:: string Msg;
  float average;
  int count;


};
