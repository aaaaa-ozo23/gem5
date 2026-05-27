"""创建一个新的简单模拟对象, 先新建一个Python类"""

from m5.params import *
from m5.SimObject import SimObject


class HelloObject(SimObject):
    type = "HelloObject"  # 要封装的C++类
    cxx_header = "learning_gem5/hello_object.hh"  # 包含C++类的头文件声明
    cxx_class = "gem5::HelloObject"  # 一个属性, 用于指定新创建的SimObject类声明在gem5命名空间中
