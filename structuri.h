/* RADUTA Alexandru - 315CB */
#ifndef _HOME_ALEXANDRU_FACULTATE_SDA_TEME_TEMA1_STRUCTURI_H_
#define _HOME_ALEXANDRU_FACULTATE_SDA_TEME_TEMA1_STRUCTURI_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct page {
    int id;
    char url[50];
    char *description;
} TPage;

typedef struct stack {
    TPage *info;
    struct stack *next;
} TCelulaStack, *TStack;

typedef struct tab {
    int id;
    TPage *currentPage;
    TStack backwardStack;
    TStack forwardStack;
} TTab;

typedef struct tabsList {
    TTab *info;
    struct tabsList *prev;
    struct tabsList *next;
} TCelulaTabsList, *TTabsList;

typedef struct browser {
    TTab *current;
    TTabsList list;
} TBrowser;

void freeStack(TStack *stack);
void pushStack(TStack *stack, TPage *page);
TPage *popStack(TStack *stack);
void printForward(TStack stack, FILE *output);
void printBackward(TStack stack, FILE *output);
void NEW_TAB(TBrowser *TB, int *idTab, TPage *pagini);
void CLOSE(TBrowser *TB, FILE *output);
void OPEN(TBrowser *TB, FILE *output, int id);
void NEXT(TBrowser *TB);
void PREV(TBrowser *TB);
void PAGE(TBrowser *TB, TPage *pagini, int nrPagini, FILE *output, int id);
void BACKWARD(TBrowser *TB, FILE *output);
void FORWARD(TBrowser *TB, FILE *output);
void PRINT(TBrowser *TB, FILE *output);
void PRINT_HISTORY(TBrowser *TB, FILE *output, int id);

#endif  // _HOME_ALEXANDRU_FACULTATE_SDA_TEME_TEMA1_STRUCTURI_H_
