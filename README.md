# PerfProbe
Android app performance diagnosis tool

#### Tool description
User-perceived performance problems occur in an unpredictable and sophisticated way in mobile apps. However, we see a lack of effective approach to facilitate app developers in debugging such intermittent performance problems. To address this limitation, we develop a systematic debugging framework PerfProbe, that automatically diagnoses intermittent user-perceived performance problems in mobile apps. PerfProbe leverages app and OS-layer runtime information and applies a novel, two-step statistical analysis to associate an app’s code-level execution and its underlying OS resource usage to the cause of a performance problem. To provide holistic understanding on the root cause and ac- tionable hints for debugging, it pinpoints critical functions and their underlying resource factors relevant to a problem.

#### Code documentation
traceview folder includes the implementation for automatically collecting function call traces using Andorid's built-in profiler

panappticon folder includes the implementaion for automatically collecting OS event traces using Panappticon

tester folder includes the implementation of Android's MonkeyRunner-based automated testing on a particular user interaction with cross-layer tracing option

analyzer folder includes the implementation of cross-layer trace analysis for pinpointing critical function calls and releant resource factors
