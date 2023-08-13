#include "libraries.h"

struct newsletter{
    tDocument** document;
    tWord** word;
    tClass** class;
    int documentsQtt;
    int wordsQtt;
};

tNewsLetter* InitializeNewsletter(){
    tNewsLetter *n = malloc(sizeof(tNewsLetter));
    n->class = InitializeClass();
    n->word = InitializeWords();
    n->documentsQtt = 0;
    n->wordsQtt = 0;

    return n;
}

tNewsLetter* GetNewsTitle(FILE* readDocuments, tNewsLetter *n){
    
    int count = 0;
    n->document = FillNewsTitle(readDocuments, &count);
    n->documentsQtt = count;
    return n;

}

void FreeNewsLetter(tNewsLetter* n){
    FreeDocuments(n->document, n->documentsQtt);
    FreeWords(n->word, n->wordsQtt);
    FreeClasses (n->class);    
    free(n);
}

tNewsLetter *ReadDocumentWords(tNewsLetter* newsletter, char *argv){
    int i = 0, j;
    char* directory;
    char* directoryCopy;
    i = strlen(argv);
    
    
    directory = strndup(argv, i-9);
    directoryCopy = strdup(directory);

    for(j = 0; j < newsletter->documentsQtt; j++){
        directory = ReadDirectory(directory, newsletter->document[j]);
        
        newsletter = ReadText(newsletter, directory, j);
        strcpy(directory,directoryCopy);
    }
    newsletter->word = GetTotalFrequency(newsletter->word, newsletter->wordsQtt, newsletter->documentsQtt);
    

    free(directoryCopy);
    free(directory);
    
    return newsletter;
}

tNewsLetter *ReadText(tNewsLetter* newsletter, char *directory, int count){
    FILE* text;
    text = fopen(directory, "r");
   
    tWord** result;
    char term[50];
    int arrayPosition;
    
    while(fscanf(text, "%s", term) == 1){
        
        result = WordSearch(term, newsletter->word, newsletter->wordsQtt);

        if(!result){
            newsletter->word = realloc(newsletter->word, (newsletter->wordsQtt+1) * sizeof(tWord*));
            newsletter->word = SaveWord(newsletter->word, newsletter->wordsQtt, term, count, newsletter->documentsQtt);
            newsletter->word = OrderWords(newsletter->word, newsletter->wordsQtt);
            newsletter->wordsQtt++;
        }
       
        if(result){
            arrayPosition = result-newsletter->word;
            newsletter->word[arrayPosition] = CountWord(count, newsletter->word[arrayPosition]);
        }
        
    }
    
    fclose(text);
    return newsletter;
}

tNewsLetter *FillDocumentWords(tNewsLetter* newsletter){
    int i, j, frequency; 
    float tfidf;

    for(i = 0; i < newsletter->documentsQtt; i++){
        newsletter->document[i] = MallocDocumentsArray(newsletter->document[i], newsletter->wordsQtt);
        
        for(j = 0; j < newsletter->wordsQtt; j++){
            frequency = GetFrequency(newsletter->word[j], i);
            tfidf = GetTfIdf(newsletter->word[j], i);
            newsletter->document[i] = FillDocumentArrays(newsletter->document[i], frequency, tfidf, j);   
        }
    }

    return newsletter;
}

tNewsLetter* ComputeTfIdf(tNewsLetter* newsletter){
    newsletter->word = CalculateTfIdf(newsletter->word, newsletter->documentsQtt, newsletter->documentsQtt, newsletter->wordsQtt);
     
    return newsletter;
}

void DisplayReport(tNewsLetter* newsletter){
    printf("Número de documentos: %d\n", newsletter->documentsQtt);
    printf("Número de palavras diferentes: %d\n", newsletter->wordsQtt);
}

