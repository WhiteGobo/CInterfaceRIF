#include "to_rdf.h"


#define _RDF_ "http://www.w3.org/1999/02/22-rdf-syntax-ns#"
#define RDF_type _RDF_"type"
#define RDF_rest _RDF_"rest"
#define RDF_first _RDF_"first"
#define RDF_nil _RDF_"nil"

#define _RIF_ "http://www.w3.org/2007/rif#"
#define RIF_Document _RIF_"Document"
#define RIF_Group _RIF_"Group"
#define RIF_sentences _RIF_"sentences"
#define RIF_payload _RIF_"payload"
#define RIF_Const _RIF_"Const"
#define RIF_Var _RIF_"Var"
#define RIF_Atom _RIF_"Atom"
#define RIF_Expr _RIF_"Expr"
#define RIF_Frame _RIF_"Frame"
#define RIF_Member _RIF_"Member"
#define RIF_Subclass _RIF_"Subclass"
#define RIF_Equal _RIF_"Equal"
#define RIF_Exists _RIF_"Exists"
#define RIF_INeg _RIF_"INeg"
#define RIF_And _RIF_"And"
#define RIF_Or _RIF_"Or"
#define RIF_External _RIF_"External"
#define RIF_Slot _RIF_"Slot"
#define RIF_List _RIF_"List"
#define RIF_New _RIF_"New"
#define RIF_INeg _RIF_"INeg"
#define RIF_target _RIF_"target"
#define RIF_Assert _RIF_"Assert"
#define RIF_Retract _RIF_"Retract"
#define RIF_Modify _RIF_"Modify"
#define RIF_Execute _RIF_"Execute"
#define RIF_Forall _RIF_"Forall"
#define RIF_Do _RIF_"Do"
#define RIF_actionVar _RIF_"actionVar"
#define RIF_actions _RIF_"actions"
#define RIF_pattern _RIF_"pattern"
#define RIF_if _RIF_"if"
#define RIF_then _RIF_"then"
#define RIF_Implies _RIF_"Implies"
#define RIF_op _RIF_"op"
#define RIF_args _RIF_"args"
#define RIF_constIRI _RIF_"constIRI"
#define RIF_value _RIF_"value"
#define RIF_varname _RIF_"varname"
#define RIF_constname _RIF_"constname"
#define RIF_object _RIF_"object"
#define RIF_slots _RIF_"slots"
#define RIF_slotkey _RIF_"slotkey"
#define RIF_slotvalue _RIF_"slotvalue"
#define RIF_instance _RIF_"instance"
#define RIF_class _RIF_"class"
#define RIF_sub _RIF_"sub"
#define RIF_super _RIF_"super"
#define RIF_left _RIF_"left"
#define RIF_right _RIF_"right"
#define RIF_vars _RIF_"vars"
#define RIF_formula _RIF_"formula"
#define RIF_formulas _RIF_"formulas"
#define RIF_content _RIF_"content"
#define RIF_items _RIF_"items"

#define _XS_ "http://www.w3.org/2001/XMLSchema#"
#define XS_anyURI _XS_"anyURI"


void Document_send(Document* document, TripleHandler* handler, void* context){
	if(document == NULL) return;
	int8_t err;
	ToRDF_Id self = Document_get_id(document);
	err = handler(self.value, self.type, RDF_type,
			RIF_Document, NULL, URI,
                	NULL, BNODE, context);
	if (document->group != NULL){
		ToRDF_Id payload = Group_get_id(document->group);
		err = handler(self.value, self.type, RIF_payload,
				payload.value, NULL, payload.type,
				NULL, BNODE, context);
		Group_send(document->group, handler, context);
	}
}

