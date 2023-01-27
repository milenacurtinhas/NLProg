#include "libraries.h"

int main(int argc, char **argv){

    if(argc != 3){
        printf ("ERRO: O programa devera ser executado como o exemplo:\n");
        printf ("./prog2 binary.bin \"numero K\"\n");
        exit(1);
    }

    FILE* binary;
    binary = fopen(argv[1], "rb");
    
    if(binary == NULL){
        printf ("ERRO: Não foi possível abrir o arquivo binário:\n");
        exit(1);
    }
    tNewsLetter* newsletter;
    
    newsletter = ReadBinary(binary);
    int i;

    switch(ShowMenu()){
        case 1:
            SearchNews(newsletter);
            break;
        case 2:
            i = atoi(argv[2]);
            ClassifyNews(newsletter, i);
            break;
        case 3:
            CreateWordReport(newsletter);
            break;
        case 4:
            CreateDocumentReport(newsletter);
            break;
    }

    fclose(binary);
    FreeNewsLetter(newsletter);
    return 0;
}