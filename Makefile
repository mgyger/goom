COPTS:=-mcpu=cortex-m3 -Wall -fno-builtin-strcpy -fno-builtin-memcpy -fno-builtin-memset -fno-jump-tables -Os

firmware.bin: main Makefile
	arm-linux-gnueabi-objcopy -O binary main firmware.bin

firmware.hex: main Makefile
	arm-linux-gnueabi-objcopy -O ihex main firmware.hex

main: startup.s main.c wave.s Makefile main.lnk
	arm-linux-gnueabi-gcc $(COPTS) -nostartfiles -nostdlib -Xlinker -i startup.s main.c wave.s -o temp.o
	arm-linux-gnueabi-ld -T main.lnk temp.o -o main
	arm-linux-gnueabi-objdump -D main >main.dump