void SaveBinary(tNewsLetter* newsletter, FILE* binarySave){
    fwrite(&newsletter->wordsQtt, sizeof(int), 1, binarySave);
    fwrite(&newsletter->documentsQtt, sizeof(int), 1, binarySave);
    
    int i;

    for(i = 0; i < newsletter->wordsQtt; i++){
        SaveEachWord(newsletter->word[i], binarySave, newsletter->documentsQtt);
        
    }
    for(i = 0; i < newsletter->documentsQtt; i++){
        SaveEachDocument(newsletter->document[i], binarySave, newsletter->wordsQtt);
        
    }
}

tNewsLetter* ReadBinary(FILE* binary){
    tNewsLetter* newsletter = InitializeNewsletter();
    
    
    fread(&newsletter->wordsQtt, sizeof(int), 1, binary);
    fread(&newsletter->documentsQtt, sizeof(int), 1, binary);

    newsletter->word = ReadBinaryWords(newsletter->word,newsletter->documentsQtt, newsletter->wordsQtt, binary);
    newsletter->document = ReadBinaryDocuments(newsletter->documentsQtt, newsletter->wordsQtt, binary);

    return newsletter;
}

void SearchNews(tNewsLetter* newsletter){
    char** search = malloc(sizeof(char*));
    char word[100], trash;
    int wordsQtt = 0, wordIndex;
    tWord** result;
    
    printf("Digite uma palavra ou um texto\n");
    while(scanf("%s%c", word, &trash)){
        LowerCaseWord(word);
        
        search = realloc(search, sizeof(char*) * (wordsQtt+1));
        search[wordsQtt] = strdup(word);
        wordsQtt++;

        if(trash == '\n'){
            break;
        }   
    }
    
    float *tfidfArray = calloc(1, sizeof(float)*newsletter->documentsQtt);

    for(int j = 0; j < wordsQtt; j++){
        result = WordSearch(search[j], newsletter->word, newsletter->wordsQtt);
        if(result){
            wordIndex = result-newsletter->word;
            for(int i = 0; i < newsletter->documentsQtt; i++){
                tfidfArray[i] += SearchTfidf(newsletter->word[wordIndex], i);
            }
        }
    } 
    
    if(!result){
        printf("Não existem notícias contendo esse texto\n");
    }
    else{
        displayTitles(tfidfArray, newsletter);
    }

    free(tfidfArray);
    for(int i = 0; i < wordsQtt; i++){
        free(search[i]);
    }
    
    free(search);
}

int GetNewsIndex(float *tfidfArray, int docQtt){
    int i, index = 0;
    float aux = 0;
    
    for(i = 0; i < docQtt; i++){
        if(tfidfArray[i] > aux){
            index = i;
            aux = tfidfArray[i];
        }
    }
    
    tfidfArray[index] = 0;
    return index;   
}

void displayTitles(float* tfidfArray, tNewsLetter* newsletter){
    int index[10];
    
    for(int k = 0; k < 10; k++){
        index[k] = GetNewsIndex(tfidfArray, newsletter->documentsQtt);
    }
    
    for(int l = 0; l < 10; l++){
        if(l == 0 || index[l] != index[l-1]){
            printf("%s\n", getNewsTitle(index[l], newsletter->document));
        }
    }
}

void LowerCaseWord(char* word){
    for(int i = 0; word[i] != '\0'; i++){
        word[i] = tolower(word[i]);
    }
}

int ShowMenu(){
    int command;

    printf("\033[0;35m");
    printf("\nDigite a opção desejada: \n\n");
    printf("\033[0;35m");
    printf("1 • ");
    printf("\033[0m");
    printf("BUSCAR NOTÍCIAS\n");
    printf("\033[0;35m");
    printf("2 • ");
    printf("\033[0m");
    printf("CLASSIFICAR NOTÍCIAS\n");
    printf("\033[0;35m");
    printf("3 • ");
    printf("\033[0m");
    printf("GERAR RELATÓRIO DE UMA PALAVRA\n");
    printf("\033[0;35m");
    printf("4 • ");
    printf("\033[0m");
    printf("GERAR RELATÓRIO DE DOCUMENTOS\n\n");

    scanf("%d", &command);

    return command;
}

