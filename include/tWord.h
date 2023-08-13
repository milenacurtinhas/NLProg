#ifndef _TWORD_H_
#define _TWORD_H_

typedef struct word tWord;

tWord **InitializeWords();

void FreeWords(tWord **w, int n);

void FreeEachWord(tWord *w);

int comp(const void *v1, const void *v2);

/**
 * @brief Busca palavra no array que contém todas as palavras
 * 
 * @param term palavra a ser procurada
 * @param WordsArray array de structs do tipo tWord contendo as palavras
 * @param wordsQtt quantidade de palvras do array
 * @return array de structs do tipo tWord
 */
tWord** WordSearch(char *term, tWord** WordsArray, int wordsQtt);

/**
 * @brief ordena palavras
 * 
 * @param WordsArray array de structs do tipo tWord contendo as palavras
 * @param wordsQtt quantidade de palavras no array
 * @return array de structs do tipo tWord
 */
tWord** OrderWords(tWord** WordsArray, int wordsQtt);

/**
 * @brief salva a frequência de palavra na struct da palavras
 * 
 * @param document posição da palavra no vetor
 * @param word struct da palavra
 * @return structs do tipo tWord da palavra
 */
tWord* CountWord(int document, tWord* word);

/**
 * @brief salva palavra no array de palavras
 * 
 * @param words array de structs do tipo tWord
 * @param qtt posicao da palavra no array de structs
 * @param term palavra
 * @param document índice do documento em que a palavra se encontra
 * @param documentsQtt quantidade de documentos
 * @return tWord** 
 */
tWord** SaveWord(tWord** words, int qtt, char *term, int document, int documentsQtt);

int GetFrequency (tWord* word, int position);

float GetTfIdf(tWord* word, int position);

/**
 * @brief Calcula Tf-Idf
 * 
 * @param words array de structs do tipo tWord
 * @param docQtt quantidade de documentos a ser calculado no tf
 * @param docTotal quantidade de documentos total
 * @param wordsQtt quantidade de palavras
 * @return array de structs do tipo tWord
 */
tWord** CalculateTfIdf(tWord** words, int docQtt, int docTotal, int wordsQtt);

void SaveEachWord(tWord* word, FILE* binarySave, int qtt);

tWord** ReadBinaryWords(tWord** word, int documentsQtt, int wordsQtt, FILE* binary);

float SearchTfidf(tWord* word, int index);

/**
 * @brief Lê texto do terminal a ser estimada a classe
 * 
 * @param CompareWords array de structs do tipo tWord para o texto do terminal
 * @param count quantidade de palavras do texto do terminal
 * @param AllWords array de structs do tipo tWord para os documentos já computados
 * @param wordsQtt total de palavras dos documentos já computados
 * @return tWord** 
 */
tWord** ReadCompareDocument(tWord** CompareWords, int* count, tWord** AllWords, int wordsQtt);

/**
 * @brief Calcula a quantidade de documentos que cada palavra aparece
 * 
 * @param word array de structs do tipo tWord
 * @param wordsQtt quantidade total de palavras da struct
 * @param documentsQtt quantidade total de documentos
 * @return array de structs do tipo tWord
 */
tWord** GetTotalFrequency(tWord** word, int wordsQtt, int documentsQtt);

/**
 * @brief Salva a posição das palavras no array com todas as palavras
 * 
 * @param CompareWords array de structs do tipo tWord com as palavras do documento a ser comparado
 * @param AllWords array de structs do tipo tWord com todas as palvras ja computadas
 * @param WordsIndex vetor com a posicao das palavras no array contendo todas as palavras
 * @param count quantidade de palavras do documento a ser comparado
 * @param wordsQtt quantidade de palvras total
 * @return vetor com a posicao das palavras no array contendo todas as palavras
 */
int *FillWordsIndex(tWord** CompareWords, tWord** AllWords, int* WordsIndex, int* count, int wordsQtt);

/**
 * @brief Lê os documentos da pasta de teste (usado no programa 3)
 * 
 * @param CompareWords array de structs do tipo tWord com as palavras do doumento da pasta teste 
 * @param count posição da palavra no array 
 * @param AllWords array de structs do tipo tWord com as palavras dos doumentos da pasta train
 * @param wordsQtt quantidade de palavras salvas no array criado com as palavras do train.txt
 * @param directory diretório dos arquivos a serem lidos
 * @return array de structs do tipo tWord com as palavras do doumento da pasta teste 
 */
tWord** ReadTestDocument(tWord** CompareWords, int* count, tWord** AllWords, int wordsQtt, char* directory);

#endif