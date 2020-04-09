# T3: Scheduling com OpenMP
Alexandre Moreira Medina - ELC139

## Resultados
Para compilar o programa:
```
g++ -std=c++11 OpenMPDemoABC.cpp -o OpenMPDemoABC -fopenmp
```

Saída:
```
Each thread should add its char to the array n times (n=20)
Correct results should total exactly nThreads*nTimes chars
Case 1: Schedule static
ACACACABCBABCBCAACBCCBCBBBBBBBBBBBCBCBACABACACACACACACACAAAA
A=20 B=20 C=20
Case 2: Schedule static, with chunk specification = 5
ACAABCBCACCBCCBABABBBBCABAAAAACAAAAAAAACBCCBCBCBBBBBCBBCCCCC
A=20 B=20 C=20
Case 3: Schedule dynamic
BACBCCBACBCBCABCBABABACBBCACACABCAAAABACBACBBCABBBACABCABACB
A=20 B=22 C=18
Case 4: Schedule dynamic, with chunk specification
BBBBBCABCABCABCABACBACBABAAABBAAAAACBCCCAAAAACCBBBBBCACCAAAA
A=25 B=20 C=15
Case 5: Schedule guided
AABABBBABABABBBABABABCABACABCABCBACBAAABBCBBBAAACBBCCCBACCCC
A=21 B=25 C=14
Case 6: Schedule guided, with chunk specification
CACABCACBCABCBABACBCABCABBBBACBABBCACACBAAABABBBAAAACCCCBCCC
A=20 B=20 C=20
Case 7: Schedule runtime
BABBBCACABACBABCBCACABCACABABACBCBACABACABCABCACBCACABACBBAC
A=21 B=20 C=19
Case 8: Schedule auto
BBBBBBBACBCBACACACBACABCBABBCBCCCABBBBBAAACCCCCCACCCAAAAAAAA
A=20 B=20 C=20
Case 9: Schedule runtime without mutex
CACBACBCABCABACBCABCABCABCABCABCABCABCABCABCABCABCABCABCAB--
A=19 B=19 C=20
Case 10: Schedule dynamic without mutex
ABCABCABCABACBACBCABCACBABACBACABCABCABCABCACBACBACBACBACB--
A=20 B=19 C=19
Case 11: Schedule dynamic, with chunk specification and without mutex
ACABBCABACABCABCACBCABCABCABCABCABCABCABCABCABACBACBACBACB--
A=20 B=19 C=19
```

Discussão:
- **static:** o escalonador divide o loop em chunks definidos pelo usuário ou não, caso não seja definido, o escalonador divide em partes iguais, ou o mais próximo disso quando o tamanho do loop não é uma divisão exata pelo número de threads.

- **dynamic:** as threads vão recebendo dinamicamente o tamanho do chunk ao iniciar, e quando termina o chunk, a próxima vez que for escalonada recebe um novo tamanho.

- **guided:** também seta o tamanho do chunk das threads dinamicamente, com a diferença de que o tamanho do chunk vai diminuindo ao passar das iterações

- **runtime:** diferente dos citados anteriormente, não é possível setar o tamanho do chunk. O que decide qual escalonador vai usar no caso **runtime** é a variável de ambiente **omp_schedule**

- **auto:** a decisão de qual escalonador usar é passada ao compilador.

Nos 3 últimos testes onde não foi utilizado mutação exclusiva ocorreu erros.


## Referências
- [Parallel Region](https://computing.llnl.gov/tutorials/openMP/#ParallelRegion)
Documentação Oficial do OpenMP, Blaise Barney, Lawrence Livermore National Laboratory
