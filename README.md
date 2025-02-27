### Copyright Oprisan Alexia-Ioana si Peiu Andreea - 314CA

# Tema 3 SD

## TASK 1

### add friend

Aceasta functie are rolul de a crea legaturi bidirectionale intre nodurile
grafului pentru a evidentia prietenia. Lista de adiacenta `friends` stocheaza
conexiunile realizate intre prieteni. Aceasta este parcursa si este creeata o
legatura intre doi utilizatori prin adaugarea unui nod in lista corespunzatoare
nodului de inceput.

### remove friend

Functia primeste ca parametri doi utilizatori si stearge legatura de prietenie
dintre acestia. Astfel, sunt parcurse listele de adiacenta asociate ambelor
id-uri si este eliminata conexiunea dintre cei doi.

### distance

Rolul acestei functii este de a verifica daca exista un drum intre cei doi
utilizatori si de a determina modalitatea cea mai scurta de a parcurge graful
de la un nod la celalalt, utilizand un algoritm pentru determinarea drumului
minim dintre doua noduri.

### suggestions

Sunt generate sugestii de prieteni pentru un utilizator dat pe baza retelei
de prietenii din graf. Functia gaseste si afiseaza toti prietenii prietenilor
care nu sunt deja prieteni cu utilizatorul dat.

### common

Functia afiseaza in ordine crescatoare, dupa id, toti prietenii comuni
ai doi utilizatori. Este parcursa lista de adiacenta si se cauta prietenii
comuni care vor fi afisati, listele asociate celor doi utilizatori fiind deja
sortate dupa id atunci cand este creeata prietenia.

### friends

Aceasta functie are rolul de a numara cati prieteni are un anumit utilizator.
Pe ecran este afisata dimensiunea listei asociate utilizatorului.

### popular

Este parcursa lista de adiacenta a unui utilizator si se verifica pentru
fiecare dintre prietenii sai cate conexiuni au. Dupa ce determina numarul
de legaturi pentru fiecare dintre acestia, este afisata persoana care are cele
mai multe.

## TASK 2

### create post

Aceasta functie creeaza o postare realocand memorie pentru vectorul de postari
in care acestea sunt stocate. O postare este de tip `struct post_t`, fiind
retinute mai multe informatii despre acestea, precum `titlu`, `user_id`
(utilizatorul care a creat postarea), `likes` (numarul de like-uri pe care le
strange postarea). De asemenea, este alocata memorie pentru un arbore generic
necesar pentru a stoca repost-urile.

### repost

Functia primeste ca parametri un nume, un post_id si optional un repost_id.
Este cautata in vectorul de structuri, postarea cu id-ul `post_id` cu ajutorul
functiei `find_post_by_id`. Daca sunt primiti doar primii doi parametri, atunci
este repostata postarea initiala si se adauga un nod in vectorul de copii al
root-ului. In caz contrar, este realizata o cautare prin arborele `events`
pentru a gasi nodul cu id-ul `repost_id` si este adaugat un nod in vectorul de
copii al nodului respectiv. Cautarea se realizeaza cu ajutorul functiei
implementate `find_node_by_id`.

### common-repost

Este cautat recursiv cel mai apropiat stramos al celor doua noduri oferite ca
parametri. Algoritmul este asemanator celui de `lowest common-ancestor`, dar
este adaptat pentru arborele generic `events`.

### like

Functia primeste ca parametri un nume, un post_id si optional un repost_id. In
structura fiecarui nod din arbore este retinut un vector de frecventa de
dimensiune `MAX_PEOPLE` pentru a retine daca utilizatorul a apreciat sau nu
postarea. Acest lucru este necesar pentru a decrementa numarul de like-uri in
cazul in care utilizatorul a apreciat deja postarea.

### ratio

Aceasta functie determina cea mai apreciata postare si verifica daca un repost
are mai multe like-uri decat postarea initiala, afisand un mesaj
corespunzator.

### delete

Functia sterge un repost sau o postare si toate nodurile descendente. Daca
este stearsa o postare, vectorul este shiftat la stanga cu o pozitie pentru
ca postarile ramase sa fie pe pozitii consecutive.

### get-likes

Afiseaza numarul de like-uri pe care le are o postare sau un repost.

### get-reposts

Afiseaza toate reposturile nodului oferit ca parametru. Este parcurs
arborele, recursiv, de la stanga la dreapta pentru fiecare nod. Daca se
apeleaza functia pentru postarea initiala, este afisat si titlul acesteia. De
asemenea, este afisat pentru fiecare repost persoana care l-a realizat.

### TASK 3

## feed

Parcurgerea se realizeaza de la finalul vectorului spre pozitia de inceput
pentru a ne asigura ca postarile afisate sunt cele mai recente. Cat timp
`feed_size` este nenul, verific daca user-ul este un prieten sau daca postarea
apartine user-ului cautat. Daca aceasta conditie este indeplinita, este
afisat numele utilizatorului impreuna cu titlul postarii realizate de acesta.

## view-profile

Acesta functie parcurge vectorul de postari si verifica daca acestea apartin
utilizatorului dorit si afiseaza titlul celor care satisfac aceasta conditie.
Apoi sunt parcursi toti arborii construiti in cautarea repost-urilor realizate
de acest utilizator.

## friends-repost

Functia parcurge arborele asociat postarii cu id-ul primit de la tastatura si
verifica recursiv pentru fiecare repost daca cel care a realizat repost-ul
este prieten cu utilizatorul oferit ca parametru.

## common-group

Functia cauta in lista de adiacenta a utilizatorului oferit ca parametru graful
complet cu cele mai multe noduri care este format. Algoritmul utilizat pentru
aceasta cautare se numeste `Bron Kerbosch`.