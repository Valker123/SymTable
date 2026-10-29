/*--------------------------------------------------------------------*/
/* symtable.h                                                         */
/* Author: Vincent Liu & Rashmiya Hasan                               */
/*--------------------------------------------------------------------*/
#ifndef SYM_TABLE
#define SYM_TABLE
#include <stddef.h>

/*A pointer to a symbol table, which is an unordered collection
of bindings with unique keys and values.*/
typedef struct SymTable *SymTable_T;

/*Creates a new symbol table without any initialized bindings. Returns
NULL if memory allocation fails for this new symbol table*/ 
SymTable_T SymTable_new(void);

/*Free's user's symbol table and all the memory that it owns, which
includes the copied defensive keys. This can only occur if the symbol
table is not NULL*/ 
void SymTable_free(SymTable_T oSymTable);

/*Returns the total number of bindings in the symbol table. This can
only occur if oSymTable isn't NULL.*/ 
size_t SymTable_getLength(SymTable_T oSymTable);

/*Adds a binding with the user provided pcKey and the value pvValue to
the symbol table. This function returns 1, if a binding was successfully
added. This function returns a 0 if the key that the user provided
is already in the symbol table or insufficient memory is available to
add the binding. oSymtable and pcKey must not be NULL.*/
int SymTable_put(SymTable_T oSymTable,
const char *pcKey, const void *pvValue);

void *SymTable_replace(SymTable_T oSymTable,
const char *pcKey, const void *pvValue);

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra);

#endif