void Group_send(Group* group, TripleHandler* handler, void* context){
	if(group == NULL) return;
	int8_t err;
	ToRDF_Id self = Group_get_id(group);
	ToRDF_Id sentences = Clauselist_get_id(group->clauselist);
	err = handler(self.value, self.type, RDF_type,
			RIF_Group, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_sentences,
			sentences.value, NULL, sentences.type,
                	NULL, BNODE, context);
	Clauselist_send(group->clauselist, handler, context);
}

void Clauselist_send(Clauselist* list, TripleHandler* handler, void* context){
	if(list == NULL) return;
	int8_t err;
	ToRDF_Id self = Clauselist_get_id(list);
	ToRDF_Id first = Clause_get_id(list->first);
	ToRDF_Id rest = Clauselist_get_id(list->rest);
	err = handler(self.value, self.type, RDF_first,
			first.value, NULL, first.type,
                	NULL, BNODE, context);
	Clause_send(list->first, handler, context);
	err = handler(self.value, self.type, RDF_rest,
			rest.value, NULL, rest.type,
                	NULL, BNODE, context);
	Clauselist_send(list->rest, handler, context);
}

void Clause_send(Clause* clause, TripleHandler* handler, void* context){
	switch (clause->type){
		case CLAUSE_GROUP:
			return Group_send(clause->group, handler, context);
		case CLAUSE_ATOM:
			return Atom_send(clause->atom, handler, context);
		case CLAUSE_FRAME:
			return Frame_send(clause->frame, handler, context);
		case CLAUSE_FORALL:
			return Forall_send(clause->forall, handler, context);
		case CLAUSE_DO:
			return Do_send(clause->do_, handler, context);
		case CLAUSE_HORN_CLAUSE:
			return HornClause_send(clause->horn_clause, handler, context);
		case CLAUSE_IMPLIES:
			return Implies_send(clause->implies, handler, context);
	}
}

void Formula_send(Formula* formula, TripleHandler* handler, void* context){
	switch (formula->type){
		case FORMULA_ATOM:
			return Atom_send(formula->atom, handler, context);
		case FORMULA_FRAME:
			return Frame_send(formula->frame, handler, context);
		case FORMULA_MEMBER:
			return Member_send(formula->member, handler, context);
		case FORMULA_SUBCLASS:
			return Subclass_send(formula->subclass, handler, context);
		case FORMULA_EQUAL:
			return Equal_send(formula->equal, handler, context);
		case FORMULA_EXISTS:
			return Exists_send(formula->exists, handler, context);
		case FORMULA_INEG:
			return Ineg_send(formula->ineg, handler, context);
		case FORMULA_AND:
			return And_send(formula->and, handler, context);
		case FORMULA_OR:
			return Or_send(formula->or, handler, context);
		case FORMULA_EXTERNAL:
			return External_send(formula->external, handler, context);
		default:
			fprintf(stderr, "Formula_send failed\n");
	}
}

void Formulalist_send(Formulalist* list, TripleHandler* handler, void* context){
	if(list == NULL) return;
	int8_t err;
	ToRDF_Id self = Formulalist_get_id(list);
	ToRDF_Id first = Formula_get_id(list->first);
	ToRDF_Id rest = Formulalist_get_id(list->rest);
	err = handler(self.value, self.type, RDF_first,
			first.value, NULL, first.type,
                	NULL, BNODE, context);
	Formula_send(list->first, handler, context);
	err = handler(self.value, self.type, RDF_rest,
			rest.value, NULL, rest.type,
                	NULL, BNODE, context);
	Formulalist_send(list->rest, handler, context);
}

static void send_iri(Term* term, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Term_get_id(term);
	err = handler(self.value, self.type, RDF_type,
			RIF_Const, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_constIRI,
			term->value, XS_anyURI, TYPEDLITERAL,
                	NULL, BNODE, context);
}

static void send_langliteral(Term* term, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Term_get_id(term);
	err = handler(self.value, self.type, RDF_type,
			RIF_Const, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_value,
			term->value, term->suffix, LANGLITERAL,
                	NULL, BNODE, context);
}

