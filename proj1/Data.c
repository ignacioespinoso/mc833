// Arquivo de Dados do Servidor - informações das salas e td mais

/*  Giovani Nascimento Pereira - 168609
	Ignacio Espinoso Ribeiro - 

	MC833 - 2S2018
	Unicamp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dados inclusos
/*

- Códigos de Disciplinas
- Ementa das disciplinas
- Informações adicionais das disciplinas (professor, sala...)
- Texto Comentário sobre a aula

*/

//Creating boolean type
typedef int bool;
enum bool {false, true, end};

//Creating struct for Subject information
typedef struct ClassData{

	char codigo[10];
	char nome[100];
	char ementa[500];
	char professor[100];
	char sala[100];
	char comentario[500];

} Disciplina;

//Lista ligada de Disciplinas
Disciplina *disciplinas = NULL;
int numeroDisciplinas = 6;


/// Importar os dados na memoria - carregar base de dados basicamente
void loadClassData(){

	//Criando Disciplina
	Disciplina MA111;
	strcpy(MA111.codigo, "MA111");
	strcpy(MA111.nome, "Topicos de Calculo 1 para engenharias");
	strcpy(MA111.ementa, "Análise de limites. Derivadas totais. Integrais simples de uma variável. Integras 2D.");
	strcpy(MA111.professor, "Ketty Mello");
	strcpy(MA111.sala, "CB03");
	strcpy(MA111.comentario, "Na proxima aula vamos estudar o desenvolvimento do diferencial em 3 planos");

	Disciplina ME322;
	strcpy(ME322.codigo, "ME322");
	strcpy(ME322.nome, "Introducao a probabilidades");
	strcpy(ME322.ementa, "Variaveis aleatorias discretas e continuas. Distribuicoes comuns: Poisson, intermédio, binário, gauss...");
	strcpy(ME322.professor, "Anderson Bernardini");
	strcpy(ME322.sala, "PB08 / IM17");
	strcpy(ME322.comentario, "Para a proxima aula, ler os textos do capitulo 3 sobre variaveis aleatorias continuas. Vamos discutir em sala os efeitos dessas variaveis nas formulacoes de probabilidade.");

	Disciplina MC102;
	strcpy(MC102.codigo, "MC102");
	strcpy(MC102.nome, "Programacao e ordenacao de computadores");
	strcpy(MC102.ementa, "Programação básica para iniciantes - em C e Python.");
	strcpy(MC102.professor, "Zanoni Dias");
	strcpy(MC102.sala, "CB05 / CC353");
	strcpy(MC102.comentario, "Labs 3 e 4 disponiveis no SuSy. Boa semana a todos.");

	Disciplina EE532;
	strcpy(EE532.codigo, "EE532");
	strcpy(EE532.nome, "Eletronica Aplicada");
	strcpy(EE532.ementa, "Estudo de circuitos eletricos nao lineares com componentes semi condutores.");
	strcpy(EE532.professor, "Lee da FEEC");
	strcpy(EE532.sala, "FE02");
	strcpy(EE532.comentario, "Entrega de terça-feira disponível no Drive da turma.");

	Disciplina MS211;
	strcpy(MS211.codigo, "MS211");
	strcpy(MS211.nome, "Cálculo Numérico");
	strcpy(MS211.ementa, "Métodos e processos de zeros de funções, aproximações nominais, resolucao e derivadas e integrais.");
	strcpy(MS211.professor, "Maria Cheti Ehrlhardtz");
	strcpy(MS211.sala, "PB12");
	strcpy(MS211.comentario, "O projeto 2 da disciplina já está disponível na plataforma. Usem os metodos de Newton e Gauss para convergir, e se quiser, aumentem a precisao dos pontos flutuantes para melhorar o resultado.");

	Disciplina F408;
	strcpy(F408.codigo, "F 408");
	strcpy(F408.nome, "Processos quanticos de radioatividade");
	strcpy(F408.ementa, "(Disciplina não oferecida pelo professor)");
	strcpy(F408.professor, "David Dogbrekeit");
	strcpy(F408.sala, "IF11");
	strcpy(F408.comentario, "A proxima aula está cancelada devido ao alto índice de câncer dos alunos...");

	/// Adicionando ao vetor de disciplinas do arquivo
	disciplinas = malloc(sizeof(Disciplina) * numeroDisciplinas);
	disciplinas[0] = MA111;
	disciplinas[1] = ME322;
	disciplinas[2] = MC102;
	disciplinas[3] = EE532;
	disciplinas[4] = MS211;
	disciplinas[5] = F408;

}

/// Checa se os dados ja estao na memoria - 
/// senao, carrega os dados para serem utilizados
/// Usado como prevenção de erros
void testIfLoaded(){
	if (disciplinas == NULL){
		loadClassData();
	}
}

/// Pega a quantidade de disciplinas da base de dados
/// Return: a quantidade de disciplinas
int getQuantidadeDisciplinas(){
	return numeroDisciplinas;
}

/// Pega os docigos de todas as disciplinas da base de dados
/// Return: Retorna um pontiro para um array de Strings (char**)
///			(alocado dinamicamente) com todos os codigos
char** getCodigosDisciplinas(){

	testIfLoaded();

	char **codigos;
	// Alocando memoria para cada codigo de disciplina
	codigos = malloc(sizeof(char*) * numeroDisciplinas);
	for (int i=0; i < numeroDisciplinas; i++){
		codigos[i] = malloc(sizeof(char) * 10);
	}
	// Copiando os codigos de disciplina no parametro
	for (int i=0; i < numeroDisciplinas; i++){
		strcpy(codigos[i], disciplinas[i].codigo);
	}
	// retornando total de codigos adicionado
	return codigos;
}

