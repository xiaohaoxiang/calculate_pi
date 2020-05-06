all : data_generator test
.PHONY : all data
data_generator : cpuinfo.o data_generator.o
	cc -o data_generator cpuinfo.o data_generator.o -O3 -std=c11
test : test.o pi.o file.o calculate.o timer.o rand.o
	cc -o test test.o pi.o file.o calculate.o timer.o rand.o -lpthread -O3 -std=c11
cpuinfo.o : cpuinfo.c cpuinfo.h
	cc -c cpuinfo.c -O3 -std=c11
data_generator.o : data_generator.c defs.h cpuinfo.h
	cc -c data_generator.c -O3 -std=c11
test.o : test.c calculate.h defs.h timer.h test.h file.h pi.h
	cc -c test.c -O3 -std=c11
pi.o : pi.c pi.h
	cc -c pi.c -O3 -std=c11
file.o : file.c file.h
	cc -c file.c -O3 -std=c11
calculate.o : calculate.c defs.h calculate.h rand.h
	cc -c calculate.c -O3 -std=c11
timer.o : timer.c timer.h
	cc -c timer.c -O3 -std=c11
rand.o : rand.c rand.h
	cc -c rand.c -O3 -std=c11

.PHONY : clean
clean :
	-rm data_generator test *.o
