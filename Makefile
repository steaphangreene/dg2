CC:=	gcc -Wall -s -O6 -ffast-math -malign-double
#-DBUILD_TIMESTAMP=\"$(shell tstamp)\" 
#-mpentium -mstack-align-double
OBJS=	$(DJDIR)/include/user/*.o
DEPS=	$(DJDIR)/include/user/*.cpp
DEPH=	$(DJDIR)/include/user/*.h
ALL=	Makefile
LHEADS=	thing.h creature.h cell.h player.h action.h map.h stats.h weapon.h game.h spell.h ammo.h struct.h
LOBJS=	player.o map.o game.o cw2.o creature.o thing.o cell.o spell.o ammo.o struct.o
#LOBJS=	*.o
LIBS=	-luser -lX11 -lm -lvga

all:	cw2 bcr rs

bcr:	bcr.cpp creature.h $(OBJS) $(ALL)
	$(CC) -o bcr bcr.cpp $(LIBS)

rs:	rs.cpp $(OBJS) $(ALL)
	$(CC) -o rs rs.cpp $(LIBS)

cw2:	$(LOBJS) $(ALL) $(LHEADS) $(OBJS)
	make -C $(DJDIR)/include/user user
	$(CC) -o cw2 $(LOBJS) $(LIBS)

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
cw2.o:		cw2.cpp $(LHEADS) $(ALL)
creature.o:	creature.cpp $(LHEADS) $(ALL)
cell.o:		cell.cpp $(LHEADS) $(ALL)
spell.o:	spell.cpp $(LHEADS) $(ALL)
ammo.o:		ammo.cpp $(LHEADS) $(ALL)
struct.o:	struct.cpp $(LHEADS) $(ALL)

