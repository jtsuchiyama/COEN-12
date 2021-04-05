#include "pack.h"
#include "pqueue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node NODE;

static NODE *mknode(int data, NODE *left, NODE *right);
static int nodecmp(NODE *t1, NODE *t2);
static int depth(NODE *np);

// Creates a new node that is the parent of the left and right node arguments (if those arguments are passed into the function) and contains a count according to the data argument
// O(1)
static NODE *mknode(int data, NODE *left, NODE *right) 
{
	NODE *np=malloc(sizeof(NODE));
	assert(np!=NULL);	
	np->parent=NULL;
	np->count=data;
	if(left!=NULL)
		left->parent=np;
	if(right!=NULL)
		right->parent=np;
	return np;
}

// Compares the counts within each node argument against each other
// O(1)
static int nodecmp(NODE *t1, NODE *t2)
{
	assert(t1!=NULL && t2!=NULL);
	return (t1->count < t2->count) ? -1 : (t1->count > t2->count);
	
}

// Recursively checks the depth of the passed node by calling itself till the root is reached and then going backwards to count the depth of the node
// O(log n)
static int depth(NODE *np)
{
	assert(np!=NULL);
	if(np->parent==NULL)
		return 0;
	else
		return 1+depth(np->parent);
}

int main(int argc, char *argv[])
{
// Step 1	
	if(argc!=3)
	{
		printf("The name of one of the files is missing \n");
		return 1;
	}
	
	FILE *fp=fopen(argv[1],"rb");
	assert(fp!=NULL);
	
	int i,c,j,k;
	int counts[257];
	NODE *leaves[257];
	for(i=0;i<257;i++)
	{
		counts[i]=0;
		leaves[i]=NULL;
	}
	
	while((c=getc(fp))!=EOF)
		counts[c]++;
	fclose(fp);
	
// Step 2
	PQ* pq=createQueue(*nodecmp);
	assert(pq!=NULL);
	for(j=0;j<257;j++)
	{
		if(counts[j]>0)
		{
			leaves[j]=mknode(counts[j],NULL,NULL);
			addEntry(pq,leaves[j]);
		}
	}
	NODE *eof=malloc(sizeof(NODE));
	assert(eof!=NULL);
	eof->count=0;
	eof->parent=NULL;
	leaves[256]=eof;
	addEntry(pq, leaves[256]);
	
// Step 3
	while(numEntries(pq)>1)
	{
		NODE *left=removeEntry(pq);
		NODE *right=removeEntry(pq);
		NODE *new=mknode(left->count+right->count,left,right);
		addEntry(pq, new);
	}

// Step 4
	int eDepth, bits;
	for(k=0;k<257;k++)
	{
		if(leaves[k]!=NULL)
		{
			eDepth=depth(leaves[k]);
			bits=eDepth*leaves[k]->count;
			printf(isprint(k) ? "%c" : "%03o", k);
			printf(": %d x %d bits = %d bits \n", counts[k], eDepth, bits);	
		}
	}

// Step 5
	pack(argv[1],argv[2],leaves);	
	return 0;
}
