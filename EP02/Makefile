# PROGRAMA
PROG = resolveEDO
OBJS = resolveEDO.o edo.o utils.o # mod1.o mod2.o

# Compilador
CC     = gcc

# Detecta automaticamente o diretório base do LIKWID
LIKWID_HOME := $(dir $(shell which likwid-perfctr))../

# Flags para uso da biblioteca LIKWID
CFLAGS = -O0 -DLIKWID_PERFMON -I$(LIKWID_HOME)/include
LFLAGS = -lm -L$(LIKWID_HOME)/lib -llikwid

# Lista de arquivos para distribuição. Acrescentar mais arquivos se necessário.
DISTFILES = *.c *.h LEIAME* Makefile edos.dat
DISTDIR = ${USER}

.PHONY: clean purge dist all

%.o: %.c
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
