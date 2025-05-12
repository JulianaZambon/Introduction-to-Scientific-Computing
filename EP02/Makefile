# PROGRAMA
    PROG = resolveEDO
    OBJS = resolveEDO.o edo.o utils.o # mod1.o mod2.o

# Compilador
    CC     = gcc

# Acrescentar onde apropriado as opções para incluir uso da biblioteca LIKWID
    CFLAGS = -O0
    LFLAGS = -lm

# Lista de arquivos para distribuição. Acrescentar mais arquivos se necessário.
DISTFILES = *.c *.h LEIAME* Makefile edos.dat
DISTDIR = ${USER}

.PHONY: clean purge dist all

%.o: %.c %.h utils.h
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	@echo "Limpando sujeira ..."
	@rm -f *~ *.bak

purge:  clean
	@echo "Limpando tudo ..."
	@rm -f $(PROG) $(OBJS) core a.out $(DISTDIR) $(DISTDIR).tgz

dist: purge
	@echo "Gerando arquivo de distribuição ($(DISTDIR).tgz) ..."
	@ln -s . $(DISTDIR)
	@tar -chvzf $(DISTDIR).tgz $(addprefix ./$(DISTDIR)/, $(DISTFILES))
	@rm -f $(DISTDIR)