Angelica Vargas - Lab03

buffer 
size   elapsed	 user	system
128	7.770s	0.151s	2.058s
256	2.700s	0.082s	1.171s
512	0.670s	0.036s	0.566s
1024	0.351s	0.013s	0.312s
2048	0.266s	0.016s	0.218s
4096	0.242s	0.016s	0.204s
8192	0.216s	0.004s	0.179s
16384	0.184s	0.004s	0.137s

The buffer size contains the number of bytes increased 
in powers of 2. The elapsed time column demonstrates how 
long the program took to run in sec. The user column represents 
the amount of time the CPU spent in user mode during the 
execution of this program. The system shows how much CPU time
is spend in system mode (in kernel/operating system) during this 
program.

By analysing these values, we can see how the performance quickens 
up as the buffer size increases. This may be due to the reduced
number of system calls and overhead. This then allows a more efficient
I/O and quickens overall performance.

NOTE:
use:
./raw_copy 256 initrd.img-5.15.0.91-generic tmp_file.txt

was not able to use:
./raw_copy 256 $(find /boot -name 'initrd.img*') tmp_file.txt
