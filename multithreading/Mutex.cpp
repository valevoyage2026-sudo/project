#include <atomic>
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
void work(int& counter)
{

for(int a=0;a<1000;a++)
{
    //std::lock_guard<std::mutex>lock(m);//not looking cause race around cndition
    counter++;//but this is expensive as it locks/unlocks 1000 so we use atomic
    //atomic is basically a single cpu instruction which can be only accesd by one enity.but it cannot perfrom complex task
    // for that we still have to use mutex


}
}


 int main()
{



    auto start=TIME;
    int counter=0;
    std::thread t1(work,std::ref(counter));
    std::thread t2(work,std::ref(counter));//pass by ref avoid copy
    std::thread t3(work,std::ref(counter));

    t1.join();
    t2.join();
    t3.join();
    auto end=TIME;

    auto TotalTime=end-start;
    LOG("Total time:"<<TotalTime);
    LOG("Count:"<<counter);


}
