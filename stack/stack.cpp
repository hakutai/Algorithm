/*
 *	@file stack.cpp
 *
 */


//#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct _Stack {
	struct _Container {
		void			*pData;
		_Container		*next;
	} *containers;
	int			sizeData;					// size of pData
	int			countContainer;				// count of containers
	void		(*printData)(const int idx, const void *p);

	void		Init(const int _sizeData,
					void(*_printData)(const int idx,const void *p)) { 
		sizeData   		= _sizeData;
		printData		= _printData;
		countContainer	= 0;	
		containers		= nullptr;
	}

	void		Remove() {
		_Container	*p,*pNext;

		if (containers == nullptr) return;

		p = containers;
		do {
			pNext = p->next;
			free(p);
		} while (pNext != nullptr);
		containers = nullptr;
	}

	int SizeOf() { return countContainer; }
					
		//--------- DATA PUSH
	_Container *Push(void *_pData) { 
		_Container	*pTmp,*p;

		pTmp = (_Container *)malloc(sizeof(_Container));
		pTmp->pData = malloc(sizeData);
		memcpy((void *)pTmp->pData,(void *)_pData,sizeData);
		pTmp->next  = nullptr;
		if (containers == nullptr) {
			containers = pTmp;
		} else {
			p = containers;
			while (p->next != nullptr) { p = p->next; }
			p->next = pTmp;
		}
		countContainer++;

		if (printData != nullptr) {
			printf("PUSH");
			printData(countContainer - 1,_pData);
			printf("\n");
		}
		return (_Container *)pTmp;
	}

		//--------- Get 
	void *Get(void *_pData,const int idx) { 
		int				k;
		_Container		*p;	

		if (containers == nullptr) return nullptr;
		if (idx < 0 && countContainer <= idx) return nullptr;

		p = containers;
		k = 0;
		while (k != idx) { p = p->next; k++; }

		memcpy(_pData,p->pData,sizeData);
		
		return _pData;

	}

		//--------- First In First Out
	void *PopFIFO(void *_pData) { 
		_Container	*pCurt;

		if (containers == nullptr) return nullptr;
		pCurt = containers;
		containers = pCurt->next;

		memcpy(_pData,pCurt->pData,sizeData);

		free(pCurt->pData);
		free(pCurt);
		countContainer--;

		if (printData != nullptr) {
			printf("POP-FIFO");
			printData(countContainer,_pData);
			printf("\n");
		}

		return _pData;
	}

		//--------- Last In First Out
	void *PopLIFO(void *_pData) { 
		_Container	*pCurt,*pPrev = nullptr;

		if (containers == nullptr) return nullptr;
		pCurt = containers;
		
		while (pCurt->next != nullptr) { pPrev = pCurt; pCurt = pCurt->next; }
		if (pPrev == nullptr) {
			memcpy(_pData,containers->pData,sizeData);
			free(containers->pData);
			free(containers);
			containers = nullptr;
		} else {
			memcpy(_pData,pCurt->pData,sizeData);
			free(pCurt->pData);
			free(pPrev->next);
			pPrev->next = nullptr;
		}
		countContainer--;

		if (printData != nullptr) {
			printf("POP-LIFO");
			printData(countContainer,_pData);
			printf("\n");
		}

		return _pData;
	}

	void PrintStack() {
		int				idx;
		_Container		*pCurt;

		printf("--- STACK ---\n");
		if (containers == nullptr)  {
			printf("   -EMPTY-\n");
		} else {
			pCurt = containers; 
			idx = 0;
			do {
				printData(idx,pCurt->pData);
				printf("\n");
				idx++;
				pCurt = pCurt->next;
			} while (pCurt != nullptr);
		}
		printf("'''''''''''''\n");
	}
			
		
} Stack;

typedef struct {
	int		i1,i2;
	double	d1,d2;
} MyData;

void PrintMyData(const int idx, const void *p) {
	MyData		tmp;

	if (p == nullptr) {
		printf("[%d] = null    ",idx);
	} else {
		tmp = *(MyData *)p;
		printf("[%d] = (%d,%f) ",idx,tmp.i1,tmp.d1);
	}
}

void PrintInt(const int idx, const void *p) {
	if (p ==  nullptr) {
		printf("[%d] = null ",idx);
	} else {
		printf("[%d] = %d   ",idx,*(int *)p);
	}
}

int main() {
	int			i,j,*k;
	Stack 		*container = NULL;
	Stack		*myDatas  = NULL;
	MyData		myPacket;

//	myDatas = new Stack;
	myDatas = (Stack *)malloc(sizeof(Stack));
	myDatas->Init(sizeof(MyData),PrintMyData);
	myPacket.i1		= 52;
	myDatas->Push((void *)&myPacket);
	myPacket.i1		= 100;
	myPacket.d1		= 153.35;
	myDatas->Push((void *)&myPacket);

	printf("myStruct : %d\n",((MyData *)myDatas->Get(&myPacket,0))->i1);
	printf(" %d --\n",myPacket.i1);
	

//	container = new Stack;
	container = (Stack *)malloc(sizeof(Stack));
	container->Init(sizeof(int),PrintInt);

	container->PrintStack();
	i = 123;   container->Push((void *)&i);
	i = 323;   container->Push((void *)&i);
	i = 10;   container->Push((void *)&i);
	i = 25;   container->Push((void *)&i);
	i = 999;   container->Push((void *)&i);
	container->PrintStack();

	if ((k = (int *)container->Get((void *)&j,3)) != nullptr) {;
		printf("get(3) %d(%d)\n",*k,j);
	}

	i = 32;   container->Push((void *)&i);

	while ((k = (int *)container->PopLIFO((void *)&j)) != nullptr);
}


