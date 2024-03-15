//Arav Tulsi
//Dr. Neslisah Torosdagli
//COP3502C
//Lab Assignment 8

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
void mergeSort(int pData[], int l, int r)
{
	if (l<r) 
	{
		int mid = (l+r) / 2; //get mid

		//recursive call
		mergeSort(pData , l, mid); //first half
		mergeSort(pData, mid+1, r); //second half

		//SET-UP MERGE
		//get tracker variables
		int i, j, k; //left, right, mainArray
		//get sizes of subarrays
		int size_L = mid - l + 1;
		int size_R = r-mid;

		//make temp arrays
		int *arr_L = Alloc(sizeof(int) * size_L);
		int *arr_R = Alloc (sizeof(int) * size_R);

		//copy data
		for (i = 0; i < size_L; i++) //left
			arr_L[i] = pData[i + l];		

		for (j = 0; j < size_R; j++) //right
			arr_R[j] = pData[mid+1+j	];

		//MERGE
		// reset pos tracker variables
		i = 0; //left subarray tracker
		j = 0; //right subarray tracker
		k = 0; //main array tracker

		//itterate arrays and merge them
		//set the pData[k] to the smaller of the values
		while (i < size_L && j < size_R)
		{
			//if in order
			//set main array to left value
			//itterate left tracker
			if (arr_L[i] <= arr_R[j])
			{
				pData[k] = arr_L[i];
				i++;
			}
			//set main array to right value
			//itterate right tracker
			else //(arr_L[i] > arr_R[j]) only other case
			{
				pData[k] = arr_R[j];
				j++;
			}
			k++;
		}
		//sorting done

		//now handle any left over data, left then right
		/*for (i; i < size_L; i++) //left tracker less than size of leftarray
		{
			pData[k] = arr_L[i];
			k++;
		}

		for (j; j < size_R; j++) // right tracker less than size of right array
		{
			pData[k] = arr_R[j];
			k++;
		}*/

		while (i < size_L)
		{
			pData[k] = arr_L[i];
			i++;
			k++;
		}

		while (j < size_R)
		{
			pData[k] = arr_R[j];
			j++;
			k++;
		}

		// laslty de-allocate temporary arrays
		DeAlloc(arr_L);
		DeAlloc(arr_R);
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
	
	for (i=0;i<2;++i)
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