//static arrays are the arrays the  do not grow
// unlike dynamic arrys these cannot be reduced or expanded


#define  TIME  std::chrono::steady_clock::now()
#define P_DEBUG 1
#if P_DEBUG==1
#define LOG(x) std::cout<<x<<std::endl
#else
#define LOG(x)
#endif



#include "iostream"
#include "array"
#include "span"
#include<thread>
#include"chrono"

class Print
{
    public:
        void  PrintArray(const std::array<int ,5>& arr);
        void PrintArray(const std::span<const int>arr);
};

void Print::PrintArray(const std::array<int ,5>& arr) //std array works same as normal array but it has additional features like debugging,size tracking,and is as fast as normal array.
{
    for(int a:arr)
    {
        LOG(a);
    }

}
void Print::PrintArray(const std::span<const int>arr) //variabel size ,we can also use template<Size S>
{

    for(int a:arr)
    {
        LOG(a);
    }

}

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

    /*
        std::array<int , 5> arr={1,2,3,4,5};
    std::array<int , 4> arr2={1,2,3,4};
    Print printer;
    printer.PrintArray(arr);
    printer.PrintArray(arr2);//variable length so 2nd function
     */

    auto start=TIME;
    std::thread t1(work);
    std::thread t2(work);
    std::thread t3(work);

    t1.join();
    t2.join();
    t3.join();
    auto end=TIME;

    auto TotalTime=end-start;
    LOG(TotalTime);


}
