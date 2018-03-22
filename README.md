# MC833 - Laboratório de Redes
## IC, Unicamp. 1S2018

### Giovani Nascimento Pereira - 168609
### Ignacio Espinoso Ribeiro - 169767


## [Projeto 1](https://github.com/ignacioespinoso/mc833/proj1)

Montar um sistema de conexão usando sockets TCP entre um servidor e 2 tipos de clinete.

O servidor armazena informações sobre disciplinas: título, ementa, sala de aula, horário, có́digo da disciplina (identificador). Adicionalmente o servidor armazena um texto de comentário sobre a próxima aula de uma disciplina escrito pelo cliente professor. 
Portanto, existem dois tipos de usuário: aluno e professor.

Pelo menos as seguintes operações devem ser realizadas:

- listar todos os códigos de disciplinas com seus respectivos títulos;
- dado o código de uma disciplina, retornar a ementa;
- dado o código de uma disciplina, retornar todas as informações desta disciplina;
- listar todas as informações de todas as disciplinas;
- escrever um texto de comentário sobre a próxima aula de uma disciplina (apenas usuário professor);
- dado o código de uma disciplina, retornar o texto de comentário sobre a próxima aula.
