#include "pqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define p(x) (((x)-1)/2)
#define l(x) ((x)*2+1)
#define r(x) ((x)*2+2)
#define INIT_LENGTH 10

struct pqueue
{
	int count;
	int length;
	void **data;
	int (*compare)();
};

// Allocates memory to the pointer to the priority queue structure and to the pointer to the void array within the structure; Also, returns the pointer to the prirotiy queue structure
// O(1)
PQ *createQueue(int (*compare)())
{
	PQ *pq;
	assert(compare!=NULL);
	pq=malloc(sizeof(PQ));
	assert(pq!=NULL);
	pq->compare=compare;
	pq->length=INIT_LENGTH;
	pq->count=0;
	pq->data=malloc(sizeof(void *)*pq->length);
	assert(pq->data!=NULL);
	return pq;
}

// Frees the allocated data; First, it frees the pointer to the void array. Then, it frees the pointer to the priority queue structure, pq
// O(1)
void destroyQueue(PQ *pq)
{
	assert(pq!=NULL);
	free(pq->data);
	free(pq);
}

// Returns the number of entries in the priority queue pointed to by pq
// 0(1)
int numEntries(PQ *pq)
{
	assert(pq!=NULL);
	return pq->count;
}

// Adds entry into the next available index in the priority queue, so that the structure of the min heap is maintained; Then, the new entry is reheaped up; If the new entry is less than the parent, then the entry is switched with its parent; Otherwise, the program ends
// O(log n)
void addEntry(PQ *pq, void *entry)
{
	assert(pq!=NULL);
	if(pq->count==pq->length)
	{
		pq->length*=2;
		pq->data=realloc(pq->data,sizeof(void*)*pq->length);
		assert(pq->data!=NULL);
	}
	pq->data[pq->count]=entry;
	int childidx=pq->count;
	int parentidx=p(pq->count);
	while((*pq->compare)(pq->data[childidx],pq->data[parentidx])<0)
	{
		void *temp=pq->data[parentidx];
		pq->data[parentidx]=pq->data[childidx];
		pq->data[childidx]=temp;
		childidx=parentidx;
		parentidx=p(parentidx);
	}
	pq->count++;
}	

// Removes and returns the smallest entry from the priority queue; A void pointer pDel is set to remember the smallest entry, so that it can be returned to the interface; The entry at the end of the priority queue replaces the smallest entry in the priority queue, so that the structure of the min heap is maintained; Then, the moved entry is reheaped down; Depending on which child is smaller than the other and if that smaller child is less than the parent, then the entries are switched; This process occurs until the children are greater than the parent or the leaves of the min heap are reached; At this point, the removed entry is returned to the interface
// O(log n)
void *removeEntry(PQ *pq)
{
	assert(pq!=NULL);
	void *pDel=pq->data[0];
	pq->data[0]=pq->data[pq->count-1];
	int parentidx=0;
	while (l(parentidx)<pq->count)
	{
		int smaller=l(parentidx);
		if(r(parentidx)<pq->count)
			if((*pq->compare)(pq->data[r(parentidx)],pq->data[l(parentidx)])<0)
				smaller=r(parentidx);
		if((*pq->compare)(pq->data[parentidx],pq->data[smaller])<0)
			break;
		void *temp=pq->data[parentidx];
		pq->data[parentidx]=pq->data[smaller];
		pq->data[smaller]=temp;
		parentidx=smaller;
	}	
	pq->count--;
	return pDel;
}
