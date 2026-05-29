#include "learning_gem5/part2/hello_object.hh"
#include "debug/HelloExample.hh" //包含调试标志的头文件

#include <iostream>
namespace gem5
{
HelloObject::HelloObject(const HelloObjectParams &params) : SimObject(params)
{
    // std::cout << "Hello World! From a SimObject!" << std::endl;
    DPRINTF(HelloExample, "Create the hello object\n");
    // 输出调试信息, 包括DPRINTF执行时的当前时钟周期,
    // 调用DPRINTF的SimObject名称 以及传递的字符串
}

HelloObject::HelloObject(const HelloObjectParams &params)
    : SimObject(params), event([this] { processEvent(); }, name())
{
    DPRINTF(HelloExample, "Created the hello object\n");
}
// 该函数接收两个参数: 1. 一个无参数无返回值的函数 2. 事件的名称,
// 使用name函数获取 this放入lambda表达式中,
// 以便在事件触发时调用processEvent函数

void
HelloObject::processEvent()
{
    timesLeft--;
    DPRINTF(HelloExample, "Hello World! Processing the event!\n");

    if (timesLeft > 0) {
        DPRINTF(HelloExample, "Done firing!\n");
    } else {
        schedule(event, latency + curTick());
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

HelloObject::HelloObject(const HelloObjectParams &params)
    : SimObject(params),
      event([this] { processEvent(); }, name()),
      latency(100),
      timesLeft(10)
{
    DPRINTF(HelloExample, "Created the hello object\n");
} // 在构造函数中添加两个变量的默认值
} // namespace gem5
