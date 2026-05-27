"""v21.0 简单配置脚本(旧版本) 新版(v25)不再支持旧版本的api 该脚本可以修改以运行ARM二进制文件"""

import m5
from m5.objects import *

system = System()  # 创建模拟对象

system.clk_domain = SrcClockDomain()  # 创建一个时钟域
system.clk_domain.clock = "1GHz"  # 设置时钟频率
system.clk_domain.voltage_domain = VoltageDomain()  # 指定电压域(默认)

system.mem_mode = "timing"  # 设置模拟方式为定时模式模拟
system.mem_ranges = [AddrRange("512MiB")]  # 设置内存范围

# system.cpu = X86TimingSimpleCPU() #创建最简单的基于时序的X86 ISA CPU
system.cpu = (
    ArmTimingSimpleCPU()
)  # 创建最简单的基于时序的ARM ISA CPU (教程中没写?)

system.membus = SystemXBar()  # 创建系统级内存总线

system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = (
    system.membus.cpu_side_ports
)  # 将CPU上的缓存端口连接到内存总线上

system.cpu.createInterruptController()  # 在CPU上创建一个I/O控制器并将其连接到内存总线
"""system.cpu.interrupts[0].pio = system.membus.mem_side_ports
system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports"""  # 除了X86之外的其他ISA不需要将PIO和中断端口连接到内存总线


system.system_port = (
    system.membus.cpu_side_ports
)  # 将系统中的一个特殊端口连接到内存总线

system.mem_ctrl = MemCtrl()  # 创建一个内存控制器并将其连接到内存总线
system.mem_ctrl.dram = DDR3_1600_8x8()  # 使用一个简单的DDR3控制器
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

binary = "tests/test-progs/hello/bin/x86/linux/hello"
system.workload = SEWorkload.init_compatible(binary)

process = Process()
# process.cmd = [binary]
process.cmd = ["cpu_tests/benchmarks/bin/arm/Bubblesort"]
system.cpu.workload = process
system.cpu.createThreads()

root = Root(full_system=False, system=system)
m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()

print(f"Exiting at tick {m5.curTick()} because {exit_event.getCause()}")
