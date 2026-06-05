#ifndef __LEARNING_GEM5_HELLO_OBJECT_HH__
#define __LEARNING_GEM5_HELLO_OBJECT_HH__

#include <string>

#include "learning_gem5/part2/goodbye_object.hh"
#include "params/HelloObject.hh"
#include "sim/sim_object.hh"

namespace gem5
{

class GoodbyeObject;

class HelloObject : public SimObject
{
  private:
    void processEvent(); // 创建简单的事件回调, 声明一个函数在事件触发时被调用

    MemberEventWrapper<&HelloObject::processEvent> event; // 新版本

    // EventFunctionWrapper event; // 旧版本

    GoodbyeObject *goodbye; // 添加一个成员变量来存储指向GoodbyeObject的指针

    const std::string myName; // 添加一个成员变量来存储对象的名称

    const Tick latency;

    int timesLeft; // 添加一个成员变量表示延迟和触发次数

  public:
    HelloObject(const HelloObjectParams &p);

    void startup() override;
};

} // namespace gem5
// 问题: 构造函数接收的参数对象是如何被自动构建的?

#endif // __LEARNING_GEM5_HELLO_OBJECT_HH__
