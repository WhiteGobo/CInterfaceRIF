#include "memory.h"
#include <stdlib.h>
#include <string.h>

#define _XS_ "http://www.w3.org/2001/XMLSchema#"
#define XS_integer _XS_ "integer"
#define XS_decimal _XS_ "decimal"
#define XS_string _XS_ "string"

#define _RDF_ "http://www.w3.org/1999/02/22-rdf-syntax-ns#"
#define RDF_nil _RDF_"nil"

#define _RIF_ "http://www.w3.org/2007/rif#"
#define RIF_local _RIF_"local"

static size_t wanderid = 0;

static char* gen_string(const char*, size_t len);


Document* Document_new(Group* group){
	Document* ret = malloc(sizeof(Document));
	ret->group = group;
	ret->meta = Meta_new();
	return ret;
}

Group* Group_new(Clauselist* sentences){
	Group* ret = malloc(sizeof(Group));
	ret->clauselist = sentences;
	ret->meta = Meta_new();
	return ret;
}

Clause* Group_to_Clause(Group* group){
	Clause* ret = malloc(sizeof(Clause));
	ret->type = CLAUSE_GROUP;
	ret->group = group;
	return ret;
}


Clause* Formula_to_Clause(Formula* formula){
	Clause* ret = malloc(sizeof(Clause));
	switch (formula->type){
		case FORMULA_ATOM:
			ret->type = CLAUSE_ATOM;
			ret->atom = formula->atom;
			break;
		case FORMULA_FRAME:
			ret->type = CLAUSE_FRAME;
			ret->frame = formula->frame;
			break;
			/*
		case FORMULA_MEMBER:
			ret->type = CLAUSE_MEMBER;
			ret->member = formula->member;
			break;
		case FORMULA_SUBCLASS:
			ret->type = CLAUSE_SUBCLASS;
			ret->subclass = formula->sublcass;
			break;
		case FORMULA_EQUAL:
			ret->type = CLAUSE_EQUAL;
			ret->equal = formula->equal;
			break;
			*/
	}
	free(formula);
	return ret;
}

Clause* Clause_from_atom(Atom* atom){
	Clause* ret = malloc(sizeof(Clause));
	ret->type = CLAUSE_ATOM;
	ret->atom = atom;
	return ret;
}

Clause* Clause_from_frame(Frame* frame){
	Clause* ret = malloc(sizeof(Clause));
	ret->type = CLAUSE_FRAME;
	ret->frame = frame;
	return ret;
}

Clause* Clause_new_frame(Term* object, Slotlist* slots){
	Clause* ret = malloc(sizeof(Clause));
	ret->type = CLAUSE_FRAME;
	ret->frame = malloc(sizeof(Frame));
	ret->frame->object = object;
	ret->frame->slots = slots;
	ret->frame->meta = Meta_new();
	return ret;
}

Clause* Clause_new_forall(Varlist* varlist, Formulalist* formulalist, Clause* clause)
{
	Clause* ret = malloc(sizeof(Clause));
	ret->type = CLAUSE_FORALL;
	ret->forall = malloc(sizeof(Forall));
	ret->forall->varlist = varlist;
	ret->forall->formulalist = formulalist;
	ret->forall->clause = clause;
	ret->forall->meta = Meta_new();
	return ret;
}

Clause* Clause_new_do(Varassignlist* varassignlist, Actionlist* actionlist){
	Clause* ret = malloc(sizeof(Clause));
	ret->type = CLAUSE_DO;
	ret->do_ = malloc(sizeof(Do));
	ret->do_->varassignlist = varassignlist;
	ret->do_->actionlist = actionlist;
	ret->do_->meta = Meta_new();
	return ret;
}

Clause* Clause_new_prd_implies(Formula* trigger, Clause* action){
	Clause* ret = malloc(sizeof(Clause));
	ret->type = CLAUSE_IMPLIES;
	ret->implies = malloc(sizeof(Implies));
	ret->implies->trigger = trigger;
	ret->implies->action = action;
	ret->implies->meta = Meta_new();
	return ret;
}

Clause* Clause_new_horn_implies(Formula* action, Formula* condition){
	Clause* ret = malloc(sizeof(Clause));
	ret->type = CLAUSE_HORN_CLAUSE;
	ret->horn_clause = malloc(sizeof(HornClause));
	ret->horn_clause->condition = condition;
	ret->horn_clause->action = action;
	ret->horn_clause->meta = Meta_new();
	return ret;
}

