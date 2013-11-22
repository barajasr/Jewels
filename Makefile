CXX    = g++
IDIR   = include
ODIR   = obj
SDIR   = source
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -O2
LIBS   = -L /usr/local/lib
LSFML  = -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-window
APP    = Jewels

all: $(ODIR) $(APP)

run: all
	./$(APP)

$(ODIR):
	mkdir -p $@

$(APP): $(ODIR)/Cascade.o $(ODIR)/Gameboard.o $(ODIR)/Gem.o \
    $(ODIR)/Main.o $(ODIR)/Resources.o $(ODIR)/Score.o $(ODIR)/Sounds.o \
    $(ODIR)/Swap.o $(ODIR)/Vanish.o
	$(CXX) -o $@ $^ $(LIBS) $(LSFML)
 
$(ODIR)/Cascade.o: $(SDIR)/Cascade.cpp $(IDIR)/Cascade.hpp \
    $(IDIR)/Gameboard.hpp $(IDIR)/Gem.hpp $(IDIR)/Resources.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR)/Gameboard.o: $(SDIR)/Gameboard.cpp $(IDIR)/Gameboard.hpp \
    $(IDIR)/Cascade.hpp $(IDIR)/Gem.hpp $(IDIR)/Icon.hpp $(IDIR)/Resources.hpp \
    $(IDIR)/Score.hpp $(IDIR)/Sounds.hpp $(IDIR)/Swap.hpp $(IDIR)/Vanish.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR)/Gem.o: $(SDIR)/Gem.cpp $(IDIR)/Gem.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR)/Main.o: $(SDIR)/Main.cpp $(IDIR)/Gameboard.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR)/Resources.o: $(SDIR)/Resources.cpp $(IDIR)/Resources.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR)/Score.o: $(SDIR)/Score.cpp $(IDIR)/Score.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR)/Sounds.o: $(SDIR)/Sounds.cpp $(IDIR)/Sounds.hpp \
    $(IDIR)/Resources.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR)/Swap.o: $(SDIR)/Swap.cpp $(IDIR)/Swap.hpp \
    $(IDIR)/Gameboard.hpp $(IDIR)/Gem.hpp $(IDIR)/Score.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR)/Vanish.o: $(SDIR)/Vanish.cpp $(IDIR)/Vanish.hpp \
    $(IDIR)/Gameboard.hpp $(IDIR)/Gem.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	-rm $(ODIR)/*.o $(APP)

