Copyright Popa Victor-Andrei 313CAb 2023-2024

Scopul programului este de a realiza un editor photo pentru imagini de tip
PPM si PGM folosind diferite functionalitati care sunt explicate mai jos:

MAIN
In main ma folosesc de structurile image si rgb pt a stoca toate
caracteristicile unei poze.Ma folosesc de variabile ajutatoare cum ar fi
operatie,cuv,propozitie si destinatie.In operatie citesc primul cuvant care
reprezinta comanda si o verific pe rand daca este una din cele mentionate de
enunt si in caz contrar afisez comanda invalida.La comenzile care necesita
parametrii dupa ei,citesc restul liniei cu fgets si apoi aflu numarul de
cuvinte pt a verifica daca este egal cu nr de parametri doriti pt a executa
functia dorita.Unde am nevoie de nr intregi,folosesc functia sscanf si apoi
verific daca toate variabilele au luat valori cu succes.Dupa ce scriu exit,
folosesc functia eliberare totala pt a dealoca poza si reprezinta
finalizarea programului.In interiorul fiecarei functii principale,verific
daca a fost incarcata o poza sau nu dar si diferite cazuri invalide.

LOAD
Intai de toate,citiesc numele fisierului din care extrag si verific daca
acesta exista.Apoi dealoc poza veche in caz ca exita una.Folosesc
fctload pt a citi dintr-o poza magic wordul, lungimea si inaltimea,dimensiunea
maxima a unui pixel pt grayscale si rgb si apoi poza,pe care le incarc
intr-o structura.Ma folosesc de functia caz_magic pt a afla formatul unei
poze pt a stii daca deschid cu r sau rb fisierul.Apoi verific formatul pozei
din nou pt a stii daca poza este grayscale sau rgb.La final, inchid fisierul
si modific selectia la intreaga poza,modificand si variabila poza->ok cu 1
pt a spune ca s-a incarcat o poza.

SELECT
Folosesc selectare pt a modifica parametrii selectiei conform enuntului.Intai
verific daca a fost incarcata o poza inainte de a selecta si apoi verific daca
coordonatele sunt valabile si apoi le ordonez crescator.Pt SELECT ALL,utilizez
selectare totala si modific selectia inapoi la toata poza.

CROP
In functia crop,ma folosesc de o auxiliara pt a salva poza decupata dupa
selectie,apoi eliberez matricea pozei vechi si o egalez cu auxiliara,modificand
parametrii de selectie la intreaga poza.

SAVE
Folosesc salvare_ascii si salvare_binar ca sa incarc poza si caracteristicile
ei in functie de formatul dorit de input.

EQUALIZE
In functia egalizare,verific daca poza este grayscale si apoi
ma folosesc de un vector de frecventa pt fiecare pixel din poza.
Apoi fac suma frecventelor, o inmultesc cu 255 si impart la aria pozei
Folosesc double si apoi round pt a rotunji inapoi la un numar intreg,apoi
folosesc functia clamp pt a avea nr in intervalul 0,255.

HISTOGRAM
Aplic functia histogram,unde prima data,pe langa cazurile invalide de baza,
verific daca poza este strict grayscale.Folosesc sscanf pt a schimba din char
in int numerele si apoi verific daca y este putere a lui 2 si daca apartine
intervalului 2,256.Calculez vectorul de frecventa pt fiecare pixel si apoi
suma a cate y frecvente si determin suma maxima.Determin numarul de stelute
pt fiecare pixel si afisez corespunzator enuntului.

APPLY
In functia apply verific daca sunt parametrii valizi si afisez Easy, Charlie
Chaplin daca imaginea este grayscale.Apoi folosesc un dintre functiile edge,
sharpen,blur si gaussian_blur pt a aplica efectul dorit selectiei.Pt fiecare
in parte,definesc matricea kernel corespunzatoare efectului si apoi inmultesc
fiecare vecin al unui pixel si pe el insusi cu elementele matricei kernel asa
cum se specifica in enunt.Pixelii de pe maginea pozei raman neschimbati.Pt
efectuarea acestor operatii,ma folosesc de o auxiliara,apoi impart la numarul
dorit fiecare pixel obtinut prin adunarea inmultirilor,egalez partea selectata
din matrice cu auxiliara si eliberez variabila auxiliara.

ROTATE
Pentru aceasta,folosesc rotate unde aplic sscanf pe elemntul citit pt a
verifica daca este numar intreg,apoi verific daca este multiplu de 90 pt a
putea efectua rotirea.Inlocuiesc unghiul cu restul impartirii lui la 360,
astfel incat daca unghiul este mai mare de 360,sa nu rotesc de mai multe ori
decat trebuie.In continuare,ma asigur ca lucrez pe selectie sau pe toata poza
si efectuez rotirea la 0 grade acolo unde este cazul.