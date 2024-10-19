//Copyright Popa Victor-Andrei 313CAb 2023-2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct rgb {
	int r, g, b;
} rgb;//structura pt poza color

typedef struct image {
	int maxdim, **pixel;
	rgb **pixel_rgb;
	char mw[5];
	int n, m;
	int ok;
	int x1, x2, y1, y2;
} image;//structura pt caracteristicile unei poze + poza

char caz_magic(char *fisier, image *poza)
{//returneaza daca o poza este in format text sau binar si o fol in LOAD
	char tip;
	FILE *fp = fopen(fisier, "rb");
	if (!fp) {
		printf("Failed to load %s\n", fisier);
		return 0;
	}
	fscanf(fp, "%s", poza->mw);
	fclose(fp);
	if (poza->mw[1] <= '3')
		tip = 't';
	else
		tip = 'b';
	return tip;
}

void load(image *poza)
{//functia de incarcare a pozelor
	char fisier[100];
	scanf("%s", fisier);
	char tip = caz_magic(fisier, poza);
	if (tip == 't') {
		FILE *fp = fopen(fisier, "r");
		fscanf(fp, "%s", poza->mw);
		fscanf(fp, "%d %d", &poza->n, &poza->m);
		fscanf(fp, "%d\n", &poza->maxdim);
		poza->ok = 1;
		if (poza->mw[1] == '2') {//load pt gri ascii
			int **aux = malloc(poza->m * sizeof(int *));
			for (int i = 0; i < poza->m; i++)
				aux[i] = malloc(poza->n * sizeof(int));
			poza->pixel = aux;
			for (int i = 0; i < poza->m; i++)
				for (int j = 0; j < poza->n; j++)
					fscanf(fp, "%d", &poza->pixel[i][j]);
		} else {//load pt rgb ascii
			rgb **aux = malloc(poza->m * sizeof(rgb *));
			for (int i = 0; i < poza->m; i++)
				aux[i] = malloc(poza->n * sizeof(rgb));
			poza->pixel_rgb = aux;
			for (int i = 0; i < poza->m; i++)
				for (int j = 0; j < poza->n; j++) {
					fscanf(fp, "%d", &poza->pixel_rgb[i][j].r);
					fscanf(fp, "%d", &poza->pixel_rgb[i][j].g);
					fscanf(fp, "%d", &poza->pixel_rgb[i][j].b);
				}
		}
		fclose(fp);
		printf("Loaded %s\n", fisier);//readuc selectia la toata poza
		poza->x1 = 0; poza->y1 = 0; poza->y2 = poza->m; poza->x2 = poza->n;
	} else if (tip == 'b') {
		FILE *fp = fopen(fisier, "rb");
		fscanf(fp, "%s", poza->mw);
		fscanf(fp, "%d %d", &poza->n, &poza->m);
		fscanf(fp, "%d\n", &poza->maxdim);
		poza->ok = 1;
		if (poza->mw[1] == '5') {//load pt gri binar
			int **aux = malloc(poza->m * sizeof(int *));
			for (int i = 0; i < poza->m; i++)
				aux[i] = malloc(poza->n * sizeof(int));
			poza->pixel = aux;
			unsigned char buffer;
			for (int i = 0; i < poza->m; i++)
				for (int j = 0; j < poza->n; j++) {
					fread(&buffer, sizeof(char), 1, fp);
					poza->pixel[i][j] = (unsigned char)buffer;
				}
		} else {//load pt rgb binar
			rgb **aux = malloc(poza->m * sizeof(rgb *));
			for (int i = 0; i < poza->m; i++)
				aux[i] = malloc(poza->n * sizeof(rgb));
			poza->pixel_rgb = aux;
			unsigned char buffer;
			for (int i = 0; i < poza->m; i++)
				for (int j = 0; j < poza->n; j++) {
					fread(&buffer, sizeof(char), 1, fp);
					poza->pixel_rgb[i][j].r = (unsigned char)buffer;
					fread(&buffer, sizeof(char), 1, fp);
					poza->pixel_rgb[i][j].g = (unsigned char)buffer;
					fread(&buffer, sizeof(char), 1, fp);
					poza->pixel_rgb[i][j].b = (unsigned char)buffer;
				}
		}
		fclose(fp);
		printf("Loaded %s\n", fisier);//readuc selectia la toata poza
		poza->x1 = 0; poza->y1 = 0; poza->y2 = poza->m; poza->x2 = poza->n;
	}
}

