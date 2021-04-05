#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include <stdbool.h>

struct set
{
	char **data;
	int length;
	int count;
};

static int search(SET *sp, char *elt, bool *found);

// Creates and allocates memory to the set that holds the pointer to the array of pointers that point to the strings, the current number of elements in the array, and the max number of elements in the array
// O(1)
SET *createSet(int maxElts)
{
	SET *sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->count=0;
	sp->length=maxElts;
	sp->data = malloc(sizeof(char*)*maxElts);
	assert(sp->data!=NULL);
	return sp;
}

// Frees up the memory allocated to the set in a backwards fashion, starting from each string and ending at pointer sp;
// O(n)
void destroySet(SET *sp) 
{
	assert(sp!=NULL);
	int i;
	for(i=0;i<sp->count;i++) 
	{
		free(sp->data[i]); 
	}
	free(sp->data);
	free(sp); 
}

// Returns the number of elements in the array pointed to by char **data
// O(1)
int numElements(SET *sp)
{
	assert(sp!=NULL);
	return sp->count;
}

// Adds the element to the array pointed to by char **data assuming that the element is not already in the array and that the array is not full, while also updating the count of elements in the array; First, it shifts the elements below the desired slot downwards in a backwards fashion; Then, it inserts the element
// O(n) 
void addElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL && sp->count < sp->length);
	int i, idx;
	bool found = false;
	idx=search(sp, elt, &found);
	if(found==false)
	{
		for(i=sp->count; i>idx; i--)
		{
			sp->data[i]=sp->data[i-1];
		}
		sp->data[idx]=strdup(elt);
		sp->count++;
	}	
}

// Removes the element from the array pointed to by char **data assuming that the element is in said array, while also updating the count of elements in the array; First, it frees the memory for the deleted element; Then, it shifts the elements upward
// O(n) 
void removeElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL);
	int i, idx;
	bool found = false;
	idx=search(sp, elt, &found);
	if(found==true)
	{
		free(sp->data[idx]);
		for(i=idx+1; i<sp->count; i++)
		{
			sp->data[i-1]=sp->data[i];
		}
		sp->count--;
	}
}

// Public search function that finds the element pointed to by char *elt and returns the string of the element if found. Else, it returns NULL to indicate that the element was not found
// O(log n) 
char *findElement(SET *sp, char *elt)
{
	assert(sp!=NULL);
	bool found = false;
	int idx;
	idx=search(sp, elt, &found);
	if(found==false)
		return NULL;
	else
		return sp->data[idx];
}

// Allocates memory to a new array that holds the data from sp->data to be returned to the interface and returns said array to the interface
// O(1)
char **getElements(SET *sp)
{
	assert(sp!=NULL);
	int i;
	char **temp=malloc(sizeof(char*)*sp->count);
	for(i=0; i<sp->count; i++)
	{
		strcpy(temp[i],sp->data[i]);
	}
	return temp;
}

// Private search function that finds the element pointed to by char *elt through binary search; For every execution of the 'while' loop, it halves the interval that the search function is parsing over until the desired element is found or the slot in the array for insertion or deletion is found; If the element is found, it returns the location of the element and changes the bool to true; If the element is not found, it returns the location where an insertion/deletion should occur and changes the bool to false
// O(log n)
static int search(SET *sp, char *elt, bool *found)
{
	assert(sp!=NULL && elt!=NULL);
	int lo, hi, mid, cmp;
	lo = 0;
	hi = sp->count-1;
	while(lo<=hi)
	{
		mid=((lo+hi)/2);
		cmp = strcmp(elt, sp->data[mid]);
		if(cmp<0)
		{
			hi=mid-1;
		}
		else if(cmp>0) 
		{
			lo=mid+1;
		}
		else
		{
			*found = true;
			return mid;	
		}
	}
	*found = false;
	return lo;
}

