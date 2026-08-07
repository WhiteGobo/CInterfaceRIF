#pragma once

#include <stddef.h>
#include <hashtable.h>
#include "CInterfaceRIF.h"

typedef struct prefixMap {
	HashTable table;
} PrefixMap;

void free_PrefixMap(PrefixMap*);
PrefixMap* PrefixMap_new();
bool PrefixMap_init(PrefixMap* map);
void PrefixMap_release(PrefixMap* map);

void PrefixMap_register_prefix(PrefixMap*, const char* prefix_name, size_t prefix_len, const char* baseiri, size_t baseiri_len);

char* PrefixMap_expand_curie(PrefixMap*, const char* prefix, size_t prefix_len, const char* suffix, size_t suffix_len);