Clauselist* Clauselist_insert(Clauselist* rest, Clause* first){
	if(first == NULL){
		fprintf(stderr, "Termlist_insert called with NULL\n");
		return NULL;
	}
	Clauselist* ret = malloc(sizeof(Clauselist));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->first = first;
	ret->rest = rest;
	ret->meta = Meta_new();
	return ret;
}


Varassign* Varassign_new_create(const char* name, size_t len_name){
	Varassign* ret = malloc(sizeof(Varassign));
	ret->var = Var_new(name, len_name);
	ret->type = VARASSIGN_CREATE;
	ret->meta_first = Meta_new();
	ret->meta_second = Meta_new();
	ret->new = Meta_new();
	return ret;
}

Varassign* Varassign_new_retrieve(const char* name, size_t len_name, Frame* frame){
	Varassign* ret = malloc(sizeof(Varassign));
	ret->type = VARASSIGN_RETRIEVE;
	ret->var = Var_new(name, len_name);
	ret->frame = frame;
	ret->meta_first = Meta_new();
	ret->meta_second = Meta_new();
	return ret;
}
Varassignlist* Varassignlist_insert(Varassignlist* rest, Varassign* first){
	if(first == NULL){
		fprintf(stderr, "Varassignlist_insert called with NULL\n");
		return NULL;
	}
	Varassignlist* ret = malloc(sizeof(Varassignlist));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->first = first;
	ret->rest = rest;
	ret->meta = Meta_new();
	return ret;
}

Termlist* Termlist_new(){
	return NULL;
}


Termlist* Termlist_insert(Termlist* rest, Term* first){
	if(first == NULL){
		fprintf(stderr, "Termlist_insert called with NULL\n");
		return NULL;
	}
	Termlist* ret = malloc(sizeof(Termlist));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->first = first;
	ret->rest = rest;
	ret->meta = Meta_new();
	return ret;
}

Uniterm* Uniterm_new(Const* op, Termlist* args){
	if (op == NULL){
		fprintf(stderr, "Uniterm_new called with NULL\n");
		return NULL;
	}
	Uniterm* ret = malloc(sizeof(Uniterm));
	if (ret == NULL){
		fprintf(stderr, "Critical error: malloc failed\n");
		return NULL;
	}
	ret->op = op;
	ret->args = args;
	return ret;
}

Actionlist* Actionlist_insert(Actionlist* rest, Action* first){
	if(first == NULL){
		fprintf(stderr, "Formulalist_insert called with NULL\n");
		return NULL;
	}
	Actionlist* ret = malloc(sizeof(Actionlist));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->first = first;
	ret->rest = rest;
	ret->meta = Meta_new();
	return ret;
}

Action* Action_new_assert_atom(Atom* atom){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_ASSERT;
	ret->assert = Assert_new_atom(atom);
	return ret;
}

Assert* Assert_new_atom(Atom* atom){
	Assert* ret = malloc(sizeof(Assert));
	ret->type = ASSERT_ATOM;
	ret->atom = atom;
	ret->meta = Meta_new();
	return ret;
}

Action* Action_new_assert_frame(Frame* frame){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_ASSERT;
	ret->assert = Assert_new_frame(frame);
	return ret;
}

Assert* Assert_new_frame(Frame* frame){
	Assert* ret = malloc(sizeof(Assert));
	ret->type = ASSERT_FRAME;
	ret->frame = frame;
	ret->meta = Meta_new();
	return ret;
}

Action* Action_new_assert_member(Member* member){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_ASSERT;
	ret->assert = Assert_new_member(member);
	return ret;
}

Assert* Assert_new_member(Member* member){
	Assert* ret = malloc(sizeof(Assert));
	ret->type = ASSERT_MEMBER;
	ret->member = member;
	ret->meta = Meta_new();
	return ret;
}

Action* Action_new_retract_atom(Atom* atom){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_RETRACT;
	ret->retract = Retract_new_atom(atom);
	return ret;
}

Retract* Retract_new_atom(Atom* atom){
	Retract* ret = malloc(sizeof(Retract));
	ret->type = RETRACT_ATOM;
	ret->atom = atom;
	ret->meta = Meta_new();
	return ret;
}

