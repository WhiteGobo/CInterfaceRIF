/* CInterfaceRIF_parser.y
 * `https://www.w3.org/TR/2013/REC-rif-prd-20130205/#Presentation_syntax_.28Informative.29`_
 * `https://www.w3.org/TR/2013/REC-rif-dtb-20130205/#Shortcuts_for_Constants_in_RIF.27s_Presentation_Syntax`_
 */

%{
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include "memory.h"
#include "prefix.h"


int yyerror(const char *s);
int yylex(void);

void send_formulalist(Formulalist* formulalist);
void send_document(Document* document);
void register_prefix(const char* prefix_name, size_t prefix_len, const char* baseiri, size_t baseiri_len);
Const* expand_curie(const char* prefix, size_t prefix_len, const char* suffix, size_t suffix_len);
Meta* generate_meta_from_curie(const char* prefix, size_t prefix_len, const char* suffix, size_t suffix_len);

typedef struct rifpsParser {
} RIFPSParser;

%}

%union{
  Triplestring	triplestring;
  Document* vdocument;
  Group* vgroup;
  Term*	vterm;
  Termlist*	vtermlist;
  Uniterm*	vuniterm;
  Atom*	vatom;
  Frame* vframe;
  Member* vmember;
  Const*	vconst;
  Meta*	vmeta;
  Formula* vformula;
  Formulalist* vformulalist;
  Slot* vslot;
  Slotlist* vslotlist;
  Varlist* vvarlist;
  Clauselist* vclauselist;
  Clause* vclause;
  Actionlist* vactionlist;
  Action* vaction;
  Varassign* vvarassign;
  Varassignlist* vvarassignlist;
}

%start	statements

%token	<triplestring>	LANGED_UNICODESTRING_T SYMSPACE_TYPED_UNICODESTRING_T IRIREF_TYPED_UNICODESTRING_T CURIE_T UNICODESTRING_T LOCAL_T ANGLEBRACKIRI_T FLOAT_T INT_T VAR_T NAME_T
%token	LPAREN RPAREN LBRACKET RBRACKET COLON ARROW_T
%token	LPARENASTERIX RPARENASTERIX
%token	HASH_T DOUBLEHASH_T EQUAL_T AT_T LIST_T
%token	GROUP_T DOCUMENT_T FORALL_T SUCHTHAT_T EXISTS_T
%token	ASSERT_T RETRACT_T MODIFY_T EXECUTE_T BASE_T PREFIX_T
%token	IF_T THEN_T HORN_IMPLIES_T DO_T NEW_T AND_T OR_T NOT_T EXTERNAL_T

%type	<vdocument>	document
%type	<vgroup>	group
%type	<vclauselist>	clauselist
%type	<vclause>	clause actionblock
%type	<vterm>	term term_undescribed gterm gterm_undescribed
%type	<vtermlist>	termlist gtermlist
%type	<vuniterm>	uniterm guniterm
%type	<vformula>	undescribed_formula formula
%type	<vformula>	atomic atomic_undescribed
%type	<vconst>	const
%type	<vmeta>	irimeta
%type	<vformulalist>	formulalist atomiclist
%type	<vslot>	slot
%type	<vslotlist>	slotlist
%type	<vvarlist>	varlist
%type	<vatom>	atom
%type	<vframe>	frame
%type	<vmember>	member
%type	<vaction>	action
%type	<vactionlist>	actionlist
%type	<vvarassign>	varassign
%type	<vvarassignlist>	varassignlist


%%

statements:
	/* empty */
	| document {send_document($1);}
	| formulalist {send_formulalist($1);}
	;

document:
	DOCUMENT_T LPAREN prefixlist RPAREN {$$ = Document_new(NULL);}
	| DOCUMENT_T LPAREN prefixlist group RPAREN {$$ = Document_new($4);}
	;

prefixlist:
	/* empty */
	| prefix prefixlist
	;

prefix:
	PREFIX_T LPAREN NAME_T ANGLEBRACKIRI_T RPAREN {
		register_prefix($3.first, $3.len_first, $4.first, $4.len_first);
	};

group:
	irimeta GROUP_T LPAREN clauselist RPAREN {
		$$ = Group_new($4);
		Group_add_meta($$, $1);
	}
	| GROUP_T LPAREN clauselist RPAREN {$$ = Group_new($3);}
	;

