/***************************
* FILE: AffineCipher.c     *
* AUTHOR: Thomas Di Pietro *
* DATE: 17/04/2019         *
*                          *
* PURPOSE: Demonstrates    *
* the use of the affine    *
* cipher                   *      
***************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AffineCipher.h"
#include "EligibleKey.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Error: command-line parameters wrong.\n");
		printf("       e.g. ./ac filename\n");
	}
	else
	{
	    FILE* input = fopen(argv[1], "r");
		int a, b, i, j = 0;
		char choice;
		char cipher[500]; /*buffer of 500 characters*/
		char newText[500];
		char line[200]; /*line buffer*/

		char textEn[20][200];
		char cipherEn[20][200];

		char textDe[20][200];

	    if (input == NULL) /*if file doesn't exist*/
	    {
	        perror("Error opening file");
	    }
	    else
	    {
			printf("\nASSIGNMENT 1 - FCC - THOMAS DI PIETRO\n");
			printf("AFFINE CIPHER:\n");

			printf("Enter your 'a' value (1-26): ");
			scanf("%d", &a);
			
			while((validA(a, 26) != 1) || a < 1 || a > 26)
			{
				printf("'a' value must be co-prime to 26\n");
				printf("Re-enter your 'a' value: ");
				scanf("%d", &a);		
			}

			printf("Enter your 'b' value (<=25): ");
			scanf("%d", &b);

			while(b > 25 || b < 0)
			{
				printf("'b' value must be between 1-26...\n");
				printf("Re-enter your 'b' value: ");
				scanf("%d", &b);			
			}
	        while(fgets(line, 200, input) != NULL)
	        {
	        	/*This loop encrypts line by line 
	        	and then adds to a 2d string for final printf*/
				encrypt(cipher, line, a, b);

				/*add to full arrays*/
				for (i = 0; i < strlen(line); i++)
				{
					textEn[j][i] = line[i];
					cipherEn[j][i] = cipher[i];				
				}

				decrypt(cipher, newText, a, b);

				for (i = 0; i < strlen(line); i++)
				{
					textDe[j][i] = newText[i];
				}

				/*resets the arrays to nothing*/
				memset(cipher, 0, strlen(cipher));
				memset(newText, 0, strlen(newText));
				j++;
			}

			/*Display before and afters*/
			printf("\nENCRYPT:\n");
			printf("  PLAINTEXT:\n\n");
			displayStr(textEn, j);
			printf("\n\n  CIPHERTEXT (after encrypt):\n\n");
			displayStr(cipherEn, j);

			printf("\n\n\nDECRYPT:\n");
			printf("  CIPHERTEXT:\n\n");
			displayStr(cipherEn, j);
			printf("\n\n  PLAINTEXT (after decrypt):\n\n");
			displayStr(textDe, j);

			/*This was supposed to be a seperate function,
			however ran into some trouble passing the 2d char arrays...*/
			printf("\n\nPlot letter distributions? (y/n):> ");
			scanf(" %c", &choice);
			if (choice == 'y')
			{
				FILE* graphFile = fopen("graphFile.txt", "w+");
				FILE* plot = popen("gnuplot -persist", "w");
				int i;
				int lower[26]; /*counts of each a-z char*/
				int upper[26]; /*counts of each A-Z char*/
				char choice;

				/*Dont make your choice other than p or c... 
				I don't think validation on user input is required in
				this assignment so I didn't include it*/
				printf("Plot plain-text or cipher-text distributions? (p/c):> ");
				scanf(" %c", &choice);


				if (choice == 'p')
				{
					/*Fetch letter distributions*/
					for (i = 0; i < 20; i++)
					{
						chFrequency(textEn[i], upper, 1);
						chFrequency(textEn[i], lower, 0);
						fprintf(plot, "set title \"Plain-Text Letter Distribution\"\n");
					}		

				}
				else if (choice == 'c')
				{
					/*Fetch letter distributions*/
					for (i = 0; i < 20; i++)
					{
						chFrequency(cipherEn[i], upper, 1);
						chFrequency(cipherEn[i], lower, 0);					
					}	
					fprintf(plot, "set title \"Cipher-Text Letter Distribution\"\n");

				}

				/*Writes character counts to file, both lower and uppercase*/
				for (i = 0; i < 26; i++)
				{
					fprintf(graphFile, "%c \t %d\r\n", i + 'a', lower[i]);
				}

				for (i = 0; i < 26; i++)
				{
					fprintf(graphFile, "%c \t %d\r\n", i + 'A', upper[i]);
				}

				/*Gives commands to gnuplot process, and plots graphFile.txt*/
				fprintf(plot, "set style fill solid 0.5\n");
				fprintf(plot, "plot \'graphFile.txt\' using 2:xticlabels(1) with histogram\n");

				fclose(graphFile);
				fclose(plot);
			}
			else
			{
				printf("Program terminated.");
			}

		}
		if (ferror(input)) /*If file read errors*/
	    {
	        perror("Error reading from file");
	    }

	    fclose(input);
	}
	return 0;
}

