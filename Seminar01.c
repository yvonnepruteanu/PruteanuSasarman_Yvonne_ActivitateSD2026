#include<stdio.h>
#include<stdlib.h>

struct Produs {
	int id;
	int stoc;
	char* denumire;
	float pret;
	char categorie;
};

struct Produs initializare(int stoc, char* denumire, float pret, char categorie, int id) {
	struct Produs s;
	s.id = id;
	s.stoc = stoc;

	//
	s.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(s.denumire, denumire);

	s.pret = pret;
	s.categorie = categorie;
	//initializare structura 
	return s;
}

void afisare(struct Produs s) {
	printf("\nId:%d \n", s.id);
	printf("Stoc:%d \n", s.stoc);
	printf("Denumire:%s \n", s.denumire);
	printf("Pret:%5.2f \n", s.pret);
	printf("Categorie:%c \n", s.categorie);
}

void modifica_Denumire(struct Produs* s, char* noua_denumire) {
	free((*s).denumire);
	s->denumire = (char*)malloc(sizeof(char) * (strlen(noua_denumire) + 1));
	strcpy(s->denumire, noua_denumire);
}

void dezalocare(struct Produs* s) {
	if (s->denumire != NULL) {
		free(s->denumire);
		s->denumire = NULL;
	}
}
float calculeazaTotal(struct Produs p) {
	return p.stoc * p.pret;
}

int main() {
	struct Produs s;
	//char* c;
	//printf("Un pointer ocupa: %d", sizeof(c));

	s = initializare(20, "Telefon", 2000, 'A', 1);
	afisare(s);
	modifica_Denumire(&s, "Mouse");
	afisare(s);
	printf("Valoarea totala pentru %s este %5.2f RON", s.denumire, calculeazaTotal(s));



	dezalocare(&s);

	return 0;
}