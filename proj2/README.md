# MC833 1S2018 Unicamp - Projeto 2
### [Giovani Nascimento Pereira](github.com/giovaninppc) - 168609
### [Ignacio Ribeiro Espinoso](github.com/ignacioespinoso) - 169767

## [Projeto 2](https://github.com/ignacioespinoso/mc833/proj2)

# I - Introdução

O projeto 2 consiste em implementar uma conexão UDP entre cliente e servidor. Para tal, foram implementadas e testadas as interações entre um socket de servidor, com uma aplicação simples rodando por trás e um socket de cliente.
No caso, a aplicação do servidor consiste em um sistema que armazena dados relativos a disciplinas (códigos, ementas e comentários) para consulta e alterações por parte do cliente. Vale notar que existem dois tipos de usuário: aluno e professor, de forma que somente um professor pode executar determinadas operações.
As seguintes operações podem ser realizadas por qualquer tipo de usuário:

- Obter todas as disciplinas
- Obter descrição de uma disciplina específica.
- Obter informação completa de uma disciplina específica.
- Obter informação da próxima aula de uma disciplina específica.
- Obter todas as informações de todas as disciplinas.
- Fechar conexão.

Operações restritas a professores:

- Definir comentário da próxima aula.
- Enviar mensagem.

# II - Sistema / Metodologia

A execução pode ser separada em atividades do **Servidor** e do **Cliente**.

O **Servidor** é composto pelos arquivos:

- server.c: [Programa principal] Faz a base das conexões
- Data.c: Simula uma base de dados
- timeManager.c: Faz o gerenciamente de IO com arquivos e o controle da medição do tempo no programa
- libraries.h: Contém todos os imports necessários

O **Cliente** é composto pelos arquivos:

- client.c: [Programa principal] Faz a base das conexões
- timeManager.c: Faz o gerenciamente de IO com arquivos e o controle da medição do tempo no programa
- libraries.h: Contém todos os imports necessários

Para executar o programa e os testes, baixe esta pasta de arquivos - ela contem o arquivo do tipo **Makefile**, que simplifica a execução do código.

### Executando Localmente

Em uma máquina execute **make run_server** para compilar e executar o socket do servidor,
e depois execute, em um terminal diferente, **make run_local_client**. Isso vai fazer os dois programas executarem e *conversarem entre si*, o cliente estará se conectando no IP 127.0.0.1.

O cliente irá abrir um Menu de opções para a conexão, que reflete as *requests* que podem ser feitas para o nosso modelo de servidor.

Caso você execute o cliente sem o servidor estar rodando, ele irá dar um erro por não conseguir conectar.

### Executando em 2 máquinas

Em uma máquina execute **make run_server** para compilar e executar o socket do servidor,
e em outra máquina execute **make client** e depois execute o programa com **./client.out xxx.xx.xx**, onde x é o endereço IP da máquina com o servidor.

Certifique-se que a conexão é possível entre essas duas máquinas.

