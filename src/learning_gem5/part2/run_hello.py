import m5
from m5.objects import *  # 导入m5和所有已编译的对象

root = Root(full_system=False)  # 实例化Root对象
"""现在可以实例化自己创建的HelloObject对象了, 只需要调用Python构造的函数即可"""

root.hello = HelloObject()

"""最后调用m5模块的instantiate函数来启动模拟器"""

m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()
print(f"Exiting @ tick {m5.curTick()} because {exit_event.getCause()}")