clauselist:
	/* empty */ { $$ = NULL; }
	| clause clauselist {$$ = Clauselist_insert($2, $1);}
	;

actionblock:
	atom {$$ = Clause_from_atom($1);}
	| frame {$$ = Clause_from_frame($1);}
	//atomic {$$ = Formula_to_Clause($1);};
	   /*
	term LPAREN termlist RPAREN {
		$$ = Clause_from_atom(Atom_new($1, $3));
	}
	| term LBRACKET slotlist RBRACKET {
		$$ = Clause_new_frame($1, $3);
	}
	| irimeta term LPAREN termlist RPAREN {
		$$ = Clause_from_atom(Atom_new($2, $4));
	}
	| irimeta term LBRACKET slotlist RBRACKET {
		$$ = Clause_new_frame($2, $4);
	}
	;
*/
clause:
	group {$$ = Group_to_Clause($1);}
	| DO_T LPAREN varassignlist action actionlist RPAREN {
		//$5 = Actionlist_insert($5, $4);
		$$ = Clause_new_do($3, Actionlist_insert($5, $4));
	}
	| FORALL_T varlist SUCHTHAT_T formula formulalist LPAREN clause RPAREN {
		//$5 = Formulalist_insert($5, $4);
		$$ = Clause_new_forall($2, Formulalist_insert($5, $4), $7);
	}
	| FORALL_T varlist LPAREN clause RPAREN {
		$$ = Clause_new_forall($2, NULL, $4);
	}
	| IF_T formula THEN_T actionblock {
		$$ = Clause_new_prd_implies($2, $4);
	}
	| atomic HORN_IMPLIES_T formula {
		$$ = Clause_new_horn_implies($1, $3);
	}
	| AND_T LPAREN atomiclist RPAREN HORN_IMPLIES_T formula {
		$$ = Clause_new_horn_implies(Formula_new_and($3), $6);
	}
	| actionblock {$$ = $1;}
	;

atomiclist:
	/* empty */ {$$=NULL;}
	| atomic atomiclist {$$ = Formulalist_insert($2, $1);}
	;

atomic:
	atomic_undescribed {
		$$ = $1;
	}
	| irimeta atomic_undescribed {
		Formula_add_meta($2, $1);
		$$ = $2;
	};

atomic_undescribed:
	atom {$$ = Formula_from_atom($1);}
	| frame {$$ = Formula_from_frame($1);}
	//term LPAREN termlist RPAREN {
	//	$$ = Formula_from_atom(Atom_new($1, $3));
	//}
	//| term LBRACKET slot slotlist RBRACKET {
	//	//$4 = Slotlist_insert($4, $3);
	//	$$ = Formula_new_frame($1, Slotlist_insert($4, $3));
	//}
	| term HASH_T term {
		$$ = Formula_new_member($1, $3);
	}
	| term DOUBLEHASH_T term {
		$$ = Formula_new_subclass($1, $3);
	}
	| term EQUAL_T term {
		$$ = Formula_new_equal($1, $3);
	}

varassignlist:
	/* empty */ {$$ = NULL;}
	| varassign varassignlist {$$ = Varassignlist_insert($2, $1);}
	;

varassign:
	LPAREN VAR_T NEW_T RPAREN {
		$$ = Varassign_new_create($2.first, $2.len_first);
	}
	| LPAREN VAR_T frame RPAREN {
		$$ = Varassign_new_retrieve($2.first, $2.len_first, $3);
	}
	;

actionlist:
	/* empty */ {$$ = NULL;}
	| action actionlist {$$ = Actionlist_insert($2, $1);}
	;

action:
	ASSERT_T LPAREN atom RPAREN {$$ = Action_new_assert_atom($3);}
	| ASSERT_T LPAREN frame RPAREN {$$ = Action_new_assert_frame($3);}
	| ASSERT_T LPAREN member RPAREN {$$ = Action_new_assert_member($3);}
	| RETRACT_T LPAREN atom RPAREN {$$ = Action_new_retract_atom($3);}
	| RETRACT_T LPAREN frame RPAREN {$$ = Action_new_retract_frame($3);}
	| RETRACT_T LPAREN term RPAREN {$$ = Action_new_retract_object($3);}
	| RETRACT_T LPAREN term term RPAREN {
		$$ = Action_new_retract_object_property($3, $4);
	}
	| MODIFY_T LPAREN frame RPAREN {$$ = Action_new_modify_frame($3);}
	| EXECUTE_T LPAREN atom RPAREN {$$ = Action_new_execute_atom($3);}
	;
	  

