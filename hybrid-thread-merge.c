#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#define MAX 70000000

int sorted[MAX];
int count;
int check;
typedef struct{
	int low;
	int mid;
	int high;
	int *point;
}th_f;
typedef struct{
	int low;
	int high;
	int *point;
}thread;
void insertion(int low, int high, int arr[]){
	int i, j, key;
	for(i = low; i <= high; i++){
		key = arr[i];
		for(j = i-1; arr[j] > key && j >= low; j--)
			arr[j+1] = arr[j];
		arr[j+1] = key;
	}
}
void merge1(int low,int mid, int high, int arr[]){
        int i, j, k, l;

        i = low, j = mid+1, k = low;

        while(i <= mid && j <= high){
                if(arr[i] <= arr[j])
                        sorted[k++] = arr[i++];
                else
                        sorted[k++] = arr[j++];
        }

        if(i <= mid)
                for(l = i; l <= mid; l++)
                        sorted[k++] = arr[l];
        else
                for(l = j; l <= high; l++)
                        sorted[k++] = arr[l];
//	if(high == (MAX-1))
//		for(l = low; l <= high; l++)
//			arr[l] = sorted[l];
}
void merge(int low,int mid, int high, int arr[]){
        int i, j, k, l;

        i = low, j = mid+1, k = low;

        while(i <= mid && j <= high){
                if(arr[i] <= arr[j])
                        sorted[k++] = arr[i++];
                else
                        sorted[k++] = arr[j++];
        }

        if(i <= mid)
                for(l = i; l <= mid; l++)
                        sorted[k++] = arr[l];
        else
                for(l = j; l <= high; l++)
                        sorted[k++] = arr[l];
	
	for(l = low; l <= high; l++)
		arr[l] = sorted[l];
}
void merge2(int low, int mid, int high, int arr[]){
	int i, j, k, l;
	i = low, j = mid+1, k = low;
	
	while(i <= mid && j <= high){
		if(sorted[i] <= sorted[j])
			arr[k++] = sorted[i++];
		else
			arr[k++] = sorted[j++];
	}
	
	if(i < mid)
		for(l = i; l <= mid; l++)
			arr[k++] = sorted[l];
	else
		for(l = j; l <= high; l++)
			arr[k++] = sorted[l];
}
void mergeSort1(int low,int high, int arr[]){
        int mid;
        if(low < high){
		if(((high - low) <= 50)/* && (check < 50)*/) insertion(low, high, arr); 
		else{
                	mid = (low + high)/2;
                	mergeSort1(low,mid,arr);
               		mergeSort1(mid+1,high,arr);
			merge(low, mid, high, arr);		
        	}
	}
}
/*
void mergeSort2(int low,int high, int arr[]){
        int mid;
        if(low < high){
		if((high - low) <= 50) insertion(low, high, arr);
		else{ 
                	mid = (low + high)/2;
              		mergeSort2(low,mid,arr);
                	mergeSort2(mid+1,high,arr);
			merge2(low, mid, high, arr);
        	}
	}	
}
*/
void *thread_f(void *data){
	thread *p = (thread*)data;
	int *arr = p->point;
	int low = p->low;
	int high = p->high;
	mergeSort1(low, high, arr);
	pthread_exit(NULL);
}
void *th1(void *data){
	th_f *p = (th_f*)data;
	int low = p->low;
	int high = p->high;
	int mid = p->mid;
	int *arr = p->point;
	merge1(low, mid, high, arr);
	pthread_exit(NULL);
}
void *th2(void *data){
	th_f *p = (th_f*)data;
	int low = p->low;
	int high = p->high;
	int mid = p->mid;
	int *arr = p->point;
	merge2(low, mid, high, arr);
	pthread_exit(NULL);
}
int main(int argc,char *argv[]){

	struct timeval start, end;  //시간 변수
   	double operatingTime; 		//시간나타내기
	
        FILE *fp = fopen(argv[1],"r"); //파일 읽기
        FILE *fp2 = NULL; //결과 파일을 만들기 위해 준비
        int i = 0;//반복문 사용 준비
	int *temp;
	int *arr;
	int tid;

	pthread_t th[17];	//
	th_f th_mf[8];	//	Thread에 필요한
	th_f th_mm[4];	//	     정보
	th_f th_ml[2];	//		
	thread th_d[16];	//	Thread 생성
	th_f th_rf;

        //sorted = (int *)malloc(sizeof(int)*MAX);   //파일에서 값을 받아오는 배열
        arr = (int *)malloc(sizeof(int)*MAX);   //파일에서 값을 받아오는 배열
        temp = arr;
	
        while((fscanf(fp, "%d", &arr[i++])) != EOF);//arr에 파일을 통해 숫자 입력
        count = i-2;    //i가 마지막보다 +2 되기때문에 -2
	
/*	for(i = 0; arr[i]%1 != 0; i++)
		if(arr[i] <= arr[i+1])
			check++;
*/
	for(i = 0; i < 16; i++){
		if(i < 15){
			th_d[i].low = i*((count+1)/16);
			th_d[i].high = ((i+1)*((count+1)/16)) - 1;
			th_d[i].point = arr;
		}
		else{
			th_d[i].low = i*((count+1)/16);
			th_d[i].high = count;
			th_d[i].point = arr;
		}
	}

	for(i = 0; i < 8; i++){
		if(i < 7){
			th_mf[i].low = th_d[i*2].low;
			th_mf[i].high = th_d[(i*2)+1].high;
			th_mf[i].mid = (th_mf[i].low+th_mf[i].high)/2;
			th_mf[i].point = arr;
		}
		else{
			th_mf[i].low = th_d[i*2].low;
			th_mf[i].high = count;
			th_mf[i].mid = th_d[14].high;
			th_mf[i].point = arr;
		}	
	}
	
	for(i = 0; i < 4; i++){
		if(i < 3){
			th_mm[i].low = th_mf[i*2].low;
			th_mm[i].high = th_mf[(i*2)+1].high;
			th_mm[i].mid = (th_mm[i].low+th_mm[i].high)/2;
			th_mm[i].point = arr;
		}
		else{
			th_mm[i].low = th_mf[i*2].low;
			th_mm[i].high = count;
			th_mm[i].mid = th_mf[6].high;
			th_mm[i].point = arr;
		}
	}

	for(i = 0; i < 2; i++){
		if(i < 1){
			th_ml[i].low = th_mm[i*2].low;
			th_ml[i].high = th_mm[(i*2)+1].high;
			th_ml[i].mid = (th_ml[i].low+th_ml[i].high)/2;
			th_ml[i].point = arr;
		}
		else{
			th_ml[i].low = th_mm[i*2].low;
			th_ml[i].high = count;
			th_ml[i].mid = th_mm[2].high;
			th_ml[i].point = arr;
		}
	}
	
	/////////////////////// Thread create & implement ///////////////////
	gettimeofday(&start, NULL);  // 시간 시작
		
	for(i = 0; i < 16; i++)
		pthread_create(&th[i], NULL, thread_f, (void *)&th_d[i]); 
	for(i = 0; i < 16; i++)
		pthread_join(th[i], NULL);
	
	for(i = 0; i < 8; i++)
		pthread_create(&th[i], NULL, th1, (void *)&th_mf[i]);
	for(i = 0; i < 8; i++)
		pthread_join(th[i], NULL);

	for(i = 0; i < 4; i++)
		pthread_create(&th[i], NULL, th2, (void *)&th_mm[i]);
	for(i = 0; i < 4; i++)
		pthread_join(th[i], NULL);

	for(i = 0; i < 2; i++)
		pthread_create(&th[i], NULL, th1, (void *)&th_ml[i]);
	for(i = 0; i < 2; i++)
		pthread_join(th[i], NULL);

	merge2(0, th_ml[0].high, count, arr);	
	gettimeofday(&end, NULL); // 시간 끝
        //////////////////// Thread exit & stop ///////////////////////////
        
	arr = temp;
	fp2 = fopen("result.txt", "wt");      //결과 파일 새로 만들기
	for (i = 0; i <= count; i++){
        fprintf(fp2, "%d ", arr[i]);
        	if((i+1)%5 == 0)
                        fprintf(fp2, "\n");
        }

	

	operatingTime = (double)(end.tv_sec) + (double)(end.tv_usec)/1000000.0 - ((double)(start.tv_sec) + (double)(start.tv_usec)/1000000.0); //시간계산

	printf("time%f\n",operatingTime); // 시간 출력	

		
	free(arr);
        free(fp);
        free(fp2);
}
