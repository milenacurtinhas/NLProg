#ifndef _TDOCUMENT_H_
#define _TDOCUMENT_H_

typedef struct document tDocument;
 
tDocument **InitializeDocuments();

void FreeDocuments(tDocument **d, int n);

/**
 * @brief Lê o título dos documentos
 * 
 * @param count quantidade de documentos
 * @return vetor contendo structs do tipo tDocument com as informações lidas
 */
tDocument** FillNewsTitle(FILE* readDocuments,  int *count);

void FreeEachDocument(tDocument *d);

/**
 * @brief Retorna diretório do documento
 * 
 * @param directory caminho da pasta contendo o documento
 * @param document nome do documento
 * @return diretório de cada documento
 */
char* ReadDirectory(char* directory, tDocument* document);

tDocument* MallocDocumentsArray(tDocument* document, int wordsQty);

/**
 * @brief Salva informações da palavra da notícia
 * 
 * @param document notícia analisada
 * @param frequency frequência da palavra no documento
 * @param tfidf tfidf da palavra no documento
 * @param index posição da palavra no array de palavras
 * @return struct do tipo tDocument com as informações lidas
 */
tDocument* FillDocumentArrays(tDocument* document, int frequency,float tfidf, int index);

void SaveEachDocument(tDocument* document, FILE* binarySave, int wordsQtt);

tDocument** ReadBinaryDocuments(int documentsQtt, int wordsQtt, FILE* binary);

char* getNewsTitle(int index, tDocument** document);

int ReturnWordsQtt(tDocument* document, int wordsQtt);

/**
 * @brief Ordena documentos e imprime relatório
 * 
 * @param document vetor contendo structs do tipo tDocument
 * @param documentsQtt quantidade de documentos
 * @param wordsQtt quantidade de palavras
 */
void DisplayDocumentReport(tDocument** document, int documentsQtt, int wordsQtt);

/**
 * @brief Imprime informações dos documentos
 * 
 * @param document vetor contendo structs do tipo tDocument
 */
void DisplayDocumentList(tDocument** document);

int BiggestToLowest(const void *v1, const void *v2);

int LowestToBiggest(const void *v1, const void *v2);

/**
 * @brief Conta quantidade de vezes que uma palavra aparece em todos os documentos
 * 
 * @param arrayposition índice da palavra no array de palavra
 * @param documents vetor contendo structs do tipo tDocument
 * @param documentsQtt quantidade de documentos
 * @return Retorna total de vezes
 */
int CountDocumentQtt(int arrayposition, tDocument** documents, int documentsQtt);

/**
 * @brief Ordena os dez documentos em que uma palavra mais aparece
 * 
 * @param arrayposition índice da palavra no array de palavra
 * @param documents vetor contendo structs do tipo tDocument
 * @param documentsQtt quantidade de documentos
 */
void MostFrequentDocuments(int arrayposition, tDocument** documents, int documentsQtt);

/**
 * @brief Imprime um documento e a quantidade de vezes que uma palavra aparece nele
 * 
 * @param document vetor contendo structs do tipo tDocument
 */
void DisplayFrequentDocuments (tDocument** document);

int GetDocumentFrequency (tDocument* document, int arrayposition);

char* GetDocumentClass(tDocument* document);

tDocument* InitializeCompareDocument();

tDocument* ReturnWordQtt(tDocument* document, int i);

/**
 * @brief Calcula cosseno
 * 
 * @param terminal vetor de tfidf do texto lido no terminal
 * @param doc vetor de tfidf do documento de comparação
 * @param WordsId vetor com os índices das palavras do texto no array de palavras
 * @param WordsQtt quantidade de palavras
 * @return float 
 */
float CalculateCoss(float* terminal, float* doc, int* WordsId, int WordsQtt, int totalWords);

float* ReturnTfidfArray(tDocument* doc);

char* ReturnDocumentClass(tDocument* doc);

#endif