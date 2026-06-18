#include "learning_gem5/simple_memobj/simple_memobj.hh"

#include "debug/SimpleMemobj.hh"

namespace gem5
{
SimpleMemobj::SimpleMemobj(const SimpleMemobjParams &params)
    : SimObject(params),
      instPort(params.name + ".inst_port", this),
      dataPort(params.name + ".data_port", this),
      memPort(params.name + ".mem_side", this),
      blocked(false)
{}

Port &
SimpleMemobj::getPort(const std::string &if_name, PortID idx)
{
    panic_if(idx != InvalidPortID, "This object doesn't support vector ports");

    // 检查是否是Python类中声明的变量, 若不是将请求名传递给父级
    if (if_name == "mem_port") {
        return memPort;
    } else if (if_name == "inst_port") {
        return instPort;
    } else if (if_name == "data_port") {
        return dataPort;
    } else {
        return SimObject::getPort(if_name, idx);
    }
}

// 实现请求和端口响应功能
AddrRangeList
SimpleMemobj::CPUSidePort::getAddrRanges() const
{
    return owner->getAddrRanges();
}

void
SimpleMemobj::CPUSidePort::recvFunctional(PacketPtr pkt)
{
    return owner->handleFunctional(pkt);
}

// 将请求传递给内存端; 使用DPRINTF进行调试
void
SimpleMemobj::handleFunctional(PacketPtr pkt)
{
    memPort.sendFunctional(pkt);
}

AddrRangeList
SimpleMemobj::getAddrRanges() const
{
    DPRINTF(SimpleMemobj, "Sending new ranges\n");
    return memPort.getAddrRanges();
}

// 对于MemSidePort实现recvRangeChange,
// 将请求通过 SimpleMemobj 转发到响应端口
void
SimpleMemobj::MemSidePort::recvRangeChange()
{
    owner->sendRangeChange();
}

void
SimpleMemobj::sendRangeChange()
{
    instPort.sendRangeChange();
    dataPort.sendRangeChange();
}

// 实现接收请求
// 向 CPUSidePort 添加一个额外的成员变量 needRetry,
// 用于存储 SimpleMemobj 每次发生故障时是否需要发送重试,
// 如果 SimpleMemobj 因请求而被阻塞, 设置在未来的某个时间发送重试请求
bool
SimpleMemobj::CPUSidePort::recvTimingReq(PacketPtr pkt)
{
    if (!owner->handleRequest(pkt)) {
        needRetry = true;
        return false;
    } else {
        return true;
    }
}

// 检查端口是否阻塞
bool
SimpleMemobj::handleRequest(PacketPtr pkt)
{
    if (blocked) {
        return false;
    }
    DPRINTF(SimpleMemobj, "Got request for addr %#x\n", pkt->getAddr());
    blocked = true;
    memPort.sendPacket(pkt);
    return true;
}

// 发送数据包, 若失败先暂存在blockedPacket中以便后续收到recvReqRetry时重发
void
SimpleMemobj::MemSidePort::sendPacket(PacketPtr pkt)
{
    panic_if(blockedPacket != nullptr, "Should never try to send if blocked!");
    if (!sendTimingReq(pkt)) {
        blockedPacket = pkt;
    }
}

void
SimpleMemobj::MemSidePort::recvReqRetry()
{
    assert(blockedPacket != nullptr);

    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    sendPacket(pkt);
}

// 实施接收响应
// 当MemSidePort收到响应后, 将响应转发出去.
// 将SimpleMemobj发送到相应的CPUSidePort.
bool
SimpleMemobj::MemSidePort::recvTimingResp(PacketPtr pkt)
{
    return owner->handleResponse(pkt);
}

// 收到响应时, 对象应始终处于阻塞状态等待响应, 发回数据包之前,
// 需要解除阻塞状态, 必须在调用sendTimingResp之前完成
// 解除后通过过指定端口发送数据包然后通知CPU端口, 重试失败的请求
bool
SimpleMemobj::handleResponse(PacketPtr pkt)
{
    assert(blocked);
    DPRINTF(SimpleMemobj, "Got response for addr %#x\n", pkt->getAddr());

    blocked = false;

    if (pkt->req->isInstFetch()) {
        instPort.sendPacket(pkt);
    } else {
        dataPort.sendPacket(pkt);
    }

    instPort.trySendRetry();
    dataPort.trySendRetry();

    return true;
}

// 类似于在MemSidePort中实现发送数据包的便捷函数
void
SimpleMemobj::CPUSidePort::sendPacket(PacketPtr pkt)
{
    panic_if(blockedPacket != nullptr, "Should never try to send if blocked!");

    if (!sendTimingResp(pkt)) {
        blockedPacket = pkt;
    }
}

void
SimpleMemobj::CPUSidePort::recvRespRetry()
{
    assert(blockedPacket != nullptr);

    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    sendPacket(pkt);
}

// 实现一个额外的函数, 在解除阻塞时调用, 检查是否需要重试
void
SimpleMemobj::CPUSidePort::trySendRetry()
{
    if (needRetry && blockedPacket == nullptr) {
        needRetry = false;
        DPRINTF(SimpleMemobj, "Sending retry req for %d\n", id);
        sendRetryReq();
    }
}

/*SimpleMemobj*
SimpleMemobjParams::create()
{
    return new SimpleMemobj(this);
}*/
// 旧版本写法, 应该去掉

} // namespace gem5
