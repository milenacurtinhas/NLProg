#include "libraries.h"

struct document{
    char *title;
    char *class;
    int *frequency;
    float *tfIdf;
    int wordTotal;
};

tDocument **InitializeDocuments(){
    tDocument **d = malloc(sizeof(tDocument*));
 
    return d;
};

void FreeDocuments(tDocument **d, int n){
    int i;
    
    for(i = 0; i < n; i++){
        FreeEachDocument(d[i]);
    }
    
    free(d);
}

void FreeEachDocument(tDocument *d){
    free(d->frequency);
    free(d->tfIdf);
    free(d->title);
    free(d->class);
    free(d);
}

tDocument** FillNewsTitle(FILE* readDocuments, int *count){
    tDocument** doc = InitializeDocuments();
    char name[100], genre[3];
    name[0] = '\0';
    
    while (!feof(readDocuments)){
        fscanf(readDocuments, "%s", name);
        
        if(name[0] == '\0' || name[0] == '\n' || name[0] == 'r'){
            break;
        }

        tDocument* d = calloc(1,sizeof(tDocument));
        
        d->title = strdup(name);

        fscanf(readDocuments, "%*c");
        fscanf(readDocuments,"%s", genre);        
        d->class = strdup(genre);
        
        doc[(*count)] = d;
        
        (*count)++;

        doc = realloc(doc, ((*count) + 1) *sizeof(tDocument*));
    }
    
    return doc;
}


char* ReadDirectory(char *directory, tDocument* document){
    int i = strlen(document->title);
    int j = strlen(directory);
    i += j;
    directory = realloc(directory, i+1 * sizeof(char));
    return strcat(directory, document->title);
}


tDocument* MallocDocumentsArray(tDocument* document, int wordsQtt){
    document->frequency = calloc(1, wordsQtt * sizeof(int));
    document->tfIdf = calloc(1, wordsQtt * sizeof(float));
    return document;
}

tDocument* FillDocumentArrays(tDocument* document, int frequency, float tfidf, int index){

    document->frequency[index] = frequency;
    document->tfIdf[index] = tfidf;
    
    return document;
}

void SaveEachDocument(tDocument* document, FILE* binarySave, int wordsQtt){
    int namesize = strlen(document->title), classsize = strlen(document->class), i;
    namesize++;
    fwrite(&namesize, sizeof(int), 1, binarySave);
    fwrite(document->title, sizeof(char), namesize, binarySave);

    classsize++;
    fwrite(&classsize, sizeof(int), 1, binarySave);
    fwrite(document->class, sizeof(char), classsize, binarySave);
    
    for(i = 0; i < wordsQtt; i++){
        fwrite(&document->frequency[i], sizeof(int), 1, binarySave);
        fwrite(&document->tfIdf[i], sizeof(float), 1, binarySave);
    }
}

tDocument** ReadBinaryDocuments(int documentsQtt, int wordsQtt, FILE* binary){
    tDocument** document = calloc(1, sizeof(tDocument*)*documentsQtt);
    int namesize, classsize;

    for(int i = 0; i < documentsQtt; i++){
        document[i] = calloc(1, sizeof(tDocument));
        document[i]->frequency = calloc(1, sizeof(int)*wordsQtt);
        document[i]->tfIdf = calloc(1, sizeof(float)*wordsQtt);
        
        fread(&namesize, sizeof(int), 1, binary);
        document[i]->title =calloc(1,sizeof(char)*namesize);
        fread(document[i]->title, sizeof(char), namesize, binary);
        
        fread(&classsize, sizeof(int), 1, binary);
        document[i]->class = calloc(1,sizeof(char)*classsize);
        fread(document[i]->class, sizeof(char), classsize, binary);

        
        
        for(int j = 0; j < wordsQtt; j++){
            fread(&document[i]->frequency[j], sizeof(int), 1, binary);
            fread(&document[i]->tfIdf[j], sizeof(float), 1, binary);
        }
    }

    return document;    
}

char* getNewsTitle(int index, tDocument** document){
    return document[index]->title;
}

int ReturnWordsQtt(tDocument* document, int wordsQtt){
    int wordTotal = 0;
    for(int i = 0; i < wordsQtt; i++){
        if(document->frequency[i] > 0){
            wordTotal+= document->frequency[i];
        }
    }
    return wordTotal;
}

