# MC833 1S2018 Unicamp - Projeto 1
### [Giovani Nascimento Pereira](github.com/giovaninppc) - 168609
### [Ignacio Ribeiro Espinoso](github.com/ignacioespinoso) - 169767


## Introdução
O conteúdo desse projeto visa simular as interações entre um socket de servidor, com uma aplicação simples rodando por trás e um socket de cliente.

O **Servidor** é composto pelos arquivos:
- server.c: [Programa principal] Faz a base das conexões
- Data.c: Simula uma base de dados
- timeManager.c: Faz o gerenciamente de IO com arquivos e o controle da medição do tempo no programa 
- libraries.h: Contém todos os imports necessários

O **Cliente** é composto pelos arquivos:
- client.c: [Programa principal] Faz a base das conexões
- timeManager.c: Faz o gerenciamente de IO com arquivos e o controle da medição do tempo no programa 
- libraries.h: Contém todos os imports necessários

> O arquivo test.c, é utilizado apenas para testar a base de dados Data.c.

## I - Metodologia
Para executar o programa e os testes, baixe esta pasta de arquivos - ela contem o arquivo do tipo **Makefile**, que simplifica a execução do código.

### Executando Localmente

Em uma máquina execute **make run_server** para compilar e executar o socket do servidor,
e depois execute, em um terminal diferente, **make run_local_client**. Isso vai fazer os dois programas executarem e *conversarem entre si*, o cliente estará se conectando na porta local 127.0.0.1.

O cliente irá abrir um Menu de opções para aconexão, que reflete as requeste que podem ser feitas para o nosso modelo de servidor.

Caso você execute o cliente sem o servidor estar rodando, ele irá dar um erro por não conseguir conectar.


### Executando em 2 máquinas

Em uma máquina execute **make run_server** para compilar e executar o socket do servidor,
e em outra máquina execute **make client** e depois execute o programa com **./client.out xxx.xx.xx**, onde x é o endereço IP da máquina com o servidor.

Certifique-se que a conexão é possível entre essas duas máquinas.