static void send_typedliteral(Term* term, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Term_get_id(term);
	err = handler(self.value, self.type, RDF_type,
			RIF_Const, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_value,
			term->value, term->suffix, TYPEDLITERAL,
                	NULL, BNODE, context);
}

static void send_local(Term* term, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Term_get_id(term);
	err = handler(self.value, self.type, RDF_type,
			RIF_Const, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_constname,
			term->value, NULL, TYPEDLITERAL,
                	NULL, BNODE, context);
}

static void send_variable(Term* term, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Term_get_id(term);
	err = handler(self.value, self.type, RDF_type,
			RIF_Var, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_varname,
			term->value, NULL, TYPEDLITERAL,
                	NULL, BNODE, context);
}

static void send_list(Term* term, TripleHandler* handler, void* context){
	if(term == NULL) return;
	int8_t err;
	ToRDF_Id self = Term_get_id(term);
	ToRDF_Id list = Termlist_get_id(term->list);
	err = handler(self.value, self.type, RDF_type,
			RIF_List, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_items,
			list.value, NULL, list.type,
                	NULL, BNODE, context);
	Termlist_send(term->list, handler, context);
}

static void send_external_term(Term* term, TripleHandler* handler, void* context){
	if(term == NULL) return;
	int8_t err;
	ToRDF_Id self = Term_get_id(term);
	ToRDF_Id content = Expr_get_id(term->expr);
	err = handler(self.value, self.type, RDF_type,
			RIF_External, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_content,
			content.value, NULL, content.type,
                	NULL, BNODE, context);
	Expr_send(term->expr, handler, context);
}

void Var_send(Var* var, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Var_get_id(var);
	err = handler(self.value, self.type, RDF_type,
			RIF_Var, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_varname,
			var->name, NULL, TYPEDLITERAL,
                	NULL, BNODE, context);
}

void Term_send(Term* term, TripleHandler* handler, void* context){
	switch (term->type){
		case TERM_IRI:
			return send_iri(term, handler, context);
		case TERM_LANGLITERAL:
			return send_langliteral(term, handler, context);
		case TERM_TYPEDLITERAL:
			return send_typedliteral(term, handler, context);
		case TERM_LOCAL:
			return send_local(term, handler, context);
		case TERM_VARIABLE:
			return send_variable(term, handler, context);
		case TERM_LIST:
			return send_list(term, handler, context);
		case TERM_EXTERNAL:
			return send_external_term(term, handler, context);
		default:
			fprintf(stderr, "CRITICAL ERROR. Broken term\n");
			return;
	}
}


void Termlist_send(Termlist* list, TripleHandler* handler, void* context){
	if(list == NULL) return;
	int8_t err;
	ToRDF_Id self = Termlist_get_id(list);
	ToRDF_Id first = Term_get_id(list->first);
	ToRDF_Id rest = Termlist_get_id(list->rest);
	err = handler(self.value, self.type, RDF_first,
			first.value, NULL, first.type,
                	NULL, BNODE, context);
	Term_send(list->first, handler, context);
	err = handler(self.value, self.type, RDF_rest,
			rest.value, NULL, rest.type,
                	NULL, BNODE, context);
	Termlist_send(list->rest, handler, context);
}

void Slot_send(Slot* slot, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Slot_get_id(slot);
	ToRDF_Id key = Term_get_id(slot->key);
	ToRDF_Id value = Term_get_id(slot->value);
	err = handler(self.value, self.type, RDF_type,
			RIF_Slot, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_slotkey,
			key.value, NULL, key.type,
                	NULL, BNODE, context);
	Term_send(slot->key, handler, context);
	err = handler(self.value, self.type, RIF_slotvalue,
			value.value, NULL, value.type,
                	NULL, BNODE, context);
	Term_send(slot->value, handler, context);
}

