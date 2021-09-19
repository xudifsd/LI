IDIR = ./inc
ODIR = ./src

CXX=clang++
CXXFLAGS = -I $(IDIR) -Wall -fPIC
LIBS = -lm

_DEPS = env.h types.h token.h lexer.h parser.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = env.o token.o types.o lexer.o parser.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

libli: $(OBJ)
	$(CXX) -shared -fPIC -o libli $<

test: t_lexer t_parser
	./t_lexer
	./t_parser

t_lexer: $(ODIR)/t_lexer.o $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

t_parser: $(ODIR)/t_parser.o $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm -f t_lexer $(OBJ)
