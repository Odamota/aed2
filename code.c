/* 
UC: AED II
EX: Trabalho Prático 
By: Artur Mendes

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
typedef struct List {
 
Aguardando mail do prof. Simões p/ decidir como proceder a seguir.
Acho que a lista está a guardar informação repetida
	
}
*/

typedef struct List {
		char *nome;
		char *rootword;
		char *analysis;
		double accuraty;
		int wordcounter;
		struct List *next;
} WordsList;

int wordcounter = 0;
	
void list_words( WordsList *lst)
{	
	printf("\nContador\tPalavra\tRootword\tAnálise\tAccuraty\n\n");
    if (lst) {
    	wordcounter++;
    	
        printf("%d\t%s\t%s\t%s\t%f\n", wordcounter, lst->nome, lst->rootword, lst->analysis, lst->accuraty);
        list_words(lst->next);
    }

//	return 0;
}

WordsList* addtoList( WordsList *lst, char original[15], char root[15], char analysis[15], double sure)
{
	//caso esteja vazia
	//adiciona na cabeça da lista
	if(!lst)
	{
		WordsList *new = (WordsList*) malloc(sizeof(WordsList));
		new->nome = strdup(original);
		new->rootword = strdup(root);
		new->analysis = strdup(analysis);
		new->accuraty = sure;
		new->next = lst;
		lst = new;

	}
	else
	{
		lst->next = addtoList(lst->next, original, root, analysis, sure);
	}
	return lst;

	/*
	printf("Adicionando palavras À lista\n Serão adicionadas as seguintes palavras:\n");
	printf("Palavra Original: %s\n", &original );
	printf("Palavra raíz: %s\n", rootword );
	printf("Análise: %s\n", analysis );
	printf("Accuraty: %d\n", accuraty );
	*/

}


int menu(void)
{
    int option;
 
    printf("1.\n\n\tAbrir o ficheiro wordslist.txt \n");
	printf("2.\tApresentar lista de palavras\n");
    printf("3.\tFrequências Absolutas do ficheiro wordslist.txt\n");
    printf("4.\tFrequências de palavras no texto\n");
    printf("5.\tMédia e Desvio padrão das palavras originais\n\n");
    printf("6.\tMédia, moda, mediana e desvio padrão relativo ao tamanho das palavras originais\n\n");
    printf("0.\tSair\n\n");
    printf("Escolha uma opção: ");
 
    while( (scanf(" %d", &option) != 1)  || (option < 0) || (option > 5))   
    {
      fflush(stdin);                    /* clear bad data from buffer */
      printf("Escolha uma opção válida.\n\n");
      printf("Opção? ");
    }
    printf("\nSelecionou a opção %d\n", option);
    return option;
}
 
void quit()
{
    printf("Terminando...Adeus!");
    exit(0);
}

WordsList* usewordslist( WordsList* Lista)
{
	printf("\nEscolheu aceder ao ficheiro Words.txt.\n");


	FILE* fh = fopen("words.txt", "r");
	if(!fh) { printf("Erro ao abrir o ficheiro!"); exit(1);}

	while(!feof(fh))
	{
		char linha[100];
		char *aux;
		fgets(linha, 100, fh);
     	char *token;

		if(!feof(fh))
		{
			//find space
			aux = linha;
			int spaceCounter = 0;
			char rootword[15];
			char word[15];
			char analysis[15];
			double accuraty;
			
			 token = strtok(aux, " ");
				   while( token != NULL ) 
				   {
				  	  	spaceCounter++;

				    	switch(spaceCounter)
				    	{
					    	case 1:
					    		//printf( "Encontrou o %i  espaço %s\n", spaceCounter, token );
					    		//word = strcpy(token);
					    		strcpy(word, token);
					    		break;
					    	case 2:
					    		strcpy(rootword, token);
					    		//rootword = strcpy(token);
					    		break;
					    	case 3:
					    		strcpy(analysis, token);
					    		//analysis = strcpy(token);
						    	break;
					    	case 4:
					    		accuraty = atof(token);
								//accuraty = atof(token);
								//atof converte p/ double				    	
						    	break;

				    	}	
					    //Adiciona à lista se palavra não for um sinal ortográfico
					    if(word != "!" && word != "." && word != "?"){
						    Lista = addtoList( Lista, word, rootword, analysis, accuraty);
					    }

				      token = strtok(NULL, " ");
				   }
	   }
	}	

	printf("\nTerminou a interpretação do ficheiro word.");
	//menu();

	list_words(Lista);	
}

int main (void) {

    printf("Trabalho Prático de AED II [Av. Conjunta c/ Estatística]\n\n");

	//opcão de selecionar o ficheiro que quer utilizar
	int choice = menu();
	
	WordsList* Mywordlist = NULL;

	switch(choice)
	{
		case 1:
		 Mywordlist = usewordslist( Mywordlist);
		 break;

	 	case 0: 
		 quit();
		 break;
		case 2:
			list_words(Mywordlist);
			//another function;
		break;
		case 3:

		 	//Another one
		break;
		case 4:
			//another one
		break;
		case 5: 
			//another one
		break;

	}
	//choosefile();
	
	return 0;
}