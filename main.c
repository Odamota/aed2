/*
Engineered by: Artur Mendes
UC: AED II
To: Alberto Simões & Ricardo Gonçalves
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


typedef struct List {
		char *nome;
		char *rootword;
		char *analysis;
		float accuraty;
		int Wordsize;
		struct List *next;
} WordsList;

typedef struct anotherList {
	char *analysis;
	float relativeFrequency;
	int counter_absolute;
	float facumulada;
	struct anotherList *next;
} ThirdRowWords;

typedef struct list3 {
		int size;
		int fAbsolute;
		float fRelative;
		float fAcumulada;
		struct list3 *next;
} WordSizeList;

char filename[20];

#define MALLOC(t)    (t*) malloc(sizeof(t))

float auxForCumulated=0;
float sizesCumulated = 0;

void listWords(WordsList *lst)
{
	if(!lst)
	{
		printf("\nA lista de palavras não contém palavras.\n");
	}
	else
	{
		printf("\n\t  Original  \t  Raíz  \t  Categoria  \t  Certeza  \t Tamanho Palavra \t\n");
		WordsList * aux;
		for(aux = lst; aux != NULL; aux = aux->next)
		{
			printf("\n\t  %s  \t  %s  \t  %s  \t  %f  \t\n", aux->nome, aux->rootword, aux->analysis, aux->accuraty);
		
		}
	}


}


/* 
	Função de Listar valores da Lista de Frequências
	- Assumo que caso não haja lista é pq não foi calculada ainda (utilizador não selecionou opção)
	- Apresenta c/ Tabelização
*/
void list_frequencies(ThirdRowWords* fList)
{
	if(!fList){
		printf("\nA lista c/ frequências não foi calculada.\n\n");

	}
	else
	{
		ThirdRowWords *aux;
		printf("\n\t Categoria \t F. Absoluta \t F. Relativa \t F. Acumulada \t\n");
		for( aux = fList; aux; aux = aux->next) {
			printf("\n\t %s \t %d \t %f \t %f \t ", aux->analysis, aux->counter_absolute, aux->relativeFrequency, aux->facumulada);
		}
	}
}


/*
	Inserção de Palavras na Lista
	- Abordagem Iterativa
	- Percorre a lista e adiciona à Cauda
	Chamada pela: useWordsList
 */
WordsList * insertInWordList( WordsList* lst, char original[15], char root[15], char analise[5], float accuraty)
{
	WordsList * aux, *new_cell;
	new_cell = (WordsList*) malloc(sizeof(WordsList));
	new_cell->nome = strdup(original);
	new_cell->rootword =strdup(root);
	new_cell->analysis = strdup(analise);
	new_cell->accuraty = accuraty;
	new_cell->Wordsize = strlen(original);
	new_cell->next = NULL;

	if(!lst)
	{
		lst = new_cell;
	}
	else
	{
		for(aux = lst; aux->next; aux = aux->next)
		{}
		aux->next = new_cell;
	}

	return lst;
}

/* Get Size of List */
int getSizeOfList(ThirdRowWords* lst)
{
	int size = 0;
	ThirdRowWords * aux;
    for(aux = lst; aux; aux = aux->next)
	{
		size = size + aux->counter_absolute;
	}

    // count is 1 + count of remaining list
    return size;
}

/* Get Size of list das wordsizes / f.Relativas */
int getWordListSize ( WordSizeList * lst)
{
	int total = 0;
	WordSizeList * myaux;
	for(myaux = lst; myaux; myaux=myaux->next)
	{
		total = total + myaux->fAbsolute;
	}

	return total;
}


/*
	Calcular Frequência Relativa de cada palavra na lista de Palavras
*/
void CalcRelativeFrequencies( ThirdRowWords *FrequencyList, int size)
{
	printf("Calculando Frequências Relativas\n");
	ThirdRowWords *aux;
	//sum = 0;
	for(aux = FrequencyList; aux; aux = aux->next)
	{
		aux->relativeFrequency = (float) aux->counter_absolute / size;
		aux->facumulada = auxForCumulated + aux->relativeFrequency;
		auxForCumulated = aux->facumulada;
	}

}

