#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* stanga;
	Nod* dreapta;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(Nod* root) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza

	if (root) {
		int st = calculeazaInaltimeArbore(root->stanga);
		int dr = calculeazaInaltimeArbore(root->dreapta);
		return 1 + max(dr, st);
	}
	return 0;
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.

void rotireStanga(Nod** root) {
	Nod* aux = (*root)->dreapta;
	(*root)->dreapta = aux->stanga;
	aux->stanga = (*root);
	*root = aux;

}

void rotireDreapta(Nod** root) {
	Nod* aux = (*root)->stanga;
	(*root)->stanga = aux->dreapta;
	aux->dreapta = (*root);
	*root = aux;

}

int grad_echilibru(Nod* root) {
	return calculeazaInaltimeArbore(root->stanga) - calculeazaInaltimeArbore(root->dreapta);
}

void adaugaMasinaInArboreEchilibrat(Nod** root, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
	//dupa o anumita cheie pe care o decideti - poate fi ID
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->stanga = NULL;
	nou->dreapta = NULL;

	if (*root) {
		if (masinaNoua.id > (*root)->info.id) {
			adaugaMasinaInArboreEchilibrat(&(*root)->dreapta, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*root)->stanga, masinaNoua);
		}
		//incepe verificarea de echilibru
		int fec = grad_echilibru(*root);
		if (fec == -2) {
			if (grad_echilibru((*root)->dreapta) == 1) {
				rotireDreapta(&(*root)->dreapta);
			}
			rotireStanga(root);
		}
		else if (fec == 2) {
			if (grad_echilibru((*root)->stanga) == -1) {
				rotireStanga(&(*root)->stanga);
			}
			rotireDreapta(root);
		}

	}
	else {
		*root = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	Nod* root = NULL;

	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		adaugaMasinaInArboreEchilibrat(&root, citireMasinaDinFisier(f));
	}
	fclose(f);
	return root;
}

void afisareMasiniDinArbore(Nod* root) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->stanga);
		afisareMasiniDinArbore(root->dreapta);
	}
}

void dezalocareArboreDeMasini(Nod** root) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->stanga);
		dezalocareArboreDeMasini(&(*root)->dreapta);

		free((*root)->info.numeSofer);
		free((*root)->info.model);

		free(*root);
		*root = NULL;

	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/* arborele de masini */ int id);

int determinaNumarNoduri(/* arborele de masini */ );

float calculeazaPretTotal(/* arbore de masini */ );

float calculeazaPretulMasinilorUnuiSofer(/* arbore de masini */ const char* numeSofer);

int main() {

	Nod* root = NULL;
	root = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(root);
	return 0;
}