all: 
	( cd anbound && make all && make clean )
	( cd ccloader && make all )
program: 
	./ccloader/CCLoader --DC=21 --DD=20 --RESET=26 binary_firmware/anbound_timer_lock.bin
clean: 
	( cd anbound && make clean )
style:
	astyle --style=1tbs --break-blocks --pad-oper --pad-comma --pad-header --delete-empty-lines -k2 -xf --add-braces -Q -R -n *.c *.h 
