#include "libraries.h"

struct class{
    char nameClass[4];
    int wordFrequency;
};

tClass** InitializeClass(){
    tClass** class = malloc(sizeof(tClass**)*ClassesQtt);
    
    for(int i = 0; i < ClassesQtt; i++){
        class[i] = calloc(1, sizeof(tClass));
    }
    strcpy(class[0]->nameClass, "int\0");
    strcpy(class[1]->nameClass, "poc\0");
    strcpy(class[2]->nameClass, "pod\0");
    strcpy(class[3]->nameClass, "eco\0");
    strcpy(class[4]->nameClass, "ept\0");
    strcpy(class[5]->nameClass, "reg\0");
    strcpy(class[6]->nameClass, "opi\0");
    strcpy(class[7]->nameClass, "inf\0");
    strcpy(class[8]->nameClass, "pot\0");
    strcpy(class[9]->nameClass, "at2\0");
    strcpy(class[10]->nameClass, "cid\0");

    return class;
}

void FreeClasses(tClass** Class){
    for(int i = 0; i < ClassesQtt; i++){
        free(Class[i]);
    }
    free(Class);
}

int ClassCmp(const void *v1, const void *v2){
    const tClass **c1 = (const tClass**) v1;
    const tClass **c2 = (const tClass**) v2;

    return ((*c2)->wordFrequency - (*c1)->wordFrequency);
}

void DisplayClasses(tClass** class){
    printf("\n");
    printf("Frequência da palavra por classe:\n");
    for(int i = 0; i < ClassesQtt; i++){
        printf("Classe: %s; ", class[i]->nameClass);
        printf("Frequência da palavra: %d\n", class[i]->wordFrequency);
    }
}

char* GetClass(char **classes, int k){
    tClass** class = InitializeClass();
    char *genre;
    SortsArray(classes, class, k);

    genre = strdup(class[0]->nameClass);
    FreeClasses(class);
    return genre;
}


void SortsArray(char **classes, tClass** class, int k){
    for(int i = 0; i < k; i++){
        for(int j = 0; j < ClassesQtt; j++){
            if(!strcmp(classes[i], class[j]->nameClass)){
                class[j]->wordFrequency++;
            }
        }
    }

    qsort(class, ClassesQtt, sizeof(tClass*), ClassCmp);
}

void FreeCharArray(char **words, int qtt){
    for(int i = 0; i < qtt; i++){
        free(words[i]);
    }
    free(words);
}

tClass* FillNewsClass(tClass* class, char* TestClass){
    strcpy(class->nameClass, TestClass);
    free(TestClass);
    return class; 
}

char* ReturnClass(tClass* class){
    return class->nameClass;
}

tClass** InitializeTestClass(int docQtt){
    tClass** class = malloc(sizeof(tClass**)*docQtt);
    
    for(int i = 0; i < docQtt; i++){
        class[i] = calloc(1, sizeof(tClass));
    }

    return class;
}

void FreeTestClasses(tClass** Class, int qtt){
    for(int i = 0; i < qtt; i++){
        free(Class[i]);
    }
    free(Class);
}
