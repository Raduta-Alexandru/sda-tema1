# RADUTA Alexandru - 315CB - Structuri de Date si Algoritmi - Tema 1

## Scopul temei

Tema 1 SDA are ca scop implementarea unui browser web cu mai multe taburi, fiecare tab avand istoric. Tema incepe de la definirea urmatoarelor structuri : 
- `struct page {int id; char url[50]; char *description;}`
- `struct tab {int id; struct page *currentPage; struct stack *backwardStack; struct stack *forwardStack;}`
- `struct browser {struct tab *current; struct tabsList list;}`

Tema doreste sa fie implementate urmatoarele comenzi pentru a putea face un browser functional :

1. **NEW_TAB** - Pentru a creea un tab nou
2. **CLOSE** - Pentru a inchide tabul curent
3. **OPEN ID** - Pentru a deschide un tab
4. **NEXT** - Pentru a trece la urmatorul tab
5. **PREV** - Pentru a trece la ultimul tab
6. **PAGE ID** - Pentru a deschide in tabul curent o pagina
7. **BACKWARD** - Pentru a ne intoarce la ultima pagina
8. **FORWARD** - Pentru a ne duce la urmatoarea pagina
9. **PRINT** - Pentru a printa idurile taburilor deschise si descrierea pagini din tabul deschis
10. **PRINT_HISTORY ID** - Pentru a arata tot istoricul unui tab

Tema foloseste conceptele de : *Lista simplu inlantuita*, *Lista dublu inlantuita* si *Stiva*. 

## Implementare Main

Functia Main incepe	prin a deschide fisierul de intrare `tema1.in`, din care citeste mai intai numarul de pagini care urmeaza sa fie citite. Aloca un vector de `TPage` de lungime n+1 pentru a putea stoca si pagina default, o aloca pe cea default si urmeaza sa citeasca din fisier si restul paginilor.

Dupa ce citeste paginile trece la comenzi, unde citeste mai intai cate comenzi o sa fie, urmand sa aloce un vector de stringuri, fiecare comanda fiind pusa in vector, iar fisierul `tema1.in` este inchis.

Browserul este alocat initial cu lista si tabul curent NULL si este creat un `int idNextTab` care incepe cu -1, si care este incrementat mereu cand este creat un tab nou, pentru a ii putea da idul corespunzator. Se deschid taburile -1 si 0, tabul cu idul -1 fiind santinela.

Este deschis fisierul de iesire `tema1.out` si este parcurs vectorul de comenzi. Functia care trebuie apelata pentru fiecare comanda este selectata folosind `if else` inlantuite si `strstr`. Pentru comenzile care au nevoie de parametri se foloseste si `atoi`.

Dupa ce sunt rulate toate comenzile este inchis fisierul `tema1.out`, este parcurs browserul, iar la fiecare `TCelulaTabsList` din lista este eliberat `backwardStack`, `forwardStack`, `TTab` si celula. Dupa ce parcurg `TTabsList`, eliberez browserul, vectorul de `TPage`, si fiecare descriere in parte si vectorul de comenzi.

## Implementare Functii Browser

1. `void NEW_TAB(TBrowser *TB, int *idTab, TPage *pagini)`

Functia verifica existenta browserului, iar apoi aloca un `TTab`, ii dau ca `id = *idTab`, initializez cu `NULL` stivele `backwardStack` si `forwardStack` si `currentPage = pagini` pentru a avea pagina default. Dupa creez celula de `TTabsList`, ii pun in `info` tabul creat, ii fac legaturile, setez `current` si incrementez `*idTab`.

2. `void CLOSE(TBrowser *TB, FILE *output)`

Functia verifica existena browserului si daca tabul curent este diferit de tabul 0, caz in care se afiseaza `403 Forbidden\n`. In cazul in care nu este, parcurge `TTabsList` pana ajunge la tabul precedent, il seteaza `current`, trece la tabul care trebuie sters, reface legaturile si elibereaza `backwardStack`, `forwardStack`, `TTab` si celula.

3. `void OPEN(TBrowser *TB, FILE *output, int id)`

