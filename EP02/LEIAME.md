# EP 02 — Introdução à Computação Científica (CI1164)

**Nome:** Juliana Zambon  
**GRR:** 20224168  
**Login DINF:** jz22

**Disciplina:** Introdução à Computação Científica – CI1164  
**Departamento:** Departamento de Informática  
**Universidade:** Universidade Federal do Paraná (UFPR)

---

## Alterações realizadas
Foi adicionada a seguinte linha ao `Makefile` para permitir a detecção
automática do diretório base do LIKWID, para o funcionamento do script
`ep02_script.sh` e do marco de métricas no arquivo `resolveEDO.c`:

```makefile
LIKWID_HOME := $(dir $(shell which likwid-perfctr))../
```

Foi adicionada a função `resolveLU(Tridiag *sl, real_t *sol)` ao 
arquivo `edo.c` e a lib `edo.h` para a realização do método de 
fatoração LU.

```edo.h
void resolveLU(Tridiag *sl, real_t *sol);
```


## Como rodar o script com LIKWID

Rode o seguinte comando:

```ep02_script.sh
./ep02_script.sh < entradateste.txt
```

O arquivo `entradateste.txt` contém o exemplo de entrada fornecido no enunciado do EP02,
é possível rodar com outros arquivos também, mas o arquivo de entrada só poderá conter 
apenas um exemplo de entrada.