#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

struct set
{
	char **data;
	int length;
	int count;
};

static int search(SET *sp, char *elt);

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

// Adds the element to the array pointed to by char **data assuming that the element is not already in the array and that the array is not full, while also updating the count of elements in the array
// O(n) due to the search function
void addElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL);
	if(search(sp,elt)==-1)
	{
		assert(sp->count<sp->length);		
		sp->data[sp->count]=strdup(elt);
		assert(sp->data[sp->count]!=NULL);
		sp->count++;
	}	
}

// Removes the element from the array pointed to by char **data assuming that the element is in said array, while also updating the count of elements in the array
// O(n) due to the search function
void removeElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL);
	int i;
	i=search(sp, elt);
	if (i==-1) 
		return;
	free(sp->data[i]);
	sp->data[i]=sp->data[sp->count-1];
	sp->count--;
}

// Public search function that finds the element pointed to by char *elt and returns the string of the element if found. Else, it returns NULL to indicate that the element was not found
// O(n) due to the search function
char *findElement(SET *sp, char *elt)
{
	assert(sp!=NULL);
	int i;
	i=search(sp, elt);
	if(i==-1)
		return NULL;
	else
		return sp->data[i];
}

// Allocates memory to a new array that holds the data from sp->data to be returned to the interface and returns said array to the interface
// O(1)
char **getElements(SET *sp)
{
	assert(sp!=NULL);
	int i;
	char **temp=malloc(sizeof(char*)*sp->count);
	for(i=0;i<sp->count;i++)
	{
		strcpy(temp[i],sp->data[i]);
	}
	return temp;
}

// Private search function that finds the element pointed to by char *elt and returns the index of the element if found. Else, it returns -1 to indicate that the element was not found
// O(n)
static int search(SET *sp, char *elt)
{
	assert(sp!=NULL);
	int i;
	for(i=0;i<sp->count;i++)
	{
		if(strcmp(sp->data[i],elt)==0)
			return i;
	}
	return -1;
}