Functia verifica existenta browserului si daca idul la care se muta este diferit de -1. Parcurge `TTabsList` pana cand gasesc tabul cu idul `id` sau cand ajunge la santinela. Daca ajunge la tabul cautat il face `current`, iar daca nu afiseaza `403 Forbidden\n`.

4. `void NEXT(TBrowser *TB)`

Functia verifica existenta browserului, dupa care parcurge `TTabsList` pana ajunge la `current`. Daca tabul urmator este santinela cand sare la urmatorul tab sare peste santinela, alfel seteaza `current` pe `p->next->info`.

5. `void PREV(TBrowser *TB)`

Functia verifica existenta browserului, dupa care parcurge `TTabsList` pana ajunge la `current`. Daca tabul precedent este santinela cand sare la tabul precedent sare peste santinela, alfel seteaza `current` pe `p->prev->info`.

6. `void PAGE(TBrowser *TB, TPage *pagini, int nrPagini, FILE *output, int id)`

Functia parcurge vectorul `pagini`. Daca nu gaseste `TPage` cu idul egal cu `id` returneaza eroare prin mesajul `403 Forbidden\n`. Daca este gasita, atunci elibereaza stiva `forwardStack`, pune pe stiva `backwardStack` pagina curenta si seteaza ca pagina curenta pagina gasita.

7. `void BACKWARD(TBrowser *TB, FILE *output)`

Functia verifica daca istoricul este gol, iar in acesta caz afiseaza mesajul de eroare `403 Forbidden\n`. Daca istoricul nu este gol, atunci pune pe stiva `forwardStack` pagina curenta si scoate din `backwardStack` prima pagina si o seteaza ca pagina curenta.

8. `void FORWARD(TBrowser *TB, FILE *output)`

Functia verifica daca istoricul este gol, iar in acesta caz afiseaza mesajul de eroare `403 Forbidden\n`. Daca istoricul nu este gol, atunci pune pe stiva `backwardStack` pagina curenta si scoate din `forwardStack` prima pagina si o seteaza ca pagina curenta.

9. `void PRINT(TBrowser *TB, FILE *output)`

Functia verifica existenta browserului, dupa care parcurge `TTabsList` pana ajunge la `current`, iar apoi continua prin a printa idul lui `current`. Dupa ce printeaza primul id, parcurge in continuare `TTabsList`, verificand sa nu printeze santinela, si de asemenea sa sara peste santinela numai daca `current` nu este tabul 0, deoarece asta ar duce la o repetare a printari lui 0. Dupa ce termina de parcurs `TTabsList`, printeaza si descrierea lui `current->currentPage`.

10. `void PRINT_HISTORY(TBrowser *TB, FILE *output, int id)`

Functia verifica existenta browserului, dupa care parcurge `TTabsList` pana cand ajunge la tabul cu idul `id` sau pana cand ajunge la santinela. Daca ajunge la santinela se printeaza mesajul de eroare `403 Forbidden\n`, iar daca ajunge la tabul cautat se printeaza stiva `forwardStack` folosind functia auxiliara, apoi pagina curenta, iar la final stiva `backwardStack` folosind functia auxiliara.

## Implementare Functii Auxiliare

- `void freeStack(TStack *stack)`

Functia parcurge stiva `*stack`, salvand in `aux` celula curenta, trecand la urmatoarea si eliberand `aux`.

- `void pushStack(TStack *stack, TPage *page)`

Functia aloca o celula noua `aux`, careia ii sunt atribuite informatile din `page`, iar `aux` este facuta inceputul stivei.

- `TPage *popStack(TStack *stack)`

Functia verifica existenta stivei si daca ea are macar un element care sa fie scos. Este salvata informatia din prima celula a lui `*stack`, este schimbat inceputul listei, astfel incat el sa fie a doua celula, iar prima celula este eliberata, functia returnand informatia care era in ea.

- `void printForward(TStack stack, FILE *output)`

Functia parcurge recursiv stiva `stack`, mai intai se apeleaza pentru urmatoarea celula din stiva, iar apoi se printeaza informatia din celula, facand astfel o printare de la coada la cap.

- `void printBackward(TStack stack, FILE *output)`

Functia parcurge recursiv stiva `stack`, mai intai se printeaza informatia din celula, iar apoi se apeleaza pentru urmatoarea celula din stiva, facand astfel o printare de la cap la coada.
 