formulalist:
	/* empty */ {$$ = NULL;}
	| formula formulalist {$$ = Formulalist_insert($2, $1);}
	;

formula:
	undescribed_formula {$$ = $1;}
	| irimeta undescribed_formula {
		Formula_add_meta($2, $1);
		$$ = $2;
	};

undescribed_formula:
	EXISTS_T varlist LPAREN formula RPAREN {
		$$ = Formula_new_exists($2, $4);
	}
	| atomic_undescribed {$$ = $1;}
/*
	| term LPAREN termlist RPAREN {
		$$ = Formula_from_atom(Atom_new($1, $3));
	}
	| term LBRACKET slot slotlist RBRACKET {
		$4 = Slotlist_insert($4, $3);
		$$ = Formula_new_frame($1, $4);
	}
	| term HASH_T term {
		$$ = Formula_new_member($1, $3);
	}
	| term DOUBLEHASH_T term {
		$$ = Formula_new_subclass($1, $3);
	}
	| term EQUAL_T term {
		$$ = Formula_new_equal($1, $3);
	}
*/
	| NOT_T LPAREN formula RPAREN {
		$$ = Formula_new_ineg($3);
	}
	| AND_T LPAREN formulalist RPAREN {
		$$ = Formula_new_and($3);
	}
	| OR_T LPAREN formulalist RPAREN {
		$$ = Formula_new_or($3);
	}
	| EXTERNAL_T LPAREN uniterm RPAREN {
		$$ = Formula_new_external(Uniterm_to_Atom($3));
	}
	;

atom:
	uniterm {$$ = Uniterm_to_Atom($1);}
	| irimeta uniterm {$$ = Uniterm_to_Atom($2);}
	;

varlist:
	/* empty */ {$$ = NULL;}
	| VAR_T varlist {
		$$ = Varlist_insert($2, Var_new($1.first, $1.len_first));
	}
	;

frame:
	term LBRACKET slotlist RBRACKET {
		$$ = Frame_new($1, $3);
	};


member:
	term HASH_T term {
		$$ = Member_new($1, $3);
	};

slotlist:
	/* empty */ {$$ = NULL;}
	| slot slotlist {$$ = Slotlist_insert($2, $1);}
	;
slot:
	term ARROW_T term {$$ = Slot_new($1, $3);}
	;

termlist:
	/* empty */ {$$ = NULL;}
	| term termlist {$$ = Termlist_insert($2, $1);}
	;

term:
	term_undescribed {$$ = $1;}
	| irimeta term_undescribed {
    		Term_add_meta($2, $1);
    		$$ = $2;
    	}
    	;

term_undescribed:
	const {$$ = Const_to_Term($1);}
	| LIST_T LPAREN gtermlist RPAREN {$$ = Term_new_list($3);}
	| VAR_T {$$ = Term_new_var($1.first, $1.len_first);}
	| EXTERNAL_T LPAREN uniterm RPAREN {
		$$ = Term_new_external(Uniterm_to_Expr($3));
	}
	;

gterm:
	irimeta gterm_undescribed {
    		Term_add_meta($2, $1);
    		$$ = $2;
    	}
	| gterm_undescribed {$$ = $1;}
    	;

gterm_undescribed:
	const {$$ = Const_to_Term($1);}
	| LIST_T LPAREN gtermlist RPAREN {$$ = Term_new_list($3);}
	| EXTERNAL_T LPAREN guniterm RPAREN {
		$$ = Term_new_external(Uniterm_to_Expr($3));
	}
	;

gtermlist:
	/* empty */ {$$ = NULL;}
	| gterm gtermlist {$$ = Termlist_insert($2, $1);}
	;

