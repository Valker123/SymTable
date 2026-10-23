/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Vincent Liu & Rashmiya Hasan                               */
/*--------------------------------------------------------------------*/
#include "symtable.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SymTableNode {
    char *pcKey;
    void *pvValue;
    struct SymTableNode *psNextNode;
};

struct SymTable {
    size_t counter;
    struct SymTableNode *psFirstNode;
};

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;

    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL)
        return NULL;

    oSymTable->psFirstNode = NULL;
    oSymTable->counter = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL;
        psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->psNextNode;
        free(psCurrentNode->pcKey);
        free(psCurrentNode);
    }
    free(oSymTable);
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psNode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psNode = oSymTable->psFirstNode; psNode != NULL;
         psNode = psNode->psNextNode) {
        if (strcmp(pcKey, psNode->pcKey) == 0) {
            return 1;
        }
    }
    return 0;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
                 const void *pvValue) 
{
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

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

        psNewNode->pvValue = (void*) pvValue;
        psNewNode->psNextNode = oSymTable->psFirstNode;
        oSymTable->psFirstNode = psNewNode;
        oSymTable->counter += 1;
        return 1;
    }
    return 0;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) 
{
    struct SymTableNode *psCurrentNode;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL;
         psCurrentNode = psCurrentNode->psNextNode) {
        (*pfApply)((void *)psCurrentNode->pcKey,
                   (void *)psCurrentNode->pvValue, (void *)pvExtra);
    }
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psNode;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psNode = oSymTable->psFirstNode; psNode != NULL;
         psNode = psNode->psNextNode) {
        if (strcmp(pcKey, psNode->pcKey) == 0) {
            return psNode->pvValue;
        }
    }
    return NULL;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
                       const void *pvValue) {
    void *psTemp;
    struct SymTableNode *psNode;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psNode = oSymTable->psFirstNode; psNode != NULL;
         psNode = psNode->psNextNode) {
        if (strcmp(pcKey, psNode->pcKey) == 0) {
            psTemp = psNode->pvValue;
            psNode->pvValue = (void*) pvValue;
            return psTemp;
        }
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) 
{
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    void *psTemp;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL;
        psCurrentNode = psNextNode) 
        {
        psNextNode = psCurrentNode->psNextNode;
        /*case where the first node is the node that is to be removed*/
            if (strcmp(pcKey, psCurrentNode->pcKey) == 0 && 
            psCurrentNode == oSymTable->psFirstNode)
            {
                psTemp = psCurrentNode->pvValue;
                oSymTable->psFirstNode = psNextNode;
                free(psCurrentNode->pcKey);
                free(psCurrentNode);
                oSymTable->counter -= 1;
                return psTemp;
            }
            else if (psNextNode != NULL &&
                strcmp(pcKey, psNextNode->pcKey) == 0)
            {
                psTemp = psNextNode->pvValue;
                psCurrentNode->psNextNode = psNextNode->psNextNode;
                free(psNextNode->pcKey);
                free(psNextNode);
                oSymTable->counter -= 1;
                return psTemp;
                }
        }
    return NULL;
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->counter;
}