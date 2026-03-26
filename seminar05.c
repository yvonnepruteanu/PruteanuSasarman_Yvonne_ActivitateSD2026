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

struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* previous;


};
typedef struct Nod Nod;

//creare structura pentru un nod dintr-o lista dublu inlantuita

//creare structura pentru Lista Dubla 

struct ListaDubla {
	int nrNoduri;
	Nod* First;
	Nod* Last;
};
typedef struct ListaDubla ListaDubla;

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

void afisareListaMasini(ListaDubla listaDubla) {
	printf("Lista contine %d noduri \n", listaDubla.nrNoduri);
	Nod* p = listaDubla.First;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void afisareInversaListaMasini(ListaDubla listaDubla) {
	Nod* p = listaDubla.Last;
	while (p) {
		afisareMasina(p->masina);
		p = p->previous;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* masina = (Nod*)malloc(sizeof(Nod));
	masina->masina = masinaNoua;
	masina->previous = lista->Last;
	masina->next = NULL;

	if (lista->Last)
	{
		lista->Last->next = masina;
	}
	else {
		lista->First = masina;
	}
	lista->Last = masina;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* masina = (Nod*)malloc(sizeof(Nod));
	masina->masina = masinaNoua;
	masina->previous = lista->Last;
	masina->next = NULL;

	if (lista->First)
	{
		lista->First->previous = masina;
	}
	else {
		lista->Last = masina;
	}
	lista->First = masina;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.Last = NULL;
	lista.First = NULL;
	lista.nrNoduri = 0;
	
	if (!f) {
		return lista;
	}
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
		
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	Nod* p = lista->First;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->masina.model) {
			free(aux->masina.model);
		}
		if (aux->masina.numeSofer) {
			free(aux->masina.numeSofer);
		}
		free(aux);
		lista->nrNoduri--;
	}
	lista->First = NULL;
	lista->First = NULL;
	
}

float calculeazaPretMediu(ListaDubla lista) {
	//calculeaza pretul mediu al masinilor din lista.
	float pret = 0;
	if (!lista.First) {
		return 0;
	}
	while (lista.First) {
		pret = pret + lista.First->masina.pret;
		lista.First = lista.First->next;
	}
	return pret/lista.nrNoduri;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	if (lista->First) {
		Nod* p;
		for (p = lista->First; p != NULL && p->masina.id!=id; p = p->next) {
			if (!p) return;
			else {
				if (p->previous == NULL) {
					if (p->next) {
						p->next->previous = NULL;
					}
					else {
						lista->Last = NULL;

					}
					lista->First = p->next;
					free(p->masina.model);
					free(p->masina.numeSofer);
					free(p);
				}
				else {
					p->previous->next = p->next;
					if (p->next) {
						p->next->previous = p->previous;
					}
					else {
						lista->Last = p->previous;

					}
					free(p->masina.model);
					free(p->masina.numeSofer);
					free(p);
				}
				lista->nrNoduri--;
				}
			}
		}
	
	
	
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	Nod* p;
	float max = 0;
	char* n = NULL;
	if (lista.First) {
		p = lista.First;
		while (p) {
			if (p->masina.pret > max) {
				max = p->masina.pret;
				n = p->masina.numeSofer;
			}
			p = p->next;
		}
	}
	//deepcopy
	char* nume = (char*)malloc(sizeof(char) * (strlen(n) + 1));
	strcpy_s(nume, strlen(n) + 1, n);
	return nume;
}

int main() {

	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	
	afisareListaMasini(lista);
	//dezalocareLDMasini(&lista);
	//printf("Dupa dezalocare: \n");
	//afisareListaMasini(lista);
	printf("Pret mediu: %5.2f\n", calculeazaPretMediu(lista));
	char* nom= getNumeSoferMasinaScumpa(lista);
	printf("Sofer: %s", nom);
	return 0;
}