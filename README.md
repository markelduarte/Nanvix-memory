# Nanvix-memory

Modified version of the Nanvix Operating System: 

	Free is shell program that returns the available memory in the system. 
	This is an unfinished version of free, where it's currently returning the used memory.

Modified Files:

	src/ubin/free/free.c
	src/kernel/sys/free.c
	src/kernel/sys/syscalls.c
	src/kernel/mm/region.c //criada a função get_used_memory() que nos dá a memória usada
	include/nanvix/region.h // EXTERN size_t get_used_memory();
	include/nanvix/syscall.h
	src/lib/libc/unistd/free_mem.c // chama a sys_free() através do syscall.h e syscalls.c
	
	
Original repository:
	https://github.com/nanvix/nanvix
