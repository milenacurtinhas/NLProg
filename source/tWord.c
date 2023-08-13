#include "libraries.h"

struct word{
    char* term;
    int *frequency;
    float *tfIdf;
    int totalDocs;
};

tWord **InitializeWords(){
    tWord **w = malloc(sizeof (tWord*));
    
    return w;
}

void FreeWords(tWord **w, int n){
    int i;
    for(i = 0; i < n; i++){
        FreeEachWord(w[i]);
    }
    free(w);
}

void FreeEachWord(tWord *w){
    free(w->term);
    free(w->frequency);
    free(w->tfIdf);
    free(w);
}

tWord** SaveWord(tWord** words, int qtt, char *term, int document, int documentsQtt){
    tWord *w = calloc(1, sizeof(tWord));
    w->frequency = calloc(1, sizeof(int) * documentsQtt);
    w->tfIdf = calloc(1, sizeof(float) * documentsQtt);
    w->term = strdup(term);
    w->frequency[document] = 1;
    
    words[qtt] = w;
    
    return words;
}


int comp(const void *v1, const void *v2){
    const tWord **c1 = (const tWord**)v1;
   
    const tWord **c2 = (const tWord**)v2;
   
    return strcmp((*c1)->term, (*c2)->term);
}

tWord** WordSearch(char *term, tWord** WordsArray, int wordsQtt){
    tWord* search = malloc(sizeof(tWord));
    
    search->term = strdup(term);
    
    tWord** result;
    result = bsearch(&search, WordsArray, wordsQtt, sizeof(tWord*), comp);
    
    free(search->term);
    free(search);

    return result;
}


tWord** OrderWords(tWord** WordsArray, int wordsQtt){
    qsort(WordsArray, wordsQtt, sizeof(tWord*), comp);
    
    return WordsArray;
}



tWord* CountWord(int document, tWord* word){
    word->frequency[document]++;
    
    return word;
}

tWord* ReallocWord(tWord* word, int documentsQtt){
    word = calloc(1, sizeof(tWord));
    word->frequency = realloc(word->frequency, (documentsQtt + 2) * sizeof(int));
    word->tfIdf = realloc(word->frequency, (documentsQtt + 2) * sizeof(float));
     
    return word;
}

int GetFrequency (tWord* word, int position){
    return word->frequency[position];
}
float GetTfIdf(tWord* word, int position){
    return word->tfIdf[position];
}

tWord** CalculateTfIdf(tWord** words, int docQtt, int docTotal, int wordsQtt){
    int i, j;
    float df = 0, tf = 0;

    for(i = 0; i < wordsQtt; i++){
        
        df = words[i]->totalDocs;
        for(j = 0; j < docQtt; j++){
            tf = words[i]->frequency[j];

            words[i]->tfIdf[j] = (float)log((1+(float)docTotal)/(1+df)) + 1;
            words[i]->tfIdf[j] = words[i]->tfIdf[j] * tf;
            
        }
    }
    
    return words;
}

void SaveEachWord(tWord* word, FILE* binarySave, int qtt){
    int namesize = strlen(word->term), i;
    namesize++;
    

    fwrite(&namesize, sizeof(int), 1, binarySave);
    fwrite(word->term, sizeof(char), namesize, binarySave);
    fwrite(&word->totalDocs, sizeof(int), 1, binarySave);
    
    for(i = 0; i < qtt; i++){
        fwrite(&word->frequency[i], sizeof(int), 1, binarySave);
        fwrite(&word->tfIdf[i], sizeof(float), 1, binarySave);
    }
}

