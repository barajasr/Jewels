CC=g++
IDIR=include
ODIR=obj
SDIR=source
CFLAGS=-std=c++11 -I $(IDIR) -Wall -Wextra -pedantic -O2
LIBS=-L /usr/local/lib
LSFML=-lsfml-audio -lsfml-graphics -lsfml-system -lsfml-window
APP=Jewels

all: $(APP)

$(APP): $(ODIR)/Cascade.o $(ODIR)/Gem.o $(ODIR)/Gameboard.o $(ODIR)/Main.o
	$(CC) -o $(APP) $(ODIR)/*.o $(LIBS) $(LSFML)

$(ODIR)/Main.o: $(SDIR)/Main.cpp $(IDIR)/Gameboard.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Main.cpp
	mv Main.o $(ODIR)

$(ODIR)/Cascade.o: $(SDIR)/Cascade.cpp $(IDIR)/Cascade.hpp $(SDIR)/Gameboard.cpp $(SDIR)/Gem.cpp
	$(CC) $(CFLAGS) -c $(SDIR)/Cascade.cpp
	mv Cascade.o $(ODIR)

$(ODIR)/Gameboard.o: $(SDIR)/Gameboard.cpp $(IDIR)/Gameboard.hpp $(IDIR)/Gem.hpp $(IDIR)/Cascade.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Gameboard.cpp
	mv Gameboard.o $(ODIR)

$(ODIR)/Gem.o: $(SDIR)/Gem.cpp $(IDIR)/Gem.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Gem.cpp
	mv Gem.o $(ODIR)

clean:
	rm $(ODIR)/*.o $(APP)

