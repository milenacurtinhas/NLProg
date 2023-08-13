#ifndef _TNEWSLETTER_H_
#define _TNEWSLETTER_H_

typedef struct newsletter tNewsLetter;

tNewsLetter* InitializeNewsletter();

/**
 * @brief Salva o nome dos documentos e a quantidade total de documentos
 * 
 * @param n tipo tNewsLetter
 * @return struct do tipo tNewsLetter com as informações lidas
 */
tNewsLetter* GetNewsTitle(FILE* readDocuments, tNewsLetter* n);

void FreeNewsLetter(tNewsLetter* n);

/**
 * @brief Lê informações de cada documento
 * 
 * @param argv nome da pasta que contém os documentos
 * @return struct do tipo tNewsLetter com as informações lidas
 */
tNewsLetter* ReadDocumentWords(tNewsLetter* newsletter, char *argv);

/**
 * @brief Lê a notícia de cada documento e salva as palavras e sua frequência no array de palavras
 * 
 * @param directory diretório do documento a ser lido
 * @param count índice do documento
 * @return struct do tipo tNewsLetter com as informações lidas
 */
tNewsLetter* ReadText(tNewsLetter* newsletter, char *directory, int count);

/**
 * @brief Salva características das palavras no array de documentos
 * @return struct do tipo tNewsLetter com as informações lidas
 */
tNewsLetter* FillDocumentWords(tNewsLetter* newsletter);

/**
 * @brief Calcula TfIdf e armazena no array de palavras
 */
tNewsLetter* ComputeTfIdf(tNewsLetter* newsletter);

/**
 * @brief imprime o número de documentos e de palavras diferentes
 */
void DisplayReport(tNewsLetter* newsletter);

void SaveBinary(tNewsLetter* newsletter, FILE* binarySave);

tNewsLetter* ReadBinary(FILE* binary);

/**
 * @brief Busca 10 notícias em quem a palavra ou o texto lido do terminal mais aparece
 */
void SearchNews(tNewsLetter* newsletter);

/**
 * @brief Retorna notícia com a maior soma de Tf-Idf
 * 
 * @param tfidfArray array que armazena as somas dos Tf-Idf em cada documento
 * @param wordsQtt quantidade total de palavras
 * @return índice da notícia com maior soma de Tf-Idf
 */
int GetNewsIndex(float *tfidfArray, int wordsQtt);

/**
 * @brief Imprime as dez notícias com maior soma de Tf-Idf
 * 
 * @param tfidfArray array que armazena as somas dos Tf-Idf em cada documento
 */
void displayTitles(float* tfidfArray, tNewsLetter* newsletter);

void LowerCaseWord(char* word);

int ShowMenu();

/**
 * @brief Chama função que cria relatório de documento
 */
void CreateDocumentReport(tNewsLetter* newsletter);

/**
 * @brief Cria relatório de uma palavra
 */
void CreateWordReport(tNewsLetter* newsletter);

/**
 * @brief Imprime a quantidade de vezes em que a palavra aparece em cada classe
 * 
 * @param arrayposition posição da palavra no array de palavras
 */
void MostFrequencyClasses(int arrayposition, tNewsLetter* newsletter);

/**
 * @brief Calcula classe do texto
 * 
 * @param k quantidade de notícias mais similares
 */
void ClassifyNews(tNewsLetter* newsletter, int k);

/**
 * @brief Calcula similaridade entre textos
 * 
 * @param CmpDoc texto lido no terminal
 * @param WordsId vetor com os índices das palavras do texto no array de palavras
 * @param WordsQtt quantidade de palavras no texto
 * @param k quantidade de notícias mais similares
 */
char* CalculateSimilarity(float *CmpDoc, int* WordsId, int WordsQtt, tNewsLetter* newsletter, int k);

/**
 * @brief 
 * 
 * @param k quantidade de notícias mais similares
 * @param directory 
 * @return char* 
 */
char* ReturnTestClass(tNewsLetter* newsletter, int k, char *directory);

/**
 * @brief Get the Test Class object
 * 
 * @param TrainNewsletter 
 * @param TestNewsletter 
 * @param argv 
 * @param k quantidade de notícias mais similares
 * @return tNewsLetter* 
 */
tNewsLetter *GetTestClass(tNewsLetter* TrainNewsletter, tNewsLetter* TestNewsletter, char *argv, int k);

/**
 * @brief 
 * 
 * @param ConfusionMatrix 
 * @param TestNewsletter 
 * @param argv 
 */
void CalculateMatrix(int** ConfusionMatrix, tNewsLetter* TestNewsletter, char* argv);

int** InitializeMatrix(int** ConfusionMatrix);

/**
 * @brief Calcula o resultado da matriz de confusão
 * 
 * @param ConfusionMatrix matriz de confusão
 * @param TrueClass classe verdadeira do documento
 * @param PredictedClass classe estimada do documento
 * @return matriz de confusão
 */
int** FillMatrix(int** ConfusionMatrix, char* TrueClass, char* PredictedClass);

/**
 * @brief calcula posicão da matriz em que a classe pertence
 * 
 * @return posição da classe na matriz
 */
int CalculatePosition(char* class);

/**
 * @brief Calcula do método de classificação dos documentos
 * 
 * @param ConfusionMatrix matriz de confusão
 * @param docsQtt quantidade de documentos
 * @return porcentagem de acurácia
 */
int CalculateAccuracy(int** ConfusionMatrix, int docsQtt);

void FreeMatrix(int** ConfusionMatrix);

/**
 * @brief cria o arquivo de resultado da matriz de confusão
 * 
 * @param argv arquivo a ser criado
 * @param ConfusionMatrix matriz de confusao
 * @param accuracy porcenagem de acurácia
 */
void MakeMatrixFile(char* argv, int** ConfusionMatrix, int accuracy);

tNewsLetter* InitializeTestNewsletter();

tNewsLetter* InitializeNewsClass(tNewsLetter* n);

void FreeTestNewsLetter(tNewsLetter* n);

#endif