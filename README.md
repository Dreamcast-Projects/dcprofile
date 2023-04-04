# dcprofiler

This project harnesses the capabilities of GCC's instrument functions to profile Dreamcast applications effectively. The instrument functions are automatically inserted at the beginning and end of every function, sending profiling data to your PC for analysis. The desktop application, dctrace, processes this data to generate a .dot file, which provides a visual representation of the call graph for all functions in your application. This detailed overview includes the number of times each function was called and the percentage of time spent executing each function, offering valuable insights into your application's performance.

There is two parts to this project:  
1. The files you include in your project (profiler.c, profiler.h)
2. The application(dctrace) that parses the data (trace.bin) your Freamcast application generates.  This application is based on 
the project [HERE](https://web.archive.org/web/20130528172555/http://www.ibm.com/developerworks/library/l-graphvis/) but with added counters to show the time spent in each function.

## Instructions:
1. Add 'profiler.c' and 'profiler.h' to you projects source directory.
2. Edit your makefile to add '-g -finstrument-functions' to your KOS_CFLAGS and 'profiler.o' to your OBJS.
3. Use dc-tool-ip to send your elf file to Dreamcast:
   ```sudo /PATH/TO/dc-tool-ip -c "/PATH/TO/PROJECT" -t 192.168.1.137 -x main.elf```
4. Profiling automatically starts when the application runs, so ensure you call shutdownProfiling() to stop profiling.
5. Run dctrace application on your elf with the command:
  ```dctrace main.elf```

This assumes a couple of things.  That your 'sh-elf-addr2line' application is in '/opt/toolchains/dc/sh-elf/bin/' directory and that your 
.elf file and trace.bin(generated by your application) file are the same directory. 

6.  That will generate a graph.dot file that you can use the following command to create a JPG:
  ```dot -Tjpg graph.dot -o graph.jpg```

You will need the dot application to generate an image. Find installation information [HERE](https://graphviz.org/download/). To install dot on macOS using Homebrew, run:
  ```brew install graphviz```

Alternatively, to view your DOT file and convert it to an image, paste the contents of the .DOT file into this online editor: https://dreampuf.github.io/GraphvizOnline/

## TIPS:

1.  To exclude a method from profiling, add __attribute__ ((no_instrument_function)) to its prototype or definition (if there isn't a prototype). Or in your Makefile, use -finstrument-functions-exclude-function-list=sym,sym,....

2.  To ignore whole files you don't want to profile, add -finstrument-functions-exclude-file-list=file,file,... to your Makefile.

Here is an example of my Makefile:

```
TARGET = main.elf
DATETIME := $(shell date '+%Y-%m-%d_%I-%M-%S_%p')
...

profileip: $(TARGET)
	sudo /PATH/TO/dc-tool-ip -c "." -t 192.168.1.137 -x $(TARGET)

dot: 
	/PATH/TO/dctrace $(TARGET)

image: dot
	dot -Tjpg graph.dot -o graph_$(DATETIME).jpg
```
After running the application using make profileip and exiting, use the command make image to generate graph.dot and graph.jpg files.

## WARNING:

This profiler generates a large amount of tracing data. It works with dcload-ser, but using it with dcload-ip is preferred.
