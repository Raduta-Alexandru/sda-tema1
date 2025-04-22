/* RADUTA Alexandru - 315CB */

#include "./structuri.h"
#define kMaxLineLength 100
#define kError "403 Forbidden\n"

void freeStack(TStack *stack) {
    // parcurg stackul
    while (*stack) {
        // salvez adresa celulei
        TStack aux = *stack;
        // trec la urmatoarea celula
        *stack = (*stack)->next;
        // eliberez celula
        free(aux);
    }
}

void pushStack(TStack *stack, TPage *page) {
    // imi aloc o celula
    TStack aux = malloc(sizeof(TCelulaStack));
    // verific alocarea
    if (aux == NULL) {
        perror("Eroare la alocarea stackului");
        return;
    }
    // ii atribui datele
    aux->info = page;
    // adaug la inceputul stackului
    aux->next = *stack;
    *stack = aux;
}

TPage *popStack(TStack *stack) {
    // verific ca exista stackul si informatia
    if (!stack || !*stack) {
        return NULL;
    }
    TPage *pagina = (*stack)->info;
    // imi salvez inceputul stackului
    TStack aux = *stack;
    // mut inceputul vechi al stackului
    *stack = aux->next;
    // eliberez vechiul inceput
    free(aux);
    return pagina;
}

void printForward(TStack stack, FILE *output) {
    // daca exista element apelez recursiv si dupa printez la ce am ajuns
    if (stack) {
        printForward(stack->next, output);
        fprintf(output, "%s\n", stack->info->url);
    }
}

void printBackward(TStack stack, FILE *output) {
    // daca exista element printez la ce am ajuns si dupa apelez recursiv
    if (stack) {
        fprintf(output, "%s\n", stack->info->url);
        printBackward(stack->next, output);
    }
}

void NEW_TAB(TBrowser *TB, int *idTab, TPage *pagini) {
    // verific sa existe browser
    if (TB == NULL) {
        return;
    }
    // imi aloc un Tab
    TTab *newTab = malloc(sizeof(TTab));
    if (newTab == NULL) {
        perror("Eroare la alocarea tabului");
        return;
    }
    newTab->id = *idTab;
    newTab->currentPage = pagini;
    newTab->backwardStack = NULL;
    newTab->forwardStack = NULL;
    // imi aloc o celula pentru lista
    TTabsList aux = malloc(sizeof(TCelulaTabsList));
    if (aux == NULL) {
        perror("Eroare la alocarea tabului");
        return;
    }
    aux->info = newTab;
    if (TB->list == NULL) {
        TB->list = aux;
        aux->next = aux;
        aux->prev = aux;
    } else {
        TTabsList l = TB->list;
        // parcurg pana ajung la final
        while (l->next->info->id != -1) {
            l = l->next;
        }
        // fac legaturile
        l->next = aux;
        aux->prev = l;
        aux->next = TB->list;
        TB->list->prev = aux;
    }
    // ii dau browserului tabul nou creat
    TB->current = newTab;
    // incrementez idul
    (*idTab)++;
}

void CLOSE(TBrowser *TB, FILE *output) {
    // verific sa existe browserul
    if (TB == NULL || TB->list == NULL) {
        return;
    }
    // verific sa nu fie pagina 0
    if (TB->current->id == 0) {
        fprintf(output, kError);
        return;
    }
    // gasesc care este tabul precedent lui
    TTabsList p = TB->list;
    while (p->next->info != TB->current) {
        p = p->next;
    }
    // am ajuns la tabul precedent, asa ca il fac tabul nou
    TB->current = p->info;
    p = p->next;
    // eliberez tabul PENTRU VALGRIND
    // refac legaturile
    p->prev->next = p->next;
    p->next->prev = p->prev;
    freeStack(&(p->info->backwardStack));
    freeStack(&(p->info->forwardStack));
    free(p->info);
    free(p);
}

void OPEN(TBrowser *TB, FILE *output, int id) {
    // verific sa existe browser
    if (TB == NULL || TB->list == NULL) {
        return;
    }
    // verific sa nu imi ceara sa ma mut la santinela
    if (id == -1) {
        return;
    }
    // sar peste santinela
    TTabsList p = TB->list->next;
    // parcurg taburile pana gasesc id-ul, sau pana ma intorc la santinela
    while ((p->info->id != id) && (p->info->id != -1)) {
        p = p->next;
    }
    // verific daca am gasit tabul sau daca am ajuns la santinela
    if (p->info->id == id) {
        // am ajuns la tabul cautat
        TB->current = p->info;
    } else {
        // tabul cautat nu exista
        fprintf(output, kError);
    }
}

