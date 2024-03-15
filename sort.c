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
	if (l>=r)
		return; 
	
	int mid = (l+r) / 2; //get mid

	//recursive call
	mergeSort(pData , l, mid); //first half
	mergeSort(pData, mid+1, r); //second half

	//SET-UP MERGE
	//get tracker variables
	int l_count, r_count, p_count; //left, right, mainArray
	//get sizes of subarrays
	int size_L = mid - l + 1;
	int size_R = r-mid;

	//make temp arrays
	int *arr_L = Alloc(sizeof(int) * size_L);
	int *arr_R = Alloc (sizeof(int) * size_R);

	//copy data
	for (l_count = 0; l_count < size_L; l_count++) //left
		arr_L[l_count] = pData[l_count + l];		

	for (r_count = 0; r_count < size_R; r_count++) //right
		arr_R[r_count] = pData[mid+1+r_count	];

	//EXECUTE MERGE
	// reset pos tracker variables
	l_count = 0; //left subarray tracker
	r_count = 0; //right subarray tracker
	p_count = l; //main array tracker

	//itterate arrays and merge them
	//set the pData[p_count] to the smaller of the values
	while (l_count < size_L && r_count < size_R)
	{
		//if in order
		//set main array to left value
		//itterate left tracker
		if (arr_L[l_count] <= arr_R[r_count])
		{
			pData[p_count] = arr_L[l_count];
			l_count++;
		}
		//set main array to right value
		//itterate right tracker
		else //(arr_L[l_count] > arr_R[r_count]) only other case
		{
			pData[p_count] = arr_R[r_count];
			r_count++;
		}
		p_count++;
	}
	//sorting done

	//now handle any left over data, left then right
	for (l_count; l_count < size_L; l_count++,p_count++) //left tracker less than size of leftarray
	{
		pData[p_count] = arr_L[l_count];
	}

	for (r_count; r_count < size_R; r_count++,p_count++) // right tracker less than size of right array
	{
		pData[p_count] = arr_R[r_count];
	}

	// laslty de-allocate temporary arrays
	DeAlloc(arr_L);
	DeAlloc(arr_R);
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
	
	for (i=0;i<3;++i)
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