# EP 02 — Introdução à Computação Científica (CI1164)

**Nome:** Juliana Zambon  
**GRR:** 20224168  
**Login DINF:** jz22

**Disciplina:** Introdução à Computação Científica – CI1164  
**Departamento:** Departamento de Informática  
**Universidade:** Universidade Federal do Paraná (UFPR)

---

## Alterações realizadas
Foi adicionada a seguinte linha ao `Makefile` para permitir a detecção automática do diretório base do LIKWID, para o funcionamento do script `ep02_script.sh` e do marco de métricas no arquivo `resolveEDO.c`:

```makefile
LIKWID_HOME := $(dir $(shell which likwid-perfctr))../
```

## Limitações do Programa

Este programa apresenta limitações nos seguintes casos:

### Exemplos de limitações: