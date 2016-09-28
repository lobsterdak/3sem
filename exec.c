#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

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
    char str[50];
    int NUMBER_OF_STRING, i, j;
    FILE *file;
    int tokensCount = 0;
    char delimiters[3] = {' ','\n', '\0'};
    file = fopen("my_command.txt", "w");
    fprintf(file, "3\n");
    fprintf(file, "1 pwd \n");
    fprintf(file, "2 echo Hello, world!! \n");
    fprintf(file, "3 ls -l \n");
    fclose(file);
    file = fopen("my_command.txt", "r");
    fgets(str, 20, file);
    NUMBER_OF_STRING = atoi(str);
    char **tokens = (char**)malloc(sizeof(char*) * 20);
    for (i = 0; i < 20 ; i++)
    {
        tokens[i] = (char*)malloc(sizeof(char) * 20);
    }
    
    for (i = 0; i < NUMBER_OF_STRING + 1; i++)
    {
        fgets(str, 20, file);
        
        for ( j = 0; j < 20; j++)
        {
            tokens[j] = (char*)malloc(sizeof(char) * 20);  
        }
        Split(str, delimiters, &tokens, &tokensCount);
        for ( j = tokensCount; j < 20; j++)
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
    
	for (i = 0; i < 20; i++)
	{
			free(tokens[i]);         
	}								
	free(tokens);
    return 0; 
}
