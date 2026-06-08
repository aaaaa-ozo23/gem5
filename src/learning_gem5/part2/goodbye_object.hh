#ifndef __LEARNING_GEM5_GOODBYE_OBJECT_HH__
#define __LEARNING_GEM5_GOODBYE_OBJECT_HH__

#include <string>

#include "params/GoodbyeObject.hh"
#include "sim/sim_object.hh"

namespace gem5
{

class GoodbyeObject : public SimObject
{
  private:
    void processEvent();

    void
    fillBuffer(); // 为一次迭代填充缓冲区。如果缓冲区未满，此函数将入队另一个事件以继续填充。

    MemberEventWrapper<&GoodbyeObject::processEvent> event; // 新版本

    float bandwidth; // 每一个tick处理的字节数

    int bufferSize; // 缓冲区的大小

    char *buffer; // 用于存储数据的缓冲区

    std::string message; // 要写入缓冲区的消息

    int bufferUsed; // 当前缓冲区中已使用的字节数

  public:
    GoodbyeObject(const GoodbyeObjectParams &p);
    ~GoodbyeObject(); // 析构函数，负责释放分配的缓冲区内存

    void sayGoodbye(std::string message);
};

} // namespace gem5

#endif // __LEARNING_GEM5_GOODBYE_OBJECT_HH__