void eliberare_poza(image *poza)
{//eliberez poza in functie de formatul ei
	if (poza->mw[1] == '2' || poza->mw[1] == '5') {
		for (int i = 0; i < poza->m; i++)
			free(poza->pixel[i]);
		free(poza->pixel);
	} else if (poza->mw[1] == '3' || poza->mw[1] == '6') {
		for (int i = 0; i < poza->m; i++)
			free(poza->pixel_rgb[i]);
	free(poza->pixel_rgb);
	}
}

void selectare(image *poza, int x1, int y1, int x2, int y2)
{//selectez coordonatele conform enuntului si le ordonez crescator
	int aux;
	if (poza->ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (x1 == x2 || y1 == y2) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (x1 > x2) {
		aux = x1; x1 = x2; x2 = aux;
	}
	if (y1 > y2) {
		aux = y1; y1 = y2; y2 = aux;
	}
	if (x1 < 0 || x2 > poza->n) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y1 < 0 || y2 > poza->m) {
		printf("Invalid set of coordinates\n");
		return;
	}
	poza->x1 = x1; poza->y1 = y1; poza->y2 = y2; poza->x2 = x2;
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

void selectare_totala(image *poza)
{//readuc selectia la toata poza
	if (poza->ok == 0) {
		printf("No image loaded\n");
		return;
	}
	poza->x1 = 0; poza->y1 = 0; poza->y2 = poza->m; poza->x2 = poza->n;
	printf("Selected ALL\n");
}

void crop(image *poza)
{//decupez imaginea dupa selectie si modific selectia la noile dimensiuni
	if (poza->ok != 1) {
		printf("No image loaded\n");
		return;
	}
	if (poza->mw[1] == '2' || poza->mw[1] == '5') {
		int **aux = malloc((poza->y2 - poza->y1) * sizeof(int *));
		for (int i = 0; i < (poza->y2 - poza->y1); i++)
			aux[i] = malloc((poza->x2 - poza->x1) * sizeof(int));
		int i1 = 0, j1 = 0;
		for (int i = poza->y1; i < poza->y2; i++) {
			for (int j = poza->x1; j < poza->x2; j++) {
				aux[i1][j1] = poza->pixel[i][j];
				j1++;
			}
			i1++; j1 = 0;
		}
		for (int i = 0; i < poza->m; i++)
			free(poza->pixel[i]);
		free(poza->pixel);
		poza->pixel = aux;
	} else if (poza->mw[1] == '3' || poza->mw[1] == '6') {
		rgb **aux = malloc((poza->y2 - poza->y1) * sizeof(rgb *));
		for (int i = 0; i < poza->y2 - poza->y1; i++)
			aux[i] = malloc((poza->x2 - poza->x1) * sizeof(rgb));
		int i1 = 0, j1 = 0;
		for (int i = poza->y1; i < poza->y2; i++) {
			for (int j = poza->x1; j < poza->x2; j++) {
				aux[i1][j1].r = poza->pixel_rgb[i][j].r;
				aux[i1][j1].g = poza->pixel_rgb[i][j].g;
				aux[i1][j1].b = poza->pixel_rgb[i][j].b;
				j1++;
			}
			i1++; j1 = 0;
		} //dupa ce calculez aux,eliberez poza veche si pun partea decupata
		for (int i = 0; i < poza->m; i++)
			free(poza->pixel_rgb[i]);
		free(poza->pixel_rgb);
		poza->pixel_rgb = aux;
	}
	poza->m = poza->y2 - poza->y1;
	poza->n = poza->x2 - poza->x1;
	poza->x1 = 0; poza->y1 = 0; poza->y2 = poza->m; poza->x2 = poza->n;
	printf("Image cropped\n");
}

int nrcuv(const char *propozitie)
{//fct care returneaza nr de cuvinte dintr-o propozitie
	char copie[100];
	strcpy(copie, propozitie);
	char *p = strtok(copie, " ");
	int nr = 0;
	while (p) {
		nr++;
		p = strtok(NULL, " ");
	}
	return nr;
}

void salvare_ascii(image poza, char *destinatie)
{//salvez poza si caracteristicile ei in functie de formatul dorit
	if (poza.ok == 0) {
		printf("No image loaded\n");
		return;
	}
	FILE *fp = fopen(destinatie, "w");
	if (poza.mw[1] == '2' || poza.mw[1] == '5') {
		fprintf(fp, "P2\n");
		fprintf(fp, "%d %d\n", poza.n, poza.m);
		fprintf(fp, "%d\n", poza.maxdim);
		for (int i = 0; i < poza.m; i++) {
			for (int j = 0; j < poza.n; j++)
				fprintf(fp, "%d ", poza.pixel[i][j]);
			fprintf(fp, "\n");
		}
	} else if (poza.mw[1] == '3' || poza.mw[1] == '6') {
		fprintf(fp, "P3\n");
		fprintf(fp, "%d %d\n", poza.n, poza.m);
		fprintf(fp, "%d\n", poza.maxdim);
		for (int i = 0; i < poza.m; i++) {
			for (int j = 0; j < poza.n; j++) {
				fprintf(fp, "%d ", poza.pixel_rgb[i][j].r);
				fprintf(fp, "%d ", poza.pixel_rgb[i][j].g);
				fprintf(fp, "%d ", poza.pixel_rgb[i][j].b);
			}
			fprintf(fp, "\n");
		}
	}
	printf("Saved %s\n", destinatie);
	fclose(fp);
}

void salvare_binar(image poza, char *destinatie)
{//salvez poza si caracteristicile ei in functie de formatul dorit
	if (poza.ok == 0) {
		printf("No image loaded\n");
		return;
	}
	FILE *fp = fopen(destinatie, "wb");
	if (poza.mw[1] == '2' || poza.mw[1] == '5') {
		fprintf(fp, "P5\n");
		fprintf(fp, "%d %d\n", poza.n, poza.m);
		fprintf(fp, "%d\n", poza.maxdim);
		for (int i = 0; i < poza.m; i++)
			for (int j = 0; j < poza.n; j++)
				fwrite(&poza.pixel[i][j], sizeof(char), 1, fp);
	} else if (poza.mw[1] == '3' || poza.mw[1] == '6') {
		fprintf(fp, "P6\n");
		fprintf(fp, "%d %d\n", poza.n, poza.m);
		fprintf(fp, "%d\n", poza.maxdim);
		for (int i = 0; i < poza.m; i++) {
			for (int j = 0; j < poza.n; j++) {
				fwrite(&poza.pixel_rgb[i][j].r, sizeof(char), 1, fp);
				fwrite(&poza.pixel_rgb[i][j].g, sizeof(char), 1, fp);
				fwrite(&poza.pixel_rgb[i][j].b, sizeof(char), 1, fp);
			}
		}
	}
	printf("Saved %s\n", destinatie);
	fclose(fp);
}

int clamp(long long x, int min, int max)
{//fct clamp care returneaza un nr intre [min,max]
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

void egalizare(image *poza)
{//fct de equalize
	if (poza->ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (poza->mw[1] == '3' || poza->mw[1] == '6') {
		printf("Black and white image needed\n");
		return;
	}
	int f[260];
	long long s;
	for (int i = 0; i < 256; i++)
		f[i] = 0;
	int area = poza->m * poza->n;
	for (int i = 0; i < poza->m; i++)
		for (int j = 0; j < poza->n; j++)
			f[poza->pixel[i][j]]++;//calc frecventa fiecarui pixel
	for (int i = 0; i < poza->m; i++)
		for (int j = 0; j < poza->n; j++) {
			s = 0;
			for (int k = 0; k <= poza->pixel[i][j]; k++)
				s = s + f[k];//aplic formula dorita si schimb pixelul
			s = round((double)(s * 255) / area);
			poza->pixel[i][j] = clamp(s, 0, 255);
		}
	printf("Equalize done\n");
}

void histogram(image poza, char *propozitie, int nr)
{//fct pt histograma
	if (poza.ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (nr != 2) {
		printf("Invalid command\n");
		return;
	}
	if (poza.mw[1] == '3' || poza.mw[1] == '6') {
		printf("Black and white image needed\n");
		return;
	}
	int x, y, max = 0;
	int numere = sscanf(propozitie, "%d %d", &x, &y);
	if (numere != 2)
		return;
	int aux = y;
	while (aux % 2 == 0)
		aux /= 2;
	if (aux != 1 || y < 2 || y > 256) {
		printf("Invalid set of parameters\n");
		return;
	}
	int f[260];
	for (int i = 0; i < 256; i++)
		f[i] = 0;
	for (int i = 0; i < poza.m; i++)
		for (int j = 0; j < poza.n; j++)
			f[poza.pixel[i][j]]++;//calculez vectorul de frecventa
	int k = 0, m[256];
	for (int i = 0 ; i < 256; i += 256 / y) {
		m[k] = 0;
		for (int j = i; j < i + 256 / y; j++)
			m[k] = m[k] + f[j];
		k++;
	} //calculez suma frecventelor din fiecare bin si aflu maximul sumei
	for (int i = 0; i < k; i++)
		if (max < m[i])
			max = m[i];
	for (int i = 0; i < y; i++) {
		int nrs = (int)((double)((m[i] * 1.0 / max) * x));
		printf("%d\t|\t", nrs);//aplic formula si afisez histograma
		for (int j = 0; j < nrs; j++)
			printf("*");
		printf("\n");
	}
}

void edge(image *poza)
{//apply ul pt edge
	int ker[4][4], s;
	ker[0][0] = -1, ker[0][1] = -1, ker[0][2] = -1;
	ker[1][0] = -1, ker[1][1] = 8, ker[1][2] = -1;
	ker[2][0] = -1, ker[2][1] = -1, ker[2][2] = -1;
	rgb **aux = malloc(poza->m * sizeof(rgb *));
	for (int i = 0; i < poza->m; i++)
		aux[i] = malloc(poza->n * sizeof(rgb));
	for (int i = poza->y1; i < poza->y2; i++)
		for (int j = poza->x1; j < poza->x2; j++) {
			if (i == 0 || j == 0 || i == poza->m - 1 || j == poza->n - 1) {
				aux[i][j].r = poza->pixel_rgb[i][j].r;
				aux[i][j].g = poza->pixel_rgb[i][j].g;
				aux[i][j].b = poza->pixel_rgb[i][j].b;
				continue;//marginile pozei le pun neschimbate
			}
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].r * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].r * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].r * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].r * ker[1][0];
			s = s + poza->pixel_rgb[i][j].r * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].r * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].r * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].r * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].r * ker[2][2];
			aux[i][j].r = clamp(s, 0, 255);
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].g * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].g * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].g * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].g * ker[1][0];
			s = s + poza->pixel_rgb[i][j].g * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].g * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].g * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].g * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].g * ker[2][2];
			aux[i][j].g = clamp(s, 0, 255);
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].b * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].b * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].b * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].b * ker[1][0];
			s = s + poza->pixel_rgb[i][j].b * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].b * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].b * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].b * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].b * ker[2][2];
			aux[i][j].b = clamp(s, 0, 255);
			s = 0;
		} //inmultesc cu matricea kernel dorita
	for (int i = poza->y1; i < poza->y2; i++)
		for (int j = poza->x1; j < poza->x2; j++) {
			poza->pixel_rgb[i][j].r = aux[i][j].r;
			poza->pixel_rgb[i][j].g = aux[i][j].g;
			poza->pixel_rgb[i][j].b = aux[i][j].b;
		} //egalez selectia din poza cu cea noua dupa efect
	for (int i = 0; i < poza->m; i++)
		free(aux[i]);
	free(aux);
} //eliberez auxiliara

