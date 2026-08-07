#pragma once
#include <stdint.h>

#ifndef RDF_TERMTYPE_DEFINED
#define RDF_TERMTYPE_DEFINED
typedef enum {
        URI = 0,
        BNODE = 1,
        TYPEDLITERAL = 2,
        LANGLITERAL = 3
} TERMTYPE;
#endif

#ifndef TRIPLEHANDLER_DEFINED
#define TRIPLEHANDLER_DEFINED
/*
 * Use TERMTYPE for subject_type, object_type and graph_type.
 * If graphid is NULL, the default graph is used.
 */
typedef int8_t TripleHandler(
                const char* subject, uint8_t subject_type,
                const char* predicate,
                const char* object, const char* object_suffix,
                uint8_t object_type,
                const char* graphid, uint8_t graph_type,
                void* user);

#endif //TRIPLEHANDLER_DEFINED


typedef struct rifpsParser RIFPSParser;

void free_RIFPSParser(RIFPSParser* config);

int64_t parse_rifps(const char *input, TripleHandler hook, void* hook_data, RIFPSParser* config);
