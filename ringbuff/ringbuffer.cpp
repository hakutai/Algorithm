/*
 * @file ringbuffer.cpp
 */
//#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct _RingBuffer {
	struct _Container {
		void			*pData;
		_Container		*prev;
		_Container		*next;
	} *containers;
	int			maxContainer;
	int			sizeData;					// size of pData
	int			countContainer;				// count of containers
	_Container	*pWrite  = nullptr;
	_Container	*pRead   = nullptr;

	void		Init(const int _maxContainer, const int _sizeData) { 
		maxContainer	= _maxContainer;
		sizeData   		= _sizeData;
		countContainer	= 0;	
		containers		= nullptr;
		pWrite  		= nullptr;
		pRead			= nullptr;
	}

	int SizeOf() { return countContainer; }

	void Reset() { pRead = pWrite; }
					
	_Container *Put(void *_pData) { 
		_Container	*pTmp,*p;

		if (countContainer < maxContainer) {
			pTmp = (_Container *)malloc(sizeof(_Container));
			pTmp->pData = malloc(sizeData);
			memcpy((void *)pTmp->pData,(void *)_pData,sizeData);
			pTmp->prev  = nullptr;
			pTmp->next  = nullptr;
			pWrite      = pTmp;
			if (containers == nullptr) {
				containers = pTmp;
			} else {
				p = containers;
				while (p->next != nullptr) { p = p->next; }
				pTmp->prev = p;
				p->next    = pTmp;
			}
			countContainer++;
			if (countContainer ==  maxContainer) {
				pWrite->next     = containers;
				containers->prev = pWrite;
			}
		} else {
			pWrite = pWrite->next;
			memcpy((void *)pWrite->pData,(void *)_pData,sizeData);
		}
		return (_Container *)pTmp;
	}

		//--------- 
	void *Get(void *_pData) { 

		if (containers == nullptr) return nullptr;
		if (pRead      == nullptr) return nullptr;

		memcpy(_pData,pRead->pData,sizeData);
		pRead = pRead->prev;
		if (pRead == pWrite) pRead = nullptr;

		return _pData;
	}

} RingBuffer;

int main() {
	int			i,j,*k;
	RingBuffer		*container = nullptr;

//	container = new RingBuffer;
	container = (RingBuffer *)malloc(sizeof(RingBuffer));
	container->Init(10,sizeof(int));

	i =  1;   container->Put((void *)&i);
	i =  2;   container->Put((void *)&i);
	i =  3;   container->Put((void *)&i);
	i =  4;   container->Put((void *)&i);
	i =  5;   container->Put((void *)&i);

	container->Reset();
	for (i = 0; i < 12; i++) {
		if ((k = (int *)container->Get((void *)&j)) == nullptr) break;
		printf("Read %d(%d)\n",j,container->SizeOf());
	}
	printf("end\n");

	i = 32;   container->Put((void *)&i);

	container->Reset();
	for (i = 0; i < 12; i++) {
		if ((k = (int *)container->Get((void *)&j)) == nullptr) break;
		printf("Read %d(%d)\n",j,container->SizeOf());
	}
	printf("end\n");

	i = 11;   container->Put((void *)&i);
	i = 12;   container->Put((void *)&i);
	i = 13;   container->Put((void *)&i);
	i = 14;   container->Put((void *)&i);
	i = 15;   container->Put((void *)&i);

	container->Reset();
	for (i = 0; i < 12; i++) {
		if ((k = (int *)container->Get((void *)&j)) == nullptr) break;
		printf("Read %d(%d)\n",j,container->SizeOf());
	}
	printf("end\n");

}
