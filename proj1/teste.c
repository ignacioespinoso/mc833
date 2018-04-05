// Codigo de teste para o arquivo Data.c
/*  Giovani Nascimento Pereira - 168609
	Ignacio Espinoso Ribeiro - 

	MC833 - 2S2018
	Unicamp
*/

#include"Data.c"

int main (){

	printf("-------- Carregando Disciplinas\n");
	testIfLoaded();
	for (int i=0; i<numeroDisciplinas; i++){
		printf("%s %s %s\n", disciplinas[i].codigo, disciplinas[i].ementa, disciplinas[i].professor);
	}

	printf("\n-------- Pegando Codigos\n");
	char **codigos = NULL;
	codigos = getCodigosDisciplinas();
	int numero = getQuantidadeDisciplinas();
	for (int i=0; i<numero; i++){
		printf("%s\n", codigos[i]);
	}

	printf("\n-------- Pegando Codigos & Nomes\n");
	char **codigosNomes = NULL;
	codigosNomes = getCodigosNomesDisciplinas();
	for (int i=0; i<numero; i++){
		printf("%s\n", codigosNomes[i]);
	}

	printf("\n-------- Procurando Ementas\n");
	printf("MC102:\n%s\n", getEmentaFromCodigo("MC102"));
	printf("ME322:\n%s\n", getEmentaFromCodigo("ME322"));
	printf("MA444:\n%s\n", getEmentaFromCodigo("MA444"));

	printf("\n-------- Procurando Comentarios de Aula\n");
	printf("MC102:\n%s\n", getComentarioFromCodigo("MC102"));
	printf("ME322:\n%s\n", getComentarioFromCodigo("ME322"));
	printf("MA444:\n%s\n", getComentarioFromCodigo("MA444"));

	printf("\n-------- Pegando Informacao total Disciplina\n");
	printf("%s\n", getInformacaoDisciplinaFromCodigo("MC102"));

	printf("\n-------- Pegando TODA Informacao\n");
	printf("%s\n", getAllInfo());

	return 0;

}