tWord** ReadBinaryWords(tWord** word, int documentsQtt, int wordsQtt, FILE* binary){
    word = realloc(word, sizeof(tWord*)*wordsQtt);

    int namesize;

    for(int i = 0; i < wordsQtt; i++){
        word[i] = malloc(sizeof(tWord));
        word[i]->frequency = calloc(1, sizeof(int)*documentsQtt);
        word[i]->tfIdf = calloc(1, sizeof(float)*documentsQtt);
        
        fread(&namesize, sizeof(int), 1, binary);
        word[i]->term = malloc(sizeof(char)*namesize);
        fread(word[i]->term, sizeof(char), namesize, binary);

        fread(&word[i]->totalDocs, sizeof(int), 1, binary);
        
        for(int j = 0; j < documentsQtt; j++){
            fread(&word[i]->frequency[j], sizeof(int), 1, binary);
            fread(&word[i]->tfIdf[j], sizeof(float), 1, binary);
        }
    }

    return word;    
}

float SearchTfidf (tWord* word, int index){
    return word->tfIdf[index];
}

tWord** ReadCompareDocument(tWord** CompareWords, int* count, tWord** AllWords, int wordsQtt){
    char word[100], trash;
    int arrayPosition;
    tWord** result;
    tWord** search;

    printf("\nDigite um texto para estimar sua classe de notícia:\n");
    while(scanf("%s%c", word, &trash)){
        result = WordSearch(word, AllWords, wordsQtt);
        search = WordSearch(word, CompareWords, (*count));

        if(!search){
            CompareWords = realloc(CompareWords, ((*count)+1) * sizeof(tWord*));
            CompareWords = SaveWord(CompareWords,(*count), word, 0, 1);
            CompareWords = OrderWords(CompareWords, (*count));         
            
            if(result){
                arrayPosition = result-AllWords;
                CompareWords[(*count)]->totalDocs = AllWords[arrayPosition]->totalDocs;
            }
            else{
                CompareWords[(*count)]->totalDocs = 0;
            }

            (*count)++;
        }
        else{
            arrayPosition = search-CompareWords;
            CompareWords[arrayPosition] = CountWord(0, CompareWords[arrayPosition]);
        }
        
        if(trash == '\n'){
            break;
        }       
    }

    return CompareWords;
}

int *FillWordsIndex(tWord** CompareWords, tWord** AllWords, int* WordsIndex, int* count, int wordsQtt){
    int i, arrayPosition;
    tWord** result;

    for(i = 0; i < (*count); i++){
        result = WordSearch(CompareWords[i]->term , AllWords, wordsQtt);
        
        if(result){
            arrayPosition = result-AllWords;
            WordsIndex[i] = arrayPosition;
        }
        else{
            WordsIndex[i] = -1;
        }
    }


    return WordsIndex;
}

tWord** GetTotalFrequency(tWord** word, int wordsQtt, int documentsQtt){
    int i, j, df;
    for(i = 0; i < wordsQtt; i++){
        df = 0;
        for(j = 0; j < documentsQtt; j++){
            if(word[i]->frequency[j] > 0){
                df++;
            }
        }
        word[i]->totalDocs = df;
    }
    return word;
}


tWord** ReadTestDocument(tWord** CompareWords, int* count, tWord** AllWords, int wordsQtt, char* directory){
    char word[100];
    int arrayPosition;
    tWord** result;
    tWord** search;
    FILE* TestDocument;
    TestDocument = fopen(directory, "r");

    while(fscanf(TestDocument, "%s", word) == 1){
        result = WordSearch(word, AllWords, wordsQtt);
        search = WordSearch(word, CompareWords, (*count));

        if(!search){
            CompareWords = realloc(CompareWords, ((*count)+1) * sizeof(tWord*));
            CompareWords = SaveWord(CompareWords,(*count), word, 0, 1);
            CompareWords = OrderWords(CompareWords, (*count));         

            if(result){
                arrayPosition = result-AllWords;
                CompareWords[(*count)]->totalDocs = AllWords[arrayPosition]->totalDocs;
            }
            else{
                CompareWords[(*count)]->totalDocs = 0;
            }

            (*count)++;
        }
        else{
            arrayPosition = search-CompareWords;
            CompareWords[arrayPosition] = CountWord(0, CompareWords[arrayPosition]);
        }
        
         
    }
    fclose(TestDocument);
    return CompareWords;
}