/* 
	Função de Associar uma categoria à lista Ligada de Categorias
    - Se não existir lista adiciona à cabeça
	- Se existir lista
 	- Validar se palavra já existe
   	- Se existe aumenta a Freq. Absoluta
	-Se Não existe adiciona à lsita.
*/
ThirdRowWords * InsertIntoFrequencyList( ThirdRowWords * list, char *categoria )
{
		ThirdRowWords * aux, *new;
		new = (ThirdRowWords*) malloc(sizeof(ThirdRowWords));
		new->analysis = strdup(categoria);
		new->relativeFrequency = 1;
		//new->facumulada = 1;
		new->counter_absolute = 1;
		new->next = NULL;
	if(!list)
	{
		
		list = new;
	}else {
			int exists = 0;
			if(strcmp(list->analysis, categoria) == 0 )
			{
				//printf("\nEncontra %s c/ %s\n", list->analysis, categoria);
				exists = 1;
				list->counter_absolute++;

			}
			for(aux = list; aux->next; aux = aux->next) {
				
				//tratar os dados de cada uma das células
				//printf("\n Processa %s  c/  %s ", aux->analysis , categoria);
				if(strcmp(aux->analysis, categoria) == 0 )
			 	{
					 exists = 1;
				//printf("\n %s c/ F.Absoluta de %s", categoria, aux->analysis);
					aux->relativeFrequency++;
					
			 	}
			}
			//adicionar na última célula
			if(exists == 0)
			{
				aux->next = new;
			}
	}
	return list;

}

/* 
	Função q vai à 3ª Coluna e leva essa categoria para a lista de Frequências

	Resumo:
	 Percorre Mainlist 
	  - Guarda Categoria e adiciona à lista de Frequencia de forma síncrona
*/
ThirdRowWords * ThirdColFrequencies( WordsList *mainList, ThirdRowWords * FrequencyList)
{
	//printf("\nGot Here");
	
	WordsList *auxWords;
	char *category;
	for(auxWords = mainList; auxWords != NULL; auxWords = auxWords->next) {
		category = auxWords->analysis;
		//printf("Encontra : %s", category);
		
		FrequencyList = InsertIntoFrequencyList(FrequencyList, category);
	}



	return FrequencyList;
}

/* 
Função da opção 1
	- Acede ao ficheiro words.txt
	- Separa as palavras por espaços
	- Insere na lista de palavras (função: insertInWordList)
*/
WordsList*  usewordsList( WordsList* lista)
{
	//não esquecer de limpar lista


	FILE* fh = fopen(filename, "r");
	//FILE* fh = fopen("words_Original.txt", "r");

	if(!fh) {
		printf("\nOcorreu um erro ao abrir o ficheiro ");
	}
	else {
		int linecounter = 0;
		while(!feof(fh))
		{
			char linha[100];
			char *aux;

			fgets(linha, 100, fh);
			//linecounter++;
			//printf("Linhha %d : %s\n",linecounter, linha);
			char *token;
			aux = linha;
			int spaceCounter = 0;
			char rootword[15], word[15], analysis[5];
			float accuraty;
			if(!feof(fh))
			{
				//token serve p/ encontrar o espaço
				token = strtok(aux, " ");
				while( token != NULL  )
				{
					spaceCounter++;
					//printf("\nX Espaço %i na palavra %s - Token: %s \n", spaceCounter, aux, token);
					switch(spaceCounter)
					{
						case 1:

								strcpy(word, token);
							break;
						case 2:
								strcpy(rootword, token);
							break;
						case 3:
								strcpy(analysis, token);

							break;
						case 4:
								accuraty = atof(token);
							//atof converte p/ double
							break;
					}
					token = strtok(NULL, " ");
				}
					//printf("\n\nToken -> %s - root: %s - analise:%s Accuraty:%f \n\n", word, rootword, analysis, accuraty);
			}
			//adiciona às lista de palavras
			if(word != "!" && word != "." && word != "?" && word != "," && !feof(fh) )
			{
				lista  = insertInWordList( lista, word, rootword, analysis, accuraty);
			}
		}
	}

	return lista;
}

