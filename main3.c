#include "libraries.h"


int main(int argc, char *argv[]){
    FILE* readDocuments;
    char directory1[1000];
    sprintf(directory1,"%s", argv[2]);
    
    readDocuments = fopen(directory1, "r");

    if(argc != 5){
        printf ("ERRO:Os arquivos de dados devem estar contidos na pasta 'datasets' e o programa devera ser executado como o exemplo:\n");
        printf ("./prog3 binary.bin datasets/\"nomedapasta\"/test.txt \"numero K\" \"nomedoarquivodesaida\"/\n");
        exit(1);
    }

    FILE* binary;
    binary = fopen(argv[1], "rb");
    
    if(binary == NULL){
        printf ("ERRO: Não foi possível abrir o arquivo binário:\n");
        exit(1);
    }

    tNewsLetter* TrainNewsletter;
    TrainNewsletter = ReadBinary(binary);

    tNewsLetter *TestNewsletter = InitializeNewsletter();
    TestNewsletter = GetNewsTitle(readDocuments, TestNewsletter);
    int k;
    k = atoi(argv[3]);
    TestNewsletter = GetTestClass(TrainNewsletter, TestNewsletter, argv[2], k);

    int** ConfusionMatrix = calloc(ClassesQtt, sizeof(int*));

    CalculateMatrix(ConfusionMatrix, TestNewsletter, argv[4]);

    FreeNewsLetter(TrainNewsletter);
    FreeNewsLetter(TestNewsletter);
    fclose(readDocuments);
    fclose(binary);

    return 0;
}