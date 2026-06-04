#include "learning_gem5/part2/hello_object.hh"

#include "debug/HelloExample.hh"

namespace gem5
{

HelloObject::HelloObject(const HelloObjectParams &params)
    : SimObject(params),
      // event(*this), //不同版本
      event([this] { processEvent(); }, name()),
      myName(params.name), // 将参数对象中的name存储到成员变量myName中
      latency(params.time_to_wait),
      timesLeft(params.number_of_fires)
{
    DPRINTF(HelloExample, "Created the hello object with the name %s\n",
            myName.c_str());
} // 将声明的参数传递给C++类的构造函数

/*HelloObject::HelloObject(const HelloObjectParams &params)
    : SimObject(params),
      event([this] { processEvent(); }, name()),
      latency(100),
      timesLeft(10)
{
    DPRINTF(HelloExample, "Created the hello object\n");
} // 在构造函数中添加两个变量的默认值*/

/*HelloObject::HelloObject(const HelloObjectParams &params) :
    SimObject(params)
{
    // std::cout << "Hello World! From a SimObject!" << std::endl;
    DPRINTF(HelloExample, "Create the hello object\n");
    // 输出调试信息, 包括DPRINTF执行时的当前时钟周期,
    // 调用DPRINTF的SimObject名称 以及传递的字符串
    // 该函数接收两个参数: 1. 一个无参数无返回值的函数 2. 事件的名称,
    // 使用name函数获取 this放入lambda表达式中,
    // 以便在事件触发时调用processEvent函数
}*/

void
HelloObject::processEvent()
{
    timesLeft--;
    DPRINTF(HelloExample, "Hello world! Processing the event! %d left\n",
            timesLeft);

    if (timesLeft <= 0) {
        DPRINTF(HelloExample, "Done firing!\n");
    } else {
        schedule(event, curTick() + latency);
    }
}
// 定义进程函数的实现, 只需打印一些内容用作调试
// 更新函数

void
HelloObject::startup()
{
    // schedule(event, 100);
    schedule(event, latency); // 更新函数
}
// 安排事件的执行时间, startup()函数在模拟器启动时被调用,
// 这里安排事件在100周期(可以使用显式时间)后执行
} // namespace gem5
