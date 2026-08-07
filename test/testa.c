#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "CInterfaceRIF.h"

#include "BasicRDFGraphComparator.h"
#include <NQuadsRDF.h>

static char *premisefile = NULL;
static char *conclusionfile = NULL;

static int parse_args(int argc, char *argv[]);
static int compare_data();
static char* load_input_into_memory(const char* filepath);
static int8_t my_handler(
                const char* subject, uint8_t subject_type,
                const char* predicate,
                const char* object, const char* object_suffix,
                uint8_t object_type,
                const char* graphid, uint8_t graph_type,
		TripleStream *rif_triples);

int main(int argc, char *argv[]){
	int err;
	err = parse_args(argc, argv);
	if (err != 0){
		exit(EXIT_FAILURE);
	}
	err = compare_data();
	switch(err){
		case 0 :
			exit(EXIT_SUCCESS);
		default:
			exit(EXIT_FAILURE);
	}
}

static struct option parse_options[] = {
	{"premise", required_argument, NULL, 'i'},
	{"conclusion", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0}
};

static int parse_args(int argc, char *argv[]){
	int err = 0;
	int c = 0;
	int option_index;
	while(c != -1){
		c = getopt_long(argc, argv, "",
				parse_options, &option_index);
		switch(c){
			case -1: //end of arguments
				break;
			case 'i':
				premisefile = optarg;
				break;
			case 'o':
				conclusionfile = optarg;
				break;
			default:
				fprintf(stderr, "unrecognized argument\n");
				err = 1;
				break;
		}
	}
	if (premisefile == NULL){
		fprintf(stderr, "no premisepath given\n");
		err = 1;
	}
	if (conclusionfile == NULL){
		fprintf(stderr, "no conclusionpath given\n");
		err = 1;
	}
	return err;
}


static int compare_data()
{
	int err;
	bool are_same_graphs;
	char* premise;
	RIFPSParser* rifparser = NULL;
	TripleStream *rif_triples = new_TripleStream();
	TripleStream *check_triples = new_TripleStream();

	premise = load_input_into_memory(premisefile);
	if (premise == NULL) return 1;
	err = parse_rifps(premise,
			//(TripleHandler*) append_TripleStream, rif_triples,
			(TripleHandler*)my_handler, rif_triples,
			rifparser);
	free_RIFPSParser(rifparser);
	free(premise);
	if (err != 0){
		fprintf(stderr, "Failed to parse rifps file: %s\n",
				premisefile);
		return 1;
	}

	err = nquads_parse_file(conclusionfile,
			(TripleHandler*) append_TripleStream, check_triples);
	if (err != 0){
		fprintf(stderr, "Failed to parse nquads file: %s\n",
				conclusionfile);
		return 1;
	}

	are_same_graphs = compare_triples(rif_triples, check_triples);
	free_TripleStream(rif_triples);
	free_TripleStream(check_triples);
	if (are_same_graphs){
		return 0;
	} else {
		fprintf(stderr, "Not the same graphs.\n");
		return 1;
	}
}

static char* load_input_into_memory(const char* filepath){
	char *ret;
	long fsize;
	FILE *f = fopen(filepath, "rb");
	if (f == NULL){
		fprintf(stderr, "Failed to open file %s\n", filepath);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	rewind(f);
	//fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

	ret = malloc(fsize + 1);
	fread(ret, fsize, 1, f);
	ret[fsize] = 0;
	fclose(f);
	return ret;
}


static void fprintf_rdfterm(FILE* f, const char* value, const char* suffix, uint8_t type){
	switch(type){
		case 0:
			fprintf(f, "<%s>", value);
			break;
		case 1:
			fprintf(f, "_:%s", value);
			break;
		case 2:
			if (suffix != NULL){
				fprintf(f, "\"%s\"^^<%s>", value, suffix);
			} else {
				fprintf(f, "\"%s\"", value);
			}
			break;
		case 3:
			fprintf(f, "\"%s\"@@%s", value, suffix);
			break;
	}
}

static int8_t my_handler(
                const char* subject, uint8_t subject_type,
                const char* predicate,
                const char* object, const char* object_suffix,
                uint8_t object_type,
                const char* graphid, uint8_t graph_type,
		TripleStream *rif_triples)
{
	fprintf_rdfterm(stderr, subject, NULL, subject_type);
	fprintf(stderr, " ");
	fprintf_rdfterm(stderr, predicate, NULL, 0);
	fprintf(stderr, " ");
	fprintf_rdfterm(stderr, object, object_suffix, object_type);
	fprintf(stderr, ".\n");
	append_TripleStream(subject, subject_type, predicate, object, object_suffix, object_type, graphid, graph_type, rif_triples);
	return 0;
}