void sharpen(image *poza)
{ //apply pt sharpen
	int ker[4][4], s;
	ker[0][0] = 0, ker[0][1] = -1, ker[0][2] = 0, ker[1][0] = -1, ker[1][1] = 5;
	ker[1][2] = -1, ker[2][0] = 0, ker[2][1] = -1, ker[2][2] = 0;
	rgb **aux = malloc(poza->m * sizeof(rgb *));
	for (int i = 0; i < poza->m; i++)
		aux[i] = malloc(poza->n * sizeof(rgb));
	for (int i = poza->y1; i < poza->y2; i++)
		for (int j = poza->x1; j < poza->x2; j++) {
			if (i == 0 || j == 0 || i == poza->m - 1 || j == poza->n - 1) {
				aux[i][j].r = poza->pixel_rgb[i][j].r;
				aux[i][j].g = poza->pixel_rgb[i][j].g;
				aux[i][j].b = poza->pixel_rgb[i][j].b;
				continue;//pun marginile pozei
			}
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].r * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].r * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].r * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].r * ker[1][0];
			s = s + poza->pixel_rgb[i][j].r * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].r * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].r * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].r * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].r * ker[2][2];
			aux[i][j].r = clamp(s, 0, 255);
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].g * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].g * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].g * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].g * ker[1][0];
			s = s + poza->pixel_rgb[i][j].g * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].g * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].g * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].g * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].g * ker[2][2];
			aux[i][j].g = clamp(s, 0, 255);
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].b * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].b * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].b * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].b * ker[1][0];
			s = s + poza->pixel_rgb[i][j].b * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].b * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].b * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].b * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].b * ker[2][2];
			aux[i][j].b = clamp(s, 0, 255);
			s = 0;
		} //aplic formula si pun pixelii noi in locul selectiei
	for (int i = poza->y1; i < poza->y2; i++)
		for (int j = poza->x1; j < poza->x2; j++) {
			poza->pixel_rgb[i][j].r = aux[i][j].r;
			poza->pixel_rgb[i][j].g = aux[i][j].g;
			poza->pixel_rgb[i][j].b = aux[i][j].b;
		}
	for (int i = 0; i < poza->m; i++)
		free(aux[i]);
	free(aux);
}