void encrypt(char* cipher, char* text, int a, int b)
{
	int i;

	for (i = 0; i < strlen(text); i++)
	{
		/*if text uppercase, start from 'A' ascii value*/
		if (text[i] >= 65 && text[i] <= 90) 
		{
			cipher[i] = ((a * (text[i] - 'A') + b) % 26 + 'A');
		}/*if text lower, start from 'a' ascii value*/
		else if (text[i] >= 97 && text[i] <= 122)
		{
			cipher[i] = ((a * (text[i] - 'a') + b) % 26 + 'a');
		}
		else
		{
			cipher[i] = text[i];
		}
	}
}

void decrypt(char* cipher, char* newText, int a, int b)
{
	int i, invA, calc, length;
	
	/*using just strlen() in for loop didnt work for some reason*/
	length = strlen(cipher); 

	invA = modInverse(a, 26);
	for (i = 0; i < length; i++)
	{

		/*if uppercase, convert down to actual value from ascii*/
		if (cipher[i] >= 65 && cipher[i] <= 90)
		{
			calc = invA * (cipher[i] - 'A' - b);

			/*seperate negative mod func for C*/
			if (calc < 0)
			{
				calc = negativeModulo(calc);
			}
			else
			{
				calc = calc % 26;
			}
			newText[i] = calc + 'A';
		}/*if lower, convert down to actual value from ascii*/
		else if (cipher[i] >= 97 && cipher[i] <= 122)
		{
			calc = invA * (cipher[i] - 'a' - b);
			/*seperate negative mod func for C*/
			if (calc < 0)
			{
				calc = negativeModulo(calc);
			}
			else
			{
				calc = calc % 26;
			}
			newText[i] = calc + 'a';
		}
		else
		{ /*ignore other characters not a-z & A-Z*/
			newText[i] = cipher[i];
		}
	}
}
int modInverse(int a, int m) 
{ 
	int x;
    a = a%m;

    while ((a*x) % m != 1)
    {
    	x++;
    }
    return x; 
} 

/*Add 26 to number until reach over 26, then take 26*/
int negativeModulo(int a)
{
	while (a <= 26)
	{
		a = a + 26;
	}	
	return a - 26;
}

void displayStr(char text[20][200], int limit)
{
	int i;
	for (i = 0; i < limit; i++)
	{
		printf("  %s", text[i]);		
	}
}  

void chFrequency(char* str, int* counts, int choice) 
{
	int ch = 0;

	if (choice == 1) /*For uppercase letter counts*/
	{
		while (str[ch] != '\0') 
		{
			if (str[ch] >= 'A' && str[ch] <= 'Z')
			{
				counts[str[ch]-'A']++;     	
			}
			ch++;
		}
	}
	else if (choice == 0) /*For lowercase letter counts*/
	{
		while (str[ch] != '\0') 
		{
			if (str[ch] >= 'a' && str[ch] <= 'z')
			{
				counts[str[ch]-'a']++;
			}
			ch++;
		}		
	}
}