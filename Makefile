IDIR = ./inc
ODIR = ./src
TDIR = ./test

CXX=clang++
CXXFLAGS = -I $(IDIR) -Wall -fPIC
LIBS = -lm
TLIBS = -lgtest -pthread

_DEPS = env.h exps.h token.h lexer.h parser.h callable.h built_in_fn.h built_in_macro.h setup.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = env.o exps.o token.o lexer.o parser.o callable.o built_in_fn.o built_in_macro.o setup.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TDEPS = test_utils.h
TDEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_TOBJ = test_utils.o t_lexer.o t_parser.o t_eval.o t_builtin.o
TOBJ = $(patsubst %,$(TDIR)/%,$(_TOBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TDIR)/%.o: %.c $(DEPS) $(TDEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

libli: $(OBJ)
	$(CXX) -shared -fPIC -o $@ $<

test: run_test
	./run_test

run_test: $(TDIR)/main.cpp $(OBJ) $(TOBJ)
	$(CXX) -o $@ $^ $(LIBS) $(TLIBS) $(CXXFLAGS)

.PHONY: clean wc

clean:
	-rm -f libli run_test $(OBJ) $(TOBJ)

wc: clean
	find . -type f -not -path "./.git/*" -not -path "*.sw[po]" | xargs wc -l
