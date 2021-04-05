#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include <stdbool.h>

#define EMPTY 0
#define FILLED 1
#define DELETED 2

struct set
{
	char **data;
	char *flags;
	int length;
	int count;
};

static int search(SET *sp, char *elt, bool *found);
static unsigned strhash(char *s);


// Creates and allocates memory to the set that holds the pointer to the array of pointers that point to the strings, the string of flags, the current number of elements in the array, and the max number of elements in the array. It also allocates memory for the flag array. In addition, it also sets all the flags in the string to EMPTY
// O(n)
SET *createSet(int maxElts)
{
	int i;
	SET *sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->data=malloc(sizeof(char*)*maxElts);
	assert(sp->data!=NULL);
	sp->flags=malloc(sizeof(char)*maxElts);
	assert(sp->flags!=NULL);
	sp->length=maxElts;
	sp->count=0;
	for(i=0;i<maxElts;i++)
		sp->flags[i]=EMPTY;
	return sp;
}

// Frees up the memory allocated to the set in a backwards fashion, starting from each string and ending at the pointer to the set
// O(n)
void destroySet(SET *sp) 
{
	assert(sp!=NULL);
	int i;
	for(i=0;i<sp->length;i++) 
	{
		if(sp->flags[i]==FILLED)
		{
			free(sp->data[i]); 
		}
	}
	free(sp->data);
	free(sp->flags);
	free(sp);
	return; 
}

// Returns the number of elements in the array pointed to by char **data
// O(1)
int numElements(SET *sp)
{
	assert(sp!=NULL);
	return sp->count;
}

// Adds the element to the array pointed to by char **data assuming that the element is not already in the array and that the array is not full, while also updating the count of elements in the array; First, it allocates memory and inserts the element into the array. Next, it changes the corresponding index in the flag string to FILLED. Finally, it updates the count
// O(n) 
void addElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL && sp->count < sp->length);
	int i, idx;
	bool found = false;
	idx=search(sp, elt, &found);
	if(found==false)
	{
		sp->data[idx]=strdup(elt);
		assert(sp->data[idx]!=NULL);
		sp->flags[idx]=FILLED;
		sp->count++;
	}	
}

// Removes the element from the array pointed to by char **data assuming that the element is in said array, while also updating the count of elements in the array; First, it changes the corresponding index in the flag string to DELETED. Then, it frees the memory for the deleted element. Finally, it updates the count 
// O(n) 
void removeElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL);
	int idx;
	bool found = false;
	idx=search(sp, elt, &found);
	if(found==true)
	{
		assert(sp->data[idx]!=NULL);
		sp->flags[idx]=DELETED;
		free(sp->data[idx]);
		sp->count--;
	}
}

// Public search function that finds the element pointed to by char *elt and returns the string of the element if found. Else, it returns NULL to indicate that the element was not found
// O(n)
char *findElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL);
	bool found = false;
	search(sp, elt, &found);
	if(found==false)
		return NULL;
	else
		return elt;
}

// Allocates memory to a new array that holds the data from sp->data to be returned to the interface. It copies the data from sp->data to the new array assuming that there is actually information present, which is indicated by the flag array. After which, the new array is returned to the interface
// O(n)
char **getElements(SET *sp)
{
	assert(sp!=NULL);
	int i;
	int j=0;
	char **temp=malloc(sizeof(char*)*sp->count);
	for(i=0; i<sp->length; i++)
	{
		if(sp->flags[i]==FILLED)
		{
			temp[j]=sp->data[i];
			j++;
		}
	}
	return temp;
}

// Private search function that finds the element pointed to by char *elt. The home hashing address is determined at the beginning of the function. Throughout the execution of the 'for' loop, the index is iterated until either the string array indicates a slot is EMPTY or the element elt is found. Furthermore, in the loop, if the string indicates a DELETED spot, that deleted index is noted in case the element ends up not being in the array. That way, that deleted index can be returned in that situation.
// O(n)
static int search(SET *sp, char *elt, bool *found)
{
	assert(sp!=NULL && elt!=NULL);
	int i, idx, delidx;
	int delFlag=0;
	unsigned hash = strhash(elt);
	*found = false;
	for(i=0;i<sp->length;i++)
	{
		idx=(hash+i)%(sp->length);
		if(sp->flags[idx]==EMPTY)
		{
			if(delFlag==1)
				return delidx;
			else
				return idx;
		}
		else if(sp->flags[idx]==FILLED)
		{
			if(strcmp(sp->data[idx],elt)==0)
			{
				*found=true;
				return idx;
			}
		}
		else
		{
			if(delFlag==0)
			{
				delFlag=1;
				delidx=idx;
			}
		}
	}	
	return idx;
}

// Determines the home hashing address for a string
// O(n)
static unsigned strhash(char *s)
{
	unsigned hash = 0;
	while(*s != '\0')
	{
		hash = 31*hash + *s ++;
	}
	return hash;
}

