CC=g++
IDIR=include
ODIR=obj
SDIR=source
CFLAGS=-std=c++11 -I $(IDIR) -Wall -Wextra -pedantic -O2
LIBS=-L /usr/local/lib
LSFML=-lsfml-audio -lsfml-graphics -lsfml-system -lsfml-window
APP=Jewels

all: $(APP)

run: $(APP)
	./$(APP)

$(APP): $(ODIR)/Cascade.o $(ODIR)/Swap.o $(ODIR)/Vanish.o $(ODIR)/Resources.o $(ODIR)/Gem.o $(ODIR)/Gameboard.o $(ODIR)/Main.o
	$(CC) -o $(APP) $(ODIR)/*.o $(LIBS) $(LSFML)
 
$(ODIR)/Main.o: $(SDIR)/Main.cpp $(IDIR)/Gameboard.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Main.cpp
	mv Main.o $(ODIR)

$(ODIR)/Gameboard.o: $(SDIR)/Gameboard.cpp $(IDIR)/Gameboard.hpp $(IDIR)/Icon.hpp $(IDIR)/Cascade.hpp $(IDIR)/Gem.hpp $(IDIR)/Resources.hpp $(IDIR)/Swap.hpp $(IDIR)/Vanish.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Gameboard.cpp
	mv Gameboard.o $(ODIR)

$(ODIR)/Gem.o: $(SDIR)/Gem.cpp $(IDIR)/Gem.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Gem.cpp
	mv Gem.o $(ODIR)

$(ODIR)/Cascade.o: $(SDIR)/Cascade.cpp $(IDIR)/Cascade.hpp $(IDIR)/Gameboard.hpp $(IDIR)/Gem.hpp $(IDIR)/Resources.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Cascade.cpp
	mv Cascade.o $(ODIR)

$(ODIR)/Resources.o: $(SDIR)/Resources.cpp $(IDIR)/Resources.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Resources.cpp
	mv Resources.o $(ODIR)

$(ODIR)/Swap.o: $(SDIR)/Swap.cpp $(IDIR)/Swap.hpp $(IDIR)/Gameboard.hpp $(IDIR)/Gem.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Swap.cpp
	mv Swap.o $(ODIR)

$(ODIR)/Vanish.o: $(SDIR)/Vanish.cpp $(IDIR)/Vanish.hpp $(IDIR)/Gameboard.hpp $(IDIR)/Gem.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Vanish.cpp
	mv Vanish.o $(ODIR)

clean:
	rm $(ODIR)/*.o $(APP)

