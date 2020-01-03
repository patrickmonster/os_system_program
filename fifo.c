#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "plist.dat"

void printResult(int pnum, int *pid, int *at, int *bt, int *tt, int *wt) {
	int i, sum_tt=0, sum_wt=0;

	printf("%15s %15s %15s %15s %15s\n", "Process id", "Arrive time", "Burst time", "Turnaround time", "Waiting time");
	for(i=0; i<pnum; ++i) {
		printf("%15d %15d %15d %15d %15d\n", pid[i], at[i], bt[i], tt[i], wt[i]);
		sum_tt+=tt[i];
		sum_wt+=wt[i];
	}

	printf("  avg. of TT: %.2f\n", (float)sum_tt/pnum);
	printf("  avg. of WT: %.2f\n", (float)sum_wt/pnum);
}
void SJF(int pnum, int *pid, int *at, int *bt, int *tt, int *wt) {
	int i,pc,resrve_size,run,ct_pros;
	int *resrve;
	printf("  Scheme: Short Job First\n");

	pc = resrve_size = run = ct_pros =0;
	/* fill in */
	resrve = (int*)calloc(pnum, sizeof(int));
	while(1){
		for(i=0;i<pnum;i++)
			if(pc==at[i]){
				resrve[resrve_size++] = i;
				if(!run){
					run = bt[i];
				}
			}

		for(i=1;i<resrve_size;i++)
			wt[resrve[i]]++;

		if (run){// 프로그램 동작 영역
			run--;	
			if(!run){
				tt[resrve[0]] = bt[resrve[0]]+wt[resrve[0]];
				resrve[0] = 0;
				resrve_size--;
				ct_pros ++;
			}
		}

		if(resrve_size){// 큐가 비어있지 않다
			if(!run){//근데 0이네?
				int select = 1;
				for(i=2;i<resrve_size+1;i++){//선별
					if(bt[resrve[select]] > bt[resrve[i]]){
						select = i;
					}
				}
				resrve[0] = resrve[select];
				memmove(&resrve[select],&resrve[select+1],sizeof(int)*(pnum-select+1));
				memset(&resrve[pnum-1],0,sizeof(int));
				run=bt[resrve[0]];
			}
		}else if(!run)
			if(ct_pros == pnum)
				break;
		pc++;
	}
	free(resrve);
}

void FIFO(int pnum, int *pid, int *at, int *bt, int *tt, int *wt) {
	int i,pc,resrve_size,run,ct_pros;
	int *resrve;
	printf("  Scheme: First-In-First-Out\n");

	pc = resrve_size = run = ct_pros =0;
	/* fill in */
	resrve = (int*)calloc(pnum, sizeof(int));
	while(1){
		for(i=0;i<pnum;i++)
			if(pc==at[i]){
				resrve[resrve_size++] = i;
				if(!run)
					run = bt[i];
			}
		for(i=1;i<resrve_size;i++)
			wt[resrve[i]]++;

		if (run){// 프로그램 동작 영역
			run--;
			if(!run){
				tt[resrve[0]] = bt[resrve[0]]+wt[resrve[0]];
				resrve[0] = 0;
				resrve_size--;
				ct_pros ++;
			}
		}

		if(resrve_size){// 큐가 비어있지 않다
			if(!run){//근데 0이네?
				memmove(&resrve[0],&resrve[1],sizeof(int)*(pnum-1));
				memset(&resrve[pnum-1],0,sizeof(int));
				run=bt[resrve[0]];
			}
		}else if(!run)
			if(ct_pros == pnum)
				break;
		pc++;
	}
	free(resrve);
}

int main(void) {
	FILE *fp;
	int pnum, i, exit=0;
	int *pid, *at, *bt, *tt, *wt;

	fp = fopen(INPUT_FILE, "r");
	//read the number of processes 
	fscanf(fp, "%d", &pnum);

	pid = (int*)calloc(pnum, sizeof(int));
	at = (int*)calloc(pnum, sizeof(int));
	bt = (int*)calloc(pnum, sizeof(int));
	tt = (int*)calloc(pnum, sizeof(int));
	wt = (int*)calloc(pnum, sizeof(int));

	for(i=0; i<pnum; ++i) {
		fscanf(fp, "%d %d %d", &pid[i], &at[i], &bt[i]);
	}

	fclose(fp);
	
	FIFO(pnum, pid, at, bt, tt, wt);
	printResult(pnum, pid, at, bt, tt, wt);
	memset(tt, 0, sizeof(int)*pnum);
	memset(wt, 0, sizeof(int)*pnum);
	
	SJF(pnum, pid, at, bt, tt, wt);
	printResult(pnum, pid, at, bt, tt, wt);
	memset(tt, 0, sizeof(int)*pnum);
	memset(wt, 0, sizeof(int)*pnum);
	return 0;
}