Action* Action_new_retract_frame(Frame* frame){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_RETRACT;
	ret->retract = Retract_new_frame(frame);
	return ret;
}

Retract* Retract_new_frame(Frame* frame){
	Retract* ret = malloc(sizeof(Retract));
	ret->type = RETRACT_FRAME;
	ret->frame = frame;
	ret->meta = Meta_new();
	return ret;
}

Action* Action_new_retract_object(Term* object){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_RETRACT;
	ret->retract = Retract_new_object(object);
	return ret;
}
Retract* Retract_new_object(Term* object){
	Retract* ret = malloc(sizeof(Retract));
	ret->type = RETRACT_OBJECT;
	ret->object = object;
	ret->meta = Meta_new();
	return ret;
}

Action* Action_new_retract_object_property(Term* object, Term* property){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_RETRACT;
	ret->retract = Retract_new_object_property(object, property);
	return ret;
}

Retract* Retract_new_object_property(Term* object, Term* property){
	Retract* ret = malloc(sizeof(Retract));
	ret->type = RETRACT_OBJECT_PROPERTY;
	ret->object_property = Termdublette_new(object, property);
	ret->meta = Meta_new();
	return ret;
}

Termdublette* Termdublette_new(Term* object, Term* property){
	Termdublette* ret = malloc(sizeof(Termdublette));
	ret->object = object;
	ret->property = property;
	ret->meta = Meta_new();
	ret->meta_second = Meta_new();
	return ret;
}

Action* Action_new_modify_frame(Frame* frame){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_MODIFY;
	ret->modify = Modify_new_frame(frame);
	return ret;
}

Modify* Modify_new_frame(Frame* frame){
	Modify* ret= malloc(sizeof(Modify));
	ret->frame = frame;
	ret->meta = Meta_new();
	return ret;
}

Action* Action_new_execute_atom(Atom* atom){
	Action* ret = malloc(sizeof(Action));
	ret->type = ACTION_EXECUTE;
	ret->execute = Execute_new(atom);
	return ret;
}

Execute* Execute_new(Atom* atom){
	Execute* ret = malloc(sizeof(Execute));
	ret->atom = atom;
	ret->meta = Meta_new();
	return ret;
}


Formulalist* Formulalist_new(){
	return NULL;
}

Formulalist* Formulalist_insert(Formulalist* rest, Formula* first){
	if(first == NULL){
		fprintf(stderr, "Formulalist_insert called with NULL\n");
		return NULL;
	}
	Formulalist* ret = malloc(sizeof(Formulalist));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->first = first;
	ret->rest = rest;
	ret->meta = Meta_new();
	return ret;
}

Formula* Formula_new_frame(Term* object, Slotlist* slots){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_FRAME;
	ret->frame = Frame_new(object, slots);
	return ret;
}
Formula* Formula_new_member(Term* instance, Term* class){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_MEMBER;
	ret->member = Member_new(instance, class);
	return ret;
}

Member* Member_new(Term* instance, Term* class){
	Member* ret =  malloc(sizeof(Member));
	ret->instance = instance;
	ret->class = class;
	ret->meta = Meta_new();
	return ret;
}

Formula* Formula_new_subclass(Term* sub, Term* super){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_SUBCLASS;
	ret->subclass = malloc(sizeof(Subclass));
	ret->subclass->sub = sub;
	ret->subclass->super = super;
	return ret;
}
Formula* Formula_new_equal(Term* left, Term* right){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_EQUAL;
	ret->equal = malloc(sizeof(Equal));
	ret->equal->left = left;
	ret->equal->right = right;
	return ret;
}

Formula* Formula_new_ineg(Formula* formula){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_INEG;
	ret->ineg = malloc(sizeof(Ineg));
	ret->ineg->formula = formula;
	ret->ineg->meta = Meta_new();
	return ret;
}

Formula* Formula_new_and(Formulalist* formulalist){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_AND;
	ret->and = malloc(sizeof(And));
	ret->and->formulalist = formulalist;
	ret->and->meta = Meta_new();
	return ret;
}

