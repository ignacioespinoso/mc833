package proj3;

public class College {
    public Discipline[] disciplines;
    public int size;

    public College(int size){
        this.disciplines = new Discipline[size];
        this.size = size;
    }

    public College CreateUnicamp() {
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

        return Unicamp;
    }

    public void TestIfUnicampCreated(){
        if(this.size == 0) {
            this = CreateUnicamp();
        }
    }
}
