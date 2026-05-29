#ifndef __LEARNING_GEM5_HELLO_OBJECT_HH__
#define __LEARNING_GEM5_HELLO_OBJECT_HH__

#include "params/HelloObject.hh"
#include "sim/sim_object.hh"

namespace gem5
{

class HelloObject : public SimObject
{
  private:
    void processEvent(); // 创建简单的事件回调, 声明一个函数在事件触发时被调用

    EventFunctionWrapper event; // 允许我们执行任何函数

    const Tick latency;

    int timesLeft; // 添加一个成员变量表示延迟和触发次数

  public:
    HelloObject(const HelloObjectParams &p);

    void startup() override;
};

} // namespace gem5
// 问题: 构造函数接收的参数对象是如何被自动构建的?

#endif // __LEARNING_GEM5_HELLO_OBJECT_HH__
