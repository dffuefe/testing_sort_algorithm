#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define DATA_SIZE 70000000

int tmp_sorted[DATA_SIZE] = {0}; //�պ� ���Ŀ� ���̴� �ӽ� ���� ����

//�Է¹޴� ������ ũ�⸦ ���� ����ü
typedef struct {
	int number;
}input_size;

void mergesort(input_size *msize, int low, int high);
void merge(input_size *msize, int low, int mid, int high);

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

	printf("Start Mergesort : %d\n", cnt);

	gettimeofday(&start, NULL); //���� �ð� ���� ����

	int low = 0;
	int high = cnt - 1;
	mergesort(msize, low, high); //�պ� ���� �Լ�

	gettimeofday(&end, NULL); //���� �ð� ���� ����

	//�ð� ���
	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	//usec�� ������ ��� �ð��� ���߱� ���� if��
	if (diff.tv_usec < 0) {
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}

	printf("�պ� ���� ����! �ҿ� �ð� : %ld.%ld\n", diff.tv_sec, diff.tv_usec);

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

//�պ����� ����
void mergesort(input_size *msize, int low, int high)
{
	if (low < high) {
		int mid = (low + high) / 2;

		mergesort(msize, low, mid);
		mergesort(msize, mid + 1, high);
		merge(msize, low, mid, high);
	}
}

//�պ����� ����
void merge(input_size *msize, int low, int mid, int high)
{
	register int i, j, k;

	i = low;
	j = mid + 1;
	k = low;

	//���� ���ĵ� �迭�� �պ�
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

	//���� �迭�� ���ҵ��� tmp �迭�� ����
	if (i > mid) { //���� �迭�� ���� �迭���� ���� ������ ���
		for (; j <= high; j++, k++) {
			tmp_sorted[k] = msize[j].number;
		}
	}
	else { //���� �迭�� ���� �迭���� ���� ������ ���
		for (; i <= mid; i++, k++) {
			tmp_sorted[k] = msize[i].number;
		}
	}

	//tmp�� ����Ǿ��ִ� ���ĵ� �迭�� msize �迭�� ����
	for (i = low; i <= high; i++) {
		msize[i].number = tmp_sorted[i];
	}
}