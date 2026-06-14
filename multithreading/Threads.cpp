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

void work()
{

for(int a=0;a<9;a++)
{
    LOG("Hello from thread  "<<std::this_thread::get_id()<<"  iteration"<<a);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

}
}


 int main()
{



    auto start=TIME;
    std::thread t1(work);
    std::thread t2(work);
    std::thread t3(work);

    t1.join();
    t2.join();
    t3.join();
    auto end=TIME;

    auto TotalTime=end-start;
    LOG("Total time:"<<TotalTime);


}
