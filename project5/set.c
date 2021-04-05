#include "list.h"
#include "set.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define AVG_CHAIN_LENGTH 20

struct set
{
	int count; 
	int length;
	LIST **lists;
	int (*compare)();
	unsigned (*hash)();
};

static int search(SET *sp, void *elt, bool *found);

// Allocates memory to the set pointer and to the array of pointers that each point at a list; Then, it adds the pointers to each list into said array of pointers
// O(n)
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
	int i;	
	SET *sp;
	assert(compare!=NULL && hash!=NULL);
	sp=malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->length=maxElts / AVG_CHAIN_LENGTH;
	sp->compare=compare;
	sp->hash=hash;
	sp->count=0;
	sp->lists=malloc(sizeof(LIST *)*sp->length);
	assert(sp->lists!=NULL);
	for(i=0;i<sp->length;i++)
	{
		sp->lists[i]=createList(compare);
	}
	return sp;
}

// First, it traverses the array of pointers to the lists, freeing the list pointed to by each pointer by calling the destroyList function; Then, it frees the allocated sp->lists and sp
// O(n^2)
void destroySet(SET *sp)
{
	assert(sp!=NULL);
	int i;
	for(i=0;i<sp->length;i++)
		destroyList(sp->lists[i]);
	free(sp->lists);
	free(sp);	
}

// Returns the number of elements in the set pointed to by sp
// O(1)
int numElements(SET *sp)
{
	assert(sp!=NULL);
	return sp->count;
}

// Adds element to the proper list as determined by the private search function, only if the element is not found
// O(n^2)
void addElement(SET *sp, void *elt)
{
	assert(sp!=NULL && elt!=NULL);
	bool found=false;		
	int idx = search(sp, elt, &found);
	if(found==false)
	{
		addLast(sp->lists[idx],elt);
		sp->count++;
	}
}

// Removes element from the approriate list, only if the element is found. This function knows which list to search through due to the private search function return value
// O(n^3)
void removeElement(SET *sp, void *elt)
{
	assert(sp!=NULL && elt!=NULL);
	bool found=false;
	int idx=search(sp, elt, &found);
	if(found==true)
	{
		removeItem(sp->lists[idx],elt);
		sp->count--;
	}
}

// Locates the element pointed to by elt; If the private search functions finds the element, the element is returned. If the element is not found, NULL is returned
// O(n^3)
void *findElement(SET *sp, void *elt)
{
	assert(sp!=NULL && elt!=NULL);
	bool found = false;
	int idx = search(sp, elt, &found);
	if(found==false)
		return NULL;
	else
		return findItem(sp->lists[idx],elt);
}

// Allocates memory to a pointer to an array of void elements; It then uses a buffer array, items, to get the data from each individual list; Finally, the data is copied from the buffer array into the array that contains all of the elements in the set; This process is repeated until all of the lists in the set pointed to by sp are traversed
// O(n^3)
void *getElements(SET *sp)
{
	assert(sp!=NULL);
	void **temp=malloc(sizeof(void *)*sp->count);
	void **items;
	int i, j;
	int count;
	int tempCounter=0;
	for(i=0;i<sp->length;i++)
	{
		count=numItems(sp->lists[i]);
		items=malloc(sizeof(void *)*count);
		items=getItems(sp->lists[i]);
		for(j=0;j<count;j++)
		{
			temp[tempCounter]=items[j];
			tempCounter++;
		}
		
	}	
	return temp;
}

// Private search function that searches for an element pointed to by elt; It locates the list to traverse through using the hashing function; Finally, it returns the proper index for insertion, deletion, etc and changes the boolean for the result of the search
// O(n^2) 
static int search(SET *sp, void *elt, bool *found)
{
	assert(sp!=NULL && elt!=NULL);
	int idx=(*sp->hash)(elt)%sp->length;
	if(findItem(sp->lists[idx],elt)!=NULL)
		*found = true;
	else
		*found=false;
	return idx;	
}
