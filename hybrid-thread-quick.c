#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

/*
< 7000만개 퀵정렬 시간측정 요약 >
* cs서버 측정시간 기준
* -O3 옵션 사용

[일반] 비정렬 vs 역정렬
비정렬 : 9.51초
역정렬(...,5,4,3,2,1) : (너무 오래 걸려서 측정 불가)

[하이브리드(퀵+합병)] 비정렬 vs 역정렬
비정렬 : 10.895초
역정렬(...,5,4,3,2,1) : 7.274초

[합병정렬이 있는 하이브리드 쓰레드(퀵+합병+쓰레드+삽입)] 비정렬 vs 역정렬
비정렬 : 2.073923초
역정렬(...,5,4,3,2,1) : 0.959581초

[합병정렬이 없는 하이브리드 쓰레드(퀵+쓰레드+삽입)] 비정렬 vs 역정렬
비정렬 : 1.745060초
역정렬(...,5,4,3,2,1) : 0.759734초
*/

#define INSERT_SORT_DEADLINE 60
int cnt= 0 ;
int sorted[70000000]; 
int hit_combo = 0;
int boolean_sort = 0;

typedef struct {
   int number;
}input_size;

typedef struct thread1 {
	input_size* array;
	int low;
	int high;
}thread1;

typedef struct thread2 {
   input_size *array;
   int low;
   int high;
   int mid;
}thread2;


void *threadFunc(void *p);
void *threadFuncFin(void *p);
void *threadFuncFin2(void *p);
void fin(int low, int mid, int high, input_size* b);
void fin2(int low, int mid, int high, input_size* b);
void quick_sort(input_size* b, int low, int high);
void insertion(int low, int high, input_size* b);
void merge_sort(input_size *b, int low, int high);
void merge(input_size *b, int low, int mid, int high);
void swap (int *a, int *b); // SWAP