void CreateDocumentReport(tNewsLetter* newsletter){
    DisplayDocumentReport(newsletter->document, newsletter->documentsQtt, newsletter->wordsQtt);
}

void CreateWordReport(tNewsLetter* newsletter){
    char word[100];
    int arrayPosition, totalQtt;
    
    printf("\nDigite uma palavra para exibir sua informações\n");
    scanf("%s", word);

    tWord **result = WordSearch(word, newsletter->word, newsletter->wordsQtt);
    if(result){
        arrayPosition = result-newsletter->word;
        
        totalQtt = CountDocumentQtt(arrayPosition, newsletter->document, newsletter->documentsQtt);
        printf("\nQuantidade de documentos contendo a palavra:\n%d\n", totalQtt);
        MostFrequentDocuments(arrayPosition, newsletter->document, newsletter->documentsQtt);
        MostFrequencyClasses(arrayPosition, newsletter);
    }
    else{
        printf("Não existem resultados para esta palavra\n");
    }
}

void MostFrequencyClasses(int arrayposition, tNewsLetter* newsletter){
    char **class = malloc(sizeof(char*));
    int count = 0;

    for(int i = 0; i < newsletter->documentsQtt; i++){
        int qtt = GetDocumentFrequency(newsletter->document[i], arrayposition);
      
        if(qtt > 0){
            class[count] = strdup(GetDocumentClass(newsletter->document[i]));
            count++;
            class = realloc(class, sizeof(char*)*(count+1));
        }
    }
    
    SortsArray(class, newsletter->class, count);
    DisplayClasses(newsletter->class);
    FreeCharArray(class, count);
}

void ClassifyNews(tNewsLetter* newsletter, int k){
    tWord **CompareWords = InitializeWords();
    int i = 0, j, frequency;
    float tfidf;
    int *WordsIndex = malloc(sizeof(int));
    char *class;
    CompareWords = ReadCompareDocument(CompareWords, &i, newsletter->word, newsletter->wordsQtt);
    WordsIndex = realloc(WordsIndex, sizeof(int)*(i+1));
    WordsIndex = FillWordsIndex(CompareWords, newsletter->word, WordsIndex, &i, newsletter->wordsQtt);
    
    CompareWords = CalculateTfIdf(CompareWords, 1, newsletter->documentsQtt, i);
    
    tDocument *CompareDocument = InitializeCompareDocument();
    CompareDocument = MallocDocumentsArray(CompareDocument, i);
    CompareDocument = ReturnWordQtt(CompareDocument, i);

    for(j = 0; j < i; j++){
        frequency = GetFrequency(CompareWords[j], 0);
        tfidf = GetTfIdf(CompareWords[j], 0);
        CompareDocument = FillDocumentArrays(CompareDocument, frequency, tfidf, j);
    }
    class = CalculateSimilarity(ReturnTfidfArray(CompareDocument), WordsIndex, i, newsletter, k);
    printf("\nEsse texto pertence a classe %s\n", class);

    free(class);
    FreeEachDocument(CompareDocument);
    FreeWords(CompareWords, i);
    free(WordsIndex);
}

char* CalculateSimilarity(float *CmpDoc, int* WordsId, int WordsQtt, tNewsLetter* newsletter, int k){
    typedef struct{
        int doc;
        float coss;
        char class[4];
    }news;
    
    news array[newsletter->documentsQtt];
    char **classes = malloc(sizeof(char*)*k);
    char *class;

    for(int i = 0; i < newsletter->documentsQtt; i++){
        array[i].doc = i;
        array[i].coss = CalculateCoss(CmpDoc, ReturnTfidfArray(newsletter->document[i]), WordsId, WordsQtt, newsletter->wordsQtt);
        strcpy(array[i].class, ReturnDocumentClass(newsletter->document[i]));
    }

    for(int i = 0; i < newsletter->documentsQtt - 1; i++) {
        int bigger = i;
        for(int j = i + 1; j < newsletter->documentsQtt; j++) {
            if(array[j].coss > array[bigger].coss) {
                bigger = j;
            } 
        }
        news aux = array[i];
        array[i] = array[bigger];
        array[bigger] = aux;
    }
    
    for(int i = 0; i < k; i++){
        if(array[i].coss == 0){
            k = i;
            break;
        }
        classes[i] = strdup(array[i].class);
    }
    
    class = GetClass(classes, k);
    
    FreeCharArray(classes, k);
    return class;
}