void Varlist_send(Varlist* list, TripleHandler* handler, void* context){
	if(list == NULL) return;
	int8_t err;
	ToRDF_Id self = Varlist_get_id(list);
	ToRDF_Id first = Var_get_id(list->first);
	ToRDF_Id rest = Varlist_get_id(list->rest);
	err = handler(self.value, self.type, RDF_first,
			first.value, NULL, first.type,
                	NULL, BNODE, context);
	Var_send(list->first, handler, context);
	err = handler(self.value, self.type, RDF_rest,
			rest.value, NULL, rest.type,
                	NULL, BNODE, context);
	Varlist_send(list->rest, handler, context);
}

void Slotlist_send(Slotlist* list, TripleHandler* handler, void* context){
	if(list == NULL) return;
	int8_t err;
	ToRDF_Id self = Slotlist_get_id(list);
	ToRDF_Id first = Slot_get_id(list->first);
	ToRDF_Id rest = Slotlist_get_id(list->rest);
	err = handler(self.value, self.type, RDF_first,
			first.value, NULL, first.type,
                	NULL, BNODE, context);
	Slot_send(list->first, handler, context);
	err = handler(self.value, self.type, RDF_rest,
			rest.value, NULL, rest.type,
                	NULL, BNODE, context);
	Slotlist_send(list->rest, handler, context);
}

void Atom_send(Atom* atom, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id op, args;
	ToRDF_Id self = Atom_get_id(atom);
	err = handler(self.value, self.type, RDF_type,
			RIF_Atom, NULL, URI,
                	NULL, BNODE, context);
	op = Term_get_id(atom->op);
	err = handler(self.value, self.type, RIF_op,
			op.value, NULL, op.type,
                	NULL, BNODE, context);
	Term_send(atom->op, handler, context);
	if (atom->args != NULL){
		args = Termlist_get_id(atom->args);
		err = handler(self.value, self.type, RIF_args,
				args.value, NULL, args.type,
				NULL, BNODE, context);
		Termlist_send(atom->args, handler, context);
	}
}

void Expr_send(Expr* expr, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id op, args;
	ToRDF_Id self = Expr_get_id(expr);
	err = handler(self.value, self.type, RDF_type,
			RIF_Expr, NULL, URI,
                	NULL, BNODE, context);
	op = Term_get_id(expr->op);
	err = handler(self.value, self.type, RIF_op,
			op.value, NULL, op.type,
                	NULL, BNODE, context);
	Term_send(expr->op, handler, context);
	if (expr->args != NULL){
		args = Termlist_get_id(expr->args);
		err = handler(self.value, self.type, RIF_args,
				args.value, NULL, args.type,
				NULL, BNODE, context);
		Termlist_send(expr->args, handler, context);
	}
}

void Forall_send(Forall* forall, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Forall_get_id(forall);
	ToRDF_Id vars = Varlist_get_id(forall->varlist);
	ToRDF_Id clause = Clause_get_id(forall->clause);
	err = handler(self.value, self.type, RDF_type,
			RIF_Forall, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_vars,
			vars.value, NULL, vars.type,
                	NULL, BNODE, context);
	Varlist_send(forall->varlist, handler, context);
	err = handler(self.value, self.type, RIF_formula,
			clause.value, NULL, clause.type,
                	NULL, BNODE, context);
	Clause_send(forall->clause, handler, context);
	for(Formulalist* x=forall->formulalist; x!=NULL; x=x->rest){
		ToRDF_Id formula = Formula_get_id(x->first);
		err = handler(self.value, self.type, RIF_pattern,
				formula.value, NULL, formula.type,
				NULL, BNODE, context);
		Formula_send(x->first, handler, context);
	}
}

