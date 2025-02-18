#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define parameter_array_size 100
#define variable_array_size 50
#define parameter_strings_long 256

typedef struct {
    int must;
    int count;
    char parameter[variable_array_size][parameter_strings_long];
} ParameterType;

extern int parameter_count;
extern char file_parameter[parameter_strings_long];
extern ParameterType parameter[parameter_array_size];

void parse_xml(char *xml_posion);
void print_parse_result();

#endif //PARSE_H