void NEXT(TBrowser *TB) {
    // verific sa existe browser
    if (TB == NULL || TB->list == NULL) {
        return;
    }
    // parcurg taburile pana gasesc pe cel curent, sarind peste santinela
    TTabsList p = TB->list->next;
    while (p->info != TB->current) {
        p = p->next;
    }
    // verific ca urmatorul tab sa nu fie santinela, si daca este o sar
    if (p->next->info->id == -1) {
        // urmatorul tab e santinela
        p = p->next;
    }
    TB->current = p->next->info;
}

void PREV(TBrowser *TB) {
    // verific sa existe browser
    if (TB == NULL || TB->list == NULL) {
        return;
    }
    // parcurg taburile pana gasesc pe cel curent, sarind peste santinela
    TTabsList p = TB->list->next;
    while (p->info != TB->current) {
        p = p->next;
    }
    // verific ca tabul precedent sa nu fie santinela, si daca este o sar
    if (p->prev->info->id == -1) {
        // urmatorul tab e santinela
        p = p->prev;
    }
    TB->current = p->prev->info;
}

void PAGE(TBrowser *TB, TPage *pagini, int nrPagini, FILE *output, int id) {
    // verific daca este un id bun si il gasesc
    TPage *pagina = NULL;
    for (int i = 0; i < nrPagini; i++) {
        if (pagini[i].id == id) {
            pagina = &(pagini[i]);
            break;
        }
    }
    // nu a fost gasit idul
    if (pagina == NULL) {
        fprintf(output, kError);
        return;
    }
    // eliberez stiva forward
    freeStack(&(TB->current->forwardStack));
    // pun pe backward pagina curenta
    pushStack(&(TB->current->backwardStack), TB->current->currentPage);
    // pun ca pagina curenta pagina cu idul corespunzator
    TB->current->currentPage = pagina;
}

void BACKWARD(TBrowser *TB, FILE *output) {
    // verific daca istoricul este gol
    if (TB->current->backwardStack == NULL) {
        fprintf(output, kError);
        return;
    }
    // introduc in forward pagina curenta
    pushStack(&(TB->current->forwardStack), TB->current->currentPage);
    // pun ca pagina curenta prima pagina din backward stack
    TB->current->currentPage = popStack(&(TB->current->backwardStack));
}

void FORWARD(TBrowser *TB, FILE *output) {
    // verific daca istoricul este gol
    if (TB->current->forwardStack == NULL) {
        fprintf(output, kError);
        return;
    }
    // introduc in backward pagina curenta
    pushStack(&(TB->current->backwardStack), TB->current->currentPage);
    // pun ca pagina curenta prima pagina din stiva forward
    TB->current->currentPage = popStack(&(TB->current->forwardStack));
}

void PRINT(TBrowser *TB, FILE *output) {
    // verific sa existe browser
    if (TB == NULL || TB->list == NULL) {
        return;
    }
    TTabsList p = TB->list;
    while (p->info != TB->current) {
        p = p->next;
    }
    fprintf(output, "%d", TB->current->id);
    p = p->next;
    while (p->info != TB->current) {
        if (p->info->id == -1) {
            // verific pentru cazul de santinela sa fie doar
            // santinela si un singur tab deschis
            if (p->next->info != TB->current) {
                p = p->next;
            } else {
                break;
            }
        }
        fprintf(output, " %d", p->info->id);
        p = p->next;
    }
    fprintf(output, "\n%s", TB->current->currentPage->description);
}

void PRINT_HISTORY(TBrowser *TB, FILE *output, int id) {
    // verific sa existe browser
    if (TB == NULL || TB->list == NULL) {
        return;
    }
    // verific sa nu imi ceara sa ma mut la santinela
    if (id == -1) {
        return;
    }
    // sar peste santinela
    TTabsList p = TB->list->next;
    // parcurg taburile pana gasesc id-ul, sau pana ma intorc la santinela
    while ((p->info->id != id) && (p->info->id != -1)) {
        p = p->next;
    }
    // verific daca am gasit tabul sau daca am ajuns la santinela
    TTab *tab = p->info;
    if (p->info->id == id) {
        // am ajuns la tabul cautat
        tab = p->info;
    } else {
        // tabul cautat nu exista
        fprintf(output, kError);
        return;
    }
    // printez stiva forward
    printForward(tab->forwardStack, output);
    // printez pagina curenta
    fprintf(output, "%s\n", tab->currentPage->url);
    // printez stiva backward
    printBackward(tab->backwardStack, output);
}

