
/* 测试1：测试性能与哈希分布 */

#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "atom.h"

int main(void) {
	// Use atom
	FILE *fp = fopen("words.txt", "r");
	struct timeval tv_start, tv_end;
	char buf[2048];
	int cnt = 0;

	gettimeofday(&tv_start, NULL);
	while (!feof(fp)) {
		fscanf(fp, "%s", buf);
		if(feof(fp)) {
			break;
		}
		atom_string(buf);
		cnt++;
	}
	gettimeofday(&tv_end, NULL);
	
	// Get used time	
	long int time_sec, time_msec;
	time_msec = (tv_end.tv_usec - tv_start.tv_usec + 1000000) % 1000000;
	time_msec /= 1000;
	if (tv_end.tv_usec < tv_start.tv_usec) {
		time_sec = tv_end.tv_sec - tv_start.tv_sec - 1;
	} else {	
		time_sec = tv_end.tv_sec - tv_start.tv_sec;
	}
	
	printf("Count of records: %d\n", cnt);
	printf("Time used: %lds %ldms\n", time_sec, time_msec);

	fclose(fp);

	// Get hash table distribution
	atom_get_distribution(cnt);

	return 0;
}

