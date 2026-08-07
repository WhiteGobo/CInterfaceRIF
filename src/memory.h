#pragma once

#include <stdio.h>
#include <stdint.h>


#ifndef RDF_TERMTYPE_DEFINED
#define RDF_TERMTYPE_DEFINED
typedef enum {
        URI = 0,
        BNODE = 1,
        TYPEDLITERAL = 2,
        LANGLITERAL = 3,
} TERMTYPE;
#endif


typedef struct s_triplestring {
	const char* first;
	size_t len_first;
	const char* second;
	size_t len_second;
	const char* third;
	size_t len_third;
} Triplestring;


typedef struct tordf_Id {
	TERMTYPE type;
	const char* value;
} ToRDF_Id;

typedef struct s_Document Document;
typedef struct s_Group Group;
typedef struct s_Clauselist Clauselist;
typedef struct s_Clause Clause;
typedef struct s_Do Do;
typedef struct s_Formula Formula;
typedef struct s_Formulalist Formulalist;
typedef struct s_Term Term;
typedef struct s_Termlist Termlist;
typedef struct s_Uniterm Uniterm;
typedef struct s_Atom Atom;
typedef struct s_Expr Expr;
typedef struct s_Frame Frame;
typedef struct s_Member Member;
typedef struct s_Subclass Subclass;
typedef struct s_Equal Equal;
typedef struct s_Exists Exists;
typedef struct s_Ineg Ineg;
typedef struct s_And And;
typedef struct s_Or Or;
typedef struct s_External External;
typedef struct s_Const Const;
typedef struct s_Meta Meta;
typedef Meta New;
typedef struct s_Slot Slot;
typedef struct s_SlotList Slotlist;
typedef struct s_Var Var;
typedef struct s_Varlist Varlist;
typedef struct s_Actionlist Actionlist;
typedef struct s_Action Action;
typedef struct s_Assert Assert;
typedef struct s_Retract Retract;
typedef struct s_Termdublette Termdublette;
typedef struct s_Modify Modify;
typedef struct s_Execute Execute;
typedef struct s_Varassign Varassign;
typedef struct s_Varassignlist Varassignlist;
typedef struct s_Forall Forall;
typedef struct s_Implies Implies;
typedef struct s_HornClause HornClause;



typedef enum {
	FORMULA_ATOM,
	FORMULA_FRAME,
	FORMULA_MEMBER,
	FORMULA_SUBCLASS,
	FORMULA_EQUAL,
	FORMULA_EXISTS,
	FORMULA_INEG,
	FORMULA_AND,
	FORMULA_OR,
	FORMULA_EXTERNAL,
} FORMULATYPE;

typedef enum {
	CLAUSE_GROUP,
	CLAUSE_ATOM,
	CLAUSE_FRAME,
	CLAUSE_FORALL,
	CLAUSE_DO,
	CLAUSE_HORN_CLAUSE,
	CLAUSE_IMPLIES,
} CLAUSETYPE;

typedef enum {
	TERM_IRI,
	TERM_LANGLITERAL,
	TERM_TYPEDLITERAL,
	TERM_LOCAL,
	TERM_VARIABLE,
	TERM_LIST,
	TERM_EXTERNAL,
} RIFTERMTYPE;

typedef enum {
	ACTION_ASSERT,
	ACTION_RETRACT,
	ACTION_MODIFY,
	ACTION_EXECUTE,
} ACTIONTYPE;

typedef enum {
	ASSERT_ATOM,
	ASSERT_FRAME,
	ASSERT_MEMBER,
} ASSERTTYPE;

typedef enum {
	RETRACT_ATOM,
	RETRACT_FRAME,
	RETRACT_OBJECT,
	RETRACT_OBJECT_PROPERTY,
} RETRACTTYPE;

typedef enum {
	VARASSIGN_CREATE,
	VARASSIGN_RETRIEVE,
} VARASSIGNTYPE;

typedef struct s_Document {
	Group* group;
	Meta* meta;
} Document;

typedef struct s_Group {
	void* clauselist;
	Meta* meta;
} Group;

typedef struct s_Clauselist {
	Clause* first;
	Clauselist* rest;
	Meta* meta;
} Clauselist;

typedef struct s_Clause {
	CLAUSETYPE type;
	union {
		Group* group;
		Atom* atom;
		Frame* frame;
		Forall* forall;
		Do* do_;
		HornClause* horn_clause;
		Implies* implies;
	};
} Clause;

