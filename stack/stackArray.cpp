/*
 *	@file stack.cpp
 *
 */


//#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct _Stack {
	void		**containers;
	int			maxContainer;
	int			sizeData;					// size of pData
	int			countContainer;				// count of containers
	int			idxTop,idxBottom;			// Read,Write position 
	void		(*PrintData)(const int idx,const void *p);

	void		Init(const int _maxContainer, const int _sizeData,
						void( *_PrintData)(const int idx, const void *p)) {
		maxContainer	= _maxContainer;
		sizeData   		= _sizeData;
		containers		= (void **)malloc(maxContainer * sizeof(void *));
		idxTop = idxBottom = 0;
		PrintData		= _PrintData;

		for (int i = 0; i < maxContainer; i++)
			containers[i] = nullptr;
	}

	int SizeOf() { return idxBottom - idxTop; }

	void Reaset() { idxTop = idxBottom = 0; }
					
	void Remove() {
		for (int i = 0; i < maxContainer; i++) {
			if (containers[i] != nullptr)
				free(containers[i]);
		}
		free(containers);
	}

		//--------- DATA PUSH
	void *Push(void *_pData) { 
		void	*pTmp = nullptr;;

		if (idxBottom >= maxContainer) {
			if (idxTop > 0) {						// Gabage collection
				if (idxTop == idxBottom) {
					idxTop = idxBottom = 0;
				} else {
					for (int i = 0; i < maxContainer - 1; i++) {
						if (i + idxTop >= maxContainer) break;
						memcpy(containers[i],containers[i+idxTop],sizeData);
					}
					idxBottom -= idxTop;
					idxTop = 0;
				}
				memcpy(containers[idxBottom],_pData,sizeData);
				pTmp = containers[idxBottom++];
			}
		} else {
			containers[idxBottom] = (void *)malloc(sizeof(sizeData));
			memcpy(containers[idxBottom],_pData,sizeData);
			pTmp = containers[idxBottom++];
		}
		if (PrintData != nullptr) {
			printf("PUSH");
			PrintData(idxBottom - 1,pTmp);
			printf("\n");
		}

		return pTmp;
	}

		//--------- Get index
	void *Get(void *_pData,const int idx) { 
		if (!(idxTop <= idx && idx < idxBottom)) return nullptr;

		memcpy(_pData,containers[idx],sizeData);
		return _pData;

	}

		//--------- First In First Out
	void *PopFIFO(void *_pData) { 

		if ((idxBottom - idxTop) <= 0) return nullptr;
		memcpy(_pData,containers[idxTop++],sizeData);
		if (PrintData != nullptr) {
			printf("POP-FIFO");
			PrintData(idxTop - 1,_pData);
			printf("\n");
		}

		return _pData;
	}

		//--------- Last In First Out
	void *PopLIFO(void *_pData) { 
		if ((idxBottom - idxTop) <= 0) return nullptr;
		memcpy(_pData,containers[--idxBottom],sizeData);
		if (PrintData != nullptr) {
			printf("POP-LIFO");
			PrintData(idxBottom + 1,_pData);
			printf("\n");
		}
		return _pData;
	}

		//--------- Print Data
	void PrintStack() {

		if (PrintData == nullptr) return;

		printf("--- STACK ---\n");
		for (int i = 0; i < maxContainer; i++) {
			PrintData(i,containers[i]);
			if (i == idxTop) printf(" <-- top");
			if (i == idxBottom) printf(" <-- bottom");
			printf("\n");
		}
		if (idxTop == maxContainer) {
			printf("    <-- top");
			if (idxBottom == maxContainer) printf("    <-- bottom");
			printf("\n");
		}
		printf("'''''''''''''\n");
		
	}
} Stack;

typedef struct {
	int		i1,i2;
	double	d1,d2;
} MyData;

void PrintMyData(const int idx, const void *p) {
	MyData	tmp;
	
	if (p == nullptr) {
		printf("[%d] = null    ",idx);
	} else {
		tmp = *(MyData *)p;
		printf("[%d] = (%d,%f) ",idx,tmp.i1,tmp.d1);
	}
}

void PrintInt(const int idx, const void *p) {
	if (p == nullptr) {
		printf("[%d] = null ",idx);
	} else {
		printf("[%d] = %d   ",idx, *(int *)p);
	}
}

int main() {
	int			i,j,*k;
	Stack 		*container = NULL;
	Stack		*myDatas  = NULL;
	MyData		myPacket,myP2;

//	myDatas = new Stack;
	myDatas = (Stack *)malloc(sizeof(Stack));
	myDatas->Init(10,sizeof(MyData),PrintMyData);
	myPacket.i1		= 52;
	myPacket.d1		= 32.3;
	myDatas->Push((void *)&myPacket);
	myPacket.i1		= 100;
	myPacket.d1		= 153.35;
	myDatas->Push((void *)&myPacket);
	myDatas->PrintStack();

	printf("\n");
/*

	printf("myStruct : %d\n",((MyData *)myDatas->Get(&myP2,0))->i1);
	printf(" %lf --\n",myP2.d1);
*/
//	container = new Stack;
	container = (Stack *)malloc(sizeof(Stack));
	container->Init(5,sizeof(int),PrintInt);
	container->PrintStack();
	i = 123;   container->Push((void *)&i);
	i = 323;   container->Push((void *)&i);
	i = 10;   container->Push((void *)&i);
	i = 25;   container->Push((void *)&i);
	i = 999;   container->Push((void *)&i);
	container->PrintStack();
	i = 473;   container->Push((void *)&i);
	container->PrintStack();

	container->PopFIFO((void *)&j);
	container->PopFIFO((void *)&j);
	container->PrintStack();

	i = 86;   container->Push((void *)&i);
	container->PrintStack();

	container->PopLIFO((void *)&j);
	container->PrintStack();

	i = 777;   container->Push((void *)&i);
	container->PrintStack();

	container->PopFIFO((void *)&j);
	container->PopFIFO((void *)&j);
	container->PopFIFO((void *)&j);
	container->PopFIFO((void *)&j);
	container->PrintStack();
	i = 555;   container->Push((void *)&i);
	container->PrintStack();
	container->PopFIFO((void *)&j);
	container->PrintStack();

	i = 32;   container->Push((void *)&i);
	container->PrintStack();

}


