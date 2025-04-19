# PROGRAMA
    PROG = verificaEP02
# Compilador
    CC     = gcc -Wno-format

# Acrescentar onde apropriado as opções para incluir uso da biblioteca LIKWID
    CFLAGS = 
    LFLAGS = 

$(PROG): % : %.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	@echo "Limpando sujeira ..."
	@rm -f *~ *.bak

purge:  clean
	@echo "Limpando tudo ..."
	@rm -f $(PROG) *.o core a.out