void DisplayDocumentReport(tDocument** document, int documentsQtt, int wordsQtt){
    tDocument **wordTotal = malloc (sizeof(tDocument*)*documentsQtt) ;
    char title[100], class[3];
    int i;

    for(i = 0; i < documentsQtt; i++){
        wordTotal[i] = calloc(1, sizeof(tDocument));
        wordTotal[i]->frequency = calloc(1, sizeof(int));
        
        wordTotal[i]->wordTotal = ReturnWordsQtt(document[i], wordsQtt);
        strcpy(title, document[i]->title);
        wordTotal[i]->title = strdup(title);
        strcpy(class, document[i]->class);
        wordTotal[i]->class = strdup(class);
    }
    
    qsort(wordTotal, documentsQtt, sizeof(tDocument*), BiggestToLowest);
    
    printf("\nLista dos 10 documentos mais longos:\n");
    DisplayDocumentList(wordTotal);
    
    qsort(wordTotal, documentsQtt, sizeof(tDocument*), LowestToBiggest);
    
    printf("\nLista dos 10 documentos mais curtos:\n");
    DisplayDocumentList(wordTotal);

    FreeDocuments(wordTotal, documentsQtt);
}

void DisplayDocumentList(tDocument** document){
    int i;
    for(i = 0; i < 10; i++){
        printf("Documento: %s; ", document[i]->title);
        printf("Quantidade de palavras: %d; ", document[i]->wordTotal);
        printf("Classe: %s\n", document[i]->class);
    }
}

int BiggestToLowest(const void *v1, const void *v2){
    const tDocument **c1 = (const tDocument**) v1;
    const tDocument **c2 = (const tDocument**) v2;

    return ((*c2)->wordTotal - (*c1)->wordTotal);
}

int LowestToBiggest(const void *v1, const void *v2){
    const tDocument **c1 = (const tDocument**) v1;
    const tDocument **c2 = (const tDocument**) v2;

    return ((*c1)->wordTotal - (*c2)->wordTotal);
}

int CountDocumentQtt(int arrayposition, tDocument** documents, int documentsQtt){
    int i, qtt = 0;
    for(i = 0; i < documentsQtt; i++){
        if(documents[i]->frequency[arrayposition] > 0){
            qtt++;
        }
    }
    return qtt;
}

void MostFrequentDocuments(int arrayposition, tDocument** documents, int documentsQtt){
    tDocument **documentsFrequency = malloc (sizeof(tDocument*)*documentsQtt) ;
    char title[100];
    int i;

    for(i = 0; i < documentsQtt; i++){
        documentsFrequency[i] = calloc(1, sizeof(tDocument));
        documentsFrequency[i]->class = calloc(1, sizeof(char));
        documentsFrequency[i]->frequency = calloc(1, sizeof(int));
        documentsFrequency[i]->wordTotal = documents[i]->frequency[arrayposition];
        strcpy(title, documents[i]->title);
        documentsFrequency[i]->title = strdup(title);

    }   
    qsort(documentsFrequency, documentsQtt, sizeof(tDocument*), BiggestToLowest);
    
    printf("\nLista dos 10 documentos em que a palavra aparace com mais frequência:\n");
    DisplayFrequentDocuments(documentsFrequency);
    
    FreeDocuments(documentsFrequency, documentsQtt);
}

void DisplayFrequentDocuments (tDocument** document){
    int i;
    for(i = 0; i < 10; i++){
        if(document[i]->wordTotal){
            printf("Documento: %s; ", document[i]->title);
            printf("Frequência da palavra: %d \n", document[i]->wordTotal);
        }
    }
}

int GetDocumentFrequency (tDocument* document, int arrayposition){
    return document->frequency[arrayposition];
}

char* GetDocumentClass(tDocument* document){
    return document->class;
}

tDocument* InitializeCompareDocument(){
    tDocument *d = calloc(1, sizeof(tDocument));
 
    return d;
}

tDocument* ReturnWordQtt(tDocument* document, int i){
    document->wordTotal = i;
    return document;
}


float CalculateCoss(float* terminal, float* doc, int* WordsId, int WordsQtt, int totalWords){
    float coss = 0, num = 0, den1 = 0, den2 = 0;

    for(int i = 0; i < WordsQtt; i++){
        if(WordsId[i] != -1){
            num += (terminal[i] * doc[WordsId[i]]);
        }
        den1 += pow(terminal[i], 2);
    }
    for(int i = 0; i < totalWords; i++){
        den2 += pow(doc[i], 2);
    }
    
    den1 = sqrt(den1) * sqrt(den2);
    if(den1){
        coss = num/den1;
    }
    return coss;
}

float* ReturnTfidfArray(tDocument* doc){
    return doc->tfIdf;
}

char* ReturnDocumentClass(tDocument* doc){
    return doc->class;
}