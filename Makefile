net: net.o netArrayAllocate.o netFileIO.o netMisc.o netMath.o net.h
	gcc -o net net.o netArrayAllocate.o netFileIO.o netMisc.o netMath.o -lpgplot -lcpgplot -lX11 -lm

net.o: net.c net.h
	gcc -c net.c

netArrayAllocate.o: netArrayAllocate.c net.h
	gcc -c netArrayAllocate.c

netFileIO.o: netFileIO.c net.h
	gcc -c netFileIO.c

netMisc.o: netMisc.c net.h
	gcc -c netMisc.c

netMath.o: netMath.c net.h
	gcc -c netMath.c