/// Pega os codigos e nomes e de todas as disciplinas da base de dados
/// Return: Retorna um pontiro para um array de Strings (char**)
///			(alocado dinamicamente) com "codigo: Nome"
char** getCodigosNomesDisciplinas(){

	testIfLoaded();

	char **codigos;
	// Alocando memoria para cada codigo de disciplina
	codigos = malloc(sizeof(char*) * numeroDisciplinas);
	for (int i=0; i < numeroDisciplinas; i++){
		codigos[i] = malloc(sizeof(char) * 300);
	}
	// Copiando os codigos de disciplina no parametro
	for (int i=0; i < numeroDisciplinas; i++){
		strcpy(codigos[i], disciplinas[i].codigo);
		strcat(codigos[i], ": ");
		strcat(codigos[i], disciplinas[i].nome);
	}
	// retornando total de codigos adicionado
	return codigos;
}

/// Pegar a ementa de uma disciplina dado um codigo
/// Param:	String (char*) com o codigo da disciplina
/// Return: String (char*) com a ementa do codigo passado
///			String de erro caso não encontrado
char* getEmentaFromCodigo(char *codigo){

	testIfLoaded();

	// Procura por disciplina com esse codigo
	for (int i=0; i<numeroDisciplinas; i++){
		if (strcmp(codigo, disciplinas[i].codigo) == 0){
			return disciplinas[i].ementa;
		}
	}
	// Nao foi encontrada
	return "Não foi encontrada nenhuma disciplina com o codigo especificado.\nVerifique o codigo que foi passado.";
}

/// Pegar o comentario da proxima aula de uma disciplina dado um codigo
/// Param:	String (char*) com o codigo da disciplina
/// Return: String (char*) com o comentario do codigo passado
///			String de erro caso não encontrado
char* getComentarioFromCodigo(char *codigo){

	testIfLoaded();

	// Procura por disciplina com esse codigo
	for (int i=0; i<numeroDisciplinas; i++){
		if (strcmp(codigo, disciplinas[i].codigo) == 0){
			return disciplinas[i].comentario;
		}
	}
	// Nao foi encontrada
	return "Não foi encontrada nenhuma disciplina com o codigo especificado.\nVerifique o codigo que foi passado.";
}

/// Pegar todas as informações de uma disciplina por codigo
/// Param:	codigo - o codigo da disciplina que se quer as informcoes
/// Return:	String (char*) com toda a descrição da disciplina
char* getInformacaoDisciplinaFromCodigo(char* codigo){

	testIfLoaded();

	char *retorno;
	retorno = malloc(sizeof(char) * 1000);

	// Procura por disciplina com esse codigo
	for (int i=0; i<numeroDisciplinas; i++){
		if (strcmp(codigo, disciplinas[i].codigo) == 0){
			strcpy(retorno, disciplinas[i].codigo);
			strcat(retorno, ": ");
			strcat(retorno, disciplinas[i].nome);
			strcat(retorno, "\nProfessor(a): ");
			strcat(retorno, disciplinas[i].professor);
			strcat(retorno, "\nSala: ");
			strcat(retorno, disciplinas[i].sala);
			strcat(retorno, "\nEmenta: ");
			strcat(retorno, disciplinas[i].ementa);
			strcat(retorno, "\nComentário: ");
			strcat(retorno, disciplinas[i].comentario);

			return retorno;
		}
	}

	return "Não foi encontrada nenhuma disciplina com o codigo especificado.\nVerifique o codigo que foi passado.";
}

/// Pegar toda a informação da base de dados
/// Return: String (char*) com toda a informacao das disciplinas
char* getAllInfo(){

	char *info;
	info = malloc(sizeof(char) * 5000);
	info[0] = '\0';
	for (int i=0; i<numeroDisciplinas; i++){
		strcat(info, disciplinas[i].codigo);
		strcat(info, ": ");
		strcat(info, disciplinas[i].nome);
		strcat(info, "\nProfessor(a): ");
		strcat(info, disciplinas[i].professor);
		strcat(info, "\nSala: ");
		strcat(info, disciplinas[i].sala);
		strcat(info, "\nEmenta: ");
		strcat(info, disciplinas[i].ementa);
		strcat(info, "\nComentário: ");
		strcat(info, disciplinas[i].comentario);
		strcat(info, "\n\n");
	}
	return info;
}

/// Escrever um comentario sobre a proxima aula
/// Sobreescreve o comentario já existente
/// Param:	codigo - o codigo da disciplina a ser alterado
///			novoComentario - o novo comentario a ser adicionado
/// Return: bool - true: O codigo foi escrito com sucesso
///				 - false: O código não foi escrito (provavelmente codigo de disciplina invalido)
bool setComentarioForCode(char *codigo, char *novoComentario){

	testIfLoaded();

	// Procura por disciplina com esse codigo
	for (int i=0; i<numeroDisciplinas; i++){
		if (strcmp(codigo, disciplinas[i].codigo) == 0){
			// Sobreescreve o comentário da próxima aula
			strcpy(disciplinas[i].comentario, novoComentario);
			return true;
		}
	}
	// Nao foi encontrada
	return false;
}
