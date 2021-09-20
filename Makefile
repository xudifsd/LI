IDIR = ./inc
ODIR = ./src

CXX=clang++
CXXFLAGS = -I $(IDIR) -Wall -fPIC
LIBS = -lm

_DEPS = env.h exps.h token.h lexer.h parser.h callable.h built_in_fn.h setup.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = env.o exps.o token.o lexer.o parser.o callable.o built_in_fn.o setup.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

libli: $(OBJ)
	$(CXX) -shared -fPIC -o libli $<

test: t_lexer t_parser t_eval
	./t_lexer
	./t_parser
	./t_eval

t_lexer: $(ODIR)/t_lexer.o $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

t_parser: $(ODIR)/t_parser.o $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

t_eval: $(ODIR)/t_eval.o $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm -f t_lexer $(OBJ)