int main(int argc,char **argv)
{
	FILE *fp2; 
	int i = 0, j = 0, k = 0,l = 0;
	
	struct thread1 Tquick[16];
	struct thread2 mquick[8];
	struct thread2 Squick[4];
	struct thread2 Hquick[2];   
	
	pthread_t p_thread[100];
	
	int thr_id;
	int status;
	
	input_size* b = (input_size*)malloc(sizeof(input_size) * 70000000); 
	char infilename[20]; 
	
	struct timeval start, high;
	double operatingTime;
	
	FILE *fp1 = fopen(argv[1],"r");
	
	while (fscanf(fp1, "%d", &b[i++].number) != EOF) 
	{
		cnt = i;
		sorted[k++];
	}
	
	printf("** 하이브리드-퀵 정렬 시작 : %d", cnt);
	gettimeofday(&start, NULL);
	
	/* 쓰레드 작업 준비 */
	/*
		쓰레드 작동원리 :
		T퀵 쓰레드에서는 16등분으로 하고, low와 high가 첫 번째 위치에 있다면
		m퀵 쓰레드에서는 8등분이긴 하나, low와 high가 Tquick의 2배 위치 선상에서 시작된다.
		
		       high
		        ↓
		Tquick - - - - - - - - - - - - - - - -
		       ↑
		      low
		      
		             high
		              ↓
		mquick -   -   -   -   -   -   -   -
		           ↑
		          low
	*/
	for(l = 0; l < 16; l++)
	{
		if(l < 15)
		{
			Tquick[l].array = b;
			Tquick[l].low = l * ((cnt)/16);
			Tquick[l].high =(l+1) * ((cnt)/16) -1;
		}
		else
		{
			Tquick[l].array = b;
			Tquick[l].low = l * ((cnt)/16);
			Tquick[l].high = cnt - 1;
		}
	}
	
	for(l = 0; l < 8; l++)
	{
		if(l < 7)
		{
			mquick[l].array = b;
			mquick[l].low = Tquick[(l * 2)].low;
			mquick[l].high = Tquick[(l * 2) + 1].high;
			mquick[l].mid = (mquick[l].low + mquick[l].high) / 2;
		}
		else
		{
			mquick[l].array = b;
			mquick[l].low = Tquick[(l * 2)].low;
			mquick[l].high = cnt - 1;
			mquick[l].mid = Tquick[14].high;
		}
	}      
	
	for(l = 0; l < 4; l++)
	{
		if(l < 3)
		{
			Squick[l].array = b;
			Squick[l].low = mquick[(l * 2)].low;
			Squick[l].high = mquick[(l * 2) + 1].high;
			Squick[l].mid = (Squick[l].low + Squick[l].high) / 2;
		}
		else
		{
			Squick[l].array = b;
			Squick[l].low = mquick[(l * 2)].low;
			Squick[l].high = cnt - 1;
			Squick[l].mid =  mquick[6].high;
		}
	}
	
	for(l = 0; l < 2; l++)
	{
		if(l < 1)
		{
			Hquick[l].array = b;
			Hquick[l].low = Squick[(l * 2)].low;
			Hquick[l].high = Squick[(l * 2) + 1].high;
			Hquick[l].mid = (Hquick[l].low + Hquick[l].high) / 2;
		}
		else
		{
			Hquick[l].array = b;
			Hquick[l].low = Squick[(l * 2)].low;
			Hquick[l].high = cnt - 1;
			Hquick[l].mid =  Squick[2].high;
		}
	}
	
	/* 쓰레드 생성 및 쓰레드 함수에 값 전달*/
	for(l=0;l<16;l++)
	{
		thr_id = pthread_create(&p_thread[l],NULL,threadFunc, (void *)&Tquick[l]);
	}
	for(l=0;l<16;l++)
	{
		pthread_join(p_thread[l],NULL);
	}
	
	for(l = 0; l < 8; l++)
	{
		thr_id = pthread_create(&p_thread[l], NULL, threadFuncFin, (void *)&mquick[l] );
	}
	for(l = 0; l < 8; l++)
	{
		pthread_join(p_thread[l],NULL);
	}
	
	for(l=0; l < 4; l++)
	{
		thr_id = pthread_create(&p_thread[l], NULL, threadFuncFin2, (void *)&Squick[l] );
	}
	for(l=0; l < 4; l++)
	{
		pthread_join(p_thread[l],NULL);
	}
	
	for(l=0; l < 2; l++)
	{
		thr_id = pthread_create(&p_thread[l], NULL, threadFuncFin, (void *)&Hquick[l]);
	}
	for(l = 0; l < 2; l++)
	{
		pthread_join(p_thread[l], NULL);
	}
	
	fin2(0, Hquick[0].high, cnt - 1, b);
	
	gettimeofday(&high, NULL);
	operatingTime = (double)(high.tv_sec) + (double)(high.tv_usec)/1000000.0 - ((double)(start.tv_sec) + (double)(start.tv_usec)/1000000.0);
	
	printf("\ntime %f\n", operatingTime);
	sprintf(infilename, "%s.txt", "hqt"); 
	fp2 = fopen(infilename, "w");
	for (j = 0; j < cnt ; j++)
	{
		fprintf(fp2, "%d\n", b[j].number);
	}
	fclose(fp2);
	fclose(fp1);
}


/* 쓰레드 함수 start : quick정렬 */
void *threadFunc(void *p)
{
	thread1* pp = (thread1*)p;
	input_size* part = pp -> array;
	int low = pp -> low;
	int high = pp -> high;
	
	quick_sort(part, low, high);
	if (boolean_sort == 1) { printf("\n*"); merge_sort(part, low, high); }
	pthread_exit(NULL);
}


/* 쓰레드 함수 start : quick정렬 종료 후 */
void *threadFuncFin(void *p)
{
	thread2* pp = (thread2*)p;
	input_size* PART = pp -> array;
	int low = pp -> low;
	int high = pp -> high;
	int mid = pp -> mid;
	fin(low, mid, high, PART);
	
	pthread_exit(NULL);
}


/* 쓰레드 함수 start : quick정렬 종료 후 */
void *threadFuncFin2(void *p)
{
	thread2* pp = (thread2*)p;
	input_size* PART = pp -> array;
	int low = pp -> low;
	int high = pp -> high;
	int mid = pp -> mid;
	fin2(low, mid, high, PART);
	
	pthread_exit(NULL);
}


