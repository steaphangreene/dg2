CC:=	gcc -Wall -ffast-math -s -O2
LIBS:=	$(shell U2-Libs)
#LIBS:=	-lu2 -L/usr/X11/lib -lX11 -lm -lXext -lXxf86dga -lXxf86vm
ALL:=	/usr/local/lib/libu2.a

all:	cre swtm swrl chcl rc rcc

cre:	cre.o $(ALL)
	$(CC) -o cre cre.o $(LIBS)

cre.o:	cre.cpp cre.h
	$(CC) -c cre.cpp

swtm:	swtm.cpp
	$(CC) -o swtm swtm.cpp $(LIBS)

swrl:	swrl.cpp
	$(CC) -o swrl swrl.cpp $(LIBS)

chcl:	chcl.cpp
	$(CC) -o chcl chcl.cpp $(LIBS)

rc:	rc.cpp
	$(CC) -o rc rc.cpp $(LIBS)

rcc:	rcc.cpp
	$(CC) -o rcc rcc.cpp $(LIBS)

zip:	system weapons guy.crf cre rc rcc chcl
	del cre.zip
	pkzip -exraP cre.zip system system/*.bmp system/*.pal system/*.sgf 
	pkzip -exraP cre.zip weapons weapons/*.bmp guy guy/*.bmp guy/*.val
	pkzip -exraP cre.zip cre.exe rc.exe rcc.exe chcl.exe guy.crf

