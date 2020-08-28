#ifndef MAP_H
#define MAP_H

#define MAX_SIZE 20000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct map {
	int total;
	char* words[MAX_SIZE];
	float freq[MAX_SIZE];
	int indics[MAX_SIZE];
} map;

void showMap(map* m) {
	for(int i=0; i<MAX_SIZE; i++) {
		if(m->freq[i] != 0) {
			printf("%s : %f\n", m->words[i], m->freq[i]);
		}
	}
	printf("\ndone\n");
}

unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void map_init (map *m) {
	m->total = 0;
	for(int i=0; i<MAX_SIZE; i++) {
		m->freq[i] = 0;
		m->indics[i] = -1;
	}
}

int increaseIndex(int i) {
	if(i<MAX_SIZE-1)
		return i+1;
	else return 0;
}

bool contains(map* m, char* str) {
	unsigned long index = hash(str);
	index = index%MAX_SIZE;
	while(m->freq[index] != 0) {
		if(strcmp(m->words[index], str) == 0)
			return true;
		else index = increaseIndex(index);
	}
	return false;
}

void map_add (map* m, char* str) {
	unsigned long index = hash(str);
	index = index%MAX_SIZE;
	while(m->freq[index] != 0) {
		if(strcmp(m->words[index], str) == 0)
			break;
		else
			index = increaseIndex(index);
	}
	if(m->freq[index] == 0)
		m->indics[m->total++] = index;
	
	m->words[index] = str;
	m->freq[index]++;
}

float get_freq(map* m, char* str)
{
	unsigned long index = hash(str);
	index = index%MAX_SIZE;
	while(m->freq[index] != 0) {
		if(strcmp(m->words[index], str) == 0)
			return m->freq[index];
		else index = increaseIndex(index);
	}
	return 0;
}

void normalize_map(map* m)
{
	float totalwords = 0;
	for(int i=0; i<m->total; i++)
		totalwords += m->freq[m->indics[i]];
	for(int i=0; i<m->total; i++)
		m->freq[m->indics[i]] = m->freq[m->indics[i]]/totalwords;
}

void update_tfidf(map *m, map **corpse_bag, int totalFiles)
{
	for(int i=0; i<m->total; i++) {
		if(m->freq[m->indics[i]]==0)
			continue;
		char* str = m->words[m->indics[i]];
		float corpse_size = totalFiles;
		float containFiles=0;
		for(int j=0; j<corpse_size; j++) {
			if(contains(corpse_bag[j], str)  == true)
				containFiles++;
		}
		float idf = log(corpse_size/(1+containFiles));
		m->freq[m->indics[i]] = (m->freq[m->indics[i]])*idf;
	}
}

void readFile(map *inputbag, FILE* fp)
{
	char prev = ' ';
	char word[25];
	char c;
	int i=0;
	while((c=fgetc(fp)) != EOF) {
        if(prev == ' ') {
			if(c == ',' || c=='.' || c == '\n' || c == ' ' || c == '\t' || c == '^' || c == '$' || c == '~' || c == '!' || c == '@')
				continue;
		}
		if(c == ',' || c=='.' || c == '\n' || c == ' ' || c == '\t' || c == '^' || c == '$' || c == '~' || c == '!' || c == '@')
			c = ' ';

        if(c>=65 && c<=90) //decapitalize characters
            c = c+(97-65);
        if(c <1) //Invalid characters
        	continue;
        prev = c;

        if(c!= ' ')
            word[i++] = c;   
        else {
            word[i]='\0';
            if(i!=0) {
                char *new = malloc(sizeof(word));
                strcpy(new, word);
                map_add(inputbag, new);
            }
            i=0;
        }
	}
    if(prev != ' ') {
        word[i] = '\0';
        char *new = malloc(sizeof(word));
        strcpy(new, word);
        map_add(inputbag, new);
    }
}
#endif
