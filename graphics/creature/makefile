CC=	gcc -O3 -s
OBJS=	d:/djgpp/include/user/*.o
DEPS=	d:/djgpp/include/user/*.cpp
DEPH=	d:/djgpp/include/user/*.h
ALL=	$(DEPS) $(DEPH) $(OBJS) 

bcr.exe:	bcr.cpp $(ALL)
	make -C d:/djgpp/include/user
	$(CC) -o bcr.exe bcr.cpp $(OBJS)
