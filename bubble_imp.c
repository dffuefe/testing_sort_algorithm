#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>

//입력받는 파일의 크기를 위한 구조체
typedef struct {
	int number;
}input_size;

void bubblesort(input_size *msize, int size);

//메인 함수
int main(int argc, char *argv[])
{
	input_size *msize = (input_size *)malloc(sizeof(input_size) * 70000000); //7천만 개의 공간

	int cnt = 0; //들어온 정수 개수

	struct timeval start, end, diff; //시간 측정을 위한 변수

	FILE *input; //입력파일
	FILE *output; //출력파일

	char filename[50] = { '\0' }; //출력할 파일의 이름

	printf("파일 여는중\n");

	//파일을 입력받는 함수
	if ((input = fopen(argv[1], "r")) == NULL) {
		printf("파일을 열지 못했습니다.\n");
		exit(0);
	}

	int i = 0;
	while (fscanf(input, "%d", &msize[i++].number) != EOF) {
		cnt = i;
	}

	printf("Start Bubblesort : %d\n", cnt);

	gettimeofday(&start, NULL); //정렬 시간 측정 시작

	bubblesort(msize, cnt); //버블 정렬 함수 호출

	gettimeofday(&end, NULL); //정렬 시간 측정 종료

	//시간 계산
	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	//usec이 음수일 경우 시간을 맞추기 위한 if문
	if (diff.tv_usec < 0) {
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}

	printf("향상된 버블 정렬 성공! 소요 시간 : %ld.%ld\n", diff.tv_sec, diff.tv_usec);

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

//버블 정렬  함수
void bubblesort(input_size *msize, int size)
{
	//조금 더 빠른가 해서 레지스터 사용해봄
	register int i, j, tmp, flag = 1;

	i = size - 1;

	while (flag && i != 0) {
		for (j = 0; j <= i - 1; j++) {
			if (msize[j].number > msize[j + 1].number) {
				flag = 1;
				tmp = msize[j].number;
				msize[j].number = msize[j + 1].number;
				msize[j + 1].number = tmp;
			}
		}
		i--;
	}
}