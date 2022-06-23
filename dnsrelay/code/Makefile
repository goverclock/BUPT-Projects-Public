
dnsrelay: main.o trans.o cache.o table.o debug.o
	gcc main.o trans.o cache.o table.o debug.o -g -o dnsrelay

win: main.o trans.o cache.o table.o debug.o
	gcc main.o trans.o cache.o table.o debug.o -g -o dnsrelay -lm -lws2_32

clean:
	rm -f *.o dnsrelay

wclean:
	del *.o dnsrelay.exe
