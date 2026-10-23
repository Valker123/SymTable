/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Vincent Liu & Rashmiya Hasan                               */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

struct SymTableNode 
{
    const void *pcKey;
    const void *pvValue;
    struct SymTableNode *psNextNode;
};

struct SymTable
{
    size_t counter;
    struct SymTableNode *psFirstNode;
};

SymTable_T SymTable_new(void)
{
    SymTable_T oSymTable;

    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL)
        return NULL;

    oSymTable->psFirstNode = NULL;
    oSymTable->counter == 0;
    return oSymTable;
}

SymTable_T SymTable_free(SymTable_T oSymTable)
{
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL;
    psCurrentNode = psNextNode)
    {
        psNextNode = psCurrentNode->psNextNode;
        free(psCurrentNode -> pcKey);
        free(psCurrentNode);
    }
    free(oSymTable);
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
    struct SymTableNode *psNode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psNode = oSymTable->psFirstNode; psNode -> psNextNode =! NULL; 
        psNode = psNode->psNextNode) 
    {
        if (strcmp(pcKey, psNode -> pcKey) == 0) {
            return 1;
        }
    }
    return 0;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                const void *pvValue) 
{
    if (SymTable_contains(oSymTable, pcKey) == 0) {
        struct SymTableNode *psNewNode;

        assert (oSymTable != NULL);
        assert (pcKey != NULL);

        psNewNode = 
        (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
        if (psNewNode ==NULL) {
            return 0;
        }
        psNewNode->pcKey = (const char*)malloc(strlen(pcKey) + 1);
        if (psNewNode -> pcKey == NULL) {
            return 0;
        }
        strcpy((char*)psNewNode->pcKey, pcKey);

        psNewNode->pvValue = pvValue;
        psNewNode->psNextNode = oSymTable->psFirstNode;
        oSymTable->psFirstNode = psNewNode;
        return 1;
    }
    return 0;
}


void Stack_map (SymTable_T oSymTable, void (*pfApply) 
               (void *pcKey, void *pvValue, void *pvExtra), 
               const void *pvExtra)
{
    struct SymTableNode *psCurrentNode;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL;
         psCurrentNode = psCurrentNode->psNextNode) {
        (*pfApply)((void *)psCurrentNode->pcKey, 
        (void *)psCurrentNode->pvValue, 
        (void *)pvExtra);
        }
}

size_t SymTable_getLength(SymTable_T oSymTable) 
{
    return oSymTable -> counter;
}
