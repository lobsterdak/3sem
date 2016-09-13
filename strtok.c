#include <stdio.h>
#include <string.h>
#include <malloc.h>

void Split(char* string, char * delimiters, char*** tokens, int* tokensCount);

int main () 
{	
	int i, tokensCount;
	char string[50], delimiters[50];
	char** tokens = (char**)malloc(sizeof(char) * 50); // количество слов 50, длина слова 10 символов
	for (i = 0; i < 50; i++)
	{
		tokens[i] = (char*)malloc(sizeof(char) * 10);
	}
	printf("Введите строку\n");
	gets(string);
	printf("Введите строку с разделителями\n");
	gets(delimiters);
	Split(string, delimiters, &tokens, &tokensCount);
	for (i = 0; i < tokensCount; i++)
	{
		printf("\n%s",tokens[i]);
	}
	/* for (i = 0; i < 50; i++)
	{
		free(tokens[i]);            если не комментить этот кусок кода, то возникает ошибка
	}								пишет, что какие-то ячейки памяти освобождаются дважды, пофиксить то я не смог.
	free(tokens); */
	return 0;
}

void Split(char* string, char * delimiters, char*** tokens, int* tokensCount)
{
	char* pointer; 
	pointer = strtok(string, delimiters);
	(*tokensCount) = 0; 
	while (pointer != NULL) 
	{
		//printf("%s\n", pointer);
		strcpy( (*tokens)[0 + *(tokensCount)], pointer);
		(*tokensCount)++;
 		pointer = strtok( NULL, delimiters); 
	}
}
