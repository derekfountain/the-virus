all : vectors.tap

#OPT=-SO3 --max-allocs-per-node200000
OPT=

vectors.tap : main.c draw_swarm.c swarm_algorithm.c vectors.c vectors.h
	zcc +zx -vn $(OPT) -clib=sdcc_iy -startup=4 \
		--math16 --math32 main.c draw_swarm.c vectors.c swarm_algorithm.c -o vectors -create-app \
		--c-code-in-asm --std-c99 --list -m -s

.PHONY: clean
clean :
	rm -f *.tap *.bin *.map *.sym *.o *.lis *~