> Para mais informações sobre o Makefile, [abra o arquivo](https://github.com/ignacioespinoso/mc833/blob/master/proj1/Makefile) e leia a documentação interna.


### Executando em TEST MODE (Modo de teste)

O TEST MODE, foi o modo de execução criado para simular todas as iterações necessárias para calcular os tempos médios de conexão do programa. Ele executa 50 vezes cada request especificado pelo projeto.

Para realizar *teste local*, rode em um terminal o servidor com **make run_server**, e em outro execute o cliente em modo de teste com **make client_local_test**, isso fará automaticamente todas as iterações de envio e recebimento de mensagem.

Para realizar *teste remoto*, execute o servidor em uma máquina com **make run_server**, e em outra compile o cliente com **make client** e execute com **./client xxx.xx.xx TEST**, o último identificador avisa o programa para entrar em modo de teste.


## II - Estrutura

### Mensagens / Requests

As requests que o cliente pode mandar para o servidor foram categorizadas para facilitar a identificação (conforme dados na especificação do laboratório):

| Categoria | Request                                 | Mensagem**                |
|-----------|-----------------------------------------|---------------------------|
| 1         | Pegar todas as disciplinas e códigos    | "1 Get all subjects"      |
| 2         | Pegar ementa de uma disciplina          |  "2 XXXX***"              |
| 3         | Pegar toda informação de uma disciplina | "3 XXXX"                  |
| 4         | Pegar comentário da próxima aula        | "4 XXXX"                  |
| 5         | Pegar toda a informação disponível      | "5 Get all subjects info" |
| 6         | Configurar comentário da próxima aula   | "6 XXXX txt"              |

** Mensagem enviada para o servidor

*** XXXX é um substituto para o código da disciplina que será requisitada

## Saídas

Toda mensagem enviada e recebida - pelo cliente e servidor, é mostrada na saída padrão, bem como o tempo de cada execução.

No *cliente*, o tempo refere-se ao intervalo entre enviar uma requisição e receber uma resposta.
No *servidor*, o tempo refere-se ao intervalo de processamento, entre a chegada da mensagem, e o momento de envio. Para se encontrar o *tempo de conexão*, ou seja, apenas o tempo gasto entre a comunicação dos dois sockets, deve-se subtrair, para uma mesma conexão, o tempo de conexão do servidor do tempo de conexão do cliente.

Todos os tempos medidos também são impressos em um arquivo de LOG - clientTimeLog.txt e serverTimeLog.txt - isso feito automaticamente, com código gerenciado no arquivo **timeManager.c**, que é reponsável por medir o tempo e imprimir as saídas referentes a ele.

Esses arquivos são Log, ou seja, toda vez que são executados eles fazem um *append* da nova informação ao final do arquivo - então cuidado para encontrar a informação correta.

No começo de cada Log novo, é adicionado um marcador no arquivo com o horário da nova execução, o que facilita para encontrar os resultados necessários.
Exemplo:

```txt
-------- SESSION  11/3/118 9:57:48  --------

Operation: 0
>>>>>Send Time: 0 μs
<<<<<Receive Time: 594951 μs
Total Interval Time: 594951 μs

Operation: 1
>>>>>Send Time: 595903 μs
<<<<<Receive Time: 596227 μs
Total Interval Time: 324 μs

```
Note o horário em que o Log foi criado, a operação que foi feita. O número da operação está com descrito na seção II - Mensages/Requests (A operação 0 é a operação de conectar um ao outro - sempre aparece no início de um novo Log). O tempo é medido em microssegundos.


## III - Resultados

### Teste Local

Executando o programa localmente, em TEST MODE, pegamos informações sobre o tempo de comunicação entre os processos distintos **cliente** e  **servidor**. Os dados completos da comunicação podem ser encontrados [nesta tabela](https://docs.google.com/spreadsheets/d/1hvKi968pbDjVrS7xe3ppGN2-uGI_9jBCDU_SB1ZoQUE/edit?usp=sharing) online.

Cada operação foi executada 50 vezes neste teste.

O tempo médio de comunicação para cada Operação (conforme descrito na seção II - Mensagens) e o desvio padrão pode ser observado na tabela abaixo:

| Categoria | Media (μs) | Desvio Padrão (μs) | Intervalo de Confiança |
|-----------|------------|--------------------|------------------------|
| 1         | 66.65      | 14.14              | 3.92                   |
| 2         | 61.15      | 12.08              | 3.40                   |
| 3         | 74.92      | 65.15              | 17.89                  |
| 4         | 74.08      | 32.72              | 8.98                   |
| 5         | 140.76     | 128.64             | 35.98                  |
| 6         | 183.78     | 176.78             | 49.28                  |

A operação que mais demorou, analisando os resultados da *Média*, foi a operação 6 **Configurar comentário da próxima aula**, que é a única que envolve a escrita de informações na base de dados - e não apenas leitura. Isso mostra como a escrita é mais custosa que operações de leitura.

> É possível notar que o desvio padrão encontrado nos testes locais foi bem alto, em relação ao valores do tempo de comunicação. Localmente, o tempo de comunicação é mais sensível a propriedades internas do sistema como, escalonamento dos porcessos, IO, dentre outras coisas.


### Teste com máquinas separadas

O programa foi executado com o **servidor** rodando em um Macbook\*, e o cliente nos computadores do Instituto de Computação\*\*.




> \* Macbook Pro early 2015. 2,7 GHz Intel Core i5;
> Conectado na rede *Eduroam* por Wi-Fi. IP: 177.220.84.48

> \*\* Computador "Ira" foi usado para a coleta de dados.
> Conectado à rede cabeada. IP: 143.106.16.25


## IV - Conclusão



## V - Referências

[1] Guide to Network Programming, Brian "Beej Jorgensen" Hall 2016. Disponível em: http://beej.us/guide/bgnet/html/multi/index.html 

[2] C Socket Programming for Linux with a Server and Client Example Code,  Himanshu Arora 2011. Disponível em: https://www.thegeekstuff.com/2011/12/c-socket-programming/

[3] Example of Client-Server Program in C, Daniel Scocco 2014. Disponível em: https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/