//#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct _Stack {
	struct _Container {
		int				sortKey;
		void			*pData;
		_Container		*prev,*next;
	} *containers;
	int			sizeData;					// size of pData
	int			countContainer;				// count of containers

	void		Init(const int _sizeData) { 
		sizeData   		= _sizeData;
		countContainer	= 0;	
		containers		= nullptr;
	}

	void 	Remove() {
		_Container	*p,*pNext;
	
		if (containers == nullptr) return;

		p = containers;
		do {
			pNext = p->next;
			free(p);
		} while (pNext != nullptr);
		countContainer	= 0;	
		containers = nullptr;
	}

	int SizeOf() { return countContainer; }
					
		//--------- DATA PUSH
	_Container *Push(int _sortKey, void *_pData) { 
		_Container	*pTmp,*p;

		pTmp = (_Container *)malloc(sizeof(_Container));
		pTmp->sortKey = _sortKey;
		pTmp->pData   = malloc(sizeData);
		memcpy((void *)pTmp->pData,(void *)_pData,sizeData);
		pTmp->prev = pTmp->next = nullptr;
		if (containers == nullptr) {
			containers = pTmp;
		} else if (pTmp->sortKey < containers->sortKey) {
			pTmp->next = containers;
			containers->prev = pTmp;
			containers = pTmp;
		} else {
			p = containers;
			while (p->next != nullptr) {
				if (pTmp->sortKey < p->next->sortKey) {
					pTmp->next    = p->next;
					pTmp->prev    = p;
					p->next       = pTmp;
					p->next->prev = pTmp;
					pTmp = nullptr;
					break;
				}
				p = p->next; 
			}
			if (pTmp != nullptr) {
				pTmp->prev = p;
				p->next = pTmp;
			}
		}
		countContainer++;
		return (_Container *)pTmp;
	}

		//--------- Get index
	void *Get(void *_pData,const int idx) { 
		int				k;
		_Container		*p;	

		if (containers == nullptr) return nullptr;
		if (idx < 0 || countContainer <= idx) return nullptr;

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

		return _pData;
	}
} Stack;

typedef struct {
	int		i1,i2;
	double	d1,d2;
} MyData;

int main() {
	int			i,j,*k;
	Stack 		*container = NULL;
	Stack		*myDatas  = NULL;
	MyData		myPacket;
/*
	myDatas = new Stack;
	myDatas->Init(sizeof(MyData));
	myPacket.i1		= 52;
	myDatas->Push((void *)&myPacket);
	myPacket.i1		= 100;
	myPacket.d1		= 153.35;
	myDatas->Push((void *)&myPacket);

	printf("myStruct : %d\n",((MyData *)myDatas->Get(&myPacket,0))->i1);
	printf(" %d --\n",myPacket.i1);
*/	

//	container = new Stack;
	container = (Stack *)malloc(sizeof(Stack));
	container->Init(sizeof(int));

	i =  10;   container->Push(i,(void *)&i);
	i =  25;   container->Push(i,(void *)&i);
	i = 13;   container->Push(i,(void *)&i);
	i = 999;   container->Push(i,(void *)&i);
	i = 123;   container->Push(i,(void *)&i);
	i = 323;   container->Push(i,(void *)&i);

	for (i = 0; i < container->SizeOf(); i++) {
		container->Get((void *)&j,i);
		printf("GET %d - %d\n",i,j);
	}

	i =  32;  container->Push(i,(void *)&i);

	while ((k = (int *)container->PopLIFO((void *)&j)) != nullptr) {
		printf("LIFO %d(%d)\n",j,container->SizeOf());
	}
}


