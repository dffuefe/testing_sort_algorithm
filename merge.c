#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define DATA_SIZE 70000000

int tmp_sorted[DATA_SIZE] = {0}; //합병 정렬에 쓰이는 임시 저장 공간

//입력받는 파일의 크기를 위한 구조체
typedef struct {
	int number;
}input_size;

void mergesort(input_size *msize, int low, int high);
void merge(input_size *msize, int low, int mid, int high);

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

	printf("Start Mergesort : %d\n", cnt);

	gettimeofday(&start, NULL); //정렬 시간 측정 시작

	int low = 0;
	int high = cnt - 1;
	mergesort(msize, low, high); //합병 정렬 함수

	gettimeofday(&end, NULL); //정렬 시간 측정 종료

	//시간 계산
	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	//usec이 음수일 경우 시간을 맞추기 위한 if문
	if (diff.tv_usec < 0) {
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}

	printf("합병 정렬 성공! 소요 시간 : %ld.%ld\n", diff.tv_sec, diff.tv_usec);

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

//합병정렬 분할
void mergesort(input_size *msize, int low, int high)
{
	if (low < high) {
		int mid = (low + high) / 2;

		mergesort(msize, low, mid);
		mergesort(msize, mid + 1, high);
		merge(msize, low, mid, high);
	}
}

//합병정렬 정복
void merge(input_size *msize, int low, int mid, int high)
{
	register int i, j, k;

	i = low;
	j = mid + 1;
	k = low;

	//분할 정렬된 배열을 합병
	while (i <= mid && j <= high) {
		if (msize[i].number <= msize[j].number) {
			tmp_sorted[k] = msize[i].number;
			i++;
		}
		else {
			tmp_sorted[k] = msize[j].number;
			j++;
		}
		k++;
	}

	//남은 배열의 원소들을 tmp 배열에 복사
	if (i > mid) { //앞쪽 배열이 뒤쪽 배열보다 빨리 끝났을 경우
		for (; j <= high; j++, k++) {
			tmp_sorted[k] = msize[j].number;
		}
	}
	else { //뒤쪽 배열이 앞쪽 배열보다 빨리 끝났을 경우
		for (; i <= mid; i++, k++) {
			tmp_sorted[k] = msize[i].number;
		}
	}

	//tmp에 저장되어있는 정렬된 배열을 msize 배열에 복사
	for (i = low; i <= high; i++) {
		msize[i].number = tmp_sorted[i];
	}
}