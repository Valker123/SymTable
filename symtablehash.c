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

void SymTable_free(SymTable_T oSymTable)
{
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
    free(oSymTable->buckets);
    free(oSymTable);
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    struct SymTableNode *psNode;
    size_t bucketTracker;
    size_t keyInBucket = 
    SymTable_hash(pcKey, oSymTable -> numOfBuckets);

    for (psNode = oSymTable->buckets[keyInBucket]; psNode != NULL;
         psNode = psNode->psNextNode) {
        if (strcmp(pcKey, psNode->pcKey) == 0) {
            return 1;
        }
    }
    return 0;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
                 const void *pvValue) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    size_t auBucketIndex;
    size_t bucketTracker = 0;
    size_t keyInBucket = 
    SymTable_hash(pcKey, oSymTable -> numOfBuckets);
    size_t reHashKeyInBucket;
    size_t auBucketCountsSize =
    (sizeof(auBucketCounts) / sizeof(auBucketCounts[0]));
    struct SymTableNode **tempBuckets;
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    if (SymTable_contains(oSymTable, pcKey) == 0) {
        struct SymTableNode *psNewNode;
        psNewNode =
            (struct SymTableNode *)malloc(sizeof(struct SymTableNode));
        if (psNewNode == NULL) {
            return 0;
        }
        psNewNode->pcKey = malloc(strlen(pcKey) + 1);
        if (psNewNode->pcKey == NULL) {
            free(psNewNode);
            return 0;
        }
        strcpy((char *)psNewNode->pcKey, pcKey);

        psNewNode->pvValue = (void *)pvValue;
        psNewNode->psNextNode = oSymTable->buckets[keyInBucket];
        oSymTable->buckets[keyInBucket] = psNewNode;
        oSymTable->numOfBindings++;
        
        if (oSymTable -> numOfBindings > oSymTable -> numOfBuckets) {
            /*loop through each index in the array. If the element
            in the array is equal to the number of bindings then 
            reallocate memory.*/
            for (auBucketIndex = 0; 
                auBucketIndex < auBucketCountsSize; 
                auBucketIndex++) 
            {
                if (auBucketCounts[auBucketIndex] == 
                    oSymTable -> numOfBuckets)
                {
                    auBucketIndex++;
                    /*setting a temperory pointer equal to where
                    the address of the new memory. Then, set buckets
                    pointer equal to the temporary pointer.*/
                    tempBuckets = 
                    (struct SymTableNode **)
                    realloc(oSymTable -> buckets, 
                    auBucketCounts[auBucketIndex]);

                    oSymTable->buckets = tempBuckets;

                    tempBuckets = NULL; 
                    if (oSymTable->buckets == NULL) {
                        free(oSymTable->buckets);
                    }

                    oSymTable->numOfBuckets =
                    auBucketCounts[auBucketIndex];
                    break;
                }
            }
        for (bucketTracker = 0; 
        bucketTracker < oSymTable->numOfBuckets;
        bucketTracker++)
        {
            /*rehash the key in the new bucket in the expanded hash
            table. Remove the old binding after reassigning key to
            bucket*/
            for (psCurrentNode = oSymTable->buckets[bucketTracker]; 
            psCurrentNode != NULL;
            psCurrentNode = psNextNode)
            {
                reHashKeyInBucket = SymTable_hash(psCurrentNode->pcKey, 
                auBucketCounts[auBucketIndex]);
                oSymTable->buckets[reHashKeyInBucket]=psCurrentNode;
                }
            }
        }
        return 1;
        }
    return 0;
}
  















