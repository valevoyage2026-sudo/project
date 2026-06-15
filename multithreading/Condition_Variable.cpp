#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#define  TIME  std::chrono::steady_clock::now()
#define P_DEBUG 1
#if P_DEBUG==1
#define LOG(x) std::cout<<x<<std::endl
#else
#define LOG(x)
#endif

#include "iostream"
#include<thread>
#include"chrono"
#include "mutex"


std::mutex m;
std::atomic<int>counter;
std::condition_variable cv;

bool ready=false;
void work()
{
   std::unique_lock lock(m);//create unique lock which is required for conditional variable
   cv.wait(lock,[]{   //here we do not specify ready because it is globaly delcared
       LOG(std::this_thread::get_id()<<"Denied");
       return ready;

   });//cv.wait makes the thread wait untill the captured variable ready is set to true
//this prevents the threds from continiously checking for avaibality of critical section.hence reducing cpu consumption
   LOG(std::this_thread::get_id()<<"Accessed");
}


 int main()
{



    auto start=TIME;
    int counter=0;

    std::thread t2(work);//pass by ref avoid copy
    std::thread t3(work);


    int i=0;
    for(i=0;i<10;i++)
    {

        LOG(i);

    }

    if(i==10)
    {
        ready=true;
    }

    cv.notify_one();//notifies a single thread that the resource is ready to use
    t2.join();
    t3.join();//join should always come at last




    auto end=TIME;

    auto TotalTime=end-start;
    LOG("Total time:"<<TotalTime);



}
