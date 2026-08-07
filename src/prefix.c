#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "prefix.h"

typedef struct s_1 {
	size_t len;
	//str is 0 terminated
	char* str;
} S1;
static S1* S1_new(const char* x, size_t len);
static void free_S1(S1* x);

typedef struct s_1 Key_PrefixMap;
typedef struct s_1 Value_PrefixMap;

static size_t hash_PrefixMap(Key_PrefixMap*, size_t);
static int compare_PrefixMap(Key_PrefixMap*, Key_PrefixMap*, size_t);
static int insert_PrefixMap(PrefixMap*, const char* prefix, size_t prefix_len, const char* expand, size_t expand_len);
static const Value_PrefixMap* get_PrefixMap(PrefixMap*, const char* prefix, size_t len);


static const Value_PrefixMap* get_PrefixMap(PrefixMap* map, const char* prefix, size_t len){
	Key_PrefixMap* key = S1_new(prefix, len);
	Value_PrefixMap* val;
	val = ht_lookup(&map->table, key);
	free_S1(key);
	if (val != NULL){
		return val;
	}
	return NULL;
}

static S1* S1_new(const char* x, size_t len){
	S1* ret = malloc(sizeof(S1));
	ret->len = len;
	ret->str = malloc(len + 1);
	memcpy(ret->str, x, len);
	ret->str[len] = '\0';
	return ret;
}

static void free_S1(S1* x){
	if(x == NULL)return;
	free(x->str);
	free(x);
}


static int insert_PrefixMap(PrefixMap* map, const char* prefix, size_t prefix_len, const char* expand, size_t expand_len)
{
	int err;
	Key_PrefixMap* key = S1_new(prefix, prefix_len);
	Value_PrefixMap* value = S1_new(expand, expand_len);
	err = ht_insert(&map->table, key, value);
	if (err != 0){
		free_S1(key);
		free_S1(value);
	}
	return err;
}

static size_t hash_PrefixMap(Key_PrefixMap* key, size_t){
	size_t hash = 5381;
	for(const char* x = key->str; *x!='\0'; x++){
		hash = ((hash << 5) + hash) + *x;
	}
	return hash;
}

static int compare_PrefixMap(Key_PrefixMap* first, Key_PrefixMap* second, size_t){
	return strcmp(first->str, second->str);
}


PrefixMap* PrefixMap_new(){
	PrefixMap* ret = malloc(sizeof(PrefixMap));
	if(PrefixMap_init(ret)){
		return ret;
	} else {
		free(ret);
		return NULL;
	}
}

bool PrefixMap_init(PrefixMap* map){
	ht_setup(&map->table, sizeof(Key_PrefixMap), sizeof(Value_PrefixMap), 10);
	map->table.hash = (hash_t) hash_PrefixMap;
	map->table.compare = (comparison_t) compare_PrefixMap;
	return true;
}

void free_PrefixMap(PrefixMap* x){
	if (x == NULL) return;
	PrefixMap_release(x);
	free(x);
}

void PrefixMap_release(PrefixMap* map){
	//TODO: go through table and release keys and values
	ht_clear(&map->table);
	ht_destroy(&map->table);
}

char* PrefixMap_expand_curie(PrefixMap *map, const char* prefix, size_t prefix_len, const char* suffix, size_t suffix_len)
{
	char* ret, *tmp;
	const Value_PrefixMap* val;
	val = get_PrefixMap(map, prefix, prefix_len);
	if (val == NULL){
		return NULL;
	}
	ret = malloc(suffix_len + val->len + 1);
	tmp = ret + val->len;
	memcpy(ret, val->str, val->len);
	memcpy(tmp, suffix, suffix_len);
	tmp[suffix_len] = '\0';
	return ret;
}

void PrefixMap_register_prefix(PrefixMap* map, const char* prefix, size_t prefix_len, const char* baseiri, size_t baseiri_len)
{
	insert_PrefixMap(map, prefix, prefix_len, baseiri, baseiri_len);
}
