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
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(Nod** root, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if (*root) {
		if ((*root)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&((*root)->dreapta), masinaNoua);
		}
		if((*root)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*root)->stanga), masinaNoua);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*root = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Nod* root = NULL;
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArbore(&root, m);
	}
	fclose(file);
	return root;
}

void afisareMasiniDinArborePreordine( Nod* root) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArborePreordine((root->stanga));
		afisareMasiniDinArborePreordine((root->dreapta));
	}
}

void dezalocareArboreDeMasini(Nod** root) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->stanga);
		dezalocareArboreDeMasini(&(*root)->dreapta);
		free((*root)->info.model);
		free((*root)->info.numeSofer);
		free((*root));
		*root = NULL;
	}
	
}

Masina getMasinaByID(Nod* root, int id) {
	Masina m;
	m.id = -1;
	if (root) {
		if (root->info.id == id) {
			m = root->info;
			m.model = (char*)malloc(strlen(root->info.model) + 1);
			strcpy_s(m.model, strlen(root->info.model) + 1, root->info.model);
			m.numeSofer = (char*)malloc(strlen(root->info.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(root->info.numeSofer) + 1, root->info.numeSofer);
			

		}
		else if (root->info.id > id) {
			return getMasinaByID(root->stanga, id);
		}
		else {
			return getMasinaByID(root->dreapta, id);
		}
	}
	return m;
}

int determinaNumarNoduri(/*arborele de masini*/ Nod* root) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	
	if (root) {
		int stanga =  determinaNumarNoduri(root->stanga);
		int dreapta = determinaNumarNoduri(root->dreapta);
		return stanga + dreapta + 1;
	}
	return 0;
}

int calculeazaInaltimeArbore(/*arbore de masini*/Nod* root) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (root) {
		int inaltimes = calculeazaInaltimeArbore(root->stanga);
		int inaltimed = calculeazaInaltimeArbore(root->dreapta);
		return max(inaltimes, inaltimed) + 1;
	}

	return 0;
}

float calculeazaPretTotal(/*arbore de masini*/Nod* root) {
	//calculeaza pretul tuturor masinilor din arbore.
	if (root) {
		float ps= calculeazaPretTotal(root->stanga);
		float pd = calculeazaPretTotal(root->dreapta);
		return root->info.pret + pd + ps;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/Nod* root, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	if (root) {
		float ps = calculeazaPretulMasinilorUnuiSofer(root->stanga, numeSofer);
		float pd = calculeazaPretulMasinilorUnuiSofer(root->dreapta, numeSofer);
		
		if (strcmp(root->info.numeSofer, numeSofer) == 0) {
			
			return root->info.pret + pd + ps;
		}
		else {
			return pd + ps;
		}
		
	}
	return 0;
}

int main() {

	Nod* root = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareMasiniDinArborePreordine(root);
	printf("===============\n");
	afisareMasina(getMasinaByID(root, 7));
	printf("===============\n");
	printf("Nr noduri : %d\n", determinaNumarNoduri(root));
	printf("===============\n");
	printf("Inalt : %d\n",  calculeazaInaltimeArbore(root));
	printf("===============\n");
	printf("Pret total : %5.2f\n", calculeazaPretTotal(root));
	printf("===============\n");
	printf("Pret sofer : %5.2f\n", calculeazaPretulMasinilorUnuiSofer(root,"Ionescu"));
	
	printf("===============\n");
	dezalocareArboreDeMasini(&root);
	afisareMasiniDinArborePreordine(root);
	return 0;
}