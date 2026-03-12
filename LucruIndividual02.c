//#include<stdio.h>
//#include<malloc.h>
//
//struct Pisica {
//	int id;
//	int varsta;
//	char* nume;
//	float greutate;
//	char culoareOchi;
//};
//struct Pisica initializare(int id, int varsta, char* nume, float greutate ,char culoareOchi) {
//	struct Pisica s;
//	s.id = id;
//	s.varsta = varsta;
//	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
//	strcpy(s.nume, nume);
//	s.greutate = greutate;
//	s.culoareOchi = culoareOchi;
//
//
//	return s;
//}
//
//void afisare(struct Pisica s) {
//	printf("Pisica cu id-ul: %d, varsta: %d, numele: %s, greutatea: %5.2f, culoarea ochilor: %c \n", s.id,s.varsta,s.nume,s.greutate,s.culoareOchi);
//}
//
//void afisareVector(struct Pisica* vector, int nrElemente) {
//	for (int i = 0; i < nrElemente; i++)
//	{
//		afisare(vector[i]);
//	}
//}
//
//
//
//void dezalocare(struct Pisica** vector, int* nrElemente) {
//	//dezalocam elementele din vector si vectorul
//	for (int i = 0; i < *nrElemente; i++)
//	{
//		free((*vector)[i].nume);
//	}
//
//	free(*vector);
//	*vector = NULL;
//	*nrElemente = 0;
//
//}
//
//
//struct Pisica* getCeaMaiBatranaPisica(struct Pisica* vector, int nrElemente) {
//	
//	int varsta = -1;
//	int iMax = -1;
//	for (int i = 0; i < nrElemente; i++)
//	{
//		if (vector[i].varsta > varsta) {
//			varsta = vector[i].varsta;
//			iMax = i;
//		}
//	}
//	return &(vector[iMax]);
//}
//
//float varstaMediePisici(struct Pisica* vector, int nrElemente) {
//	float medie = 0;
//	for (int i = 0; i < nrElemente; i++)
//	{
//		medie += (float)(vector[i].varsta);
//	}
//	return medie / nrElemente;
//
//}
//	
//
//
//int main() {
//
//	struct Pisica p;
//	p = initializare(1, 10, "Lola", 2, 'G');
//
//	
//	int numarDePisici = 3;
//	struct Pisica* pisici = (struct Pisica*)malloc(sizeof(struct Pisica) * numarDePisici);
//	pisici[0] = p;
//	pisici[1] = initializare(2, 1, "Remi", 5, 'B');
//	pisici[2] = initializare(3, 11, "Zuss", 6, 'Y');
//
//	//afisare(p);
//	afisareVector(pisici, numarDePisici);
//
//
//	printf("cea mai batrana pisica:\n");
//	
//	afisare(*getCeaMaiBatranaPisica(pisici, numarDePisici));
//
//	printf("Varsta medie pisici este: %.3f\n", varstaMediePisici(pisici, numarDePisici));
//
//	dezalocare(&pisici, &numarDePisici);
//
//	
//
//
//
//	return 0;
//}