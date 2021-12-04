all : stv.tap

#OPT=-SO3 --max-allocs-per-node200000
OPT=

stv.tap : main.c draw_swarm.c main.h
	zcc +zx -vn $(OPT) -clib=sdcc_iy -startup=4 \
		--math16 --math32 main.c draw_swarm.c -o stv -create-app \
		--c-code-in-asm --std-c99 --list -m -s

.PHONY: clean
clean :
	rm -f *.tap *.bin *.map *.sym *.o *.lis *~