typedef struct s_Do {
	Varassignlist* varassignlist;
	Actionlist* actionlist;
	Meta* meta;
} Do;

typedef struct s_Term {
	RIFTERMTYPE type;
	union {
		char* value;
		Termlist* list;
		Expr* expr;
	};
	union {
		char* suffix;
	};
	Meta* meta;
} Term;

typedef struct s_Termlist {
	Term* first;
	Termlist* rest;
	Meta* meta;
} Termlist;

typedef struct s_Formula {
	FORMULATYPE type;
	union {
		Atom* atom;
		Frame* frame;
		Member* member;
		Subclass* subclass;
		Equal* equal;
		Exists* exists;
		Ineg* ineg;
		Formulalist* formulas;
		And* and;
		Or* or;
		External* external;
	};
} Formula;

typedef struct s_Uniterm {
	Const* op;
	Termlist* args;
} Uniterm;

typedef struct s_Atom {
	Term* op;
	Termlist* args;
	Meta* meta;
} Atom;

typedef struct s_Expr {
	Term* op;
	Termlist* args;
	Meta* meta;
} Expr;

typedef struct s_Frame {
	Term* object;
	Slotlist* slots;
	Meta* meta;
} Frame;

typedef struct s_Member {
	Term* instance;
	Term* class;
	Meta* meta;
} Member;

typedef struct s_Subclass {
	Term* sub;
	Term* super;
	Meta* meta;
} Subclass;

typedef struct s_Equal {
	Term* left;
	Term* right;
	Meta* meta;
} Equal;

typedef struct s_Exists {
	Varlist* vars;
	Formula* formula;
	Meta* meta;
} Exists;

typedef struct s_Ineg {
	Formula* formula;
	Meta* meta;
} Ineg;

typedef struct s_And {
	Formulalist* formulalist;
	Meta* meta;
} And;
typedef struct s_Or {
	Formulalist* formulalist;
	Meta* meta;
} Or;
typedef struct s_External {
	Atom* atom;
	Meta* meta;
} External;

typedef struct s_Const {
	RIFTERMTYPE type;
	char* value;
	char* suffix;
	Meta* meta;
} Const;

typedef struct s_Slot {
	Term* key;
	Term* value;
	Meta* meta;
} Slot;

typedef struct s_SlotList {
	Slot* first;
	Slotlist* rest;
	Meta* meta;
} Slotlist;

typedef struct s_Var {
	char* name;
	Meta* meta;
} Var;

typedef struct s_Varlist {
	Var* first;
	Varlist* rest;
	Meta* meta;
} Varlist;

typedef struct s_Meta {
	TERMTYPE id_type;
	char* id;
} Meta;



typedef struct s_Formulalist {
	Formula* first;
	Formulalist* rest;
	Meta* meta;
} Formulalist;

typedef struct s_Actionlist {
	Action* first;
	Actionlist* rest;
	Meta* meta;
} Actionlist;

typedef struct s_Action {
	ACTIONTYPE type;
	union {
		Assert* assert;
		Retract* retract;
		Modify* modify;
		Execute* execute;
	};
	Meta* meta;
} Action;

typedef struct s_Assert {
	ASSERTTYPE type;
	union {
		Atom* atom;
		Frame* frame;
		Member* member;
	};
	Meta* meta;
} Assert;

typedef struct s_Retract {
	RETRACTTYPE type;
	union {
		Atom* atom;
		Frame* frame;
		Term* object;
		Termdublette* object_property;
	};
	Meta* meta;
} Retract;

typedef struct s_Termdublette {
	Term* object;
	Term* property;
	Meta* meta;
	Meta* meta_second;
} Termdublette;

typedef struct s_Modify {
	Frame* frame;
	Meta* meta;
} Modify;

typedef struct s_Execute {
	Atom* atom;
	Meta* meta;
} Execute;

typedef struct s_Varassign {
	VARASSIGNTYPE type;
	Var* var;
	union {
		Meta* new;
		Frame* frame;
	};
	Meta* meta_first;
	Meta* meta_second;
} Varassign;

typedef struct s_Varassignlist {
	Varassign* first;
	Varassignlist* rest;
	Meta* meta;
} Varassignlist;

typedef struct s_Forall {
	Varlist* varlist;
	Formulalist* formulalist;
	Clause* clause;
	Meta* meta;
} Forall;

