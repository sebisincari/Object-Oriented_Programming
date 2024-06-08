#Harti
### Cerinta:
 O firma de software a primit cerinta de a scrie o aplicatie care determina drumul
minim intre doua orase alese dintr-o regiune, folosind sistemul de strazi existent.
Diferite regiuni au diferete sisteme de strazi, unele putand forma un graf linie, altele un
arbore, altele un dag, altele un graf general si altele un graf complet. Fiecare oras are
coordonatele (x,y) si drumul dintre doua orase (daca exista strada intre ele) este
distanta plana (euclidiana) intre ele. Firma de software ruleaza algoritmul pe propriile
servere, tinand datele pentru o serie de regiuni, pentru care primeste cereri de la clienti
(drum minim de la A la B regiunea R; introduce drum intre A si B in regiunea R; creaza o
regiune). Algoritmul trebuie sa ruleze cat de cat rapid avand in vedere tipul de graf
asociat.
Precizari:

   Drumul minim returnat consta doar in distanta totala de parcurs.
   Observatii: Intr-un graf complet drumul minim de la A la B este fix strada
AB. Intr-un arbore exista un singur drum intre doua noduri (care se poate
calcula tinand cont de stramosul comun). Intr-un graf linie costul
drumului minim se poate calcula in O(1) usor.
   Strazile sunt bidirectionale, mai putin in dag.
  
## Acest proiect are 3 abordari de codare
### 1.Abordare non OOP
Prima sursa `main_1.cpp` este un cod scris fara vreo notiune oop, realizat complet fara STL, 
folosind `struct` pentru a crea diverse containere.
<br/>
### 2.Abordare OOP medie
A doua sursa `main_2.cpp` se remarca prin upgrade ul codului din prima sursa si crearea diferitelor clase in functie de tipul grafului introdus.
Astfel obiectele acum au metode specifice. Ca tehnici OOP am folosit `override`, `virtualizare`, `supraincarcare de functii si de operatori` si `mostenire`.

### 3.Abordare totala OOP
A treia sursa `main_3.cpp` foloseste concepte avansate de OOP cum ar fi `design paterns`,`intefata`, `upcasting` si  `tratarea exceptiilor`
  
