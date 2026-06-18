import m5
from m5.objects import *

system = System()
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = "1GHz"
system.clk_domain.voltage_domain = VoltageDomain()
system.mem_mode = "timing"
system.mem_ranges = [AddrRange("8192MiB")]

system.cpu = X86TimingSimpleCPU()

system.memobj = SimpleMemobj()

system.cpu.icache_port = system.memobj.inst_port
system.cpu.dcache_port = system.memobj.data_port

system.membus = SystemXBar()

system.memobj.mem_port = system.membus.cpu_side_ports

system.cpu.createInterruptController()

system.cpu.interrupts[0].pio = system.membus.mem_side_ports
system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports
# DDR3_1600_8x8 不是内存控制器, 它是 DRAMInterface, 也就是 DRAM 介质/时序模型.
# 它没有 port 参数. 有port参数的是MemCtrl(内存控制器).

system.system_port = system.membus.cpu_side_ports

binary = "tests/test-progs/hello/bin/x86/linux/hello"

system.workload = SEWorkload.init_compatible(binary)
# 给 System 设置 SE 模式的 workload 对象
process = Process()
process.cmd = [binary]
system.cpu.workload = process  # 设定CPU要运行的进程
system.cpu.createThreads()

root = Root(full_system=False, system=system)
m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()
print("Exiting @ tick %i because %s" % (m5.curTick(), exit_event.getCause()))
