CC:=	gcc -Wall -s -O2 -ffast-math
OBJS=	$(DJDIR)/include/user/*.o
DEPS=	$(DJDIR)/include/user/*.cpp
DEPH=	$(DJDIR)/include/user/*.h
ALL=	Makefile
LHEADS=	thing.h creature.h cell.h player.h action.h map.h stats.h weapon.h game.h spell.h ammo.h struct.h graphics/creature/cre.h
LOBJS=	map.o game.o dg2.o creature.o thing.o cell.o spell.o ammo.o struct.o player.o
#LOBJS=	*.o
LIBS=	-luser
TSTR=   $(shell date +"%Y%m%d%H%M")

all:	dg2 rs

tar:	dg2.cpp
	cd .. ; tar czhvf ~/c/archive/dg2.$(TSTR).tar.gz \
		dg2/*.cpp dg2/*.[hc] dg2/Makefile dg2/graphics \
		dg2/maps dg2/mapedit/*.cpp dg2/mapedit/*.[tc]w2 \
		dg2/mapedit/*.bmp dg2/*.crf


bcr:	bcr.cpp creature.h $(OBJS) $(ALL)
	$(CC) -o bcr bcr.cpp $(LIBS)

rs:	rs.cpp $(OBJS) $(ALL)
	$(CC) -o rs rs.cpp $(LIBS)

dg2:	$(LOBJS) $(ALL) $(LHEADS) $(OBJS)
	make -C $(DJDIR)/include/user user
	$(CC) -o dg2 $(LOBJS) $(LIBS)

$(DJDIR)/include/user/%.o:	$(DJDIR)/include/user/screen.h \
#				$(DJDIR)/include/user/%.cpp \
                                $(DJDIR)/include/user/makefile
	@echo -n
	make -C $(DJDIR)/include/user

%.o:	%.cpp $(ALL)
	$(CC) -c $<

player.o:	player.cpp $(LHEADS) $(ALL)
game.o:		game.cpp $(LHEADS) $(ALL)
map.o:		map.cpp $(LHEADS) $(ALL)
thing.o:	thing.cpp $(LHEADS) $(ALL)
dg2.o:		dg2.cpp $(LHEADS) $(ALL)
creature.o:	creature.cpp $(LHEADS) $(ALL)
cell.o:		cell.cpp $(LHEADS) $(ALL)
spell.o:	spell.cpp $(LHEADS) $(ALL)
ammo.o:		ammo.cpp $(LHEADS) $(ALL)
struct.o:	struct.cpp $(LHEADS) $(ALL)