void Do_send(Do* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id varid;
	ToRDF_Id self = Do_get_id(x);
	err = handler(self.value, self.type, RDF_type,
			RIF_Do, NULL, URI,
                	NULL, BNODE, context);
	ToRDF_Id actionlist = Actionlist_get_id(x->actionlist);
	err = handler(self.value, self.type, RIF_actions,
			actionlist.value, NULL, actionlist.type,
                	NULL, BNODE, context);
	Actionlist_send(x->actionlist, handler, context);
	for(Varassignlist* v= x->varassignlist; v != NULL; v = v->rest){
		varid = Varassign_get_id(v->first);
		err = handler(self.value, self.type, RIF_actionVar,
				varid.value, NULL, varid.type,
				NULL, BNODE, context);
		Varassign_send(v->first, handler, context);
	}
}
void HornClause_send(HornClause* horn_clause, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = HornClause_get_id(horn_clause);
	ToRDF_Id trigger = Formula_get_id(horn_clause->condition);
	ToRDF_Id action = Formula_get_id(horn_clause->action);
	err = handler(self.value, self.type, RDF_type,
			RIF_Implies, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_if,
			trigger.value, NULL, trigger.type,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_then,
			action.value, NULL, action.type,
                	NULL, BNODE, context);
	Formula_send(horn_clause->condition, handler, context);
	Formula_send(horn_clause->action, handler, context);
}
void Implies_send(Implies* implies, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Implies_get_id(implies);
	ToRDF_Id trigger = Formula_get_id(implies->trigger);
	ToRDF_Id action = Clause_get_id(implies->action);
	err = handler(self.value, self.type, RDF_type,
			RIF_Implies, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_if,
			trigger.value, NULL, trigger.type,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_then,
			action.value, NULL, action.type,
                	NULL, BNODE, context);
	Formula_send(implies->trigger, handler, context);
	Clause_send(implies->action, handler, context);
}

void Action_send(Action* x, TripleHandler* handler, void* context){
	switch(x->type){
		case ACTION_ASSERT:
			return Assert_send(x->assert, handler, context);
		case ACTION_RETRACT:
			return Retract_send(x->retract, handler, context);
		case ACTION_MODIFY:
			return Modify_send(x->modify, handler, context);
		case ACTION_EXECUTE:
			return Execute_send(x->execute, handler, context);
	}
}

void Actionlist_send(Actionlist* list, TripleHandler* handler, void* context){
	if(list == NULL) return;
	int8_t err;
	ToRDF_Id self = Actionlist_get_id(list);
	ToRDF_Id first = Action_get_id(list->first);
	ToRDF_Id rest = Actionlist_get_id(list->rest);
	err = handler(self.value, self.type, RDF_first,
			first.value, NULL, first.type,
                	NULL, BNODE, context);
	Action_send(list->first, handler, context);
	err = handler(self.value, self.type, RDF_rest,
			rest.value, NULL, rest.type,
                	NULL, BNODE, context);
	Actionlist_send(list->rest, handler, context);
}

