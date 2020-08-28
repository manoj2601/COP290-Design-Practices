#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include "map.h"
#define cout printf(
#define endl )

int main(int argc, char* argv[]) {
	FILE *fp;
	FILE *fp2;
	char* file_name1 = argv[1];
	fp = fopen(file_name1, "r");
	if(fp == NULL) {
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	int i=0;
	int j=0;
	char c;
	char prev = ' ';
    char word[25];
    map inputbag;
    map_init(&inputbag);
    readFile(&inputbag, fp);
    normalize_map(&inputbag);
    //showMap(&inputbag);
    fclose(fp);
    //INPUT BAG DONE
	DIR *d;
	struct dirent *dir;
	d = opendir(argv[2]);
	char * files[30];
	int totalFiles=0;
	if(d) {
		i=0;
		while((dir = readdir(d)) != NULL) {
			if(dir->d_name[0] == '.')
				continue;
			files[i] = dir->d_name;
			i++;
		}
		totalFiles = i;
	}
	else {
		perror("Error while opening the corpus directory.\n");
		exit(EXIT_FAILURE);
	}
	char* folder = argv[2];

    map *corpus_bag[totalFiles];
    for(int m=0; m<totalFiles; m++) {
        char file_name[30];
        int k=0;
        for(k=0; k<strlen(folder); k++)
            file_name[k] = folder[k];
        file_name[k++] = '/';
        file_name[k] = '\0';
        strcat(file_name, files[m]);
        map* bag = malloc(sizeof (struct map));
        map_init(bag);
        fp = fopen(file_name, "r");
        readFile(bag, fp);
        normalize_map(bag);
        corpus_bag[m] = bag;
        fclose(fp);
    }
//////////////TF*IDF///////////////////
    update_tfidf(&inputbag, corpus_bag, totalFiles);
    // showMap(&inputbag);
    for(i=0; i<totalFiles; i++)
    	update_tfidf(corpus_bag[i], corpus_bag, totalFiles);
//////////////COMPUTE COSINE SIMILARITY/////////
    char* ret1[totalFiles];
    float ret2[totalFiles];
    for(i=0; i<totalFiles; i++) {
        double similarity = 0;
        double modinput = 0;
        for(int j=0; j<inputbag.total; j++) {
            float freqinput = inputbag.freq[inputbag.indics[j]];
            if(freqinput == 0)
                continue;
            modinput += freqinput*freqinput;
            float freq2 = get_freq(corpus_bag[i], inputbag.words[inputbag.indics[j]]);
            similarity += freqinput*freq2;
        }
        modinput = sqrt(modinput);
        long double modfile = 0;
        for(int j=0; j<corpus_bag[i]->total; j++)
            modfile += pow(((struct map *)corpus_bag[i])->freq[corpus_bag[i]->indics[j]], 2);
        modfile = sqrt(modfile);
        similarity = similarity/(modfile*modinput);
        ret1[i] = files[i];
        ret2[i] = similarity*100;
    }
    //SORTING
    i=0;
    j=0;
    for(i=0; i<totalFiles-1; i++)
    	for(j=0; j<totalFiles-i-1; j++)
    		if(ret2[j] > ret2[j+1]) {
    			float temp1 = ret2[j];
    			ret2[j] = ret2[j+1];
    			ret2[j+1] = temp1;

    			char *temp = malloc(strlen(ret1[j]));
    			strcpy(temp, ret1[j]);
    			ret1[j] = ret1[j+1];
    			ret1[j+1] = temp; 
    		}
    //PRINTING RESULT
    for(int i=totalFiles-1; i>=0; i--)
    	printf("%s %.2f%%\n", ret1[i], ret2[i]);
}
