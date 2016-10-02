#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define MAX_LENGTH_OF_STRING 20

void Split(char* string, char * delimiters, char*** tokens, int* tokensCount){
	char* pointer = NULL; 
	pointer = strtok(string, delimiters);
	*tokensCount = 0;
    if (pointer == NULL) printf("ERROR\n");
	while (pointer != NULL) 
	{
		strcpy( (*tokens)[0 + *(tokensCount)], pointer);
		(*tokensCount)++;
 		pointer = strtok( NULL, delimiters); 
	}
}

int main () 
{
    char str[MAX_LENGTH_OF_STRING];
    int NUMBER_OF_STRING, i, j;
    FILE *file;
    int tokensCount = 0;
    char delimiters[] = {' ','\n', '\0'};
    
    file = fopen("my_command.txt", "r");
    
    fgets(str, MAX_LENGTH_OF_STRING, file);
    NUMBER_OF_STRING = atoi(str);
    char **tokens = (char**)malloc(sizeof(char*) * MAX_LENGTH_OF_STRING);
    for (i = 0; i < MAX_LENGTH_OF_STRING ; i++)
    {
        tokens[i] = (char*)malloc(sizeof(char) * MAX_LENGTH_OF_STRING);
    }
    
    for (i = 0; i < NUMBER_OF_STRING + 1; i++)
    {
        fgets(str, MAX_LENGTH_OF_STRING, file);
        
        for (j = 0; j < MAX_LENGTH_OF_STRING; j++)
        {
            tokens[j] = (char*)malloc(sizeof(char) * MAX_LENGTH_OF_STRING);  
        }
        Split(str, delimiters, &tokens, &tokensCount);
        for (j = tokensCount; j < MAX_LENGTH_OF_STRING; j++)
        {
            tokens[j] = NULL;
        }
        int pid = fork();
        if (pid == 0)
        {
            sleep(atoi(tokens[0]));
            execvp(tokens[1], tokens + 1);
            exit(0);
        }
    }
    
	for (i = 0; i < MAX_LENGTH_OF_STRING; i++)
	{
			free(tokens[i]);         
	}
	free(tokens);
    return 0; 
}