void Assert_send(Assert* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Assert_get_id(x);
	ToRDF_Id target;
	err = handler(self.value, self.type, RDF_type,
			RIF_Assert, NULL, URI,
			NULL, BNODE, context);
	switch(x->type){
		case ASSERT_ATOM:
			target = Atom_get_id(x->atom);
			Atom_send(x->atom, handler, context);
			break;
		case ASSERT_FRAME:
			target = Frame_get_id(x->frame);
			Frame_send(x->frame, handler, context);
			break;
		case ASSERT_MEMBER:
			target = Member_get_id(x->member);
			Member_send(x->member, handler, context);
			break;
	}
	err = handler(self.value, self.type, RIF_target,
			target.value, NULL, target.type,
                	NULL, BNODE, context);
}
void Retract_send(Retract* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id target;
	ToRDF_Id self = Retract_get_id(x);
	err = handler(self.value, self.type, RDF_type,
			RIF_Retract, NULL, URI,
			NULL, BNODE, context);
	switch(x->type){
		case RETRACT_ATOM:
			target = Atom_get_id(x->atom);
			Atom_send(x->atom, handler, context);
			break;
		case RETRACT_FRAME:
			target = Frame_get_id(x->frame);
			Frame_send(x->frame, handler, context);
			break;
		case RETRACT_OBJECT:
			target = Term_get_id(x->object);
			Term_send(x->object, handler, context);
			break;
		case RETRACT_OBJECT_PROPERTY:
			target = Termdublette_get_id(x->object_property);
			Termdublette_send(x->object_property, handler, context);
			break;
	}
	err = handler(self.value, self.type, RIF_target,
			target.value, NULL, target.type,
                	NULL, BNODE, context);
}
void Termdublette_send(Termdublette* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Termdublette_get_id(x);
	ToRDF_Id second = Termdublette_get_second_id(x);
	ToRDF_Id object = Term_get_id(x->object);
	ToRDF_Id property = Term_get_id(x->property);
	err = handler(self.value, self.type, RDF_rest,
			second.value, NULL, second.type,
			NULL, BNODE, context);
	err = handler(self.value, self.type, RDF_first,
			object.value, NULL, object.type,
			NULL, BNODE, context);
	err = handler(second.value, second.type, RDF_rest,
			RDF_nil, NULL, URI,
			NULL, BNODE, context);
	err = handler(second.value, second.type, RDF_first,
			property.value, NULL, property.type,
			NULL, BNODE, context);
	Term_send(x->object, handler, context);
	Term_send(x->property, handler, context);
}
void Modify_send(Modify* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id target;
	ToRDF_Id self = Modify_get_id(x);
	err = handler(self.value, self.type, RDF_type,
			RIF_Modify, NULL, URI,
			NULL, BNODE, context);
	target = Frame_get_id(x->frame);
	Frame_send(x->frame, handler, context);
	err = handler(self.value, self.type, RIF_target,
			target.value, NULL, target.type,
                	NULL, BNODE, context);
}
void Execute_send(Execute* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id target;
	ToRDF_Id self = Execute_get_id(x);
	err = handler(self.value, self.type, RDF_type,
			RIF_Execute, NULL, URI,
			NULL, BNODE, context);
	target = Atom_get_id(x->atom);
	Atom_send(x->atom, handler, context);
	err = handler(self.value, self.type, RIF_target,
			target.value, NULL, target.type,
                	NULL, BNODE, context);
}

void Varassign_send(Varassign* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id assign;
	ToRDF_Id self = Varassign_get_id(x);
	ToRDF_Id var = Var_get_id(x->var);
	ToRDF_Id second = Varassign_get_second_id(x);
	Var_send(x->var, handler, context);
	switch(x->type){
		case VARASSIGN_CREATE:
			assign = New_get_id(x->new);
			err = handler(assign.value, assign.type, RDF_type,
					RIF_New, NULL, URI,
					NULL, BNODE, context);
			break;
		case VARASSIGN_RETRIEVE:
			assign = Frame_get_id(x->frame);
			Frame_send(x->frame, handler, context);
			break;
	}
	err = handler(self.value, self.type, RDF_first,
			var.value, NULL, var.type,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RDF_rest,
			second.value, NULL, second.type,
                	NULL, BNODE, context);
	err = handler(second.value, second.type, RDF_first,
			assign.value, NULL, assign.type,
                	NULL, BNODE, context);
	err = handler(second.value, second.type, RDF_rest,
			RDF_nil, NULL, URI,
                	NULL, BNODE, context);
}


void Frame_send(Frame* frame, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Frame_get_id(frame);
	ToRDF_Id slots = Slotlist_get_id(frame->slots);
	ToRDF_Id object = Term_get_id(frame->object);
	err = handler(self.value, self.type, RDF_type,
			RIF_Frame, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_object,
			object.value, NULL, object.type,
                	NULL, BNODE, context);
	Term_send(frame->object, handler, context);
	err = handler(self.value, self.type, RIF_slots,
			slots.value, NULL, slots.type,
                	NULL, BNODE, context);
	Slotlist_send(frame->slots, handler, context);
}

