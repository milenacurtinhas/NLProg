#include "libraries.h"


int main(int argc, char *argv[] ){
    if(argc != 3){
        printf ("ERRO:Os arquivos de dados devem estar contidos na pasta 'datasets' e o programa devera ser executado como o exemplo:\n");
        printf ("./prog1 datasets/\"nomedapasta\"/train.txt binary.bin\n");
        exit(1);
    }
    
    FILE* readDocuments;
    char directory1[1000];
    sprintf(directory1,"%s", argv[1]);
    
    readDocuments = fopen(directory1, "r");

    if(directory1 == NULL){
        printf ("ERRO:Os arquivos de dados devem estar contidos na pasta 'datasets' e o programa devera ser executado como o exemplo:\n");
        printf ("./prog1 datasets/\"nomedapasta\"/train.txt binary.bin\n");
        exit(1);
    }

    FILE* binarySave;
    char directory2[1000];
    sprintf(directory2,"%s", argv[2]);
    binarySave = fopen(directory2, "wb");

    if (directory2 == NULL){
        printf("ERRO: Verifique se a pasta 'binaries' existe e se ela esta no mesmo diretorio do programa.\n");
    }

    tNewsLetter *newsletter = InitializeNewsletter();
    
    newsletter = GetNewsTitle(readDocuments, newsletter);


    newsletter = ReadDocumentWords(newsletter, argv[1]);
    newsletter = ComputeTfIdf(newsletter);
    newsletter = FillDocumentWords(newsletter);
    
    SaveBinary(newsletter, binarySave);
    DisplayReport(newsletter);
   
    FreeNewsLetter(newsletter);
   
    fclose(readDocuments);
    fclose(binarySave);
    return 0;
}