const:
	ANGLEBRACKIRI_T {$$ = Const_new_IRI($1.first, $1.len_first);}
	| CURIE_T {
		$$ = expand_curie($1.first, $1.len_first,
					$1.second, $1.len_second);
		if ($$ == NULL) yyerror("couldnt expand with prefix");
	}
	| LANGED_UNICODESTRING_T {$$ = Const_new_LangLiteral($1.first, $1.len_first, $1.second, $1.len_second);}
	| IRIREF_TYPED_UNICODESTRING_T {$$ = Const_new_TypedLiteral($1.first, $1.len_first, $1.second, $1.len_second);}
	| SYMSPACE_TYPED_UNICODESTRING_T {$$ = Const_new_TypedLiteral($1.first, $1.len_first, $1.second, $1.len_second);}
	| UNICODESTRING_T {
		$$ = Const_new_Literal($1.first, $1.len_first);
	}
	| LOCAL_T {$$ = Const_new_local($1.first, $1.len_first);}
	| FLOAT_T {$$ = Const_new_float($1.first, $1.len_first);}
	| INT_T {$$ = Const_new_int($1.first, $1.len_first);}
	;

uniterm:
	const LPAREN termlist RPAREN {
		$$ = Uniterm_new($1, $3);
	};
guniterm:
	const LPAREN gtermlist RPAREN {
		$$ = Uniterm_new($1, $3);
	};

irimeta:
	LPARENASTERIX RPARENASTERIX {$$=NULL;}
	| LPARENASTERIX ANGLEBRACKIRI_T RPARENASTERIX {
		$$ = Meta_new_iri($2.first, $2.len_first);
	}
	| LPARENASTERIX CURIE_T RPARENASTERIX {
		$$ = generate_meta_from_curie($2.first, $2.len_first, $2.second, $2.len_second);
	}
	;

%%

#include "to_rdf.h"

static RIFPSParser* config;

static PrefixMap* prefix_map;

static TripleHandler* my_triplehandler;
static void* my_triplehandler_context;

void send_document(Document* document){
	Document_send(document, my_triplehandler, my_triplehandler_context);
}

void send_formulalist(Formulalist* formulalist){
	fprintf(stderr, "found formulas:\n");
	for(Formulalist* x=formulalist; x!=NULL; x=x->rest){
		Formula_send(x->first,
			my_triplehandler, my_triplehandler_context);
		fprintf_debug_formula(stderr, x->first);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}

int yyerror(const char *s)
{
	extern int yylineno;	// defined and maintained in lex.c
	extern char *yytext;	// defined and maintained in lex.c
	fprintf(stderr, "ERROR at symbol %s on line %d with \"%s\"\n",
			yytext, yylineno, s);
	return 1;
}

int64_t parse_rifps(const char *input, TripleHandler hook, void* hook_data, RIFPSParser* config)
{
	int err;
	extern FILE* yyin;
	FILE* myfile;
	if (hook == NULL) return 1;
	myfile = fmemopen((void*) input, strlen(input), "r");
	if (myfile == NULL) return 1;
	my_triplehandler = hook;
	my_triplehandler_context = hook_data;
	prefix_map = PrefixMap_new();
	//TODO: copy prefixes given with config
	yyin = myfile;
	fprintf(stderr, "start parsing\n");
	err = yyparse();
	//nquads_scanner_tidy_up();
	fclose(myfile);
	free_PrefixMap(prefix_map);

	prefix_map = NULL;
	my_triplehandler = NULL;
	my_triplehandler_context = NULL;

	fprintf(stderr, "end parsing with %d\n", err);
	return err;
}


void register_prefix(const char* prefix_name, size_t prefix_len,
			const char* baseiri, size_t baseiri_len)
{
	PrefixMap_register_prefix(prefix_map,
					prefix_name, prefix_len,
					baseiri, baseiri_len);
}


Const* expand_curie(const char* prefix, size_t prefix_len,
			const char* suffix, size_t suffix_len)
{
	Const* ret = NULL;
	char* tmp = PrefixMap_expand_curie(prefix_map,
					prefix, prefix_len,
					suffix, suffix_len);
	if (tmp != NULL){
		ret = Const_new_IRI(tmp, strlen(tmp));
		free(tmp);
	}
	return ret;
}

Meta* generate_meta_from_curie(const char* prefix, size_t prefix_len, const char* suffix, size_t suffix_len)
{
	Meta* ret = NULL;
	char* tmp = PrefixMap_expand_curie(prefix_map,
					prefix, prefix_len,
					suffix, suffix_len);
	if (tmp != NULL){
		ret = Meta_new_iri(tmp, strlen(tmp));
		free(tmp);
	}
	return ret;
}

RIFPSParser* RIFPSParser_new(){
	RIFPSParser* ret = malloc(sizeof(RIFPSParser));
	return ret;
}

void free_RIFPSParser(RIFPSParser* config){
	if(config == NULL) return;
	free(config);
}

