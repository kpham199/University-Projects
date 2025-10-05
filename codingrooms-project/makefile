# Makefile is a shortcut, kinda like a function to run terminal commands
build:
	rm -f app.exe
	gcc app.c MinPopVote.c -o app.exe

run:
	./app.exe

run_quiet:
	./app.exe -q

valgrind:
	rm -f app.exe
	gcc -g app.c MinPopVote.c -o app.exe
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./app.exe -f


# TODO: Task 1 - extend the makefile to build & run testing suite:
#         - add a target build_test to compile test.c with the functions
#           in MinPopVote.c and build the executable test.exe
#         - add a target run_test to exectue the testing suite test.exe 
build_test: #Creating an executable file
	rm -f test.exe #Needs to remove the old file
	gcc test.c MinPopVote.c -o test.exe 

run_test: #Running an executable file, doesn't need to remove old file
	./test.exe


# TODO: Task 4 - extend the makefile for the following targets:
#         - run_fast to execute the program app.exe with fast mode ON
#           and default values for all other command-line arguments. 
#         - at least 2 additional run targets for app.exe, similar 
#           to run_quiet but with other meaningful combinations of 
#           program settings set by command-line arguments
#         - any additional targets you find useful
run_fast:
	./app.exe -f

gdb_app:
	gdb ./app.exe

build_timing:
	rm -f app_timing.exe
	gcc -pg app.c MinPopVote.c -o app_timing.exe

build_timing_fast:
	./app_timing.exe -f



