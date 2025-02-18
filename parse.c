#include "parse.h"

int parameter_count = 0;
char file_parameter[parameter_strings_long];
ParameterType parameter[parameter_array_size];

static void parseParameter(xmlDocPtr doc, xmlNode *cur_node) {
    char *element;
    char *must = NULL;
    
    if (parameter_count >= parameter_array_size) {
        fprintf(stderr, "Error: parameter_count exceeded limit\n");
        exit(1);
    }

    while (cur_node != NULL) {
        if (parameter[parameter_count].count >= variable_array_size) {
            fprintf(stderr, "Error: parameter[%d].count exceeded limit\n", parameter_count);
            exit(1);
        }

        if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"MUST"))) {
            must = ((char *)(xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1)));
            if (must) {
                if (strcmp("true", must) == 0) {
                    parameter[parameter_count].must = 1;
                } else if (strcmp("false", must) == 0) {
                    parameter[parameter_count].must = 0;
                }
                xmlFree(must);
            }
        } else if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"ELEMENT"))) {
            element = ((char *)(xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1)));

            if (element) {
                if (strlen(element) < parameter_strings_long) {
                    int p = parameter[parameter_count].count;
                    strncpy(parameter[parameter_count].parameter[p], element, parameter_strings_long - 1);
                    parameter[parameter_count].parameter[p][parameter_strings_long - 1] = '\0'; // added '0'
                } else {
                    fprintf(stderr, "Error: ELEMENT string too long\n");
                    exit(1);
                }
                xmlFree(element);
            }
            parameter[parameter_count].count++;
        }

        cur_node = cur_node->next;
    }
    parameter_count++;
}

static void parsefileParameter(xmlDocPtr doc, xmlNode *cur_node) {
    char *element;
    while (cur_node != NULL) {
        if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"ELEMENT"))) {
            element = ((char *)(xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1)));
            if (element) {
                if (strlen(element) < parameter_strings_long) {
                    strncpy(file_parameter, element, parameter_strings_long - 1);
                    file_parameter[parameter_strings_long - 1] = '\0'; // added '0'
                } else {
                    fprintf(stderr, "Error: file_parameter string too long\n");
                    exit(1);
                }
                xmlFree(element);
            }
        }
        cur_node = cur_node->next;
    }
}

static void parseRoot(xmlDocPtr doc, xmlNode *a_node) {
    a_node = a_node->children;
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (!(xmlStrcmp(cur_node->name, (const xmlChar *)"PARAMETER"))) {
            parseParameter(doc, cur_node->xmlChildrenNode);
        } else if (!(xmlStrcmp(cur_node->name, (const xmlChar *)"FILE"))) {
            parsefileParameter(doc, cur_node->xmlChildrenNode);
        }
    }
}

void print_parse_result() {
    printf("parameter:\n");
    for (int i = 0; i < parameter_count; i++) {
        printf("must = %d, count = %d\n", parameter[i].must, parameter[i].count);
        for (int j = 0; j < parameter[i].count; j++) {
            printf("%s\n", parameter[i].parameter[j]);
        }
    }
    printf("parameter count = %d\n", parameter_count);
    printf("file:\n%s\n", file_parameter);
}

void parse_xml(char *xml_posion) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
    doc = xmlReadFile(xml_posion, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Error: could not parse file %s\n", xml_posion);
        return;
    }
    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);
    parseRoot(doc, root_element);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    print_parse_result();
}
