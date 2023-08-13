#include "libraries.h"

int main(int argc, char *argv[]){
    FILE* readDocuments;
    FILE* binary;
    char directory1[1000];
    sprintf(directory1,"%s", argv[2]);
    
    if(argc != 5){
        printf ("ERRO: O programa deve ser executado como o exemplo:\n");
        printf ("./prog3 binary.bin \"CaminhoParaPasta\"/test.txt \"numero K\" \"NomeArquivoDeSaida\".txt\n");
        exit(1);
    }
    readDocuments = fopen(directory1, "r");
    binary = fopen(argv[1], "rb");
    
    if(binary == NULL){
        printf ("ERRO: Não foi possível abrir o arquivo binário:\n");
        exit(1);
    }

    if(readDocuments == NULL){
        printf ("ERRO:Não foi possível abrir o arquivo .txt, o programa deve ser executado como o exemplo:\n");
        printf ("./prog3 binary.bin \"CaminhoParaPasta\"/test.txt \"numero K\" \"NomeArquivoDeSaida\".txt\n");
        exit(1);
    }
    
    tNewsLetter* TrainNewsletter;
    TrainNewsletter = ReadBinary(binary);
    
    tNewsLetter *TestNewsletter = InitializeTestNewsletter();
    TestNewsletter = GetNewsTitle(readDocuments, TestNewsletter);
    TestNewsletter = InitializeNewsClass(TestNewsletter);
    
    int k;
    k = atoi(argv[3]);
    TestNewsletter = GetTestClass(TrainNewsletter, TestNewsletter, argv[2], k);
    
    int** ConfusionMatrix = calloc(ClassesQtt, sizeof(int*));
    CalculateMatrix(ConfusionMatrix, TestNewsletter, argv[4]);
    
    FreeNewsLetter(TrainNewsletter);
    FreeTestNewsLetter(TestNewsletter);
    
    fclose(readDocuments);
    fclose(binary);
    
    return 0;
}
