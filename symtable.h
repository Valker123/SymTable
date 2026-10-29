/*--------------------------------------------------------------------*/
/* symtable.h                                                         */
/* Author: Vincent Liu & Rashmiya Hasan                               */
/*--------------------------------------------------------------------*/
#ifndef SYM_TABLE
#define SYM_TABLE
#include <stddef.h>

/*A pointer to a symbol table, which is an
unordered collection of bindings. A binding consists of a key, which
is a string that identifies the binding, and a value, which is a
pointer to data of any type. The symbol table owns its keys but not its
values. */
typedef struct SymTable *SymTable_T;

/*Returns a new symbol table without any initialized bindings. Returns
NULL if memory allocation fails for this new symbol table*/
SymTable_T SymTable_new(void);

/*Frees all memory occupied by oSymTable, which
includes the copied defensive keys. The values are not freed as the
client owns them. This can only occur if the symbol table is not NULL*/
void SymTable_free(SymTable_T oSymTable);

/*Returns the total number of bindings in the symbol table. This can
only occur if oSymTable is not NULL.*/
size_t SymTable_getLength(SymTable_T oSymTable);

/*Adds a binding with the user provided pcKey and the value pvValue to 
oSymTable. This function returns 1 if a binding was successfully
added. This function returns a 0 if the key that the user provided
is already in the symbol table or insufficient memory is available to
add the binding. oSymTable and pcKey must not be NULL.*/
int SymTable_put(SymTable_T oSymTable, 
    const char *pcKey, const void *pvValue);

/*If oSymTable contains a binding with key pcKey, then the value of the
binding is replaced with pvValue and returns the old value.
Otherwise does not change oSymTable and returns NULL.*/
void *SymTable_replace(SymTable_T oSymTable, 
    const char *pcKey, const void *pvValue);

/*Return 1 (True) if oSymTable contains a binding with key pcKey, or
returns 0 (False) otherwise. */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*Returns the value of the binding in oSymTable with key pcKey,
or returns NULL if no such binding exists.*/
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*If oSymTable contains a binding with key pcKey, then that binding
from oSymTable is removed and the binding's value is returned.
Otherwise oSymTable is not changed and returns NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/* Apply function pointer pfApply to each binding in oSymTable, also
able to pass pvExtra as an extra parameter. */
void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra);

#endif