void blur(image *poza)
{//apply pt blur
	int ker[4][4], s;
	ker[0][0] = 1, ker[0][1] = 1, ker[0][2] = 1, ker[1][0] = 1, ker[1][1] = 1;
	ker[1][2] = 1, ker[2][0] = 1, ker[2][1] = 1, ker[2][2] = 1;
	rgb **aux = malloc(poza->m * sizeof(rgb *));
	for (int i = 0; i < poza->m; i++)
		aux[i] = malloc(poza->n * sizeof(rgb));
	for (int i = poza->y1; i < poza->y2; i++)
		for (int j = poza->x1; j < poza->x2; j++) {
			if (i == 0 || j == 0 || i == poza->m - 1 || j == poza->n - 1) {
				aux[i][j].r = poza->pixel_rgb[i][j].r;
				aux[i][j].g = poza->pixel_rgb[i][j].g;
				aux[i][j].b = poza->pixel_rgb[i][j].b;
				continue;//pun marginile pozei
			}
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].r * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].r * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].r * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].r * ker[1][0];
			s = s + poza->pixel_rgb[i][j].r * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].r * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].r * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].r * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].r * ker[2][2];
			s = s / 9;
			aux[i][j].r = clamp(s, 0, 255);
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].g * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].g * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].g * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].g * ker[1][0];
			s = s + poza->pixel_rgb[i][j].g * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].g * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].g * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].g * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].g * ker[2][2];
			s = s / 9;
			aux[i][j].g = clamp(s, 0, 255);
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].b * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].b * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].b * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].b * ker[1][0];
			s = s + poza->pixel_rgb[i][j].b * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].b * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].b * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].b * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].b * ker[2][2];
			s = s / 9;
			aux[i][j].b = clamp(s, 0, 255);
			s = 0;
		} //inmultesc cu kernel si modific selectia cu noii pixeli
	for (int i = poza->y1; i < poza->y2; i++)
		for (int j = poza->x1; j < poza->x2; j++) {
			poza->pixel_rgb[i][j].r = aux[i][j].r;
			poza->pixel_rgb[i][j].g = aux[i][j].g;
			poza->pixel_rgb[i][j].b = aux[i][j].b;
		}
	for (int i = 0; i < poza->m; i++)
		free(aux[i]);
	free(aux);
}

