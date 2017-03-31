#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

#define MAXNUM 100
struct Node 
{
       int value;
       int order=0;
       Node* nextByOrder;   // link for next node ordered by when insertion occurred ( sequence ordered  l-list) 
       Node* nextByValue;   // link for next node by ordered by value  ( value ordered  l-list )
};

void printNode(const Node* node );
void printResults(const Node* llHead);
int compareNodeValue(const void* a,const void* b);
void destroy(Node* node);
void version1();
void version2();

void LinkListSortedByValueAndInsertion()
{
    version1();
    version2();
}

//  create two linked lists. Insert into each l-list appropriately as values are generated. 
void version1()
{
   printf("start version1\n");
   Node* headNode = ( Node*) malloc(sizeof(Node));
   headNode->value = 0;
   headNode->order = 0;   
   headNode->nextByOrder = 0;
   headNode->nextByValue = 0;
   
    Node* llHead = headNode;          //  dummy head node for head of both lists.
    Node* llTailByOrder = headNode;   //  tail node of sequence ordered l-list  
   
   for( int i = 1; i < MAXNUM; i++ )
   {
        // generate random numbers between 0-1000 and out them in a node structure. 
        Node* newNode = (Node*) malloc( sizeof(Node));
        newNode->value = rand() % 1000;
        newNode->order = i;
        newNode->nextByOrder = 0;
        newNode->nextByValue = 0;
        
        // insert into ordered sequence list  
        llTailByOrder->nextByOrder = newNode; 
        llTailByOrder = newNode;
        
        // insert into value ordered l-list; 
        // ok for small lists..   
        Node* node = llHead;
        while( node->nextByValue ) 
        {
            Node* next = node->nextByValue;
            if( next->value > newNode->value )
            {
                // insert newNode before the next node. 
                newNode->nextByValue = next;
                node->nextByValue = newNode;
                break;
            }
            node = next;
        }
        if( !newNode->nextByValue )  // no insertion
        {
            // insert at end
            node->nextByValue = newNode;  
        }        
   } 
   printResults(llHead);
   printf("\n\n");
   destroy(llHead);
   printf("end version1\n");
}

// same as version 1 but use qsort to help construct value ordered link list. 
void version2()
{
   printf("start version2\n");
   Node* headNode = ( Node*) malloc(sizeof(Node));
   headNode->value = 0;
   headNode->order = 0;   
   headNode->nextByOrder = 0;
   headNode->nextByValue = 0;
   
    Node* llHead = headNode;          //  dummy head node for both lists.
    Node* llTailByOrder = headNode;   //  tail node of the sequence ordered l-list  
   
   for( int i = 1; i < MAXNUM; i++ )
   {
        // generate random numbers between 0-1000; insert each in a node structure; 
        // insert nodes into sequence ordered l-list 
        Node* newNode = (Node*) malloc( sizeof(Node));
        newNode->value = rand() % 1000;
        newNode->order = i;
        newNode->nextByOrder = 0;
        newNode->nextByValue = 0;
        
        // insert into sequence ordered sequence list  
        llTailByOrder->nextByOrder = newNode; 
        llTailByOrder = newNode;
   }
   
   // make an array of elements, qsort them, then make l-list of value ordered nodes 
   int numElements = llTailByOrder->order;
   Node* nodePtrArray[numElements];
   
   // insert elements into array
   Node* node = llHead->nextByOrder;
   int i=0;
   while( node ) 
   {
       nodePtrArray[i++] = node;   // insert into array
       node = node->nextByOrder;
       if( i > numElements ) { printf("i=%d\n", i ); break; }
     //  assert( i<numElements );      // bounds check
   }
   
   // sort array node elements by value;
   qsort(nodePtrArray, numElements, sizeof(Node*), compareNodeValue); 
   
   // construct l-list ordered by value from sorted array
   node = llHead;
   for( int k=0; k<numElements; k++ )
   {
       node->nextByValue = nodePtrArray[k];
       node = nodePtrArray[k];
   }
   
   printResults(llHead);
   printf("\n\n");
   destroy(llHead);
   printf("end version2\n");
}

void printResults(const Node* llHead)
{  
   // print both linked lists 
   Node* node = llHead->nextByOrder;
   printf("Print sequence ordered l-list \n");
   while ( node  ) 
   {
       printNode( node ); 
       node = node->nextByOrder; 
   }
   
   printf("\n\n");
   printf("Print value ordered l-list\n");
   node = llHead->nextByValue;
   while ( node ) 
   {
       printNode( node ); 
       node = node->nextByValue; 
   }
}

void printNode(const Node* node )
{
    printf("value=%d order=%d", node->value, node->order);
    printf(" node=%p nextByValue=%p nextByOrder=%p\n", (void*) (node), (void*)(node->nextByValue), (void*)(node->nextByOrder));
}

void destroy(Node* node)
{
    if(node->nextByOrder ) destroy( node->nextByOrder );
    delete node;
}

int compareNodeValue(const void* a, const void* b)
{
  const Node** a_nodePtrPtr=  (const Node**) a;
  const Node** b_nodePtrPtr=  (const Node**) b;
  if ( (*a_nodePtrPtr)->value <  (*b_nodePtrPtr)->value ) return -1;
  if ( (*a_nodePtrPtr)->value ==  (*b_nodePtrPtr)->value ) return 0;
  if ( (*a_nodePtrPtr)->value >  (*b_nodePtrPtr)->value ) return 1;
  return 0;
}