typedef struct s_Implies {
	Formula* trigger;
	Clause* action;
	Meta* meta;
} Implies;

typedef struct s_HornClause {
	Formula* condition;
	Formula* action;
	Meta* meta;
} HornClause;


Document* Document_new(Group*);
Group* Group_new(Clauselist* sentences);

Clauselist* Clauselist_insert(Clauselist*, Clause*);
Clause* Group_to_Clause(Group*);
Clause* Formula_to_Clause(Formula*);
Clause* Clause_from_atom(Atom*);
Clause* Clause_from_frame(Frame*);
Clause* Clause_new_do(Varassignlist*, Actionlist*);
Clause* Clause_new_frame(Term* object, Slotlist* slots);
Clause* Clause_new_forall(Varlist*, Formulalist*, Clause*);
Clause* Clause_new_prd_implies(Formula* trigger, Clause* action);
Clause* Clause_new_horn_implies(Formula* action, Formula* condition);

Termlist* Termlist_new();
Termlist* Termlist_insert(Termlist*, Term*);
Uniterm* Uniterm_new(Const*, Termlist*);

Formulalist* Formulalist_new();
Formulalist* Formulalist_insert(Formulalist*, Formula*);

Formula* Formula_new_frame(Term* object, Slotlist* slots);
Formula* Formula_new_member(Term* instance, Term* class);
Formula* Formula_new_subclass(Term* sub, Term* super);
Formula* Formula_new_equal(Term* left, Term* right);
Formula* Formula_from_atom(Atom*);
Formula* Formula_from_frame(Frame*);
Formula* Formula_new_exists(Varlist*, Formula*);
Formula* Formula_new_ineg(Formula*);
Formula* Formula_new_and(Formulalist*);
Formula* Formula_new_or(Formulalist*);
Formula* Formula_new_external(Atom*);
Atom* Atom_new(Term*, Termlist*);
Frame* Frame_new(Term*, Slotlist*);
Member* Member_new(Term*, Term*);
Atom* Uniterm_to_Atom(Uniterm*);
Expr* Uniterm_to_Expr(Uniterm*);

Var* Var_new(const char*, size_t);
Varlist* Varlist_insert(Varlist*, Var*);

Term* Const_to_Term(Const*);
//Term* Var_to_Term(Var*);
//Term* List_to_Term(List*);
//Term* External_to_Term(External*);
Term* Term_new_var(const char*, size_t);
Term* Term_new_list(Termlist*);
Term* Term_new_external(Expr*);
void Term_add_meta(Term*, Meta*);
void Formula_add_meta(Formula*, Meta*);

Const* Const_new_IRI(const char*, size_t);
Const* Const_new_CURIE(const char* prefix, size_t len_prefix, const char* suffix, size_t len_suffix);
Const* Const_new_local(const char* value, size_t len);
Const* Const_new_Literal(const char* value, size_t len_value);
Const* Const_new_TypedLiteral(const char* prefix, size_t len_prefix, const char* suffix, size_t len_suffix);
Const* Const_new_LangLiteral(const char* prefix, size_t len_prefix, const char* suffix, size_t len_suffix);
Const* Const_new_int(const char*, size_t);
Const* Const_new_float(const char*, size_t);

Slot* Slot_new(Term* key, Term* value);
Slotlist* Slotlist_insert(Slotlist*, Slot*);

Uniterm* Uniterm_new(Const* op, Termlist* args);

Actionlist* Actionlist_insert(Actionlist*, Action*);
Action* Action_new_assert_atom(Atom* atom);
Action* Action_new_assert_frame(Frame* frame);
Action* Action_new_assert_member(Member* member);
Action* Action_new_retract_atom(Atom* atom);
Action* Action_new_retract_frame(Frame* frame);
Action* Action_new_retract_object(Term* object);
Action* Action_new_retract_object_property(Term* object, Term* property);
Action* Action_new_modify_frame(Frame* frame);
Action* Action_new_execute_atom(Atom* atom);

Assert* Assert_new_atom(Atom* atom);
Assert* Assert_new_frame(Frame* frame);
Assert* Assert_new_member(Member* member);
Retract* Retract_new_atom(Atom* atom);
Retract* Retract_new_frame(Frame* frame);
Retract* Retract_new_object(Term* object);
Retract* Retract_new_object_property(Term* object, Term* property);
Termdublette* Termdublette_new(Term* object, Term* property);
Modify* Modify_new_frame(Frame* frame);
Execute* Execute_new(Atom* atom);

