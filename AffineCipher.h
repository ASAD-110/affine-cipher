/*Save space in the .c file, implicit declarations otherwise*/
void encrypt(char* cipher, char* text, int a, int b);
void decrypt(char* cipher, char* newText, int a, int b);
int modInverse(int a, int m); 
int negativeModulo(int a);
void displayStr(char text[20][200], int limit);
void chFrequency(char* str, int* counts, int choice);
void plot(char text[20][200], char cipher[20][200]);