void gaussian_blur(image *poza)
{//apply pt gaussian blur
	int ker[4][4], s;
	ker[0][0] = 1, ker[0][1] = 2, ker[0][2] = 1, ker[1][0] = 2, ker[1][1] = 4;
	ker[1][2] = 2, ker[2][0] = 1, ker[2][1] = 2, ker[2][2] = 1;
	rgb **aux = malloc(poza->m * sizeof(rgb *));
	for (int i = 0; i < poza->m; i++)
		aux[i] = malloc(poza->n * sizeof(rgb));
	for (int i = poza->y1; i < poza->y2; i++)
		for (int j = poza->x1; j < poza->x2; j++) {
			if (i == 0 || j == 0 || i == poza->m - 1 || j == poza->n - 1) {
				aux[i][j].r = poza->pixel_rgb[i][j].r;
				aux[i][j].g = poza->pixel_rgb[i][j].g;
				aux[i][j].b = poza->pixel_rgb[i][j].b;
				continue;//pun marginile pozei
			}
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].r * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].r * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].r * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].r * ker[1][0];
			s = s + poza->pixel_rgb[i][j].r * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].r * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].r * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].r * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].r * ker[2][2];
			s = s / 16;
			aux[i][j].r = clamp(s, 0, 255);
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].g * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].g * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].g * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].g * ker[1][0];
			s = s + poza->pixel_rgb[i][j].g * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].g * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].g * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].g * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].g * ker[2][2];
			s = s / 16;
			aux[i][j].g = clamp(s, 0, 255);
			s = 0;
			s = s + poza->pixel_rgb[i - 1][j - 1].b * ker[0][0];
			s = s + poza->pixel_rgb[i - 1][j].b * ker[0][1];
			s = s + poza->pixel_rgb[i - 1][j + 1].b * ker[0][2];
			s = s + poza->pixel_rgb[i][j - 1].b * ker[1][0];
			s = s + poza->pixel_rgb[i][j].b * ker[1][1];
			s = s + poza->pixel_rgb[i][j + 1].b * ker[1][2];
			s = s + poza->pixel_rgb[i + 1][j - 1].b * ker[2][0];
			s = s + poza->pixel_rgb[i + 1][j].b * ker[2][1];
			s = s + poza->pixel_rgb[i + 1][j + 1].b * ker[2][2];
			s = s / 16;
			aux[i][j].b = clamp(s, 0, 255);
			s = 0;
		} //am calculat noii pixeli si ii pun in locul selectiei
	for (int i = poza->y1; i < poza->y2; i++)
		for (int j = poza->x1; j < poza->x2; j++) {
			poza->pixel_rgb[i][j].r = aux[i][j].r;
			poza->pixel_rgb[i][j].g = aux[i][j].g;
			poza->pixel_rgb[i][j].b = aux[i][j].b;
		}
	for (int i = 0; i < poza->m; i++)
		free(aux[i]);
	free(aux);
}

