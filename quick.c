#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

//입력받는 파일의 크기를 위한 구조체
typedef struct {
	int number;
}input_size;

void swap(int *a, int *b);
void partition(input_size *msize, int low, int high, int *pivotpoint);
void quicksort(input_size *msize, int low, int high);

//메인 함수
int main(int argc, char *argv[])
{
	input_size *msize = (input_size *)malloc(sizeof(input_size) * 70000000); //7천만 개의 공간

	int cnt = 0; //들어온 정수 개수

	struct timeval start, end, diff; //시간 측정을 위한 변수

	FILE *input; //입력파일
	FILE *output; //출력파일

	char filename[50] = {'\0'}; //출력할 파일의 이름

	printf("파일 여는중\n");

	//파일을 입력받는 함수
	int i = 0;

	while (fscanf(input, "%d", &msize[i++].number) != EOF) {
		cnt = i;
	}

	printf("Start Quicksort : %d\n", cnt);

	gettimeofday(&start, NULL); //정렬 시간 측정 시작

	int low = 0;
	int high = cnt - 1;
	quicksort(msize, 0, cnt - 1); //빠른 정렬 함수

	gettimeofday(&end, NULL); //정렬 시간 측정 종료

	//시간 계산
	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	//usec이 음수일 경우 시간을 맞추기 위한 if문
	if (diff.tv_usec < 0) {
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}

	printf("퀵 정렬 성공! 소요 시간 : %ld.%ld\n", diff.tv_sec, diff.tv_usec);

	//파일을 출력하는 함수
	sprintf(filename, "%s_result_%s", argv[0], argv[1]); //실행파일이름_result_데이터파일
	output = fopen(filename, "w");

	//출력 형태 정리(5개씩 출력)
	int j = 0;
	for (j = 0; j < cnt; j++) {
		fprintf(output, "%10d ", msize[j].number);
		if ((j + 1) % 5 == 0) {
			fprintf(output, "\n");
		}
	}

	printf("파일쓰기 성공! 프로그램을 종료합니다.\n");

	fclose(output);
	fclose(input);

	return 0;
}

//swap 함수
void swap(int *a, int *b)
{
	int tmp = 0;

	tmp = *a;
	*a = *b;
	*b = tmp;
}