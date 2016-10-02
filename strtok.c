#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 50
#define MAX_DELIMETERS_COUNT 50

/*
 * У вас использование объявленных выше констант неверное.
 * 
 * Не хватает ещё MAX_TOKENS_COUNT
 * 
  char** tokens = (char**)malloc(sizeof(char*) * MAX_TOKENS_COUNT);
  for (i = 0; i < MAX_TOKENS_COUNT; i++)
  {
    tokens[i] = (char*)malloc(sizeof(char) * MAX_STRING_SIZE);
  }
 */
 
void Split(char* string, char * delimiters, char*** tokens, int* tokensCount);

int main () 
{	
	int i, tokensCount = 0;
	char string[MAX_STRING_SIZE], delimiters[MAX_DELIMETERS_COUNT];
	char** tokens = (char**)malloc(sizeof(char*) * MAX_STRING_SIZE); // количество слов 50, длина слова 10 символоd
  
  for (i = 0; i < MAX_STRING_SIZE; i++)
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
	for (i = 0; i < MAX_STRING_SIZE; i++)
	{
			free(tokens[i]);         
	}
	free(tokens); 
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