Formula* Formula_new_or(Formulalist* formulalist){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_OR;
	ret->or = malloc(sizeof(Or));
	ret->or->formulalist = formulalist;
	ret->or->meta = Meta_new();
	return ret;
}

Formula* Formula_new_external(Atom* atom){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_EXTERNAL;
	ret->external = malloc(sizeof(External));
	ret->external->atom = atom;
	ret->external->meta = Meta_new();
	return ret;
}

Formula* Formula_from_atom(Atom* atom){
	if(atom == NULL){
		fprintf(stderr, "Formula_from_atom doesnt support NULL\n");
		return NULL;
	}
	Formula* ret = malloc(sizeof(Formula));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->type = FORMULA_ATOM;
	ret->atom = atom;
	return ret;
}

Formula* Formula_from_frame(Frame* frame){
	if(frame == NULL){
		fprintf(stderr, "Formula_from_frame doesnt support NULL\n");
		return NULL;
	}
	Formula* ret = malloc(sizeof(Formula));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->type = FORMULA_FRAME;
	ret->frame = frame;
	return ret;
}

Formula* Formula_new_exists(Varlist* vars, Formula* formula){
	Formula* ret = malloc(sizeof(Formula));
	ret->type = FORMULA_EXISTS;
	ret->exists = malloc(sizeof(Exists));
	ret->exists->vars = vars;
	ret->exists->formula = formula;
	ret->exists->meta = Meta_new();
	return ret;
}

