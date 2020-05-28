INC=-I./common/
COMAND=g++

all:
	mkdir common/obj
	mkdir generator/obj
	mkdir tester/obj
	mkdir sorter/obj
	$(COMAND) -c common/common.cpp -o common/obj/common.o
	$(COMAND) -c common/PreciseTimer.cpp -o common/obj/PreciseTimer.o
	ar rcs common.a common/obj/common.o common/obj/PreciseTimer.o
	$(COMAND) -c generator/main.cpp $(INC) -o generator/obj/main.o
	$(COMAND) generator/obj/main.o -o generator_exe common.a
	$(COMAND) -c tester/main.cpp $(INC) -o tester/obj/main.o
	$(COMAND) tester/obj/main.o -o tester_exe common.a
	$(COMAND) -c sorter/main.cpp $(INC) -o sorter/obj/main.o
	$(COMAND) -c sorter/FilePool.cpp $(INC) -o sorter/obj/FilePool.o
	$(COMAND) -c sorter/FilePools.cpp $(INC) -o sorter/obj/FilePools.o
	$(COMAND) -c sorter/Sorter.cpp $(INC) -o sorter/obj/Sorter.o
	$(COMAND) sorter/obj/main.o sorter/obj/FilePool.o sorter/obj/FilePools.o sorter/obj/Sorter.o -o sorter_exe common.a
	
clean:
	rm -rf ./*/obj
	rm -rf common.a
	rm -rf generator_exe
	rm -rf tester_exe
	rm -rf sorter_exe