/* 
Devolve o nome do ficheiro para a função aceder
*/
void SelectFile(void)
{
	printf("Introduza o nome do ficheiro que pretende usar:");
	scanf("%s", filename);

}

int menu(void)
{
    int option;
	printf("\n1. Listar as palavras do ficheiro %s \n", filename);
	//printf("\n1a.Abrir outro ficheiro (.txt) \n");
	printf("2. Frequências das Categorias Gramaticais na lista (Crescente Abs.)\n");
    printf("3. Frequências de tamanhos das palavras do ficheiro\n");
    printf("4. Média e Desvio Padrão de Certezas \n");
    printf("5. Média, moda, mediana e desvio padrão relativo ao tamanho das palavras originais\n");
    printf("6. Calcular Quartis de frequências de palavras\n");
	printf("7. Calcular Histograma de Probabilidades\n");
    printf("0. Sair\n");
/*
    printf("\n1.Abrir o ficheiro wordslist.txt \n");
	printf("2.Apresentar lista de palavras\n");
    printf("3.Tabela de Frequências da 3ª coluna do ficheiro\n");
    printf("4.Frequências de tamanhos das palavras no texto\n");
    printf("5.Média e Desvio padrão c/ base no nº de caracteres das palavras originais\n");
    printf("6.Média, moda, mediana e desvio padrão relativo ao tamanho das palavras originais\n");
    printf("0.Sair\n");

*/
    printf("Escolha uma opção: ");

    while( (scanf(" %d", &option) != 1)  || (option < 0) || (option > 5))
    {
      fflush(stdin); /* clear bad data from buffer */
      printf("Escolha uma opção válida.\n");
      printf("Opção: ");
    }
    //printf("\nSelecionou a opção %d\n", option);
    return option;
}

/* Função de Listar Valores da Lista */

void List_WordSizesFrequencies( WordSizeList *lst)
{
//	printf("Entra na listagem de %ld itens", strlen(lst));
	if(lst)
	{
		printf("\nListando...\n");

		printf("\n\t Tamanho \t F. Absoluta \t F.Relativa \t F.Acumulada \t\n");
		WordSizeList * aux;
		for(aux=lst; aux; aux = aux->next)
		{
			printf("\n\t %d \t %d \t %f \t %f \t\n", aux->size, aux->fAbsolute, aux->fRelative, aux->fAcumulada);
		}
	}
	else
	{
		printf("A lista não contém dados");
	}

}


/* 
	Função de Adicionar Tamanhos de palavras à lista de frequencias de tamanhos 

*/
WordSizeList * addtoWordsSizesFrequencyList( WordSizeList * lst, int WordSize)
{
	//printf("\nRecebeu palavra c/ tamanho %d\n", WordSize);

	//aloca espaço
	WordSizeList *aux;
	WordSizeList *tmp = (WordSizeList*) malloc(sizeof(WordSizeList));

	tmp->size = WordSize;
	tmp->fAbsolute = 1;
	tmp->fRelative = 1;
	tmp->fAcumulada = 1;
	tmp->next = NULL;

	if(!lst)
	{
		//printf("\nAdiciona ocorrência de palavra c/ %d caracteres", WordSize);
		lst  = tmp;
	}
	else{
		
		int added = 0; 
		
		for( aux=lst; aux->next; aux = aux->next )
		{
			//printf("\nProcura ocorrências c/ %d caracteres", WordSize);
				if(aux->size == WordSize)
				{
					//Já existe palavra c/ este tamanho
					aux->fAbsolute = aux->fAbsolute +1;
					added = 1;
				}
		}
		if(added==0)
		{
			aux->next  = tmp;
		}
	}
	
	return lst;

}

/* Função de atribuir os tamanhos das palavras à lista */
WordSizeList * getWordsSize(WordsList * MyWordList, WordSizeList * list){
	WordsList *aux;
	for(aux = MyWordList; aux; aux= aux->next)
	{
		//printf("\nEncontra palavra %s, c/ %d letras\n\n",aux->nome, aux->Wordsize);
		list = addtoWordsSizesFrequencyList(list, aux->Wordsize);
		//List_WordSizesFrequencies(list);
	}

	return list;

}

