CFLAGS = -O -I.
OGLOBJS = readobj.o drawobj_ogl.o readmtl.o initobj_ogl.o readmap.o misc.o \
	mem.o writeobj.o transform.o normals.o ray.o
IGLOBJS = readobj.o drawobj.o readmtl.o initobj.o readmap.o misc.o mem.o \
	writeobj.o transform.o normals.o ray.o

libwave_ogl.a: $(OGLOBJS)
	rm -f libwave_ogl.a
	ar scr libwave_ogl.a $(OGLOBJS)

libwave.a: $(IGLOBJS)
	rm -f libwave.a
	ar scr libwave.a $(IGLOBJS)

$(IGLOBJS): wave.h
