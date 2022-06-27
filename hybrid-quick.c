/* 퀵정렬 소스 일부 참고 : https://coderkoo.tistory.com/7 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h> 
int cnt = 0; // 정수 겟수 카운트


// 구조체를 이용한 배열공간 할당
typedef struct {
   int number;
}input_size;

// 반복문에 임시적으로 쓰일 변수
int i = 0, j = 0, k = 0;

void quick_sort(input_size* b, int low, int high); // 퀵정렬
void swap (int *a, int *b); // SWAP

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
	
	printf("** start Quicksort : %d", cnt);
	gettimeofday(&start, NULL); // 파일 읽은 후, 정렬 연산에 대한 시간 측정 시작
	
	/* 퀵정렬 시작 */
	low = 0;
	high = cnt-1;
	quick_sort(b, low, high);
	
	gettimeofday(&end, NULL); // 시간 측정 종료
	operatingTime = (double)(end.tv_sec) + (double)(end.tv_usec)/1000000.0 - ((double)(start.tv_sec) + (double)(start.tv_usec)/1000000.0); // 시, 분, 초 계산
	printf("\ntime %f\n", operatingTime); // 시간 측정 결과
	printf("... 시간 측정 종료, 파일 쓰기 진행 ...\n");

	/* 마무리 과정 : 정렬된 결과를 파일로 출력 */
	sprintf(infilename, "%s.txt", "quick"); 
	fp2 = fopen(infilename, "w");
	for (j = 0; j < cnt ; j++)
	{
		fprintf(fp2, "%d\n", b[j].number);
	}
	
	printf("... 파일쓰기 종료! ...\n");
	
	fclose(fp2);
	fclose(fp1);
}

void quick_sort(input_size* b, int low, int high)
{
	int pivot = low;
	int i = low+1;
	int j = pivot;
	
	if (low < high)
	{
		while(i <= high)
		{
			if (b[i].number < b[pivot].number)
			{
				j++;
				swap(&b[j].number, &b[i].number);
			}
			i++;
		}
		swap(&b[low].number, &b[j].number);
		pivot = j;
		
		quick_sort(b, low, pivot-1);
		quick_sort(b, pivot+1, high);
	}
}

void swap (int *a, int *b)
{
	int tmp;
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}