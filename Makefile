CC:=	gcc $(shell U2-CFlags)
#CC:=	gcc -Wall -s -O2 -ffast-math
UDIR:=	$(shell U2-Dir)
OBJS:=	$(shell csh -c "echo $(UDIR)*.o")
DEPS:=	$(shell csh -c "echo $(UDIR)*.cpp")
DEPH:=	$(shell csh -c "echo $(UDIR)*.h")
ALL=	Makefile
LHEADS=	thing.h creature.h cell.h player.h action.h map.h stats.h weapon.h game.h spell.h ammo.h struct.h graphics/creature/cre.h
LOBJS=	map.o game.o dg2.o creature.o thing.o cell.o spell.o ammo.o struct.o player.o
#LOBJS=	*.o
LIBS:=	$(shell U2-Libs)
TSTR=   $(shell date +"%Y%m%d%H%M")

all:	dg2 palconv

tar:	dg2.cpp
	cd .. ; tar czhvf ~/c/archive/dg2.$(TSTR).tar.gz \
		dg2/*.cpp dg2/*.[hc] dg2/Makefile dg2/graphics \
		dg2/maps dg2/mapedit/*.cpp dg2/mapedit/*.[tc]w2 \
		dg2/mapedit/*.bmp dg2/*.crf dg2/*.sgf


bcr:	bcr.cpp creature.h $(OBJS) $(ALL)
	$(CC) -o bcr bcr.cpp $(LIBS)

rs:	rs.cpp $(ALL) $(DEPS) $(DEPH)
	$(CC) -o rs rs.cpp $(LIBS)

dg2:	$(ALL) $(LOBJS) $(LHEADS) $(DEPS) $(DEPH)
	make -C $(UDIR) user
	$(CC) -o dg2 $(LOBJS) $(LIBS)

tg:	tg.o $(ALL) $(LHEADS)
	$(CC) -o tg tg.o $(LIBS)

palconv:	palconv.o $(ALL) $(LHEADS)
	$(CC) -o palconv palconv.o $(LIBS)

%.o:	%.cpp $(LHEADS) $(ALL) $(DEPH)
	$(CC) -c $<

clean:
	rm -f *.o *.so *.a dg2 tg rs bcr
