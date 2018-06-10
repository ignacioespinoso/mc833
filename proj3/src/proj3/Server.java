package proj3;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Server implements Compute {
    private College unicamp;
    public ConnectionTime op;
    private int usr_type;

    public Server() {
        super();
    }

    public String analyzeRequest(String message) {
        String answer;
        int request;
        String code, comment;


        request = Character.getNumericValue(message.charAt(0));

        this.op = new ConnectionTime(request, System.nanoTime(), 0);
        //process the request string (op number and parameters) on the database (unicamp).
        switch(request) {
            case 1:
                answer = "All Subjects:\n";
                answer += getCodigosNomesDisciplinas();
                break;
            case 2:
                code = getCodeFromRequest(message);
                answer = code + "\n";
                answer += getEmentaFromCodigo(code);
                break;
            case 3:
                code = getCodeFromRequest(message);
                answer = code + "\n" + getInformacaoDisciplinaFromCodigo(code);
                break;
            case 4:
                code = getCodeFromRequest(message);
                answer = code + "\n" + getComentarioFromCodigo(code);
                break;
            case 5:
                answer = "All Subjects:\n";
                answer += getAllInfo();
                break;
            case 6:
                code = getCodeFromRequest(message);
                if(usr_type == 1) {
                    comment = getCommentFromRequest(message);
                    answer = "Next class " + code + ": " + comment;

                    if (!setComentarioForCode(code, comment)) {
                        answer = "The comment couldnt be written. Check the subject code\n";
                    }
                } else {
                    answer = "You don't have permission to do that!\n";
                }
                break;
            case 9:
                answer = "You are now logged as a teacher!\n";
                usr_type = 1;
                break;
            default:
                answer = message + " [Unrecognized Message/Request]";
                break;
        }

        this.op.receiveTime = System.nanoTime();
        return answer;
    }

    public static void main(String[] args) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        try {
            String name = "Compute";
            Compute engine = new Server();
            ((Server) engine).CreateUnicamp();
            Compute stub = (Compute) UnicastRemoteObject.exportObject(engine, 0);
            Registry registry = LocateRegistry.getRegistry();
            registry.rebind(name, stub);
            System.out.println("Server bound");
        } catch (Exception e) {
            System.err.println("Server exception:");
            e.printStackTrace();
        }
    }

    private void CreateUnicamp() {
        College Unicamp = new College(6);

        Unicamp.disciplines[0].codigo = "MA111";
        Unicamp.disciplines[0].nome = "Topicos de Calculo 1 para engenharias";
        Unicamp.disciplines[0].ementa = "Análise de limites. Derivadas totais. Integrais simples de uma variável. Integrais 2D.";
        Unicamp.disciplines[0].professor = "Ketty Mello";
        Unicamp.disciplines[0].sala = "CB03";
        Unicamp.disciplines[0].comentario = "Na proxima aula vamos estudar o desenvolvimento do diferencial em 3 planos";

        Unicamp.disciplines[1].codigo = "ME322";
        Unicamp.disciplines[1].nome = "Introducao a probabilidades";
        Unicamp.disciplines[1].ementa = "Variaveis aleatorias discretas e continuas. Distribuicoes comuns: Poisson, intermédio, binário, gauss...";
        Unicamp.disciplines[1].professor = "Anderson Bernardini";
        Unicamp.disciplines[1].sala = "PB08 / IM17";
        Unicamp.disciplines[1].comentario = "Para a proxima aula, ler os textos do capitulo 3 sobre variaveis aleatorias continuas. Vamos discutir em sala os efeitos dessas variaveis nas formulacoes de probabilidade.";

        Unicamp.disciplines[2].codigo = "MC102";
        Unicamp.disciplines[2].nome = "Programacao e ordenacao de computadores";
        Unicamp.disciplines[2].ementa = "Programação básica para iniciantes - em C e Python.";
        Unicamp.disciplines[2].professor = "Zanoni Dias";
        Unicamp.disciplines[2].sala = "CB05 / CC353";
        Unicamp.disciplines[2].comentario = "Labs 3 e 4 disponiveis no SuSy. Boa semana a todos.";

        Unicamp.disciplines[3].codigo = "EE532";
        Unicamp.disciplines[3].nome = "Eletronica Aplicada";
        Unicamp.disciplines[3].ementa = "Estudo de circuitos eletricos nao lineares com componentes semi condutores.";
        Unicamp.disciplines[3].professor = "Lee da FEEC";
        Unicamp.disciplines[3].sala = "FE02";
        Unicamp.disciplines[3].comentario = "Entrega de terça-feira disponível no Drive da turma.";

        Unicamp.disciplines[4].codigo = "MS211";
        Unicamp.disciplines[4].nome = "Cálculo Numérico";
        Unicamp.disciplines[4].ementa = "Métodos e processos de zeros de funções, aproximações nominais, resolucao e derivadas e integrais.";
        Unicamp.disciplines[4].professor = "Maria Cheti Ehrlhardtz";
        Unicamp.disciplines[4].sala = "PB12";
        Unicamp.disciplines[4].comentario = "O projeto 2 da disciplina já está disponível na plataforma. Usem os metodos de Newton e Gauss para convergir, e se quiser, aumentem a precisao dos pontos flutuantes para melhorar o resultado.";

        Unicamp.disciplines[5].codigo = "F 408";
        Unicamp.disciplines[5].nome = "Processos quanticos de radioatividade";
        Unicamp.disciplines[5].ementa = "Disciplina não oferecida pelo professor";
        Unicamp.disciplines[5].professor = "David Dogbrekeit";
        Unicamp.disciplines[5].sala = "IF11";
        Unicamp.disciplines[5].comentario = "A proxima aula está cancelada devido ao alto índice de câncer dos alunos...";

        this.unicamp = Unicamp;
    }

