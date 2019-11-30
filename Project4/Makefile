# makefile for scheduling program
#
# make rr - for round-robin scheduling
# make fcfs - for FCFS scheduling
# make sjf - for SJF scheduling
# make priority - for priority scheduling
# make priority_rr - for priority with round robin scheduling

CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o
	rm -rf fcfs
	rm -rf sjf
	rm -rf rr
	rm -rf priority
	rm -rf priority_rr

rr: driver.o vector.o cpu.o round_robin.o
	$(CC) $(CFLAGS) -o rr driver.o round_robin.o vector.o cpu.o

sjf: driver.o vector.o cpu.o sjf.o
	$(CC) $(CFLAGS) -o sjf driver.o sjf.o vector.o cpu.o

fcfs: driver.o vector.o cpu.o fcfs.o
	$(CC) $(CFLAGS) -o fcfs driver.o fcfs.o vector.o cpu.o

priority: driver.o vector.o cpu.o priority.o
	$(CC) $(CFLAGS) -o priority driver.o priority.o vector.o cpu.o

fcfs.o: fcfs.c
	$(CC) $(CFLAGS) -c fcfs.c

rr_priority: driver.o vector.o cpu.o rr_priority.o
	$(CC) $(CFLAGS) -o rr_priority driver.o rr_priority.o vector.o cpu.o

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

sjf.o: sjf.c
	$(CC) $(CFLAGS) -c sjf.c

rr_priority.o: rr_priority.c
	$(CC) $(CFLAGS) -c rr_priority.c

priority.o: priority.c
	$(CC) $(CFLAGS) -c priority.c

round_robin.o: round_robin.c
	$(CC) $(CFLAGS) -c round_robin.c

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c

cpu.o: cpu.c cpu.h
	$(CC) $(CFLAGS) -c cpu.c
