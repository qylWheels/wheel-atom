
/* 测试2：测试原子的唯一性 */

#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "atom.h"

int main(void) {
	FILE *fp = fopen("words.txt", "r");
	char buf[2048];
	int cnt = 0;
	int atom_cnt = 0;

	while (!feof(fp)) {
		fscanf(fp, "%s", buf);
		if(feof(fp)) {
			break;
		}
		atom_string(buf);
		cnt++;
	}
	
	// Show count of records
	printf("Count of records: %d\n", cnt);

	// Show count of atoms
	printf("Count of atoms: %d\n", atom_get_count());

	fclose(fp);
	return 0;
}

