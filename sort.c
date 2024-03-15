#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {
	if (l < r) {

        // get the mid point of the array
        int m = (l+r)/2;

        // Sort each half of the array 
        mergeSort(pData, l, m);
        mergeSort(pData, m+1, r);

        // Merge the two halves
        int i, j, k;
        int num1 = m - l + 1;
        int num2 = r - m;
        
        // create temporary arrays
        int *Left = (int *)Alloc(sizeof(int) * num1);
        int *Right = (int *)Alloc(sizeof(int) * num2);
        
        // Copy data to temporary arrays L[] and R[]
        for (i = 0; i < num1; i++) {
            Left[i] = pData[l + i];
		}
        for (j = 0; j < num2; j++) {
            Right[j] = pData[m + 1 + j];
		}
        
		i = 0;
		j = 0;
        k = l; // represents index in the pData array

		//Goes thorugh both arrays and determines which sorted index is the lowest and which is the highest
		//Then appends lowest element back to pData and adds 1 to each of the indexs
        while (i < num1 && j < num2) {
            if (Left[i] <= Right[j]) {
                pData[k] = Left[i];
                i++;
            } else {
                pData[k] = Right[j];
                j++;
            }
            k++;
        }
        
        // Adds remaining elements of array L if any remain
        while (i < num1) {
            pData[k] = Left[i];
            i++;
            k++;
        }
        
        // Adds remaining elements of array R if any remain
        while (j < num2) {
            pData[k] = Right[j];
            j++;
            k++;
        }
        
        // Deallocate temporary arrays memory
        DeAlloc(Left);
        DeAlloc(Right);
    }
}
// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
	int firstHundred = (dataSz < 100 ? dataSz : 100);
	printf("\tData:\n\t");
	for (i=0;i<firstHundred;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}