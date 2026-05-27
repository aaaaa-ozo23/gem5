"""简单配置脚本"""

from gem5.prebuilt.demo.x86_demo_board import X86DemoBoard
from gem5.resources.resource import obtain_resource
from gem5.simulate.simulator import Simulator

board = X86DemoBoard()  # 在此脚本中添加一个看版, demo只是一个预制版
board.set_workload(
    obtain_resource("x86-ubuntu-24.04-boot-no-systemd")
)  # 下载工作负载和资源

# sim = Simulator(board)
# sim.run(2_000_000_000) #运行200亿个时钟周期, 但达不到教程中的预期输出结果

sim = Simulator(board=board)
sim.run()  # 更改模拟运行时间, 可以成功
