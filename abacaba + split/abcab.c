#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define k 6


void GenerateString(int n, char * string);

int main() 
{
	char* string;
	string = malloc(sizeof(char) * (1 << (k + 1)));
	string[0] = 'a';
	GenerateString(k, string);
	free(string);
	return 0;
}

void GenerateString(int n, char * string)
{
	int i;
	char * temp;
	temp =  malloc(sizeof(char) * (1 << (k + 1)));
	for (i = 1; i < n + 1; i++)
	{
		printf("%d. %s \n", i, string);
		strcpy( temp, string);
		string[strlen(string)] = 'a' + i;
		string[strlen(string) + 1] = 0;
		strcat(string, temp);
	}
	free(temp);	
}