Varassign* Varassign_new_create(const char* name, size_t len_name);
Varassign* Varassign_new_retrieve(const char* name, size_t len_name, Frame*);
Varassignlist* Varassignlist_insert(Varassignlist*, Varassign*);


Meta* Meta_new();
Meta* Meta_new_iri(const char* iri, size_t len);
void free_Meta(Meta*);

void fprintf_debug_const(FILE*, const Const*);
void fprintf_debug_termlist(FILE*, const Termlist*);
void fprintf_debug_term(FILE*, const Term*);
void fprintf_debug_uniterm(FILE*, const Uniterm*);
void fprintf_debug_atom(FILE*, const Atom*);
void fprintf_debug_expr(FILE*, const Expr*);
void fprintf_debug_frame(FILE* f, const Frame* frame);
void fprintf_debug_member(FILE* f, const Member* member);
void fprintf_debug_subclass(FILE* f, const Subclass* subclass);
void fprintf_debug_equal(FILE* f, const Equal* equal);
void fprintf_debug_exists(FILE* f, const Exists* exists);
void fprintf_debug_ineg(FILE* f, const Ineg*);
void fprintf_debug_and(FILE* f, const And*);
void fprintf_debug_or(FILE* f, const Or*);
void fprintf_debug_external(FILE* f, const External*);
void fprintf_debug_formula(FILE*, const Formula*);
void fprintf_debug_formulalist(FILE*, const Formulalist*);
	

void Exists_add_meta(Exists* x, Meta* meta);
void Equal_add_meta(Equal* x, Meta* meta);
void Subclass_add_meta(Subclass* x, Meta* meta);
void Member_add_meta(Member* x, Meta* meta);
void Frame_add_meta(Frame* x, Meta* meta);
void Atom_add_meta(Atom* x, Meta* meta);
void Action_add_meta(Action* x, Meta* meta);
void Ineg_add_meta(Ineg* x, Meta* meta);
void And_add_meta(And* x, Meta* meta);
void Or_add_meta(Or* x, Meta* meta);
void External_add_meta(External* x, Meta* meta);
void Group_add_meta(Group* x, Meta* meta);


ToRDF_Id Document_get_id(Document*);
ToRDF_Id Group_get_id(Group*);
ToRDF_Id Clauselist_get_id(Clauselist*);
ToRDF_Id Clause_get_id(Clause*);
ToRDF_Id Formula_get_id(Formula*);
ToRDF_Id Formulalist_get_id(Formulalist*);
ToRDF_Id Var_get_id(Var*);
ToRDF_Id Term_get_id(Term*);
ToRDF_Id Termlist_get_id(Termlist*);
ToRDF_Id Varlist_get_id(Varlist*);
ToRDF_Id Slotlist_get_id(Slotlist*);
ToRDF_Id Formula_get_id(Formula*);
ToRDF_Id Atom_get_id(Atom*);
ToRDF_Id Expr_get_id(Expr*);
ToRDF_Id Frame_get_id(Frame*);
ToRDF_Id Slot_get_id(Slot*);
ToRDF_Id Member_get_id(Member*);
ToRDF_Id Subclass_get_id(Subclass*);
ToRDF_Id Exists_get_id(Exists*);
ToRDF_Id Ineg_get_id(Ineg*);
ToRDF_Id And_get_id(And*);
ToRDF_Id Or_get_id(Or*);
ToRDF_Id External_get_id(External*);
ToRDF_Id Equal_get_id(Equal*);
ToRDF_Id Action_get_id(Action*);
ToRDF_Id Actionlist_get_id(Actionlist*);
ToRDF_Id Assert_get_id(Assert*);
ToRDF_Id Retract_get_id(Retract*);
ToRDF_Id Termdublette_get_id(Termdublette*);
ToRDF_Id Termdublette_get_second_id(Termdublette*);
ToRDF_Id Modify_get_id(Modify*);
ToRDF_Id Execute_get_id(Execute*);
ToRDF_Id Varassign_get_id(Varassign*);
ToRDF_Id Varassign_get_second_id(Varassign*);
ToRDF_Id Varassignlist_get_id(Varassignlist*);

ToRDF_Id Forall_get_id(Forall*);
ToRDF_Id Do_get_id(Do*);
ToRDF_Id HornClause_get_id(HornClause*);
ToRDF_Id Implies_get_id(Implies*);
ToRDF_Id New_get_id(New*);
