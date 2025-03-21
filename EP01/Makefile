    CC     = gcc

    CFLAGS = -O0
    LFLAGS = -lm

      PROG = labZero
      OBJS = utils.o ZeroFuncao.o

# Lista de arquivos para distribuição
DISTFILES = *.c *.h Makefile polinomios.dat
DISTDIR = ${USER}

.PHONY: clean purge distclean all

%.o: %.c %.h DoubleType.h utils.h
	$(CC) -c $(CFLAGS) $<

$(PROG):  $(OBJS) $(PROG).o
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	@rm -f *~ *.bak

purge:   clean
	@rm -f *.o core a.out
	@rm -f $(PROG)

dist: purge
	@echo "Gerando arquivo de distribuição ($(DISTDIR).tar) ..."
	@ln -s . $(DISTDIR)
	@tar -cvf $(DISTDIR).tar $(addprefix ./$(DISTDIR)/, $(DISTFILES))
	@rm -f $(DISTDIR)
