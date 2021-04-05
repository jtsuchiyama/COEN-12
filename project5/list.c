#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node
{
	struct node *prev;
	struct node *next;
	void *data;
} NODE;

typedef struct list
{
	int count;
	int (*compare)();
	NODE *head;
} LIST;

LIST *createList(int(*compare)());
void destroyList(LIST *lp);
int numItems(LIST *lp);
void addFirst(LIST *lp, void *item);
void addLast(LIST *lp, void *item);
void *removeFirst(LIST *lp);
void *removeLast(LIST *lp);
void *getFirst(LIST *lp);
void *getLast(LIST *lp);
void removeItem(LIST *lp, void *item);
void *findItem(LIST *lp, void *item);
void *getItems(LIST *lp);

// Allocates memory to the list and to the dummy node, which will simplify the cases for list operations
// O(1)
LIST *createList(int(*compare)())
{
	LIST *lp;
	lp=malloc(sizeof(LIST));
	assert(lp!=NULL);
	lp->count=0;
	lp->compare=compare;
	lp->head=malloc(sizeof(NODE));
	assert(lp->head!=NULL);
	lp->head->next=lp->head;
	lp->head->prev=lp->head;
	return lp;
}

// Allocates memory to the new item in the list pointed to by lp, adds the new item to the front of the list, and updates the count
// O(1)
void addFirst(LIST *lp, void *item)
{
	assert(lp!=NULL && item!=NULL);
	NODE *new=malloc(sizeof(NODE));
	assert(new!=NULL);
	new->data=item;
	new->prev=lp->head;
	new->next=lp->head->next;
	lp->head->next->prev=new;
	lp->head->next=new;
	lp->count++;
}

// Allocates memory to the new item in the list pointed to by lp, adds the new item to the back of the list, and updates the count
// O(1)
void addLast(LIST *lp, void *item)
{
	assert(lp!=NULL && item!=NULL);
	NODE *new=malloc(sizeof(NODE));
	assert(new!=NULL);
	new->data=item;
	new->prev=lp->head->prev;
	new->next=lp->head;
	lp->head->prev->next=new;
	lp->head->prev=new;
	lp->count++;
}

// Returns the first item in the list pointed to by lp, assuming that the list is not empty
// O(1)
void *getFirst(LIST *lp)
{
	assert(lp!=NULL && lp->count>0);
	return lp->head->next->data;
}

// Returns the last item in the list pointed to by lp, assuming that the list is not empty
// 0(1)
void *getLast(LIST *lp)
{
	assert(lp!=NULL && lp->count>0);
	return lp->head->prev->data;
}

// Frees the first item from the list pointed to by lp, updates the count, and returns its data to the interface, assuming that the list is not empty
// O(1) 
void *removeFirst(LIST *lp)
{
	assert(lp!=NULL && lp->count>0);
	NODE *del=lp->head->next;
	void *delData = del->data;
	lp->head->next=del->next;
	del->next->prev=lp->head;
	free(del);
	lp->count--;
	return delData;
}

// Frees the last item from the list pointed to by lp, updates the count, and returns its data to the interface, assuming that the list is not empty
// O(1)
void *removeLast(LIST *lp)
{
	assert(lp!=NULL && lp->count>0);
	NODE *del=lp->head->prev;
	void *delData = del->data;
	lp->head->prev=del->prev;
	del->prev->next=lp->head;
	free(del);
	lp->count--;
	return delData;
}

// Iterates through the list pointed to by lp starting from the last item and frees the items; After all of the items are freed, it frees the list structure
// O(n)
void destroyList(LIST *lp)
{
	assert(lp!=NULL);
	NODE *p=lp->head->prev;
	NODE *del;
	while(p!=lp->head)
	{
		del=p;
		p=p->prev;
		free(del);
	}
	free(lp);
}

// Returns the number of items in the list pointed to by lp
// O(1)
int numItems(LIST *lp)
{
	assert(lp!=NULL);
	return lp->count;
}

// Searches for the value pointed to by item in the list pointed to by lp; If found, it frees the item from the list and updates the count
// O(n)
void removeItem(LIST *lp, void* item)
{
	assert(lp!=NULL);
	NODE *p=lp->head->next;
	while(p!=lp->head)
	{
		if((*lp->compare)(p->data, item)==0)
		{
			p->prev->next=p->next;
			p->next->prev=p->prev;
			free(p);
			lp->count--;
			break;
		}
		p=p->next;
	}
	
}

// Iterates through the list pointed to by lp from the beginning, searching for the value pointed to by item; If found, it will return the item; If not found, it will return NULL
// O(n)
void *findItem(LIST *lp, void *item)
{
	assert(lp!=NULL);
	NODE *p=lp->head->next;
	while(p!=lp->head)
	{
		if((*lp->compare)(p->data,item)==0)
			return p->data;
		p=p->next;
	}
	return NULL;
}


// Allocates memory to the pointer to the void array, temp, that will be passed to the interface; Then, it copies all of the items from the list pointed to by lp and returns temp
// O(n)
void *getItems(LIST *lp)
{
	void **temp=malloc(sizeof(NODE)*lp->count);
	assert(temp!=NULL);
	NODE *p=lp->head->next;
	int i=0;
	while(p!=lp->head && i<lp->count)
	{
		temp[i]=p->data;
		p=p->next;
		i++;
	}	
	return temp;
}