tNewsLetter *GetTestClass(tNewsLetter* TrainNewsletter, tNewsLetter* TestNewsletter, char *argv, int k){
    int i = 0, j;
    char* directory;
    char* directoryCopy;
    i = strlen(argv);
    
    TestNewsletter->class = realloc(TestNewsletter->class, sizeof(tClass*) * TestNewsletter->documentsQtt);

    directory = strndup(argv, i-8);
    directoryCopy = strdup(directory);
    
    for(j = 0; j < TestNewsletter->documentsQtt; j++){
        directory = ReadDirectory(directory, TestNewsletter->document[j]);
        TestNewsletter->class[j] = FillNewsClass(TestNewsletter->class[j], ReturnTestClass(TrainNewsletter, k, directory));
        
        strcpy(directory,directoryCopy);
    }
    free(directoryCopy);
    free(directory);
    
    return TestNewsletter;
}

char* ReturnTestClass(tNewsLetter* newsletter, int k, char *directory){
    tWord **CompareWords = InitializeWords();
    int i = 0, j, frequency;
    float tfidf;
    int *WordsIndex = malloc(sizeof(int));
    
    CompareWords = ReadTestDocument(CompareWords, &i, newsletter->word, newsletter->wordsQtt, directory);
    WordsIndex = realloc(WordsIndex, sizeof(int)*(i+1));
 
    WordsIndex = FillWordsIndex(CompareWords, newsletter->word, WordsIndex, &i, newsletter->wordsQtt);
   
    CompareWords = CalculateTfIdf(CompareWords, 1, newsletter->documentsQtt, i);
    
    tDocument *CompareDocument = InitializeCompareDocument();
    CompareDocument = MallocDocumentsArray(CompareDocument, i);
    
    CompareDocument = ReturnWordQtt(CompareDocument, i);
    for(j = 0; j < i; j++){
        frequency = GetFrequency(CompareWords[j], 0);
        tfidf = GetTfIdf(CompareWords[j], 0);
        CompareDocument = FillDocumentArrays(CompareDocument, frequency, tfidf, j);
    }
    char* class;
    class = CalculateSimilarity(ReturnTfidfArray(CompareDocument), WordsIndex, i, newsletter, k);
    FreeEachDocument(CompareDocument);
    FreeWords(CompareWords, i);
    free(WordsIndex);
    return class;
}

void CalculateMatrix(int** ConfusionMatrix, tNewsLetter* TestNewsletter, char* argv){
    int accuracy;

    ConfusionMatrix = InitializeMatrix(ConfusionMatrix);
    
    for(int i = 0; i < TestNewsletter->documentsQtt; i++){
        ConfusionMatrix = FillMatrix(ConfusionMatrix, GetDocumentClass(TestNewsletter->document[i]), ReturnClass(TestNewsletter->class[i]));   
    }
    accuracy = CalculateAccuracy(ConfusionMatrix, TestNewsletter->documentsQtt);

    MakeMatrixFile(argv, ConfusionMatrix, accuracy);
    FreeMatrix(ConfusionMatrix);
}

int** InitializeMatrix(int** ConfusionMatrix){
    for(int i = 0; i < ClassesQtt; i++){
        ConfusionMatrix[i] = calloc(ClassesQtt, sizeof(int));
    }

    return ConfusionMatrix;
}