//    Pega os códigos e nomes de todas as disciplinas do banco de dados.
//    Return: String  contendo todos os codigos e nomes das disciplinas.
    private String getCodigosNomesDisciplinas(){
        String codigos = "";

        for(int i = 0; i < this.unicamp.size; i++) {
            codigos += this.unicamp.disciplines[i].codigo + ": " + this.unicamp.disciplines[i].nome + "\n";
        }

        return codigos;
    }

//    Extrai o código de uma disciplina da mensagem.
//    Param: String contendo mensagem a ter seu código extraido.
//    Return: String com código extraído da mensagem.
    private String getCodeFromRequest(String message) {
        return message.substring(2, 7);
    }

//    Pega a ementa de uma disciplina dado um codigo
//    Param: String  com o codigo da disciplina
//    Return: String com a ementa do codigo passado, ou uma string de erro caso não encontrado.
    private String getEmentaFromCodigo(String code) {
//        Procura por disciplina com codigo
        for(int i = 0; i < this.unicamp.size; i++) {
            if(code.equals(this.unicamp.disciplines[i].codigo)) {
                return this.unicamp.disciplines[i].ementa;
            }
        }
//        Não foi encontrada
        return "Não foi encontrada nenhuma disciplina com o codigo especificado.\nVerifique o codigo que foi passado.";
    }

//    Pega o comentário da próxima aula de de uma disciplina dado um codigo
//    Param: String  com o codigo da disciplina
//    Return: String com a ementa do codigo passado, ou uma string de erro caso não encontrado.
    private String getComentarioFromCodigo(String code) {
        for(int i = 0; i < this.unicamp.size; i++) {
            if(code.equals(this.unicamp.disciplines[i].codigo)) {
                return this.unicamp.disciplines[i].comentario;
            }
        }
//        Não foi encontrada
        return "Não foi encontrada nenhuma disciplina com o codigo especificado.\nVerifique o codigo que foi passado.";

    }

//    Pega todas as informações de uma disciplina, dado um codigo.
//    Param: String com codigo da disciplina.
//    Return: String com toda descrição da disciplina, ou uma String de erro caso não encontrado o código.

    private String getInformacaoDisciplinaFromCodigo(String code) {
        String info_disciplina = "Não foi encontrada nenhuma disciplina com o codigo especificado.\nVerifique o codigo que foi passado.";

//        Procura por disciplina com o codigo desejado e imprime toda a informação caso encontrada.
        for(int i = 0; i < this.unicamp.size; i++) {
            if(code.equals(this.unicamp.disciplines[i].codigo)) {
                info_disciplina = this.unicamp.disciplines[i].codigo + ": ";
                info_disciplina += this.unicamp.disciplines[i].nome;
                info_disciplina += "\nProfessor(a): " + this.unicamp.disciplines[i].professor;
                info_disciplina += "\nSala: " + this.unicamp.disciplines[i].sala;
                info_disciplina += "\nEmenta: " + this.unicamp.disciplines[i].ementa;
                info_disciplina += "\nComentario: " + this.unicamp.disciplines[i].comentario;
            }
        }

        return info_disciplina;
    }

//    Pega todas informações de todas as disciplinas.
//    Return: String com toda a descrição de todas as disciplinas.
    private String getAllInfo() {
        String info_disciplina = "";

        for(int i = 0; i < this.unicamp.size; i++) {
            info_disciplina = this.unicamp.disciplines[i].codigo + ": ";
            info_disciplina += this.unicamp.disciplines[i].nome;
            info_disciplina += "\nProfessor(a): " + this.unicamp.disciplines[i].professor;
            info_disciplina += "\nSala: " + this.unicamp.disciplines[i].sala;
            info_disciplina += "\nEmenta: " + this.unicamp.disciplines[i].ementa;
            info_disciplina += "\nComentario: " + this.unicamp.disciplines[i].comentario;
        }

        return info_disciplina;
    }

//    Extrai o comentário da mensagem
//    Param: String contendo a mensagem.
//    Return: String contendo comentário da mensagem.
    private String getCommentFromRequest(String message) {
        return message.substring(8);
    }

//    Define um comentário da próxima aula de uma disciplina, dado seu código.
//    Param: String de código da disciplina, String de comentário a ser definido.
//    Return: Boolean True, caso código de disciplina foi encontrado, false caso contrário.
    private boolean setComentarioForCode(String code, String comment) {

        for(int i = 0; i < this.unicamp.size; i++) {
            if (code.equals(this.unicamp.disciplines[i].codigo)) {
                this.unicamp.disciplines[i].comentario = comment;
                return true;
            }
        }
        return false;
    }
}