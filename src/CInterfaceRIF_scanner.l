
%Start doublequotedstring singlequotedstring iriref_type_literal symspace_prefix_type_literal symspace_suffix_type_literal local lang_literal PREFIX curiesuffix
%{
#include "memory.h"
#include "CInterfaceRIF_parser.h"

__attribute__((visibility("hidden")))
int yyerror(const char *s);
char* gen_string(const char*, size_t len);

%}

float [0-9]*[.][0-9]+
floatlike [0-9]+[.]
digit [0-9]
whitespace [ \t\n]+

irirefval [^\x00-\x20<>"{}|^`\\]+
anglebrackiri (<[^\x00-\x20<>"{}|^`\\]+>)
prefix (Prefix)
singlequotedstringval ([^\x27\x5C\xA\xD])+
doublequotedstringval ([^\x22\x5C\xA\xD])+
pname ([A-Za-z][A-Za-z0-9]*)
curieprefix ([A-Za-z][A-Za-z0-9]*\:)
variable ([?][A-Za-z][A-Za-z0-9]*)
suchthat (such[ \t]+that)
curie ([A-Za-z][A-Za-z0-9]*)\:(<[^\x00-\x20<>"{}|^`\\]+>)


%%
<INITIAL>{prefix} {
	BEGIN(PREFIX);
	return PREFIX_T;
}
<PREFIX>"(" { return LPAREN; }
<PREFIX>{pname} {
	yylval.triplestring.first = yytext;
	yylval.triplestring.len_first = yyleng;
	return NAME_T;
}
<PREFIX>{anglebrackiri} {
	yylval.triplestring.first = yytext + 1;
	yylval.triplestring.len_first = yyleng - 2;
	return ANGLEBRACKIRI_T;
}
<PREFIX>")" {
	BEGIN(INITIAL);
	return RPAREN;
}
<INITIAL>"Exists" { return EXISTS_T;}
<INITIAL>"Document" { return DOCUMENT_T;}
<INITIAL>"Forall" { return FORALL_T;}
<INITIAL>{suchthat} { return SUCHTHAT_T;}
<INITIAL>"Group" { return GROUP_T;}
<INITIAL>"Assert" { return ASSERT_T;}
<INITIAL>"Retract" { return RETRACT_T;}
<INITIAL>"Modify" { return MODIFY_T;}
<INITIAL>"Execute" { return EXECUTE_T;}
<INITIAL>"Base" { return BASE_T;}
<INITIAL>":-" { return HORN_IMPLIES_T;}
<INITIAL>"If" { return IF_T;}
<INITIAL>"Then" { return THEN_T;}
<INITIAL>"Do" { return DO_T;}
<INITIAL>"New()" { return NEW_T;}
<INITIAL>"And" { return AND_T;}
<INITIAL>"Or" { return OR_T;}
<INITIAL>"List" { return LIST_T;}
<INITIAL>"Not" { return NOT_T;}
<INITIAL>"INeg" { return NOT_T;}
<INITIAL>"External" { return EXTERNAL_T;}


<INITIAL>{anglebrackiri} {
	yylval.triplestring.first = yytext + 1;
	yylval.triplestring.len_first = yyleng - 2;
	return ANGLEBRACKIRI_T;
}
<INITIAL>{curieprefix} {
	yylval.triplestring.first = yytext;
	yylval.triplestring.len_first = yyleng - 1;
	BEGIN(curiesuffix);
}
<curiesuffix>"\n" {
	yylval.triplestring.second = NULL;
	yylval.triplestring.len_second = 0;
	BEGIN(INITIAL);
	yylineno+=1;
	return CURIE_T;
}
<curiesuffix>{whitespace} {
	yylval.triplestring.second = NULL;
	yylval.triplestring.len_second = 0;
	BEGIN(INITIAL);
	return CURIE_T;
}
<curiesuffix>{pname} {
	yylval.triplestring.second = yytext;
	yylval.triplestring.len_second = yyleng;
	BEGIN(INITIAL);
	return CURIE_T;
}
<INITIAL>{float} {
	yylval.triplestring.first = yytext;
	yylval.triplestring.len_first = yyleng;
	return FLOAT_T;
}
<INITIAL>{floatlike} {
	yylval.triplestring.first = yytext;
	yylval.triplestring.len_first = yyleng;
	return FLOAT_T;
}
<INITIAL>{digit} {
	yylval.triplestring.first = yytext;
	yylval.triplestring.len_first = yyleng;
	return INT_T;
}
<INITIAL>"_" {
	yylval.triplestring.first = yytext + 1;
	BEGIN(local);
}
<local>{pname} {
	yylval.triplestring.len_first = yyleng;
	BEGIN(INITIAL);
	return LOCAL_T;
}
<INITIAL>{variable} {
	yylval.triplestring.first = yytext + 1;
	yylval.triplestring.len_first = yyleng - 1;
	return VAR_T;
}
<INITIAL>"\"" {
	yylval.triplestring.first = yytext + 1;
	yylval.triplestring.len_first = 0;
	BEGIN(doublequotedstring);
}
<doublequotedstring>{doublequotedstringval} {
	yylval.triplestring.len_first += yyleng;
}
<INITIAL>"\'" {
	yylval.triplestring.first = yytext + 1;
	yylval.triplestring.len_first = 0;
	BEGIN(singlequotedstring);
}
<singlequotedstring>{singlequotedstringval} {
	yylval.triplestring.len_first += yyleng;
}
<doublequotedstring>"\"^^<" {
	yylval.triplestring.second = yytext + 4;
	BEGIN(iriref_type_literal);
}
<singlequotedstring>"\'^^<" {
	yylval.triplestring.len_first = yytext - yylval.triplestring.first;
	yylval.triplestring.second = yytext + 4;
	BEGIN(iriref_type_literal);
}
<doublequotedstring>"\"^^[A-Za-z]" {
	yylval.triplestring.len_first = yytext - yylval.triplestring.first;
	yylval.triplestring.second = yytext + 3;
	BEGIN(symspace_prefix_type_literal);
}
<singlequotedstring>"\'^^[A-Za-Z]" {
	yylval.triplestring.len_first = yytext - yylval.triplestring.first;
	yylval.triplestring.second = yytext + 3;
	BEGIN(symspace_prefix_type_literal);
}
<symspace_prefix_type_literal>{pname} {
	yylval.triplestring.len_first += yyleng;
}
<symspace_prefix_type_literal>":" {
	yylval.triplestring.len_second = yytext - yylval.triplestring.second;
	yylval.triplestring.third = yytext + 1;
	BEGIN(symspace_suffix_type_literal);
}
<symspace_suffix_type_literal>{pname} {
	yylval.triplestring.len_third = yyleng;
	BEGIN(INITIAL);
	return SYMSPACE_TYPED_UNICODESTRING_T;
}
<iriref_type_literal>{irirefval} {}
<iriref_type_literal>{whitespace} {
	yyerror("Whitespace in IRI");
	yyterminate();
}
<iriref_type_literal>">" {
	yylval.triplestring.len_second = yytext - yylval.triplestring.second;
	BEGIN(INITIAL);
	return IRIREF_TYPED_UNICODESTRING_T;
}
<doublequotedstring>"\"@" {
	yylval.triplestring.len_first = yytext - yylval.triplestring.first;
	yylval.triplestring.second = yytext + 2;
	BEGIN(lang_literal);
}
<singlequotedstring>"\'@" {
	yylval.triplestring.len_first = yytext - yylval.triplestring.first;
	yylval.triplestring.second = yytext + 2;
	BEGIN(lang_literal);
}
<lang_literal>{pname} {
	yylval.triplestring.len_second = yyleng;
	BEGIN(INITIAL);
	return LANGED_UNICODESTRING_T;
}

<doublequotedstring>"\"" {
	yylval.triplestring.len_first = yytext - yylval.triplestring.first;
	BEGIN(INITIAL);
	return UNICODESTRING_T;
}
<singlequotedstring>"\'" {
	yylval.triplestring.len_first = yytext - yylval.triplestring.first;
	BEGIN(INITIAL);
	return UNICODESTRING_T;
}
<INITIAL>"->" {return ARROW_T;}
<INITIAL>"(" {return LPAREN;}
<INITIAL>")" {return RPAREN;}
<INITIAL>"(*" {return LPARENASTERIX;}
<INITIAL>"*)" {return RPARENASTERIX;}
<INITIAL>"[" {return LBRACKET;}
<INITIAL>"]" {return RBRACKET;}
<INITIAL>":" {return COLON;}
<INITIAL>"##" {return DOUBLEHASH_T;}
<INITIAL>"#" {return HASH_T;}
<INITIAL>"@" {return AT_T;}
<INITIAL>"=" {return EQUAL_T;}

"\n" {yylineno+=1;}

%%

int yywrap() { /* need this to avoid link problem */
	return 1;
}

char* gen_string(const char* input, size_t length){
	//ReleaseList* tmp = malloc(sizeof(ReleaseList));
	char* ret = malloc(length+1);
	memcpy(ret, input, length);
	ret[length] = '\0';
	//tmp->item = ret;
	//tmp->next = release_list;
	//release_list = tmp;
	return ret;
}

void qqwe() {
    int token;
    do {
        token = yylex();
        switch (token) {
            case ANGLEBRACKIRI_T:
                fprintf(stderr, "anglebrackiri found\n");
                break;
            case LPAREN:
            case RPAREN:
		fprintf(stderr, "parenthihs\n");
		break;
            case -1:
		fprintf(stderr, "brubru\n");
		return;
	    case 0:
		break;
	    default:
		fprintf(stderr, "unhandled token%d\n", token);
		break;
        }
    } while (token != 0);
}
