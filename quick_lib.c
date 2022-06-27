#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

//�Է¹޴� ������ ũ�⸦ ���� ����ü
typedef struct {
	int number;
}input_size;

int static compare(const void *first, const void *second);

//���� �Լ�
int main(int argc, char *argv[])
{
	input_size *msize = (input_size *)malloc(sizeof(input_size) * 70000000); //7õ�� ���� ����

	int cnt = 0; //���� ���� ����

	struct timeval start, end, diff; //�ð� ������ ���� ����

	FILE *input; //�Է�����
	FILE *output; //�������

	char filename[50] = {'\0'}; //����� ������ �̸�

	printf("���� ������\n");

	//������ �Է¹޴� �Լ�
	int i = 0;

	while (fscanf(input, "%d", &msize[i++].number) != EOF) {
		cnt = i;
	}

	printf("Start Quicksort : %d\n", cnt);

	gettimeofday(&start, NULL);
	qsort(msize, cnt, sizeof(int), compare);
	gettimeofday(&end, NULL);

	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	if (diff.tv_usec < 0) {
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}

	printf("���̺귯�� ������ ����!  �ҿ� �ð� : %ld.%ld\n", diff.tv_sec, diff.tv_usec);

	//������ ����ϴ� �Լ�
	sprintf(filename, "%s_result_%s", argv[0], argv[1]); //���������̸�_result_����������

	output = fopen(filename, "w");

	//��� ���� ����(5���� ���)
	int j = 0;

	for (j = 0; j < cnt; j++) {
		fprintf(output, "%10d ", msize[j].number);

		if ((j + 1) % 5 == 0) {
			fprintf(output, "\n");
		}
	}

	printf("���Ͼ��� ����! ���α׷��� �����մϴ�.\n");

	fclose(output);
	fclose(input);

	return 0;
}

int static compare(const void *first, const void *second)
{
	if (*(int*)first > *(int*)second)
		return 1;
	else if (*(int*)first < *(int*)second)
		return -1;
	else
		return 0;
}