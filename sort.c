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
	//terminating condition
	if (l>=r) //list already sorted
		return;
	// else operation valid

	int midpoint = (l+r) / 2; //get midpoint

	//recursive call
	mergeSort(pData , l, midpoint); //first half
	mergeSort(pData, midpoint+1, r); //second half

	//SET-UP MERGE
	//get tracker variables
	int L_Tracker, R_Tracker, mainArray_Tracker; //left, right, mainArray
	//get sizes of subarrays
	int size_L = midpoint - l + 1;
	int size_R = r-midpoint;

	//split the array into halves
		//first make subarrays and populate them
		int *L_SubArray = Alloc(sizeof(int) * size_L);
		for (L_Tracker = 0; L_Tracker < midpoint-l+1; L_Tracker++) //populate
		{
			L_SubArray[L_Tracker] = pData[L_Tracker];
		}

		int *R_SubArray = Alloc (sizeof(int) * size_R);
		for (R_Tracker = 0; R_Tracker < r-midpoint; R_Tracker++) //populate
		{
			R_SubArray[R_Tracker] = pData[R_Tracker+midpoint+1];
		}

	//MERGE
	// reset pos tracker variables
	L_Tracker = 0; //left subarray tracker
	R_Tracker = 0; //right subarray tracker
	mainArray_Tracker = 0; //main array tracker

	//then itterate arrays and merge them
		//magnitude checked to determine position
		//set the main array to the smaller of the values
	while (L_Tracker < size_L && R_Tracker < size_R) // at midpoinnt: left counter end, right counter start
	{
		//if in order
		if (L_SubArray[L_Tracker] <= R_SubArray[R_Tracker])
		{
			//set main array to left value
			//itterate left tracker
			pData[mainArray_Tracker] = L_SubArray[L_Tracker];
			L_Tracker++;
		}

		//if reverse-order
		else if (L_SubArray[L_Tracker] > R_SubArray[R_Tracker]) //only other case
		{
			//set main array to right value
			//itterate right tracker
			pData[mainArray_Tracker] = R_SubArray[R_Tracker];
			R_Tracker++;
		}

		//update main arrray tracker
		mainArray_Tracker++;
	}
	//sorting done

	//now handle any left over data, left then right
	for (L_Tracker; L_Tracker < size_L; L_Tracker++, mainArray_Tracker++) //left tracker less than size of leftarray
	{
		pData[mainArray_Tracker] = L_SubArray[L_Tracker];
	}

	for (R_Tracker; R_Tracker < size_R; R_Tracker++, mainArray_Tracker++) // right tracker less than size of right array
	{
		pData[mainArray_Tracker] = R_SubArray[R_Tracker];
	}

	// laslty de-allocate temporary arrays
	DeAlloc(L_SubArray);
	DeAlloc(R_SubArray);
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