/* 쓰레드 함수 처리결과에서 임시배열(sorted)에 b배열을 담아낸다. */
void fin(int low, int mid, int high, input_size* b)
{
	int i, j, k, l;
	i = low; j = mid + 1; k = low;
	
	/* 분할 정렬된 list의 합병 */
	/* 배열 sorted[]의 리스트를 배열 list[]로 재복사 */
	while (i <= mid && j <= high)
	{
		if (b[i].number <= b[j].number) sorted[k++] = b[i++].number;
		else sorted[k++] = b[j++].number;
	}
	/* sorted 배열에 임시 값 저장 */
	if (i > mid)
	{
		for (l = j; l <= high; l++) sorted[k++] = b[l].number;
	}
	else
	{
		for (l = i; l <= mid; l++) sorted[k++] = b[l].number;
	}
}


/* 쓰레드 함수 처리결과에서 b 배열에 임시배열(sorted)을 담아낸다. */
void fin2(int low, int mid, int high, input_size* b)
{
	int i, j, k, l;
	i = low; j = mid + 1; k = low;
	
	/* 분할 정렬된 list의 합병 */
	while (i <= mid && j <= high)
	{
		if (sorted[i] <= sorted[j]) b[k++].number = sorted[i++];
		else b[k++].number = sorted[j++];
	}
	if (i > mid) 
	{
		for (l = j; l <= high; l++) b[k++].number = sorted[l];
	}
	else
	{
		for (l = i; l <= mid; l++) b[k++].number = sorted[l];
	}
}


/* 퀵정렬 */
void quick_sort(input_size* b, int low, int high)
{
	int i, j, pivotindex,tmp;
	if(high - low < INSERT_SORT_DEADLINE)
	{
		insertion(low, high,b); return;
	}

	pivotindex  = low + (high - low)/3;
	
	swap(&b[pivotindex].number, &b[low].number);
	pivotindex = low;

	j = low;

	for(i = low ; i <= high ; i++)
	{
		// if (low == 0) { hit_combo++; } // quick_sort 함수가 불러와지면 일단 combo가 쌓인다.
		
		if(b[i].number < b[pivotindex].number)
		{
			// if (low == 0) { hit_combo = 0; } // 비교대상이 pivot보다 작으면 combo는 0으로 초기화
			// if ((cnt >= 100000) && (hit_combo >= 50)) // (배열 총 갯수가 10만개 이상) && (hit_combo >= 30 인 경우) => 합병 정렬로 전환
			// {
			// 	printf("*\n");
			// 	boolean_sort = 1;
			// 	return; // 재귀함수 탈출, Main함수로 돌아감.
			// }
					
			j++;
			swap(&b[j].number, &b[i].number);
		}
	}
	swap(&b[j].number, &b[pivotindex].number);

	quick_sort(b, low, j-1);
	quick_sort(b, j+1, high);
}


/* 합병정렬 : 분할 */
void merge_sort(input_size *b, int low, int high)
{
	if (low < high)
	{
	    int mid = (low+high)/2;
	    
		merge_sort(b, low, mid);
		merge_sort(b, mid+1, high);
		merge(b, low, mid, high);
	}
}


/* 합병정렬 : 정복 */
void merge(input_size *b, int low, int mid, int high)
{
	int i, j, k ,l;
    i = low;
    j = mid + 1;
    k = low;
    
    /* 분할 정렬된 list의 합병 */
    while(i <= mid && j <= high)
    {
        if (b[i].number <= b[j].number) { sorted[k++] = b[i++].number; }
        else { sorted[k++] = b[j++].number; }
    }
    
    // 남아 있는 값들을 일괄 복사
    if(i > mid)
    {
        for(l = j ; l <= high ; l++) sorted[k++] = b[l].number;
    }
    // 남아 있는 값들을 일괄 복사
    else
    {
        for(l=i ; l <= mid ; l++)
        sorted[k++] = b[l].number;
    }
    
    // 배열 sorted[](임시 배열)의 리스트를 배열 list[]로 재복사
    for(l = low ; l <= high ; l++) { b[l].number = sorted[l]; }
}


/* 삽입정렬 */
void insertion(int low, int high, input_size* b)
{
	int i,j,tmp;
	
	for(i=low + 1;i <= high;i++)
	{
		tmp = b[i].number;
		for(j = i ; j > low ; j--)
		{
			if( b[j-1].number > tmp)
			{
				b[j].number = b[j-1].number;
			} else break;
		}
		b[j].number = tmp;
	}
}


/* SWAP */
void swap (int *a, int *b)
{
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}