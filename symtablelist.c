/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Vincent Liu                                                */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "symtable.h"

struct SymTableNode 
{
    const void *pvItem;
    struct SymtableNode *psNextNode;
};

struct SymTable
{
    struct SymtableNode *psFirstNode;
};

SymTable_T SymTable_new(void)
{
    SymTable_T oSymTable;

    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL)
        return NULL;

    oSymTable->psFirstNode = NULL;
    return oSymTable;
}