Atom* Atom_new(Term* op, Termlist* args){
	Atom* atom = malloc(sizeof(Atom));
	if (atom == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	atom->op = op;
	atom->args = args;
	return atom;
}

Atom* Uniterm_to_Atom(Uniterm* old){
	Atom* ret = malloc(sizeof(Atom));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->op = Const_to_Term(old->op);
	ret->args = old->args;
	ret->meta = Meta_new();
	free(old);
	return ret;
}

Frame* Frame_new(Term* object, Slotlist* slots){
	Frame* ret = malloc(sizeof(Frame));
	ret->object = object;
	ret->slots = slots;
	ret->meta = Meta_new();
	return ret;
}

Expr* Uniterm_to_Expr(Uniterm* old){
	if (old == NULL) return NULL;
	Expr* ret = malloc(sizeof(Expr));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->op = Const_to_Term(old->op);
	ret->args = old->args;
	ret->meta = Meta_new();
	free(old);
	return ret;
}

Term* Const_to_Term(Const* term){
	Term* ret = malloc(sizeof(Term));
	ret->type = term->type;
	ret->value = term->value;
	ret->suffix = term->suffix;
	ret->meta = term->meta;
	free(term);
	return ret;
}

//Term* Var_to_Term(Var*);
//Term* List_to_Term(List*);
//Term* External_to_Term(External*);


void Term_add_meta(Term* term, Meta* meta){
	if (meta != NULL){
		free_Meta(term->meta);
		term->meta = meta;
	}
}

void Formula_add_meta(Formula* formula, Meta* meta){
	switch(formula->type){
		case FORMULA_ATOM:
			return Atom_add_meta(formula->atom, meta);
		case FORMULA_FRAME:
			return Frame_add_meta(formula->frame, meta);
		case FORMULA_MEMBER:
			return Member_add_meta(formula->member, meta);
		case FORMULA_SUBCLASS:
			return Subclass_add_meta(formula->subclass, meta);
		case FORMULA_EQUAL:
			return Equal_add_meta(formula->equal, meta);
		case FORMULA_EXISTS:
			return Exists_add_meta(formula->exists, meta);
		case FORMULA_INEG:
			return Ineg_add_meta(formula->ineg, meta);
		case FORMULA_AND:
			return And_add_meta(formula->and, meta);
		case FORMULA_OR:
			return Or_add_meta(formula->or, meta);
		case FORMULA_EXTERNAL:
			return External_add_meta(formula->external, meta);
	}
}

void Exists_add_meta(Exists* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void Atom_add_meta(Atom* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void Frame_add_meta(Frame* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void Member_add_meta(Member* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void Subclass_add_meta(Subclass* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void Equal_add_meta(Equal* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void Ineg_add_meta(Ineg* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void And_add_meta(And* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void Or_add_meta(Or* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void External_add_meta(External* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}

void Group_add_meta(Group* x, Meta* meta){
	if (meta != NULL){
		free_Meta(x->meta);
		x->meta = meta;
	}
}


Meta* Meta_new(){
	Meta* ret = malloc(sizeof(Meta));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->id_type = BNODE;
	ret->id = malloc(20);
	sprintf(ret->id, "b%d", wanderid++);
	return ret;
}

Meta* Meta_new_iri(const char* iri, size_t len){
	Meta* ret = malloc(sizeof(Meta));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->id_type = URI;
	ret->id = malloc(len+1);
	memcpy(ret->id, iri, len);
	ret->id[len] = '\0';
	return ret;
}

void free_Meta(Meta* x){
	if (x == NULL) return;
	if(x->id != NULL){
		free(x->id);
	}
	free(x);
}

Const* Const_new_IRI(const char* value, size_t len){
	Const* ret = malloc(sizeof(Const));
	ret->type = TERM_IRI;
	ret->value = gen_string(value, len);
	ret->suffix = NULL;
	ret->meta = Meta_new();
	return ret;
}

Const* Const_new_CURIE(const char* prefix, size_t len_prefix, const char* suffix, size_t len_suffix)
{
	fprintf(stderr, "Const_new_CURIE not implemented\n");
	return NULL;
}

Const* Const_new_local(const char* value, size_t len){
	Const* ret = malloc(sizeof(Const));
	ret->type = TERM_LOCAL;
	ret->value = gen_string(value, len);
	ret->suffix = NULL;
	ret->meta = Meta_new();
	return ret;
}

Const* Const_new_TypedLiteral(const char* prefix, size_t len_prefix, const char* suffix, size_t len_suffix)
{
	Const* ret = malloc(sizeof(Const));
	ret->value = gen_string(prefix, len_prefix);
	if (
			len_suffix == sizeof(RIF_local)-1
			&& 0 == memcmp(suffix, RIF_local, len_suffix)
	) {
		ret->type = TERM_LOCAL;
		ret->suffix = NULL;
	} else {
		ret->type = TERM_TYPEDLITERAL;
		ret->suffix = gen_string(suffix, len_suffix);
	}
	ret->meta = Meta_new();
	return ret;
}

Const* Const_new_LangLiteral(const char* prefix, size_t len_prefix, const char* suffix, size_t len_suffix){
	Const* ret = malloc(sizeof(Const));
	ret->type = TERM_LANGLITERAL;
	ret->value = gen_string(prefix, len_prefix);
	ret->suffix = gen_string(suffix, len_suffix);
	ret->meta = Meta_new();
	return ret;
}

Const* Const_new_int(const char* value, size_t len){
	return Const_new_TypedLiteral(value, len,
			XS_integer, sizeof(XS_integer)-1);
}

Const* Const_new_float(const char* value, size_t len){
	return Const_new_TypedLiteral(value, len,
			XS_decimal, sizeof(XS_decimal)-1);
}

Const* Const_new_Literal(const char* val, size_t len){
	return Const_new_TypedLiteral(val, len, XS_string, sizeof(XS_string)-1);
}


Slot* Slot_new(Term* key, Term* value){
	Slot* ret = malloc(sizeof(Slot));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->key = key;
	ret->value = value;
	ret->meta = Meta_new();
	return ret;
}

Slotlist* Slotlist_insert(Slotlist* rest, Slot* slot){
	if(slot == NULL) return NULL;
	Slotlist* ret = malloc(sizeof(Slotlist));
	if (ret == NULL){
		fprintf(stderr, "CRITICAL ERROR: malloc failed\n");
		return NULL;
	}
	ret->first = slot;
	ret->rest = rest;
	ret->meta = Meta_new();
	return ret;
}


void fprintf_debug_const(FILE* f, const Const* x){
	fprintf_debug_term(f, (const Term*) x);
}

void fprintf_debug_termlist(FILE* f, const Termlist* list){
	for(const Termlist* x=list; x != NULL; x=x->rest){
		fprintf_debug_term(f, x->first);
		fprintf(f, " ");
	}
}

void fprintf_debug_term(FILE* f, const Term* x){
	if (x == NULL){
		fprintf(f, "(NULL)");
		return;
	}
	switch(x->type){
		case TERM_IRI:
			fprintf(f, "<%s>", x->value);
			break;
		case TERM_LANGLITERAL:
			fprintf(f, "\"%s\"@@<%s>", x->value, x->suffix);
			break;
		case TERM_TYPEDLITERAL:
			if (0 == strcmp(x->suffix, XS_string)){
				fprintf(f, "\"%s\"", x->value);
			} else {
				fprintf(f, "\"%s\"^^<%s>", x->value, x->suffix);
			}
			break;
		case TERM_LOCAL:
			fprintf(f, "_%s", x->value);
			break;
		case TERM_VARIABLE:
			fprintf(f, "?%s", x->value);
			break;
		case TERM_LIST:
			fprintf(f, "List(");
			fprintf_debug_termlist(f, x->list);
			fprintf(f, ")");
			break;
		case TERM_EXTERNAL:
			fprintf(f, "External(");
			fprintf_debug_expr(f, x->expr);
			fprintf(f, ")");
			break;
		default:
			fprintf(f, "Const(BROKEN)");
	}
}

void fprintf_debug_uniterm(FILE* f, const Uniterm* x){
	if (x == NULL){
		fprintf(f, "Uniterm(NULL)");
		return;
	}
	fprintf_debug_const(f, x->op);
	fprintf(f, "(");
	fprintf_debug_termlist(f, x->args);
	fprintf(f, ")");
}

void fprintf_debug_expr(FILE* f, const Expr* x){
	if (x == NULL){
		fprintf(f, "Atom(NULL)");
		return;
	}
	fprintf_debug_term(f, x->op);
	fprintf(f, "(");
	fprintf_debug_termlist(f, x->args);
	fprintf(f, ")");
}

void fprintf_debug_atom(FILE* f, const Atom* x){
	if (x == NULL){
		fprintf(f, "Atom(NULL)");
		return;
	}
	fprintf_debug_term(f, x->op);
	fprintf(f, "(");
	fprintf_debug_termlist(f, x->args);
	fprintf(f, ")");
}

void fprintf_debug_frame(FILE* f, const Frame* frame){
	if (frame == NULL){
		fprintf(f, "Frame(NULL)");
		return;
	}
	fprintf_debug_term(f, frame->object);
	fprintf(f, "[");
	for(Slotlist* x = frame->slots; x!= NULL; x=x->rest){
		fprintf_debug_term(f,x->first->key);
		fprintf(f, " -> ");
		fprintf_debug_term(f, x->first->value);
	}
	fprintf(f, "]");
}

void fprintf_debug_member(FILE* f, const Member* member){
	if (member == NULL){
		fprintf(f, "Member(NULL)");
		return;
	}
	fprintf_debug_term(f, member->instance);
	fprintf(f, " # ");
	fprintf_debug_term(f, member->class);
}

void fprintf_debug_subclass(FILE* f, const Subclass* subclass){
	if (subclass == NULL){
		fprintf(f, "Subclass(NULL)");
		return;
	}
	fprintf_debug_term(f, subclass->sub);
	fprintf(f, " ## ");
	fprintf_debug_term(f, subclass->super);
}

void fprintf_debug_equal(FILE* f, const Equal* equal){
	if (equal == NULL){
		fprintf(f, "Equal(NULL)");
		return;
	}
	fprintf_debug_term(f, equal->left);
	fprintf(f, " = ");
	fprintf_debug_term(f, equal->right);
}

void fprintf_debug_exists(FILE* f, const Exists* exists){
	if (exists == NULL){
		fprintf(f, "Exists(BROKEN)");
		return;
	}
	fprintf(f, "Exists(");
	fprintf_debug_formula(f, exists->formula);
	fprintf(f, ")");
}
void fprintf_debug_ineg(FILE* f, const Ineg* x){
	if (x == NULL){
		fprintf(f, "Ineg(BROKEN)");
		return;
	}
	fprintf(f, "Not(");
	fprintf_debug_formula(f, x->formula);
	fprintf(f, ")");
}
void fprintf_debug_and(FILE* f, const And* x){
	if (x == NULL){
		fprintf(f, "And(BROKEN)");
		return;
	}
	fprintf(f, "And(");
	fprintf_debug_formulalist(f, x->formulalist);
	fprintf(f, ")");
}
void fprintf_debug_or(FILE* f, const Or* x){
	if (x == NULL){
		fprintf(f, "Or(BROKEN)");
		return;
	}
	fprintf(f, "Or(");
	fprintf_debug_formulalist(f, x->formulalist);
	fprintf(f, ")");
}
void fprintf_debug_external(FILE* f, const External* x){
	if (x == NULL){
		fprintf(f, "External(BROKEN)");
		return;
	}
	fprintf(f, "External(");
	fprintf_debug_atom(f, x->atom);
	fprintf(f, ")");
}

	
void fprintf_debug_formula(FILE* f, const Formula* x){
	if (x == NULL){
		fprintf(f, "Formula(NULL)");
		return;
	}
	switch (x->type){
		case FORMULA_ATOM:
			fprintf_debug_atom(f, x->atom);
			break;
		case FORMULA_FRAME:
			fprintf_debug_frame(f, x->frame);
			break;
		case FORMULA_MEMBER:
			fprintf_debug_member(f, x->member);
			break;
		case FORMULA_SUBCLASS:
			fprintf_debug_subclass(f, x->subclass);
			break;
		case FORMULA_EQUAL:
			fprintf_debug_equal(f, x->equal);
			break;
		case FORMULA_EXISTS:
			fprintf_debug_exists(f, x->exists);
			break;
		case FORMULA_INEG:
			fprintf_debug_ineg(f, x->ineg);
			break;
		case FORMULA_AND:
			fprintf_debug_and(f, x->and);
			break;
		case FORMULA_OR:
			fprintf_debug_or(f, x->or);
			break;
		case FORMULA_EXTERNAL:
			fprintf_debug_external(f, x->external);
			break;

		default:
			fprintf(f, "Formula(BROKEN)");
	}
}

void fprintf_debug_formulalist(FILE* f, const Formulalist* list){
	for(const Formulalist* x=list; x != NULL; x=x->rest){
		fprintf_debug_formula(f, x->first);
		fprintf(f, " ");
	}
}


Term* Term_new_var(const char* name, size_t len){
	Term* ret = malloc(sizeof(Term));
	ret->type = TERM_VARIABLE;
	ret->value = gen_string(name, len);
	ret->suffix = NULL;
	ret->meta = Meta_new();
	return ret;
}

Term* Term_new_list(Termlist* x){
	Term* ret = malloc(sizeof(Term));
	ret->type = TERM_LIST;
	ret->list = x;
	ret->meta = Meta_new();
	return ret;
}

Term* Term_new_external(Expr* x){
	Term* ret = malloc(sizeof(Term));
	ret->type = TERM_EXTERNAL;
	ret->expr = x;
	ret->meta = Meta_new();
	return ret;
}

Var* Var_new(const char* name, size_t len){
	Var* ret = malloc(sizeof(Var));
	ret->meta = Meta_new();
	ret->name = gen_string(name, len);
	return ret;
}

Varlist* Varlist_insert(Varlist* rest, Var* first){
	Varlist* ret = malloc(sizeof(Varlist));
	ret->first = first;
	ret->rest = rest;
	ret->meta = Meta_new();
	return ret;
}


static char* gen_string(const char* input, size_t length){
	char* ret = malloc(length+1);
	memcpy(ret, input, length);
	ret[length] = '\0';
	return ret;
}



ToRDF_Id Meta_produce_id(Meta* meta){
	ToRDF_Id ret = {
		.type=BNODE,
		.value="unknown",
	};
	if (meta != NULL){
		ret.type = meta->id_type;
		ret.value = meta->id;
	}
	return ret;
}

ToRDF_Id Document_get_id(Document* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Group_get_id(Group* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Clause_get_id(Clause* clause){
	switch(clause->type){
		case CLAUSE_GROUP:
			return Group_get_id(clause->group);
		case CLAUSE_ATOM:
			return Atom_get_id(clause->atom);
		case CLAUSE_FRAME:
			return Frame_get_id(clause->frame);
		case CLAUSE_FORALL:
			return Forall_get_id(clause->forall);
		case CLAUSE_DO:
			return Do_get_id(clause->do_);
		case CLAUSE_HORN_CLAUSE:
			return HornClause_get_id(clause->horn_clause);
		case CLAUSE_IMPLIES:
			return Implies_get_id(clause->implies);
	}
}

ToRDF_Id Clauselist_get_id(Clauselist* list){
	ToRDF_Id ret_nil = {
		.value = RDF_nil,
		.type = URI,
	};
	if (list == NULL) {
		return ret_nil;
	}
	return Meta_produce_id(list->meta);
}

ToRDF_Id Formula_get_id(Formula* formula){
	switch(formula->type){
		case FORMULA_ATOM:
			return Atom_get_id(formula->atom);
		case FORMULA_FRAME:
			return Frame_get_id(formula->frame);
		case FORMULA_MEMBER:
			return Member_get_id(formula->member);
		case FORMULA_SUBCLASS:
			return Subclass_get_id(formula->subclass);
		case FORMULA_EQUAL:
			return Equal_get_id(formula->equal);
		case FORMULA_EXISTS:
			return Exists_get_id(formula->exists);
		case FORMULA_INEG:
			return Ineg_get_id(formula->ineg);
		case FORMULA_AND:
			return And_get_id(formula->and);
		case FORMULA_OR:
			return Or_get_id(formula->or);
		case FORMULA_EXTERNAL:
			return External_get_id(formula->external);
	}
}

ToRDF_Id Formulalist_get_id(Formulalist* list){
	ToRDF_Id ret;
	if (list == NULL) {
		ret.value = RDF_nil;
		ret.type = URI;
		return ret;
	}
	return Meta_produce_id(list->meta);
}

ToRDF_Id Var_get_id(Var* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Term_get_id(Term* term){
	return Meta_produce_id(term->meta);
}


ToRDF_Id Termlist_get_id(Termlist* list){
	ToRDF_Id ret;
	if (list == NULL) {
		ret.value = RDF_nil;
		ret.type = URI;
		return ret;
	}
	return Meta_produce_id(list->meta);
}

ToRDF_Id Varlist_get_id(Varlist* list){
	ToRDF_Id ret;
	if (list == NULL) {
		ret.value = RDF_nil;
		ret.type = URI;
		return ret;
	}
	return Meta_produce_id(list->meta);
}

ToRDF_Id Slotlist_get_id(Slotlist* list){
	ToRDF_Id ret;
	if (list == NULL) {
		ret.value = RDF_nil;
		ret.type = URI;
		return ret;
	}
	return Meta_produce_id(list->meta);
}

ToRDF_Id Atom_get_id(Atom* atom){
	return Meta_produce_id(atom->meta);
}

ToRDF_Id Expr_get_id(Expr* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Frame_get_id(Frame* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Member_get_id(Member* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Subclass_get_id(Subclass* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Exists_get_id(Exists* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Ineg_get_id(Ineg* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id And_get_id(And* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id Or_get_id(Or* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id External_get_id(External* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Equal_get_id(Equal* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Action_get_id(Action* x){
	switch(x->type){
		case ACTION_ASSERT:
			return Assert_get_id(x->assert);
		case ACTION_RETRACT:
			return Retract_get_id(x->retract);
		case ACTION_MODIFY:
			return Modify_get_id(x->modify);
		case ACTION_EXECUTE:
			return Execute_get_id(x->execute);
	}
}
ToRDF_Id Actionlist_get_id(Actionlist* list){
	ToRDF_Id ret;
	if (list == NULL) {
		ret.value = RDF_nil;
		ret.type = URI;
		return ret;
	}
	return Meta_produce_id(list->meta);
}
ToRDF_Id Assert_get_id(Assert* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id Retract_get_id(Retract* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id Termdublette_get_id(Termdublette* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id Termdublette_get_second_id(Termdublette* x){
	return Meta_produce_id(x->meta_second);
}
ToRDF_Id Modify_get_id(Modify* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id Execute_get_id(Execute* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Slot_get_id(Slot* x){
	return Meta_produce_id(x->meta);
}

ToRDF_Id Forall_get_id(Forall* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id Do_get_id(Do* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id HornClause_get_id(HornClause* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id Implies_get_id(Implies* x){
	return Meta_produce_id(x->meta);
}
ToRDF_Id Varassign_get_id(Varassign* x){
	return Meta_produce_id(x->meta_first);
}
ToRDF_Id Varassign_get_second_id(Varassign* x){
	return Meta_produce_id(x->meta_second);
}

ToRDF_Id New_get_id(New* x){
	return Meta_produce_id(x);
}
