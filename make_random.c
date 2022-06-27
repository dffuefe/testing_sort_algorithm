/*
< 랜덤정수 생성기 >
5000만개 이상으로 랜덤정수를 생성시켜 줍니다.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 50000000

//input_size라는 구조체 정의
typedef struct {
	int number;
}input_size;

int main() {
	FILE *fp = fopen("random_num.txt", "w"); //읽기 전용으로 list.txt라는 텍스트 파일을 생성 
	int k;

	/* 32767의 하한을 뚫기위해 유동적인 메모리인 malloc 적용 */
	input_size* a = (input_size*)malloc(sizeof(input_size) * 50000000);

	// n개의 정수 생성 후 list.txt파일로 만들기.
	int input;
	printf("랜덤정수 생성 갯수 : ");
	scanf("%d", &input);
	srand((unsigned)time(NULL)); // Seed 생성
	//input만큼 랜덤하게 생성
	for (k = 1; k <= input; k++) {
		a->number = rand() % 100000000 + 1;
		fprintf(fp, "%d\t", a->number);
		if (k % 5 == 0) { fprintf(fp, "\n"); }
	}
	fclose(fp);
	printf("** 랜덤정수 생성 완료했습니다! **\n");
}