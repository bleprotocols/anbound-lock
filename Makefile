CC=sdcc
CFLAGS=--model-medium --xram-loc 0x0000 --xram-size 0x2000 --iram-size 0x0100 -I/usr/share/sdcc/include -DUART0
AS=sdas8051
ASFLAGS=-glos -p

LIBS=util.rel pm_sleep.rel
BIN=timer_lock.bin

%.rel: %.c
	$(CC) $(CFLAGS) -c $? -o $@

%.rel: %.s
	$(AS) $(ASFLAGS) $?

%.hex: %.c $(LIBS)
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

%.bin: %.hex
	objcopy -Iihex -Obinary $< $@
clean:
	rm  *.asm *.hex *.lst *.map *.mem *.rel *.lk *.rst *.sym *.adb *.cdb *.omf -f

all: ${LIBS} ${BIN}
	gcc CCLoader.c -std=c99 -lm -o CCLoader
