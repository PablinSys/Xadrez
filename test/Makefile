CXX := g++
CXXFLAGS := -std=c++17 -Wall -I../include -I../../include
OBJDIR := ../build
SRCDIR := ../src
TESTDIR := ../test
BINDIR := ../bin

OBJ_FILES := \
  $(OBJDIR)/test_tabuleiro.o \
  $(OBJDIR)/tabuleiro.o \
  $(OBJDIR)/pecas.o \
  $(OBJDIR)/algoritmo.o \
  $(OBJDIR)/bot.o \
  $(OBJDIR)/gameController.o \
  $(OBJDIR)/game.o \
  $(OBJDIR)/gameUI.o


all: $(BINDIR)/test_tabuleiro

$(OBJDIR)/test_tabuleiro.o: $(TESTDIR)/test_tabuleiro.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/tabuleiro.o: $(SRCDIR)/tabuleiro.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/pecas.o: $(SRCDIR)/peças.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/algoritmo.o: $(SRCDIR)/algoritmo.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/bot.o: $(SRCDIR)/bot.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/gameController.o: $(SRCDIR)/gameController.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/game.o: $(SRCDIR)/game.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/gameUI.o: $(SRCDIR)/gameUI.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR)/test_tabuleiro: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L/usr/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/test_tabuleiro

