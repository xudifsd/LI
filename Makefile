IDIR = ./inc
ODIR = ./src

CXX=clang++
CXXFLAGS = -I $(IDIR) -Wall -fPIC
LIBS = -lm

_DEPS = env.h types.h token.h lexer.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = env.o token.o types.o lexer.o t_lexer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

test: t_lexer
	./t_lexer

t_lexer: $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm -f t_lexer $(OBJ)
