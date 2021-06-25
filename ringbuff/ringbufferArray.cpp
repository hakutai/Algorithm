/*
 * @filf ringbufferArray.cpp
 */
//#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct _RingBuffer {
	void		**containers;
	int			maxContainer;
	int			sizeData;					// size of pData
	int			countContainer;				// count of containers
	int			idxWrite,idxRead;

	int			AdjIdx(int idx) {				// private FUNCTION
		if (idx < 0) {
			return countContainer - 1;
		} else {
			return (idx % countContainer);
		}
	}

	void		Init(const int _maxContainer, const int _sizeData) { 
		maxContainer	= _maxContainer;
		sizeData   		= _sizeData;
		countContainer	= 0;	
		containers		= (void **)malloc(maxContainer * sizeof(void *));
		idxWrite = idxRead = 0;

		for (int i = 0; i < maxContainer; i++)
			containers[i] = nullptr;
	}

	int SizeOf() { return countContainer; }

	void Reset() { idxRead = AdjIdx(idxWrite - 1); }
					
	void Remove() {
		for (int i = 0; i < maxContainer; i++ ) {
			if (containers[i] != nullptr) 
				free(containers[i]);
		}
		free(containers);
	}

	void *Put(void *_pData) { 
		void	*pData;

		if (countContainer < maxContainer) {
			pData = malloc(sizeData);
			memcpy(pData,_pData,sizeData);
			containers[idxWrite] = pData;
			idxWrite = (idxWrite + 1) % maxContainer;;
			if (countContainer++ > maxContainer) 
				countContainer = maxContainer;
		} else {
			pData = memcpy(containers[idxWrite],_pData,sizeData);
			idxWrite = AdjIdx(++idxWrite);
		}

		idxRead = idxWrite - 1;
		return pData;
	}

		//--------- 
	void *Get(void *_pData) { 
		if (countContainer == 0) return nullptr;
		if (idxRead == -1) return nullptr;		
		memcpy(_pData,containers[idxRead],sizeData);
		idxRead = AdjIdx(--idxRead);
		if (idxRead == AdjIdx(idxWrite - 1)) idxRead = -1;

//printf("          R/W: %d %d\n",idxRead,idxWrite);
		return _pData;
	}

} RingBuffer;

int main() {
	int			i,j,*k;
	RingBuffer		*container = nullptr;

//	container = new RingBuffer;
	container = (RingBuffer *)malloc(sizeof(RingBuffer));
	container->Init(7,sizeof(int));
	i =  1;   container->Put((void *)&i);
	i =  2;   container->Put((void *)&i);
	i =  3;   container->Put((void *)&i);
	i =  4;   container->Put((void *)&i);
	i =  5;   container->Put((void *)&i);

	printf("size of : %d\n",container->SizeOf());
	container->Reset();
	for (i = 0; i < 12; i++) {
		if ((k = (int *)container->Get((void *)&j)) == nullptr) break;
		printf("Read %d(%d)\n",j,i);
	}
	printf("end\n");

	i = 32;   container->Put((void *)&i);

	printf("size of : %d\n",container->SizeOf());
	container->Reset();
	for (i = 0; i < 12; i++) {
		if ((k = (int *)container->Get((void *)&j)) == nullptr) break;
		printf("Read %d(%d)\n",j,i);
	}
	printf("end\n");

	i = 11;   container->Put((void *)&i);
	i = 12;   container->Put((void *)&i);
	i = 13;   container->Put((void *)&i);
	i = 14;   container->Put((void *)&i);
	i = 15;   container->Put((void *)&i);

	printf("size of : %d\n",container->SizeOf());
	container->Reset();
	for (i = 0; i < 12; i++) {
		if ((k = (int *)container->Get((void *)&j)) == nullptr) break;
		printf("Read %d(%d)\n",j,i);
	}
	printf("end\n");

	free(container);
}
