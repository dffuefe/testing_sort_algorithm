#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h> 
int cnt = 0; // 정수 겟수 카운트
int hit_combo = 0; // 연속적인 정렬 횟수 체크
int tmp_sorted[70000000]; // 합병정렬 때 쓰일 임시 메모리 공간

/*
< 합병정렬 시간측정 요약 (-O3 옵션 넣음.) >
[일반] 10만개 비정렬 vs 역정렬
비정렬 : 0.008123초
역정렬(...,5,4,3,2,1) : 0.004454초

[일반] 10만개 비정렬 vs 역정렬
비정렬 : 10.163946초
역정렬(...,5,4,3,2,1) : 6.486494초
*/


// 구조체를 이용한 배열공간 할당
typedef struct {
   int number;
}input_size;

// 반복문에 임시적으로 쓰일 변수
int i = 0, j = 0, k = 0;

void merge_sort(input_size *b, int low, int high); // 합병정렬 : 분할
void merge(input_size *b, int low, int mid, int high); // 합병정렬 : 정복

int main(int argc, char **argv) {
	input_size* b = (input_size*)malloc(sizeof(input_size) * 70000000); // 7000만개 공간 동적할당
	FILE *fp1 = fopen(argv[1],"r"); // 랜덤정수 파일을 읽음.
	FILE *fp2; // 정렬 결과를 파일로 출력
	char infilename[20]; // 파일을 출력하면서 파일이름 설정
	
	/* 시간 측정 관련 */
	double operatingTime; // 연산 시간 측정결과
	struct timeval start, end;
	
	int pivot, low, high, high2;
	
	/* 처음 시작 : 파일을 읽음. */
	while (fscanf(fp1, "%d", &b[i++].number) != EOF) 
	{
		cnt = i;
	}
	
	printf("** start Merge sort : %d", cnt);
	gettimeofday(&start, NULL); // 파일 읽은 후, 정렬 연산에 대한 시간 측정 시작
	
	/* 합병정렬 시작 */
	high = cnt-1;
	merge_sort(b, 0, high);
	
	gettimeofday(&end, NULL); // 시간 측정 종료
	operatingTime = (double)(end.tv_sec) + (double)(end.tv_usec)/1000000.0 - ((double)(start.tv_sec) + (double)(start.tv_usec)/1000000.0); // 시, 분, 초 계산
	printf("\ntime %f\n", operatingTime); // 시간 측정 결과
	printf("... 시간 측정 종료, 파일 쓰기 진행 ...\n");

	/* 마무리 과정 : 정렬된 결과를 파일로 출력 */
	sprintf(infilename, "%s.txt", "merge"); 
	fp2 = fopen(infilename, "w");
	for (j = 0; j < cnt ; j++)
	{
		fprintf(fp2, "%d\n", b[j].number);
	}
	
	printf("... 파일쓰기 종료! ...\n");
	
	fclose(fp2);
	fclose(fp1);
}

// 합병정렬 : 분할
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

// 합병정렬 : 정복
void merge(input_size *b, int low, int mid, int high)
{
    int i, j, k ,l;
    i = low;
    j = mid + 1;
    k = low;
    
    /* 분할 정렬된 list의 합병 */
    while(i <= mid && j <= high)
    {
        if (b[i].number <= b[j].number) { tmp_sorted[k++] = b[i++].number; }
        else { tmp_sorted[k++] = b[j++].number; }
    }
    
    // 남아 있는 값들을 일괄 복사
    if(i > mid)
    {
        for(l = j ; l <= high ; l++) tmp_sorted[k++] = b[l].number;
    }
    // 남아 있는 값들을 일괄 복사
    else
    {
        for(l=i ; l <= mid ; l++)
        tmp_sorted[k++] = b[l].number;
    }
    
    // 배열 sorted[](임시 배열)의 리스트를 배열 list[]로 재복사
    for(l = low ; l <= high ; l++) { b[l].number = tmp_sorted[l]; }
}