int main() {
    // citesc fisierul de intrare
    FILE *input = fopen("tema1.in", "r");
    if (input == NULL) {
        perror("Eroare la deshiderea fisierului de input");
        return 1;
    }
    // imi fac un vector pentru citirea fiecarei linie din input
    char linie[kMaxLineLength];
    // citesc cate pagini o sa am
    fgets(linie, kMaxLineLength, input);
    int nrPagini = atoi(linie) + 1;
    // imi aloc vectorul de pagini
    TPage *pagini = malloc(nrPagini * sizeof(TPage));
    if (pagini == NULL) {
        perror("Eroare la alocarea vectorului de pagini");
        return 1;
    }
    // imi declar pagina 0
    pagini[0].id = 0;
    strncpy(pagini[0].url, "https://acs.pub.ro/", 20);
    pagini[0].description = malloc(18 * sizeof(char));
    if (pagini[0].description == NULL) {
        perror("Eroare la alocarea spatiului pentru descriere");
        return 1;
    }
    strncpy(pagini[0].description, "Computer Science\n", 18);
    for (int i = 1; i < nrPagini; i++) {
        // citesc idul
        fgets(linie, kMaxLineLength, input);
        pagini[i].id = atoi(linie);
        // citesc url-ul
        fgets(linie, kMaxLineLength, input);
        // elimin \n
        if (strchr(linie, '\n')) {
            linie[strlen(linie) - 1] = '\0';
        }
        strncpy(pagini[i].url, linie, strlen(linie) + 1);
        // citesc descrierea
        fgets(linie, kMaxLineLength, input);
        pagini[i].description = malloc(strlen(linie) + 1);
        if (pagini[i].description == NULL) {
            perror("Eroare la alocarea spatiului pentru descriere");
            return 1;
        }
        strncpy(pagini[i].description, linie, strlen(linie) + 1);
    }
    // citesc numarul de comenzi care urmeaza sa fie rulate
    fgets(linie, kMaxLineLength, input);
    int nrComenzi = atoi(linie);
    char **comenzi = malloc(nrComenzi * sizeof(char *));
    if (comenzi == NULL) {
        perror("Eroare la alocarea vectorului de comenzi");
        return 1;
    }
    for (int i = 0; i < nrComenzi; i++) {
        fgets(linie, kMaxLineLength, input);
        // elimin \n
        if (strchr(linie, '\n')) {
            linie[strlen(linie) - 1] = '\0';
        }
        // aloc spatiu pentru comanda
        comenzi[i] = malloc(strlen(linie) + 1);
        if (comenzi[i] == NULL) {
            perror("Eroare la alocarea spatiului pentru descriere");
            return 1;
        }
        strncpy(comenzi[i], linie, strlen(linie) + 1);
    }
    fclose(input);
    // imi aloc browserul
    TBrowser *browser = malloc(sizeof(TBrowser));
    if (browser == NULL) {
        perror("Eroare la alocarea spatiului pentru browser");
        return 1;
    }
    browser->list = NULL;
    browser->current = NULL;
    // imi creez un int, care retine idul pentru urmatorul tab
    // si il initializez cu -1, pentru a ii da id santinelei
    int idNextTab = -1;
    // creez santinela si tabul 0
    NEW_TAB(browser, &idNextTab, pagini);
    NEW_TAB(browser, &idNextTab, pagini);
    // imi deschid fisierul de output pentru functii de print
    FILE *output = fopen("tema1.out", "w");
    for (int i = 0; i < nrComenzi; i++) {
        if (strstr(comenzi[i], "NEW_TAB")) {
            NEW_TAB(browser, &idNextTab, pagini);
        } else if (strstr(comenzi[i], "CLOSE")) {
            CLOSE(browser, output);
        } else if (strstr(comenzi[i], "OPEN")) {
            OPEN(browser, output, atoi(comenzi[i] + 5));
        } else if (strstr(comenzi[i], "NEXT")) {
            NEXT(browser);
        } else if (strstr(comenzi[i], "PREV")) {
            PREV(browser);
        } else if (strstr(comenzi[i], "PAGE")) {
            PAGE(browser, pagini, nrPagini, output, atoi(comenzi[i] + 5));
        } else if (strstr(comenzi[i], "BACKWARD")) {
            BACKWARD(browser, output);
        } else if (strstr(comenzi[i], "FORWARD")) {
            FORWARD(browser, output);
        } else if (strstr(comenzi[i], "PRINT_HISTORY")) {
            PRINT_HISTORY(browser, output, atoi(comenzi[i] + 14));
        } else if (strstr(comenzi[i], "PRINT")) {
            PRINT(browser, output);
        } else {
            perror("Comanda gresita");
        }
    }
    fclose(output);
    // eliberez spatiul
    // eliberez toate taburile
    // parcurg lista de taburi
    // eliberez stivele, info si celula in sine
    TTabsList p = browser->list;
    do {
        TTabsList aux = p;
        p = p->next;
        freeStack(&(aux->info->backwardStack));
        freeStack(&(aux->info->forwardStack));
        free(aux->info);
        free(aux);
    } while (p != browser->list);
    // eliberez browserul
    free(browser);
    // eliberez vectorul de pagini
    for (int i = 0; i < nrPagini; i++) {
        free(pagini[i].description);
    }
    free(pagini);
    // eliberez vectorul de comenzi
    for (int i = 0; i < nrComenzi; i++) {
        free(comenzi[i]);
    }
    free(comenzi);
    return 0;
}
