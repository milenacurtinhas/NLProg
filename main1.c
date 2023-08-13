#include "libraries.h"


int main(int argc, char *argv[] ){
    if(argc != 3){
        printf ("ERRO: O programa deve ser executado como o exemplo:\n");
        printf ("./prog1 \"CaminhoParaPasta\"/train.txt binary.bin\n");
        exit(1);
    }
    
    FILE* readDocuments;
    char directory1[1000];
    sprintf(directory1,"%s", argv[1]);
    
    readDocuments = fopen(directory1, "r");

    if(readDocuments == NULL){
        printf ("ERRO:Não foi possível abrir o arquivo .txt, o programa deve ser executado como o exemplo:\n");
        printf ("./prog1 \"CaminhoParaPasta\"/train.txt binary.bin\n");
        exit(1);
    }

    FILE* binarySave;
    char directory2[1000];
    sprintf(directory2,"%s", argv[2]);
    binarySave = fopen(directory2, "wb");

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
