/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Vincent Liu & Rashmiya Hasan                               */
/*--------------------------------------------------------------------*/
#include "symtable.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A binding that contains a key, a value, and a link to the next
binding */
struct SymTableNode {
    /* The dynamically allocated copy of the binding's key, owned by
    the table*/
    char* pcKey;
    /* The associated value pointer. The client owns this value*/
    void* pvValue;
    /* The next binding in the list. This is equal to NULL if this is
    the last binding */
    struct SymTableNode* psNextNode;
};

/* A symbol table implemented as a linked list of bindings. */
struct SymTable {
    /* The number of bindings in the table. */
    size_t counter;
    /*A pointer to the first binding in the list, or NULL if the table
    is empty */
    struct SymTableNode* psFirstNode;
};

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;

    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL) {
        return NULL;
    }
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
                 const void *pvValue) {
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
        strcpy(psNewNode->pcKey, pcKey);

        psNewNode->pvValue = (void *)pvValue;
        psNewNode->psNextNode = oSymTable->psFirstNode;
        oSymTable->psFirstNode = psNewNode;
        oSymTable->counter += 1;
        return 1;
    }
    return 0;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    struct SymTableNode *psCurrentNode;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL;
         psCurrentNode = psCurrentNode->psNextNode) {
        (*pfApply)(psCurrentNode->pcKey,
                   psCurrentNode->pvValue, pvExtra);
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
    void *pvTemp;
    struct SymTableNode *psNode;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psNode = oSymTable->psFirstNode; psNode != NULL;
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
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL;
         psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->psNextNode;
        /*case where the first node is the node that is to be removed*/
        if (psCurrentNode == oSymTable->psFirstNode &&
            strcmp(pcKey, psCurrentNode->pcKey) == 0) {
            pvTemp = psCurrentNode->pvValue;
            oSymTable->psFirstNode = psNextNode;
            free(psCurrentNode->pcKey);
            free(psCurrentNode);
            oSymTable->counter -= 1;
            return pvTemp;
        } else if (psNextNode != NULL &&
                   strcmp(pcKey, psNextNode->pcKey) == 0) {
            pvTemp = psNextNode->pvValue;
            psCurrentNode->psNextNode = psNextNode->psNextNode;
            free(psNextNode->pcKey);
            free(psNextNode);
            oSymTable->counter -= 1;
            return pvTemp;
        }
    }
    return NULL;
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->counter;
}