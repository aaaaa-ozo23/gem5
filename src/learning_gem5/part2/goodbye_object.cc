#include "learning_gem5/part2/goodbye_object.hh"

#include "base/trace.hh"
#include "debug/HelloExample.hh"
#include "sim/sim_exit.hh"

namespace gem5
{
GoodbyeObject::GoodbyeObject(const GoodbyeObjectParams &params)
    : SimObject(params),
      event(*this), // 新版本
      // event([this] { processEvent(); }, name()), //旧版本
      bandwidth(params.write_bandwidth),
      bufferSize(params.buffer_size),
      buffer(nullptr),
      bufferUsed(0)
{
    buffer = new char[bufferSize];
    DPRINTF(HelloExample, "Created the goodbye object\n");
} // 将声明的参数传递给C++类的构造函数

GoodbyeObject::~GoodbyeObject()
{
    delete[] buffer;
}
void
GoodbyeObject::processEvent()
{
    DPRINTF(HelloExample, "Processing the event!\n");
    fillBuffer();
}

void
GoodbyeObject::sayGoodbye(std::string other_name)
{
    DPRINTF(HelloExample, "Saying goodbye to %s\n", other_name.c_str());

    message = "Goodbye " + other_name + "!! ";

    fillBuffer();
}

void
GoodbyeObject::fillBuffer()
{
    assert(message.length() > 0);

    int bytes_copied = 0;
    for (auto it = message.begin();
         it < message.end() && bufferUsed < bufferSize - 1;
         it++, bufferUsed++, bytes_copied++) {
        buffer[bufferUsed] = *it; // 将消息的字符复制到缓冲区中
    }
    if (bufferUsed < bufferSize - 1) {
        DPRINTF(HelloExample, "Scheduling another fillBuffer in %d ticks\n",
                bandwidth * bytes_copied);
        schedule(event, curTick() + bandwidth * bytes_copied);
        // 等待足够的时间来处理已经复制的字节数，然后继续填充缓冲区
    } else {
        DPRINTF(HelloExample, "Goodbye done copying!\n");
        exitSimLoop(buffer, 0, curTick() + bandwidth * bytes_copied);
        // 确认考虑到了处理最后一个字节所需的时间，并且在完成复制后退出模拟循环
        // 该函数接收三个参数:
        // 返回给Python配置脚本的消息、一个状态码（这里是0，表示成功）和退出时间
    }
}

} // namespace gem5
