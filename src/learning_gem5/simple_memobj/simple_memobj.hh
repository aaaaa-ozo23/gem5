#ifndef __LEARNING_GEM5_PART2_SIMPLE_MEMOBJ_HH__
#define __LEARNING_GEM5_PART2_SIMPLE_MEMOBJ_HH__

#include "mem/port.hh"
#include "params/SimpleMemobj.hh"
#include "sim/sim_object.hh"

namespace gem5
{

class SimpleMemobj : public SimObject // 定义 SimObject 接口
{
  private:
    class CPUSidePort : public ResponsePort // 定义响应端口类型
    {
      private:
        SimpleMemobj *owner;
        bool needRetry;
        PacketPtr blockedPacket;
        // 该对象有一个成员变量, 即它的所有者, 因此它可以调用该对象上的函数

      public:
        CPUSidePort(const std::string &name, SimpleMemobj *owner)
            : ResponsePort(name),
              owner(owner),
              needRetry(false),
              blockedPacket(nullptr)
        {}

        AddrRangeList getAddrRanges() const override;
        void sendPacket(PacketPtr pkt);
        void trySendRetry();

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
        PacketPtr blockedPacket;

      public:
        MemSidePort(const std::string &name, SimpleMemobj *owner)
            : RequestPort(name), owner(owner), blockedPacket(nullptr)
        {}

        void sendPacket(PacketPtr pkt);

      protected:
        bool recvTimingResp(PacketPtr pkt) override;
        void recvReqRetry() override;
        void recvRangeChange() override;
    };

    bool blocked;
    CPUSidePort instPort;
    CPUSidePort dataPort;

    MemSidePort memPort;

  public:
    SimpleMemobj(const SimpleMemobjParams &params);
    // 新版本SimObject的构造函数需要参数对象的引用, 不是指针

    Port &getPort(const std::string &if_name,
                  PortID idx = InvalidPortID) override;

    void handleFunctional(PacketPtr pkt);
    bool handleRequest(PacketPtr pkt);
    bool handleResponse(PacketPtr pkt);
    AddrRangeList getAddrRanges() const;
    void sendRangeChange();
};

} // namespace gem5

#endif // __LEARNING_GEM5_PART2_SIMPLE_MEMOBJ_HH__
