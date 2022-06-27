#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>

//�Է¹޴� ������ ũ�⸦ ���� ����ü
typedef struct {
	int number;
}input_size;

void bubblesort(input_size *msize, int size);

//���� �Լ�
int main(int argc, char *argv[])
{
	input_size *msize = (input_size *)malloc(sizeof(input_size) * 70000000); //7õ�� ���� ����

	int cnt = 0; //���� ���� ����

	struct timeval start, end, diff; //�ð� ������ ���� ����

	FILE *input; //�Է�����
	FILE *output; //�������

	char filename[50] = { '\0' }; //����� ������ �̸�

	printf("���� ������\n");

	//������ �Է¹޴� �Լ�
	if ((input = fopen(argv[1], "r")) == NULL) {
		printf("������ ���� ���߽��ϴ�.\n");
		exit(0);
	}

	int i = 0;
	while (fscanf(input, "%d", &msize[i++].number) != EOF) {
		cnt = i;
	}

	printf("Start Bubblesort : %d\n", cnt);

	gettimeofday(&start, NULL); //���� �ð� ���� ����

	bubblesort(msize, cnt); //���� ���� �Լ� ȣ��

	gettimeofday(&end, NULL); //���� �ð� ���� ����

	//�ð� ���
	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	//usec�� ������ ��� �ð��� ���߱� ���� if��
	if (diff.tv_usec < 0) {
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}

	printf("���� ���� ���� ����! �ҿ� �ð� : %ld.%ld\n", diff.tv_sec, diff.tv_usec);

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

//���� ����  �Լ�
void bubblesort(input_size *msize, int size)
{
	//���� �� ������ �ؼ� �������� ����غ�
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