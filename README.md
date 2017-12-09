# PerfProbe
Android app performance diagnosis tool

#### Tool description
User-perceived performance variance in mobile apps can occur in unpredictable and sophisticated ways. PerfProbe is a performance diagnosis system that supports automatic detection and systematic diagnosis of intermittent performance problems on mobile platforms. PerfProbe records app and OS-layer runtime information on a mobile device in a lightweight manner to support in-the-wild deployment and applies novel statistical analysis on them to identify critical functions as the source of performance variance and pinpoint relevant resource factors for each critical function to help understand the root cause of performance variance. 

#### Code documentation
traceview folder includes the implementation for automatically collecting function call traces using Andorid's built-in profiler

panappticon folder includes the implementaion for automatically collecting OS event traces using Panappticon

ui folder includeds the instrumentation on common UI components in Android's framework

tester folder includes the implementation of Android's MonkeyRunner-based automated controlled testing on a particular user interaction with cross-layer tracing option

analyzer folder includes the implementation of PerfProbe's offline diagnosis module that performs cross-layer trace analysis for pinpointing critical functions and relevant resource factors

monitor folder includes the implementation of PerfProbe's performance monitoring module that supports in-the-wild toapp and OS-layer traces recording 
