"""创建一个新的简单模拟对象, 先新建一个Python类"""

from m5.params import *
from m5.SimObject import SimObject


class HelloObject(SimObject):
    type = "HelloObject"  # 要封装的C++类
    cxx_header = "learning_gem5/part2/hello_object.hh"  # 包含C++类的头文件声明
    cxx_class = "gem5::HelloObject"  # 一个属性, 用于指定新创建的SimObject类声明在gem5命名空间中

    # 为HelloObject添加更多参数: 延迟和触发事件次数的参数
    # gem5 Python接口可以将参数从Python传递给C++对象
    time_to_wait = Param.Latency("Time before firing the event")
    number_of_fires = Param.Int(
        1, "Number of times to fire the event before" " goodbye"
    )
    # Param类型声明一个参数, 常见的有Int Float String Bool等, 还可以使用自定义的Param类型, 例如Latency
    # 每个参数都需要一个默认值(不指定时为1)和一个描述字符串, 这些信息在gem5的文档中会被使用
    # Latency不是内置类型, 将参数转化为模拟器的滴答数, 默认是1皮秒, 也有其他参数: Percent, Cycles, MemorySize等