void apply(image *poza, char *propozitie, int nr)
{//APPLY ul general,unde aplic una din functiile de mai sus in functie de input
	int ok = 0;
	if (poza->ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (nr != 1) {
		printf("Invalid command\n");
		return;
	}
	char cuv[100];
	if (nr == 1) {
		propozitie[strcspn(propozitie, "\n")] = '\0';
		if (propozitie[0] == '\0') {
			printf("Invalid command\n");
			return;
		}
		sscanf(propozitie, "%s", cuv);
		if (strcmp(cuv, "EDGE") == 0 || strcmp(cuv, "SHARPEN") == 0)
			ok = 1;
		if (strcmp(cuv, "BLUR") == 0 || strcmp(cuv, "GAUSSIAN_BLUR") == 0)
			ok = 1;
		if (ok != 1) {
			printf("APPLY parameter invalid\n");
			return;
		}
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}
	ok = 0;
	if (poza->mw[1] == '2' || poza->mw[1] == '5') {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	ok = 0;
	if (strcmp(cuv, "EDGE") == 0) {
		edge(poza);
		ok = 1;
	}
	if (strcmp(cuv, "SHARPEN") == 0) {
		sharpen(poza);
		ok = 1;
	}
	if (strcmp(cuv, "BLUR") == 0) {
		blur(poza);
		ok = 1;
	}
	if (strcmp(cuv, "GAUSSIAN_BLUR") == 0) {
		gaussian_blur(poza);
		ok = 1;
	}
	if (ok == 0) {
		printf("Invalid command\n");
		return;
	} else if (ok == 1) {
		printf("APPLY %s done\n", cuv);
		return;
	}
}

void rotate(image *poza, char *propozitie, int nr)
{//fct pt rotate
	if (poza->ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (nr != 1) {
		printf("Invalid command\n");
		return;
	}
	int unghi, aux;
	int numere = sscanf(propozitie, "%d", &unghi);
	if (numere != 1)
		return;
	aux = unghi;
	unghi = unghi % 360;
	if (unghi < 0)
		unghi = unghi + 360;//micsorez unghiul pt a nu roti in cerc
	if (unghi % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;//verific daca rotirea este posibila
	}
	int m = poza->m, n = poza->n;//ma asigur intai ca ma aflu intr o selectie
	if (poza->y1 != 0 || poza->x1 != 0 || poza->y2 != m || poza->x2 != n) {
		if (poza->y2 - poza->y1 != poza->x2 - poza->x1) {
			printf("The selection must be square\n");
			return;//verific daca selectia este patratica
		}
		if (unghi == 0) {
			printf("Rotated %d\n", aux);
			return;
		}
	} else {//in acest caz,inseamna ca trb sa rotesc toata poza
		if (unghi == 0) {
			printf("Rotated %d\n", aux);
			return;
		}
	}
}

int main(void)
{
	image poza;//initializez cu NULL matricile pozelor
	poza.pixel = NULL;
	poza.pixel_rgb = NULL;
	poza.ok = 0;
	char operatie[100], propozitie[100], cuv[100], destinatie[100];
	do {//citesc cate un cuvant
		scanf("%s", operatie);
		if (strcmp(operatie, "EXIT") == 0) {
			break;//daca e exit,ma opresc
		} else if (strcmp(operatie, "LOAD") == 0) {
			if (poza.ok == 1) {
				eliberare_poza(&poza);
				poza.ok = 0;
			} //la load,incarc matricea
			load(&poza);
		} else if (strcmp(operatie, "SELECT") == 0) {
			fgets(propozitie, sizeof(propozitie), stdin);
			int nr = nrcuv(propozitie);
			if (nr == 4) {//verific nr cuvintelor pt a stii daca se doreste
				int x1, x2, y1, y2;//SELECT
				int n = sscanf(propozitie, "%d %d %d %d", &x1, &y1, &x2, &y2);
				if (n == 4)
					selectare(&poza, x1, y1, x2, y2);
				else
					printf("Invalid command\n");
			} else {//in caz contrar,verific daca se doreste SELECT ALL
				int b = sscanf(propozitie, "%s", cuv);
				if (strcmp(cuv, "ALL") == 0 && b == 1)
					selectare_totala(&poza);
				else
					printf("Invalid command\n");
			}
		} else if (strcmp(operatie, "CROP") == 0) {
			crop(&poza);//tai poza
		} else if (strcmp(operatie, "SAVE") == 0) {
			fgets(propozitie, sizeof(propozitie), stdin);
			int nr = nrcuv(propozitie);//verific daca salvez binar sau ascii
			if (nr == 1) {
				int n = sscanf(propozitie, "%s", destinatie);
				if (n == 1)
					salvare_binar(poza, destinatie);
			} else if (nr == 2) {
				int n = sscanf(propozitie, "%s %s", destinatie, cuv);
				if (strcmp(cuv, "ascii") == 0 && n == 2)
					salvare_ascii(poza, destinatie);
			} else {
				printf("Invalid command\n");
			}
		} else if (strcmp(operatie, "EQUALIZE") == 0) {
			egalizare(&poza);//egalizarea pozei
		} else if (strcmp(operatie, "HISTOGRAM") == 0) {
			fgets(propozitie, sizeof(propozitie), stdin);
			int nr = nrcuv(propozitie);
			histogram(poza, propozitie, nr);//histograma
		}  else if (strcmp(operatie, "APPLY") == 0) {
			fgets(propozitie, sizeof(propozitie), stdin);
			int nr = nrcuv(propozitie);
			apply(&poza, propozitie, nr);//apply-ul
		} else if (strcmp(operatie, "ROTATE") == 0) {
			fgets(propozitie, sizeof(propozitie), stdin);
			int nr = nrcuv(propozitie);//rotirea
			rotate(&poza, propozitie, nr);
		} else {//daca nu s-a executat nimic,citesc restul liniei
			fgets(propozitie, sizeof(propozitie), stdin);
			printf("Invalid command\n");
		} //pt celelalte cazuri invalide,le verific pe fiecare la inceput de fct
	} while (1);
	if (poza.ok != 1)//verific daca a fost vreodata incarcata vreo poza
		printf("No image loaded\n");
	else
		eliberare_poza(&poza);//eliberez toata memoria alocata daca e nevoie
	return 0;
}