/*
Fechar Aplicação
*/
void quit()
{
    printf("Terminando...Adeus!");
    exit(0);
}

/* 
	Função que calcula a moda da lista
	E a Média
*/

void getMedidasofList(WordSizeList * lst )
{
	WordSizeList *aux;
	int soma = 0;
	float media;
	int total=0;
	int moda = 0;
	int maior = 0;
	
	for(aux = lst; aux; aux= aux->next)
	{
		if(aux->size > maior)
		{
			moda = aux->size;
		}
		total = total + 1;
		soma = soma + (aux->fAbsolute * aux->size ) ;

	}

	media = soma / total;

	if(moda = 0)
	{
		printf("Não calculou corretamente a moda\n");
	}
	else
	{
		// calcula desvio padrão
		float dp;
		dp = sqrt((total*total) / 2 );
		printf("\n\nO desvio padrão é de %f", dp );
		printf("\nA moda é de %d\n", moda);
		printf("\nA média é de %f caracteres por palavra\n", media );

	}
}

void WordSizesRelativeFrequencies( WordSizeList * lst, int size )
{
	printf("\nCalculando Frequências de tamanhos das palavras... \n");

	WordSizeList *aux;

	for(aux = lst; aux; aux= aux->next)
	{
		aux->fRelative = (float) aux->fAbsolute / size;
		aux->fAcumulada = sizesCumulated + aux->fRelative;
		sizesCumulated = aux->fAcumulada;
	}

}



int main(void)
{
	WordsList* MyWordList = NULL;
	ThirdRowWords*  FrequencyList = NULL;
	WordSizeList * WordSizeFrequencies = NULL;
	int choice = 899, size;
	
	printf("\n Bem-Vindo à sua Ferramenta de interpretação de Palavras\n");
	SelectFile();

	printf("\nImportando palavras de %s...\n",filename);
	MyWordList = usewordsList (MyWordList);
	FrequencyList = ThirdColFrequencies(MyWordList,FrequencyList);
	size = getSizeOfList(FrequencyList);
	//printf("\nA lista tem %i categorias.", size);
	//Depois de ter a lista c/ Frequências Absolutas vai calcular Frequência Relativa p/ cada uma das categorias
	CalcRelativeFrequencies(FrequencyList, size);
	WordSizeFrequencies = getWordsSize(MyWordList, WordSizeFrequencies);
	size = getWordListSize(WordSizeFrequencies);
	WordSizesRelativeFrequencies( WordSizeFrequencies, size);
	do {
		choice = menu();

		switch (choice) {
			case 0:
				quit();
				break;
			case 1:
				listWords(MyWordList);
				break;
			case 2:
				//CalcCumulatedFrequencies(FrequencyList,);
				printf("\nOcorrências de Categorias das Palavras no Texto:\n");
				//printf("\nCategoria \t F. Absoluta \t F. Relativa \t F. Acumulada \t\n");
				list_frequencies(FrequencyList);
				break;
			case 3:
				printf("\nOcorrências de Tamanhos das Palavras (Originais) no Text \n");
				List_WordSizesFrequencies(WordSizeFrequencies);
				//Criar Função q pega nos valores e calcula o desvio padrão
				// A F. Relativa aCumulada é igual ao cumulatedForWordSizes + f_relativa 
				//Falta calcular e apresentar o desvio padrão
				break;
				break;
			case 4:
				/*
					Calcular Média, Moda e Mediana dos Tamanhos das Palavras

				*/ 
				getMedidasofList(WordSizeFrequencies);
				/* terminar Função da mediana
					WordsizeFrequencies deve estar ordenada!
					Bem como FrequencyList
					
					Devia garantir que a lista fica criada de forma ordenada...
				 */
				//getMedianaofList(WordSizeFrequencies);
				//getMediana(WordSizeFrequencies);
				//Opção 5
			case 5:
				
				

			break;
			case 6:
			 
			 break;

			case 7:

				break;
		

		}

	}while(choice != 6);

}
