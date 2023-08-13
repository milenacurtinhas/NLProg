#ifndef _TCLASS_H
#define _TCLASS_H

typedef struct class tClass;

tClass** InitializeClass();

void FreeClasses(tClass** Class);

int ClassCmp(const void *v1, const void *v2);

/**
 * @brief Imprime frequência das palavras por classe
 * 
 * @param class vetor contendo structs do tipo tClass
 */
void DisplayClasses(tClass** class);

/**
 * @brief Analisa qual classe o texto pertence
 * 
 * @param classes vetor com a k classes
 * @param k quantidade de notícias mais similares
 */
char* GetClass(char **classes, int k);

/**
 * @brief Conta a frequência das classes e as ordena
 * 
 * @param classes vetor com a k classes
 * @param class vetor contendo structs do tipo tClass
 * @param k quantidade de notícias mais similares
 */
void SortsArray(char **classes, tClass** class, int k);

void FreeCharArray(char **words, int qtt);

tClass* FillNewsClass(tClass* class, char* TestClass);


char* ReturnClass(tClass* class);

tClass** InitializeTestClass(int docQtt);

void FreeTestClasses(tClass** Class, int qtt);

#endif