int** FillMatrix(int** ConfusionMatrix, char* TrueClass, char* PredictedClass){
    int i, j;

    i = CalculatePosition(TrueClass);
    j = CalculatePosition(PredictedClass);

    ConfusionMatrix[i][j]++;

    return ConfusionMatrix;
}

int CalculatePosition(char* class){
    if(!strcmp(class, "int")){
        return 0;
    }
    else if(!strcmp(class, "poc")){
        return 1;
    }
    else if(!strcmp(class, "pod")){
        return 2;
    }
    else if(!strcmp(class, "eco")){
        return 3;
    }
    else if(!strcmp(class, "ept")){
        return 4;
    }
    else if(!strcmp(class, "reg")){
        return 5;
    }
    else if(!strcmp(class, "opi")){
        return 6;
    }
    else if(!strcmp(class, "inf")){
        return 7;
    }
    else if(!strcmp(class, "pot")){
        return 8;
    }
    else if(!strcmp(class, "at2")){
        return 9;
    }
    else if(!strcmp(class, "cid")){
        return 10;
    }
    return -1;
}

int CalculateAccuracy(int** ConfusionMatrix, int docsQtt){
    int accuracy, count = 0;

    for(int i = 0; i < ClassesQtt; i++){
        count += ConfusionMatrix[i][i];
    }

    accuracy = (100*count)/docsQtt;

    return accuracy;
}

void MakeMatrixFile(char* argv, int** ConfusionMatrix, int accuracy){
    FILE* MatrixFile;
    MatrixFile = fopen(argv, "w");

    fprintf(MatrixFile, "Acurácia do método: %d%%\n\nMatriz de confusão:\n", accuracy);
    
    //alteração: nomes das classes foram colocados na tabela
    fprintf(MatrixFile, "    int poc pod eco ept reg opi inf pot at2 cid\n");
    fprintf(MatrixFile, "    ───────────────────────────────────────────\n");
    for(int i = 0; i < ClassesQtt; i++){
        for(int j = 0; j < ClassesQtt; j++){
            if(j == 0){
                switch (i){
                case 0:
                    fprintf(MatrixFile, "int|");
                    break;
                case 1:
                    fprintf(MatrixFile, "poc|");
                    break;
                case 2:
                    fprintf(MatrixFile, "pod|");
                    break;
                case 3:
                    fprintf(MatrixFile, "eco|");
                    break;
                case 4:
                    fprintf(MatrixFile, "ept|");
                    break;
                case 5:
                    fprintf(MatrixFile, "reg|");
                    break;
                case 6:
                    fprintf(MatrixFile, "opi|");
                    break;
                case 7:
                    fprintf(MatrixFile, "inf|");
                    break;
                case 8:
                    fprintf(MatrixFile, "pot|");
                    break;
                case 9:
                    fprintf(MatrixFile, "at2|");
                    break;
                case 10:
                    fprintf(MatrixFile, "cid|");
                    break;
                }
            }
            fprintf(MatrixFile, " %d  ", ConfusionMatrix[i][j]);
        }
        if(i != 10){
            fprintf(MatrixFile, "\n");
        }
    }
    fclose(MatrixFile);
}

void FreeMatrix(int** ConfusionMatrix){
    for(int i = 0; i < ClassesQtt; i++){
        free(ConfusionMatrix[i]);
    }
    free(ConfusionMatrix);
}

tNewsLetter* InitializeTestNewsletter(){
    tNewsLetter* n = malloc(sizeof(tNewsLetter));
    n->word = InitializeWords();
    n->documentsQtt = 0;
    n->wordsQtt = 0;

    return n;   
}

tNewsLetter* InitializeNewsClass(tNewsLetter* n){
    n->class = InitializeTestClass(n->documentsQtt);
    return n;
}

void FreeTestNewsLetter(tNewsLetter* n){
    FreeWords(n->word, n->wordsQtt);
    FreeDocuments(n->document, n->documentsQtt);
    FreeTestClasses(n->class, n->documentsQtt);
    free(n);   
}