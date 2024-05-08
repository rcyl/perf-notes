## Analysis tools
```
iostat -x 1
netstat -i
ss (socket stat)
sar -n DEV 1 (network interfaces) (sar is system activity reporter)
vmstat 1
strace -p `pgrep lab002` (BG doesn't like it, but may have its use cases)
pidstat 1
```
## uptime
```
uptime
07:50:17 up 5 days, 11:53,  1 user,  load average: 0.05, 0.01, 0.00
```
- The 3 numbers represent the 1, 5 and 15 minute load averages
- If load > # of CPUs, may mean CPU saturation (Don't spend more than 5 seconds studying these)
- Includes more than just CPU load, which makes interpratation more difficult

## top
- %CPU is summed across CPUs
- can miss short live processes (atop won't)
- can consume noticable CPU to read /proc
- htop got more visualization 

## iostat
```
iostat -xmdz 1
Device            r/s     rMB/s   rrqm/s  %rrqm r_await rareq-sz     w/s     wMB/s   wrqm/s  %wrqm w_await wareq-sz     d/s     dMB/s   drqm/s  %drqm d_await dareq-sz     f/s f_await  aqu-sz  %util
```
rrqm/wrqm are read and write requests merged per second. The scheduler 
can combine operations, when operations are physically adjacent to each other
on the device. 

## mpstat
Multi processor statistics per-CPU
```
mpstat -P aLL 1
```
## tcpdump
```
tcpdump -i eth0 -w /tmp/out/tcpdump
```
CPU overhead can be significant

## ss
```
ss -mop
ss -i
```

## slabtop
```
slabtop
```
Kernel slab allocator memory usage

## pcstat
```
./pcstat data0*
```
Shows page cache residency by file, useful for database performance analysis

## tiptop
```
tiptop
```
Shows IPC by process, %MISS (cache misses) and %BUS (handling bus request), %BMIS 
(branch misprediction), Mcycle (million cycles), Minstr (million instructions)
Still can't use in cloud yet (as per time of talk in 2015, as it needs PMCS enabled)

## rdmsr
- Model specific registers (unlike PMCs) can be read by default in Xen guests. 
- Can show timestamp lclock, temp, power
- https://github.com/brendangregg/msr-cloud-tools

## Other advanced tools
- ltrace : Library call tracer
- ethtool : Interface tuning, some stats
- snmpget : SNMP network host statistics
- lldptool : Get LLDP broadcast stats
- blktrace : Block IO event tracer
- /proc : Raw kernel counters
- pmu-tools : On/off core CPU counter tools

## Benchmarking
- 100% of benchmarks are wrong
- You benchmark A, but actually measure B and conclude you measure C
- The energy needed to refute benchmarks is multiple orders of magnitude
bigger than to run them

## lmbench
- CPU, memory and kernel micro-benchmarks
- Eg, memory latency by stride size
```
lat_mem_red 100m 128 > out.latencies
```
## fio
```
fio --name=seqwrite --rw=write --bs=128k --size=122374m
```
FS or disk I/O micro-benchmarks

## ip route get
```
netstat -rn
ip route get 54.214.28.210
```
## perf_events: CPU profiling
Sampling full stack traces at 99 Hertz for 30 secs:
```
perf record -F 99 -ag -- sleep 30
perf report -n --stdio
```
## perf_events: FlameGraphs
```
perf record -F 99 -ag -- sleep 30
perf script | ./stackcollapse-perf.pl |  ./flamegraph.pl > perf.svg
```
Flame Graphs:
- per -F 99 : 99 hertz
- x-axis: alphabetical stack sort, to maximise merging
- y-axis: stack depth
- color: random, or hue can be a dimension
- intrepretation: Top edge is on-CPU, beneath is ancestry

## perf_events: Gotchas
- Stack traces and symbols often don't work. Can take significant effort
to fix them but it is worth it. 
- C: stacks: --fno-omit-frame-pointer

## perf_events: Counters
```
perf list | grep -i hardware
```
- Peformance monitoring counters (PMCs)
- Can be time-consuming to use (need to read CPU manuals)

## perf_events skb
```
perf record -e skb:consume_skb -ag
perf report -n --stdio
```
## perf_events Tracing tracepoints
```
perf record -e block:block_rq_complete -a sleep 10
```
## Linux peformance analysis in 60 seconds

https://www.flamingbytes.com/blog/linux-performance-analysis-in-60-seconds/