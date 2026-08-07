#pragma once

#include <stdint.h>
#include "memory.h"

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


void Document_send(Document*, TripleHandler*, void* context);
void Group_send(Group*, TripleHandler*, void* context);
void Clauselist_send(Clauselist*, TripleHandler* handler, void* context);
void Clause_send(Clause*, TripleHandler* handler, void* context);
void Formula_send(Formula*, TripleHandler*, void* context);
void Formulalist_send(Formulalist*, TripleHandler*, void* context);
void Term_send(Term*, TripleHandler*, void* context);
void Termlist_send(Termlist*, TripleHandler*, void* context);
void Termdublette_send(Termdublette*, TripleHandler*, void* context);
void Slotlist_send(Slotlist*, TripleHandler*, void* context);
void Var_send(Var* var, TripleHandler* handler, void* context);
void Varlist_send(Varlist*, TripleHandler*, void* context);

void Forall_send(Forall*, TripleHandler* handler, void* context);
void Do_send(Do*, TripleHandler* handler, void* context);
void HornClause_send(HornClause*, TripleHandler* handler, void* context);
void Implies_send(Implies*, TripleHandler* handler, void* context);
void Action_send(Action*, TripleHandler* handler, void* context);
void Actionlist_send(Actionlist*, TripleHandler* handler, void* context);
void Assert_send(Assert*, TripleHandler* handler, void* context);
void Retract_send(Retract*, TripleHandler* handler, void* context);
void Modify_send(Modify*, TripleHandler* handler, void* context);
void Execute_send(Execute*, TripleHandler* handler, void* context);
void Varassign_send(Varassign*, TripleHandler* handler, void* context);

void Atom_send(Atom*, TripleHandler*, void* context);
void Expr_send(Expr*, TripleHandler* handler, void* context);
void Frame_send(Frame*, TripleHandler*, void* context);
void Member_send(Member*, TripleHandler*, void* context);
void Subclass_send(Subclass*, TripleHandler*, void* context);
void Equal_send(Equal*, TripleHandler*, void* context);
void Exists_send(Exists*, TripleHandler*, void* context);
void Ineg_send(Ineg*, TripleHandler* handler, void* context);
void And_send(And*, TripleHandler* handler, void* context);
void Or_send(Or*, TripleHandler* handler, void* context);
void External_send(External*, TripleHandler* handler, void* context);
