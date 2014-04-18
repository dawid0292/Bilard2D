CFLAGS = -I.. -O
LINKFLAGS = -O
LIBS = ../libwave.a -L$(HOME)/lib -ldave -limage -lgl_s -lm

wfdraw: main.o spin.o ../libwave.a
	$(CC) -o wfdraw $(LINKFLAGS) main.o spin.o $(LIBS)

main.o: main.c ../wave.h

spin.o: spin.c

wfcopy: wfcopy.o ../libwave.a
	$(CC) -o wfcopy $(LINKFLAGS) wfcopy.o $(LIBS)

wfaddtex: wfaddtex.o ../libwave.a
	$(CC) -o wfaddtex $(LINKFLAGS) wfaddtex.o $(LIBS)

wftrans: wftrans.o ../libwave.a
	$(CC) -o wftrans $(LINKFLAGS) wftrans.o $(LIBS)

wfscale: wfscale.o ../libwave.a
	$(CC) -o wfscale $(LINKFLAGS) wfscale.o $(LIBS)

wfmerge: wfmerge.o ../libwave.a
	$(CC) -o wfmerge $(LINKFLAGS) wfmerge.o $(LIBS)

wfbounds: wfbounds.o ../libwave.a
	$(CC) -o wfbounds $(LINKFLAGS) wfbounds.o $(LIBS)

