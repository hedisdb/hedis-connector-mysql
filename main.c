#include <stdio.h>
#include "hedis.h"

hedisConfigEntry **hedis_entries;
int hedis_entry_count;

int init(hedisConfigEntry **entries, int entry_count){
	hedis_entries = entries;
	hedis_entry_count = entry_count;

	for(int i = 0; i < entry_count; i++){
		printf("%s: %s\n", entries[i]->key, entries[i]->value);
	}

	return 0;
}

char *get_value(){
	return hedis_entries[0]->key;
}