void Member_send(Member* member, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Member_get_id(member);
	ToRDF_Id instance = Term_get_id(member->instance);
	ToRDF_Id class = Term_get_id(member->class);
	err = handler(self.value, self.type, RDF_type,
			RIF_Member, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_instance,
			instance.value, NULL, instance.type,
                	NULL, BNODE, context);
	Term_send(member->instance, handler, context);
	err = handler(self.value, self.type, RIF_class,
			class.value, NULL, class.type,
                	NULL, BNODE, context);
	Term_send(member->class, handler, context);
}

void Subclass_send(Subclass* subclass, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Subclass_get_id(subclass);
	ToRDF_Id sub = Term_get_id(subclass->sub);
	ToRDF_Id super = Term_get_id(subclass->super);
	err = handler(self.value, self.type, RDF_type,
			RIF_Subclass, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_sub,
			sub.value, NULL, sub.type,
                	NULL, BNODE, context);
	Term_send(subclass->sub, handler, context);
	err = handler(self.value, self.type, RIF_super,
			super.value, NULL, super.type,
                	NULL, BNODE, context);
	Term_send(subclass->super, handler, context);
}

void Exists_send(Exists* exists, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Exists_get_id(exists);
	ToRDF_Id vars = Varlist_get_id(exists->vars);
	ToRDF_Id formula = Formula_get_id(exists->formula);
	err = handler(self.value, self.type, RDF_type,
			RIF_Exists, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_vars,
			vars.value, NULL, vars.type,
                	NULL, BNODE, context);
	Varlist_send(exists->vars, handler, context);
	err = handler(self.value, self.type, RIF_formula,
			formula.value, NULL, formula.type,
                	NULL, BNODE, context);
	Formula_send(exists->formula, handler, context);
}

void Ineg_send(Ineg* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Ineg_get_id(x);
	ToRDF_Id formula = Formula_get_id(x->formula);
	err = handler(self.value, self.type, RDF_type,
			RIF_INeg, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_formula,
			formula.value, NULL, formula.type,
                	NULL, BNODE, context);
	Formula_send(x->formula, handler, context);
}

void And_send(And* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = And_get_id(x);
	ToRDF_Id formulas = Formulalist_get_id(x->formulalist);
	err = handler(self.value, self.type, RDF_type,
			RIF_And, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_formulas,
			formulas.value, NULL, formulas.type,
                	NULL, BNODE, context);
	Formulalist_send(x->formulalist, handler, context);
}
void Or_send(Or* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Or_get_id(x);
	ToRDF_Id formulas = Formulalist_get_id(x->formulalist);
	err = handler(self.value, self.type, RDF_type,
			RIF_Or, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_formulas,
			formulas.value, NULL, formulas.type,
                	NULL, BNODE, context);
	Formulalist_send(x->formulalist, handler, context);
}

void External_send(External* x, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = External_get_id(x);
	ToRDF_Id content = Atom_get_id(x->atom);
	err = handler(self.value, self.type, RDF_type,
			RIF_External, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_content,
			content.value, NULL, content.type,
                	NULL, BNODE, context);
	Atom_send(x->atom, handler, context);
}

void Equal_send(Equal* equal, TripleHandler* handler, void* context){
	int8_t err;
	ToRDF_Id self = Equal_get_id(equal);
	ToRDF_Id left = Term_get_id(equal->left);
	ToRDF_Id right = Term_get_id(equal->right);
	err = handler(self.value, self.type, RDF_type,
			RIF_Equal, NULL, URI,
                	NULL, BNODE, context);
	err = handler(self.value, self.type, RIF_left,
			left.value, NULL, left.type,
                	NULL, BNODE, context);
	Term_send(equal->left, handler, context);
	err = handler(self.value, self.type, RIF_right,
			right.value, NULL, right.type,
                	NULL, BNODE, context);
	Term_send(equal->right, handler, context);
}
