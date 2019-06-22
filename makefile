CC = gcc
OBJ = AffineCipher.o EligibleKey.o
EXEC = ac

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

AffineCipher.o : AffineCipher.c AffineCipher.h
	$(CC) -c AffineCipher.c

EligibleKey.o : EligibleKey.c EligibleKey.h
	$(CC) -c EligibleKey.c

clean:
	rm -f $(EXEC) $(OBJ)