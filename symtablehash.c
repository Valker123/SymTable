/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Vincent Liu & Rashmiya Hasan                               */
/*--------------------------------------------------------------------*/
#include "symtable.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t 
auBucketCounts[] = {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521}; 

struct SymTableNode {
    char *pcKey;
    void *pvValue;
    struct SymTableNode *psNextNode;
};

struct SymTable {
    size_t numOfBindings;
    size_t numOfBuckets;
    /*pointer to pointer of a SymTable Node*/
    struct SymTableNode **buckets; 
};

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;
    size_t nullBuckets;
    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL) 
    {
        return NULL;
    }
    /*allocating memory for an array of pointers to nodes*/
    oSymTable->buckets = 
    malloc(auBucketCounts[0] * sizeof(struct SymTableNode *));
    if (oSymTable->buckets == NULL) 
    {
        free(oSymTable);
        return NULL;
    }
    for (nullBuckets = 0; nullBuckets < auBucketCounts[0]; nullBuckets++)
    {
        oSymTable->buckets[nullBuckets] = NULL;
    }
    oSymTable->numOfBindings = 0;
    oSymTable->numOfBuckets = auBucketCounts[0];
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    size_t bucketTracker;
    assert(oSymTable != NULL);

    for (bucketTracker = 0; bucketTracker < oSymTable->numOfBuckets;
        bucketTracker++)
        for (psCurrentNode = oSymTable->buckets[bucketTracker]; 
            psCurrentNode != NULL;
            psCurrentNode = psNextNode)
        {
            psNextNode = psCurrentNode->psNextNode;
            free(psCurrentNode->pcKey);
            free(psCurrentNode);
        }
    free(oSymTable);
}