## CPU Profiling
- Record stacks at a timed interval: simple and effective
- Pros: Low (deterministic overhead)
- Cons: Coarse accuracy, but usually sufficent

## Linux perf_events
- Can sample stacks of (almost) everything on CPU
- Can miss hard hard interrupt ISRs, but these should be near zero. They
can be measured using BR's tools. 

## Flame graph interpretation
- X axis is not a timestamp! 
- X axis have stack names are sorted alphabetically, ie function b() will be shown first after
function h(), but it does not mean function b got called first
- Top edge shows who is running on-CPU and how much (width)

## Mixed mode flame graphs
- green : JIT (Java)
- aqua : inlined
- red : user-level (kernel modules too)
- orange : kernel
- yellow : C++

## Differential flame graphs
Hues:
- red : more samples
- blue : less amples

Intensity - degree of difference

flamegraphdiff

But sometimes code changes can lead to "towers" being gone and therefore nothing
to paint blue

## Icicle Graph
- Top down instead of bottom up

## Flame chart
- From Chrome dev tools 
- X axis is time
- Works well for Chrome dev tools because they are profiling V8 which is single threaded

## Caveats
Broken stack traces are common because: 
- Profilers use frame pointer walking by default
- Compilers reuse the frame pointer register (rbp in  x86_64) as a general purpose register -
for a (usually very small) performance optimization

## Fixing stack walking
- Frame pointer based: gcc -fno-omit-frame-pointer
- Debug info (DWARF) walking: costs disk space and not supported by all profilers
- JIT runtime walkers: include more internals such as inline frames, but no limited
to application internals - no kernel
- last branch record (LBR) - usually gets blocked by cloud hypervisors

## Inlining
- Many frames may be missing (inlined) though flame graph may still make sense
- Inlining can often be tuned
- Runtimes can un-inline on demand

## Fixing native symbols
- Add a -dbgsym package, if available
- usually need to recompile
- JIT runtimes (Java, Node.js) don't have a pre-compiled symbol table
- Linux perf looks for an externally provided JIT symbol

## Symbol Churn
- For JIT runtimes, symbols can change during a profile
- Symbols may be mistranslated by perf's map snapshot
- Solution: 
    - take before and after snapshot and compare
    - perf's support for timestamped symbol logs

## Containers
- Inside containers, perf can't find any symbol sources unless you copy them into the host

## GUI Automation
- There are tools to show flamegraph in a GUI, for example Netlix's vector

## Page faults
- Show that triggered main memory (resident) to grow:
```
perf record -e page-faults -p PID -g -- sleep 120
```
- "fault" as physical main memory is allocated on demand, when a virtual page
is first populated
- Low overhead to solve some types of memory leaks
```
perf record -e page-faults ./your_program
perf report
```

## Context witches
```
perf record -e context-switches -p PID -g --sleep 5
```
Idenfies locks, IO, sleep

## Disk IO Requests
```
perf record -e block:block_rq_insert -ag -- sleep 60
```
Shows who issued disk I/O (sync reads and writes)

## TCP Events
- see slides https://www.slideshare.net/brendangregg/usenix-atc-2017-visualizing-performance-with-flame-graphs

## CPU Cache Misses
```
perf record -e LLC-loads -c 10000 -a -g -- sleep 5; jmaps
perf script -f comm,pid,tid,cpu,time,event,ip,sys,dso > out.stacks
```
LLC is Last Level Cache loads
-c is the count (samples once per count)

## CPU Flame Graph
- red : instruction heavy
- blue : cycle heavy

## execsnoop
```
execsnoop -t
```
Can discover short lived process using execsnoop

## ext4slower
```
/usr/share/bcc/tools/ext4slower 1
```
Trace ext4 operations slower than 1 ms

## biolatency
```
biolatency -mT 10
```
Identify multimodal disk I/O latency and outliers with biolatency

## tcplife
```
/usr/share/bcc/tools/tcplife
```
Efficiently trace TCP sessions with PID, bytes and duration using tcplife

## gethostlatency
```
/usr/share/bcc/tools/gethostlatency
```
Identify DNS latency issues system wide

## runqlat
```
/usr/share/bcc/tools/runqlat 10
```
Examine CPU scheduler run queue latency as a histogram.
Run queue latency is when a process becomes runnable but it has to wait its turn

## In kernel summaries with argdist
```
argdist -H 'p::tcp_cleanup_rbuf(struct sock *sk, int copied):int:copied'
```
Histogram of tcp_cleanup_rbuf and show the copied argument

## bpftrace one liner tutorial

https://github.com/bpftrace/bpftrace/blob/master/docs/tutorial_one_liners.md
