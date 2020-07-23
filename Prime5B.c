#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


// Les composants associés, http://compoasso.free.fr/primelistweb
//
// AMD 3200+ 2Ghz
// Amorcer  :  0.4s
// Mask	    :  1.7s
// Segment X: 27s - 20s bornes de temps sup et inf pour un segment
// 10 milliards  :  3mn 35s environ
// 100 milliards : 35mn (extrapolation)

typedef double LONG; // %I64d -> %llu
typedef unsigned INT;
typedef unsigned char BYTE;
typedef enum {
	false, true
} BOOL;

INT MAX_AMORCE = 1000000;
INT MAX_AMORCE_SIZE = -1;
INT *amorce;

INT NB_SEGMENT = 100;
LONG MAX_SEGMENT = 1000089090; // 3 * 5 * 7 * 11 * 13 * 17 * 3918

INT MEMORY_SEGMENT_SIZE = 62505569; // (MAX_SEGMENT >> 4) + 1
BYTE segment[62505569];
BYTE mask[62505569];

char FILENAME_AMORCE[] = "amorce.txt";
char FILENAME_SEGMENT_PATTERN_WS[] = "prime_%u_%I64d.bin";
char FILENAME_SEGMENT_PATTERN_WD[] = "prime_%u_%I64d.txt";

void amorcer(void);
void calculerMask(void);
void calculerSegment(INT);
void writeAmorce(void);
void writeSegment(INT);
void writeDebug(INT,INT);
clock_t benchStart(void);
void benchStop(clock_t start, const char *label);

int main(int argc, char **argv) {

	clock_t debut = benchStart();

	clock_t s = benchStart();
	amorcer();
	writeAmorce();
	benchStop(s, "Amorcer");

	s = benchStart();
	calculerMask();
	benchStop(s, "Mask");

	// Find prime
	char label[50];
	INT i;
	for (i = 1; i < NB_SEGMENT; i++) {
		s = benchStart();
		calculerSegment(i);
		writeSegment(i);
		sprintf(label, "Segment %u", i);
		benchStop(s, label);

		// Mettre en commentaire ci dessous pour calculer les 100
		// milliards
		if (i == 2) {
			clock_t sDebug = benchStart();
			writeDebug(i, 100000);
			sprintf(label, "Debug %u", i);
			benchStop(sDebug, label);
			break;
		}
	}

	benchStop(debut, "T: ");

	return 0;
}

void amorcer() {

	INT amorceTmp[(MAX_AMORCE >> 2)];

	INT pos = 0;
	amorceTmp[pos++] = 2;
	amorceTmp[pos++] = 3;
	amorceTmp[pos++] = 5;

	BOOL composite = false;

	INT i;
	for (i = 7; i < MAX_AMORCE; i += 2) {

		LONG sqrtN = round(sqrt(i)) + 1;

		INT j;
		for (j = 0; j < pos; j++) {
			INT value = amorceTmp[j];
			if (value > sqrtN) {
				composite = false;
				break;
			}
			if ((i % value) == 0) {
				composite = true;
				break;
			}
		}

		if (!composite) {
			amorceTmp[pos++] = i;
		}
	}

	MAX_AMORCE_SIZE = pos;
	amorce = (INT*)calloc(MAX_AMORCE_SIZE, sizeof(INT));
	memcpy(amorce, amorceTmp, MAX_AMORCE_SIZE * sizeof(INT));
}

void calculerMask() {

	memset(mask, 0, MEMORY_SEGMENT_SIZE);

	INT p = 0;
	INT i = 1;
	LONG p2 = 0;
	while (i < 7) {
		p = amorce[i++];
		p2 = p * 2;
		while (p < MAX_SEGMENT) {
			mask[(p >> 4)] |= (1 << ((p >> 1) & 7));
			p += p2;
		}
	}
}

void calculerSegment(INT numSegment) {

	memcpy(segment, mask, MEMORY_SEGMENT_SIZE);

	LONG debut = numSegment * MAX_SEGMENT;
	LONG fin = debut + MAX_SEGMENT;
	LONG finSqrt = round(sqrt(fin)) + 1;

	INT p = 0;
	INT i = 7;
	LONG j = 0;
	LONG p2 = 0;
	while (p < finSqrt) {
		p = amorce[i++];
		j = p - (debut % p);
		j += ((j & 1) == 0 ? p : 0);

		p2 = p * 2;
		while (j < MAX_SEGMENT) {
			segment[(j >> 4)] |= (1 << ((j >> 1) & 7));
			j += p2;
		}
	}
}

void writeAmorce() {

	FILE *fp;
	fp = fopen(FILENAME_AMORCE, "w");

	INT i;
	for (i = 0; i < MAX_AMORCE_SIZE; i++) {
		fprintf(fp, "%u\n", amorce[i]);
	}

	fclose(fp);
}

void writeSegment(INT numSegment) {

	LONG debut = numSegment * MAX_SEGMENT;

	FILE *fp;
	char filename[50];
	sprintf(filename, FILENAME_SEGMENT_PATTERN_WS, numSegment, debut);

	fp = fopen(filename, "wb+");
	fwrite(segment, sizeof(BYTE), MEMORY_SEGMENT_SIZE, fp);

	fclose(fp);
}

void writeDebug(INT numSegment, INT nbEltInverse) {

	LONG debut = numSegment * MAX_SEGMENT;

	FILE *fp;
	char filename[50];
	sprintf(filename, FILENAME_SEGMENT_PATTERN_WD, numSegment, debut);

	fp = fopen(filename, "w");

	LONG fin = debut + MAX_SEGMENT;
	LONG debutEltInverse = fin - nbEltInverse;
	debutEltInverse += ((debutEltInverse & 1) == 0 ? 1 : 0);


	LONG i;
	for (i = debutEltInverse; i < fin; i += 2) {
		if ((segment[((i - debut) >> 4)] & (1 << (((i - debut) >> 1) & 7))) == 0) {
			fprintf(fp, "%I64d\n", i);
		}
	}

	fclose(fp);
}

clock_t benchStart() {
	return clock();
}

void benchStop(clock_t start, const char *label) {
	clock_t end = clock();
	double duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%s : %5.3f\n", label, duration);
}