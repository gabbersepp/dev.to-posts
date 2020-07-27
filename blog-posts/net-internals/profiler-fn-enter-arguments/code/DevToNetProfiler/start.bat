SET COR_ENABLE_PROFILING=1
SET COR_PROFILER={b45048d5-6f44-4fbe-ae88-b468a5e4927a}
#SET COR_PROFILER_PATH=D:/git/dev.to-posts/blog-posts/net-internals/profiler-fn-enter-arguments/code/DevToNetProfiler/DevToNetProfiler/bin/Debug/DevToNetProfiler.dll
SET COR_PROFILER_PATH=DevToNetProfiler/bin/Debug/DevToNetProfiler.dll
SET COMPLUS_ProfAPI_ProfilerCompatibilitySetting=EnableV2Profiler

#START "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\"
START TestApp/bin/Debug/TestApp.exe
#START C:\Tools\WinDbg\windbg.exe