#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod Nod;

struct Nod {
	Masina info;
	Nod* next;
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

void afisareListaMasini(Nod* primul) {
	while(primul != NULL) {
		afisareMasina(primul->info);
		primul = primul->next;

	}
}

void adaugaMasinaInLista(Nod** primul, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = masinaNoua;
	nod->next = NULL;
	if (*primul == NULL) {
		*primul = nod;
	}else{
		Nod* aux = *primul;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nod;
	}

}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = masinaNoua;
	nod->next = *cap;
	*cap = nod;

}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		Nod* prima = NULL;
		while (!feof(file)) {
			adaugaMasinaInLista(&prima,citireMasinaDinFisier(file));
		}
		fclose(file);
		return prima;
	}
	return NULL;
}

void dezalocareListaMasini(Nod**cap) {
	//sunt dezalocate toate masinile si lista de elemente
	while (*cap) {
		Nod* p = *cap;
		(*cap) = p->next;
		if (p->info.numeSofer) {
			free(p->info.numeSofer);
		}
		if (p->info.model) {
			free(p->info.model);
		}
		free(p);

	}
}

float calculeazaPretMediu(Nod* primul) {
	float suma = 0;
	int k = 0;

	while (primul) {
		suma += primul->info.pret;
		k++;
		primul = primul->next;
	}
	if (k > 0) {
		return suma / k;
	}
	else {
		return 0;
	}
	
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	while ((*cap) && (*cap)->info.serie == serieCautata) {
		
			Nod* temp = *cap;
			(*cap) = temp->next;
			if (temp->info.numeSofer) {
				free(temp->info.numeSofer);
			}
			if (temp->info.model) {
				free(temp->info.model);
			}
			free(temp);

		
	}
	
	Nod* p = *cap;
	while (p) {
		/*Nod* temp = *cap;
		(*cap) = temp->next;*/

		while (p->next && p->next->info.serie != serieCautata) {
			p = p->next;

		}
		if (p->next) {
			Nod* temp = p->next;
			p->next = temp->next;

			if (temp->info.numeSofer) {
				free(temp->info.numeSofer);
			}
			if (temp->info.model) {
				free(temp->info.model);
			}
			free(temp);

		}
		else {
			p = p->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod*prima, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float pretTotal = 0;
	while (prima) {
		if (strcmp(prima->info.numeSofer, numeSofer) == 0) {
			pretTotal += prima->info.pret;
			
		}

		prima = prima->next;
	}
	return pretTotal;
}

int main() {

	Nod* primul = NULL;
	primul = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(primul);
	//dezalocareListaMasini(&primul);
	//afisareListaMasini(primul);
	printf("stergere seria A\n");
	stergeMasiniDinSeria(&primul, 'A');
	afisareListaMasini(primul);

	Masina mas;
	mas.id = 11;
	mas.model = "Sportage";
	mas.nrUsi = 2;
	mas.numeSofer = "Yvonne";
	mas.pret = 2700;
	mas.serie = 'A';

	printf("\n\nDupa adaugare la inceput:\n");
	adaugaLaInceputInLista(&primul, mas);
	afisareListaMasini(primul);

	printf("\n\nSuma pt Ionescu:\n");
	char nume[8] = "Ionescu";
	printf("%5.2f", calculeazaPretulMasinilorUnuiSofer(primul, nume));
	return 0;
}