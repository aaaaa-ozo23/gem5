#ifndef __LEARNING_GEM5_PART2_SIMPLE_MEMOBJ_HH__
#define __LEARNING_GEM5_PART2_SIMPLE_MEMOBJ_HH__

#include "mem/port.hh"
#include "params/SimpleMemobj.hh"
#include "sim/sim_object.hh"

namespace gem5
{

class SimpleMemobj : public SimObject
{
  private:
  public:
    SimpleMemobj(const SimpleMemobjParams &params);
};

class CPUSidePort : public ResponsePort // 定义响应端口类型
{
  private:
    SimpleMemobj *owner;
    // 该对象有一个成员变量, 即它的所有者, 因此它可以调用该对象上的函数

  public:
    CPUSidePort(const std::string &name, SimpleMemobj *owner)
        : ResponsePort(name, owner), owner(owner)
    {}

    AddrRangeList getAddrRanges() const override;

  protected:
    Tick
    recvAtomic(PacketPtr pkt) override
    {
        panic("recvAtomic unimpl.");
    }

    void recvFunctional(PacketPtr pkt) override;
    bool recvTimingReq(PacketPtr pkt) override;
    void recvRespRetry() override;
};

class MemSidePort : public RequestPort // 定义请求端口类型
{
  private:
    SimpleMemobj *owner;

  public:
    MemSidePort(const std::string &name, SimpleMemobj *owner)
        : RequestPort(name, owner), owner(owner)
    {}

  protected:
    bool recvTimingResp(PacketPtr pkt) override;
    void recvReqRetry() override;
    void recvRangeChange() override;
};

class SimpleMemobj : public SimObject // 定义 SimObject 接口
{
  private:
    <CPUSidePort declaration><MemSidePort declaration>

        CPUSidePort instPort;
    CPUSidePort dataPort;

    MemSidePort memPort;

  public:
    SimpleMemobj(SimpleMemobjParams *params);

    Port &getPort(const std::string &if_name,
                  PortID idx = InvalidPortID) override;
};

} // namespace gem5

#endif __LEARNING_GEM5_PART2_SIMPLE_MEMOBJ_HH__
