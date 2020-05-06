all : data_generator test data_dir
.PHONY : all data
data_generator : cpuinfo.o data_generator.o
	cc -o data_generator cpuinfo.o data_generator.o
test : test.o pi.o file.o calculate.o timer.o rand.o
	cc -o test test.o pi.o file.o calculate.o timer.o rand.o -lpthread
cpuinfo.o : cpuinfo.c cpuinfo.h
	cc -c cpuinfo.c
data_generator.o : data_generator.c defs.h cpuinfo.h
	cc -c data_generator.c
test.o : test.c calculate.h defs.h timer.h test.h file.h pi.h
	cc -c test.c
pi.o : pi.c pi.h
	cc -c pi.c
file.o : file.c file.h
	cc -c file.c
calculate.o : calculate.c defs.h calculate.h rand.h
	cc -c calculate.c
timer.o : timer.c timer.h
	cc -c timer.c
rand.o : rand.c rand.h
	cc -c rand.c
data_dir : 
	-mkdir data

.PHONY : clean
clean :
	-rm data_generator test *.o