> Para mais informações sobre o Makefile, [abra o arquivo](https://github.com/ignacioespinoso/mc833/blob/master/proj2/Makefile) e leia a documentação interna.


### Executando em TEST MODE (Modo de teste)

O TEST MODE, foi o modo de execução criado para simular todas as iterações necessárias para calcular os tempos médios de conexão do programa. Ele executa 50 vezes cada request especificado pelo projeto.

Para realizar *teste local*, rode em um terminal o servidor com **make run_server**, e em outro execute o cliente em modo de teste com **make client_local_test**, isso fará automaticamente todas as iterações de envio e recebimento de mensagem.

Para realizar *teste remoto*, execute o servidor em uma máquina com **make run_server**, e em outra compile o cliente com **make client** e execute com **./client xxx.xx.xx TEST**, o último identificador avisa o programa para entrar em modo de teste.


# III - Estrutura
### Armazenamento de Dados do Servidor

Para armazenar os dados do servidor, foi feita a simulação de uma base de dados, que é carregada na memória junto com o programa do servidor (essa parte do código está disponível no arquivo *Data.c*).

Para cada aula, foi definida uma *struct* que contém todas as informações que o usuário pode requisitar, de acordo com cada disciplina.

```C
//Creating struct for Subject information
typedef struct ClassData{

	char codigo[10];
	char nome[100];
	char ementa[500];
	char professor[100];
	char sala[100];
	char comentario[500];

} Disciplina;
```

E toda a informação é armazenada em um vetor dessa *struct*, que é manipulado em memória durante a execução. Para fazer acesso e alterações aos dados, o servidor usa um conjunto de operações definidas no mesmo arquivo que executam em conjunto com o restante do código do servidor, no mesmo processo/thread, havendo assim menos problemas com comunicação.

Essa abordagem foi selecionada, pois para os resultados esperados, a maneira como os dados estavam sendo armazenados ou dispostos, não interferia no resultado final esperado, que é o tempo de comunicação entre sistemas.
E, além de ser uma abordagem simples, e de fácil implementação, ela é executada mais rápidamente que requisições a bancos de dados, ou leituras em arquivos.

Um dos problemas que ela acarreta, por outro lado, é o aumento do tamanho do código do servidor, pois toda a base de dados faz parte desse conjunto de instruções, então não é uma abordagem viável para um sistema real - ou que contenha um volume maior de dados.


> Não há persistência dos dados neste sistema.

# IV - Implementação 

### Formato

A comunicação entre cliente e servidor se deu através de uma conexão UDP, assim a conexão estabelecida entre cliente e servidor é *stateless*. Com isso, o sistema se resume a um cliente, também denominado *talker*, que envia mensagens e requisições a um servidor, também denominado *listener*, que recebe requisições e as responde.

### Mensagens / Requests

As requests que o cliente pode mandar para o servidor foram categorizadas para facilitar a identificação (conforme dados na especificação do laboratório):

| Categoria | Request                                 | Mensagem**                |
|-----------|-----------------------------------------|---------------------------|
| 1         | Pegar todas as disciplinas e códigos    | "1 Get all subjects"      |
| 2         | Pegar ementa de uma disciplina          | "2 XXXX***"               |
| 3         | Pegar toda informação de uma disciplina | "3 XXXX"                  |
| 4         | Pegar comentário da próxima aula        | "4 XXXX"                  |
| 5         | Pegar toda a informação disponível      | "5 Get all subjects info" |
| 6         | Configurar comentário da próxima aula   | "6 XXXX txt"              |

** Mensagem enviada para o servidor

*** XXXX é um substituto para o código da disciplina que será requisitada

Note que a primeira informação de cada mensagem é um número, referente ao identificador (categoria) da operação. Essa é a informação que é inicialmente analisada pelo servidor, para identificar qual o tipo de requisição o usuário está fazendo, e depois, o restante da mensagem de acordo com a operação.

Como a troca de mensagens é feita inteiramente por Strings, esse método foi escolhido por formar uma maneira simples de identificar as mensagens (decodificar a requisição), e definir quais repostas deveriam partir do servidor em cada caso.

Mensagens não identificadas nesta tabela, ou fora deste formato, são respondidas com uma mensagem padrão do servidor *[[Unrecognized Message/Request]]*, precedida da mensagem recebida.

E as resposta padrão, para todas as mensagens identificadas na tabela acima, é uma string contendo a resposta esperada pelo cliente.

### Usuários Aluno/Professor

A implementação dos tipos de usuário foi adotada de forma que o acesso padrão ao sistema é feito por um aluno. Este tipo de usuário tem acesso a todas operações, exceto poder definir comentários sobre uma disciplina. Outro tipo de usuário é o professor, que tem acesso a todas as operações do sistema.
Para que um usuário acesse o sistema como professor (TODO).

### Saídas

Toda mensagem enviada e recebida - pelo cliente e servidor, é mostrada na saída padrão, bem como o tempo de cada execução.

No *cliente*, o tempo refere-se ao intervalo entre enviar uma requisição e receber uma resposta.
No *servidor*, o tempo refere-se ao intervalo de processamento, entre a chegada da mensagem, e o momento de envio. Para se encontrar o *tempo de conexão*, ou seja, apenas o tempo gasto entre a comunicação dos dois sockets, deve-se subtrair, para uma mesma conexão, o tempo de conexão do servidor do tempo de conexão do cliente.

Todos os tempos medidos também são impressos em um arquivo de LOG - clientTimeLog.txt e serverTimeLog.txt - isso feito automaticamente, com código gerenciado no arquivo **timeManager.c**, que é reponsável por medir o tempo e imprimir as saídas referentes a ele.

Esses arquivos são Log, ou seja, toda vez que são executados eles fazem um *append* da nova informação ao final do arquivo - então cuidado para encontrar a informação correta.

No começo de cada Log novo, é adicionado um marcador no arquivo com o horário da nova execução, o que facilita para encontrar os resultados necessários.
Exemplo:

```txt
-------- SESSION  11/5/118 9:57:48  --------

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

# V - Resultados
### Teste Local
### Teste com máquinas separadas

# VI - Conclusão

# VII - Referências

[1] Guide to Network Programming, Brian "Beej Jorgensen" Hall 2016. Disponível em: http://beej.us/guide/bgnet/html/multi/index.html

[2] C Socket Programming for Linux with a Server and Client Example Code,  Himanshu Arora 2011. Disponível em: https://www.thegeekstuff.com/2011/12/c-socket-programming/

[3] Example of Client-Server Program in C, Daniel Scocco 2014. Disponível em: https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/

[4] Programming UDP Sockets in Linux, Silver Moon 2012. Disponível em: https://www.binarytides.com/programming-udp-sockets-c-linux/