from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject


class SimpleMemobj(SimObject):
    type = "SimpleMemobj"
    cxx_header = "learning_gem5/simple_memobj/simple_memobj.hh"
    cxx_class = "gem5::SimpleMemobj"

    inst_port = ResponsePort("CPU side port, receives requests from CPU")
    data_port = ResponsePort("CPU side port, receives requests from CPU")
    mem_port = RequestPort("Memory side port, sends requests to memory")
    # 后面需要在C++中实现这个端口的功能, 定义函数getPort, 该对象的参数是三个端口
    # 两个用于连接CPU指令和数据, 一个用于连接内存总线
