/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Vincent Liu & Rashmiya Hasan                               */
/*--------------------------------------------------------------------*/
#include "symtable.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*The possible number of buckets in increasing order. A SymTable starts
with the first and moves to the next when it expands. */
static const size_t auBucketCounts[] = {509,  1021,  2039,  4093,
                                        8191, 16381, 32749, 65521};

/* A binding that contains a key, a value and a link to the next
binding*/
struct SymTableNode {
    /* The dynamically allocated copy of the binding's key in which the
    user defined*/
    char *pcKey;
    /* The associated value pointer. The client owns this value*/
    void *pvValue;
    /* The next binding in the list. This is equal to NULL if this is
    the last binding */
    struct SymTableNode *psNextNode;
};

/* A symbol table that is implemented as an array of buckets, with each
bucket containing a linked list of bindings.*/
struct SymTable {
    /* The total number of bindings in the symbol table*/
    size_t numOfBindings;
    /* The total number of buckets in the symbol table*/
    size_t numOfBuckets;
    /* A pointer to a dynamically allocated array of pointers to the
    first binding in each bucket. An empty bucket contains NULL. */
    struct SymTableNode **buckets;
};

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
inclusive. */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount) {
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
    if (oSymTable == NULL) {
        return NULL;
    }
    /*allocating memory for an array of pointers to nodes*/
    oSymTable->buckets =
        malloc(auBucketCounts[0] * sizeof(struct SymTableNode *));
    if (oSymTable->buckets == NULL) {
        free(oSymTable);
        return NULL;
    }
    for (nullBuckets = 0; nullBuckets < auBucketCounts[0];
         nullBuckets++) {
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
             psCurrentNode != NULL; psCurrentNode = psNextNode) {
            psNextNode = psCurrentNode->psNextNode;
            /* Free the key (owned by table) and node, but not value */
            free(psCurrentNode->pcKey);
            free(psCurrentNode);
        }
    free(oSymTable->buckets);
    free(oSymTable);
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psNode;
    size_t keyInBucket;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    keyInBucket = SymTable_hash(pcKey, oSymTable->numOfBuckets);
    for (psNode = oSymTable->buckets[keyInBucket]; psNode != NULL;
         psNode = psNode->psNextNode) {
        if (strcmp(pcKey, psNode->pcKey) == 0) {
            return 1;
        }
    }
    return 0;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
                       const void *pvValue) {
    void *pvTemp;
    struct SymTableNode *psNode;
    size_t keyInBucket;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    keyInBucket = SymTable_hash(pcKey, oSymTable->numOfBuckets);

    for (psNode = oSymTable->buckets[keyInBucket]; psNode != NULL;
         psNode = psNode->psNextNode) {
        if (strcmp(pcKey, psNode->pcKey) == 0) {
            pvTemp = psNode->pvValue;
            psNode->pvValue = (void *)pvValue;
            return pvTemp;
        }
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    void *pvTemp;
    size_t keyInBucket;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    keyInBucket = SymTable_hash(pcKey, oSymTable->numOfBuckets);

    for (psCurrentNode = oSymTable->buckets[keyInBucket];
         psCurrentNode != NULL; psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->psNextNode;
        /*case where the first node is the node that is to be removed*/
        if (strcmp(pcKey, psCurrentNode->pcKey) == 0 &&
            psCurrentNode == oSymTable->buckets[keyInBucket]) {
            pvTemp = psCurrentNode->pvValue;
            oSymTable->buckets[keyInBucket] = psNextNode;
            free(psCurrentNode->pcKey);
            free(psCurrentNode);
            oSymTable->numOfBindings -= 1;
            return pvTemp;
        }
        /* case where the node to be removed is not the first node */
        else if (psNextNode != NULL &&
                 strcmp(pcKey, psNextNode->pcKey) == 0) {
            pvTemp = psNextNode->pvValue;
            psCurrentNode->psNextNode = psNextNode->psNextNode;
            free(psNextNode->pcKey);
            free(psNextNode);
            oSymTable->numOfBindings -= 1;
            return pvTemp;
        }
    }
    return NULL;
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->numOfBindings;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psNode;
    size_t keyInBucket;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    keyInBucket = SymTable_hash(pcKey, oSymTable->numOfBuckets);

    for (psNode = oSymTable->buckets[keyInBucket]; psNode != NULL;
         psNode = psNode->psNextNode) {
        if (strcmp(pcKey, psNode->pcKey) == 0) {
            return psNode->pvValue;
        }
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const char *pcKey, void *pvValue,
                                  void *pvExtra),
                  const void *pvExtra) {
    struct SymTableNode *psCurrentNode;
    size_t bucketTracker = 0;
    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    for (bucketTracker = 0; bucketTracker < (oSymTable->numOfBuckets);
         bucketTracker++) {
        for (psCurrentNode = oSymTable->buckets[bucketTracker];
             psCurrentNode != NULL;
             psCurrentNode = psCurrentNode->psNextNode) {
            (*pfApply)((void *)psCurrentNode->pcKey,
                       (void *)psCurrentNode->pvValue, (void *)pvExtra);
        }
    }
}

/* This helper function expands the bucket array of oSymTable. */
static void SymTable_expand(SymTable_T oSymTable) {
    size_t bucketTracker = 0;
    size_t reHashKeyInBucket;
    size_t auBucketSize =
        (sizeof(auBucketCounts) / sizeof(auBucketCounts[0]));
    size_t auBucketIndex;
    size_t preExpandBucket;
    size_t nullBuckets;
    struct SymTableNode **oldBuckets;
    struct SymTableNode **newBuckets;
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNodeNext;
    
        /*loop through each index in the auBuckets array to find
        current bucket count, the next entry is the new count */
        for (auBucketIndex = 0; auBucketIndex < auBucketSize;
             auBucketIndex++) {
            if (auBucketCounts[auBucketIndex] ==
                    oSymTable->numOfBuckets &&
                auBucketIndex < auBucketSize - 1) {
                preExpandBucket = auBucketIndex;
                auBucketIndex++;
                /*creating temporary pointer to point to original
                array*/
                oldBuckets = oSymTable->buckets;
                /*allocating memory for the new bucket array,
                an array of pointers to nodes. If it fails, keep
                using old array; new binding was already added. */
                newBuckets = malloc(auBucketCounts[auBucketIndex] *
                                    sizeof(struct SymTableNode *));

                if (newBuckets == NULL) {
                    return;
                }
                oSymTable->buckets = newBuckets;
                for (nullBuckets = 0;
                     nullBuckets < auBucketCounts[auBucketIndex];
                     nullBuckets++) {
                    oSymTable->buckets[nullBuckets] = NULL;
                }
                oSymTable->numOfBuckets = auBucketCounts[auBucketIndex];

                for (bucketTracker = 0;
                     bucketTracker < auBucketCounts[preExpandBucket];
                     bucketTracker++) {
                    /*rehash the key in the new bucket in the expanded
                    hash table. Point the pointer in the new bucket to
                    the node that contains the key*/
                    for (psCurrentNode = oldBuckets[bucketTracker];
                         psCurrentNode != NULL;
                         psCurrentNode = psNodeNext) {
                        psNodeNext = psCurrentNode->psNextNode;
                        reHashKeyInBucket = SymTable_hash(
                            psCurrentNode->pcKey,
                            auBucketCounts[auBucketIndex]);
                        psCurrentNode->psNextNode =
                            oSymTable->buckets[reHashKeyInBucket];
                        oSymTable->buckets[reHashKeyInBucket] =
                            psCurrentNode;
                    }
                }
                /*Free the old bucket array only, the nodes are in the
                new array. */
                free(oldBuckets);
                break;
            }
        }
    }

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
                 const void *pvValue) {
    size_t keyInBucket;
    struct SymTableNode *psFind;
    enum nodeInTable { IN_TABLE, NOT_IN_TABLE };
    enum nodeInTable nodeInTable = NOT_IN_TABLE;
        size_t auBucketSize =
        (sizeof(auBucketCounts) / sizeof(auBucketCounts[0]));

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* Hash the key once, then search only that bucket for a
    duplicate key */
    keyInBucket = SymTable_hash(pcKey, oSymTable->numOfBuckets);
    for (psFind = oSymTable->buckets[keyInBucket]; psFind != NULL;
         psFind = psFind->psNextNode) {
        if (strcmp(pcKey, psFind->pcKey) == 0) {
            nodeInTable = IN_TABLE;
            break;
        }
    }

    if (nodeInTable == NOT_IN_TABLE) {
        struct SymTableNode *psNewNode;
        psNewNode =
            (struct SymTableNode *)malloc(sizeof(struct SymTableNode));
        if (psNewNode == NULL) {
            return 0;
        }
        /* Makes a defensive copy of the key; the table owns copy */
        psNewNode->pcKey = malloc(strlen(pcKey) + 1);
        if (psNewNode->pcKey == NULL) {
            free(psNewNode);
            return 0;
        }
        strcpy((char *)psNewNode->pcKey, pcKey);

        psNewNode->pvValue = (void *)pvValue;
        psNewNode->psNextNode = oSymTable->buckets[keyInBucket];
        oSymTable->buckets[keyInBucket] = psNewNode;
        oSymTable->numOfBindings += 1;
        /*hash expansion condition. The hash table expands when the 
        number of bindings exceeds the number of buckets and the number
        of buckets is not the last bucket size in the auBuckets array*/
        if (oSymTable->numOfBindings > oSymTable->numOfBuckets &&
            oSymTable->numOfBuckets != auBucketCounts[auBucketSize - 1]) {
            SymTable_expand(oSymTable);
        }
            return 1;
    }
    return 0;
}