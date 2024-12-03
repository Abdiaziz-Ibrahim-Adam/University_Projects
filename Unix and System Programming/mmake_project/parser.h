/**
 * This module is used to parse a minimal makefile. The module is provided to 
 * the student as part of the mmake assignment in the course C Programming and 
 * Unix (5DV088).
 *
 * @file parse.h
 * @author Elias Åström, Fredrik Peteri
 * @date 2020-09-04
 * @author Isak Öhman, Linus Svedberg, Jonny Pettersson
 * @date 2024-09-19
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

typedef struct makefile makefile;
typedef struct rule rule;


/**
 * Parse a makefile. The function allocates memory for a structure of the type 
 * makefile. The structure will contain all the rules in the makefile. If there
 * is no rule in the makefile then the function returns NULL and no memory for 
 * the structure is allocated. The caller of this function is responsible to 
 * deallocate the memory by using the function makefile_del.
 *
 * @param fp    The file to parse.
 * @return      A pointer to a structure of the type makefile.
 */
makefile *parse_makefile(FILE *fp);


/**
 * Returns a pointer to the name of the default target for a makefile. (The 
 * default target is the target for the first rule.)
 *
 * @param make  A pointer to a structue of type makefile.
 * @return      A pointer to the name of the default target for a makefile.
 */
const char *makefile_default_target(makefile *make);


/**
 * Returns a pointer to the rule for building a specific target in a makefile. 
 * If a rule for the target can not be found, NULL is returned.
 *
 * @param make      A pointer to a structue of type makefile.
 * @param target    A pointer to the name of the target.
 * @return          A pointer to the rule for building the target.
 */
rule *makefile_rule(makefile *make, const char *target);


/**
 * Returns a pointer to an array containing the prerequisites for the rule. The 
 * array is terminated with NULL.
 *
 * @param rule  A pointer to the rule.
 * @return      A pointer to the array containing the prerequisites for the 
 *              rule.  
 */
const char **rule_prereq(rule *rule);


/**
 * Returns a pointer to an array containing a command, and its arguments, used 
 * to build the rule. The first argument is the name of the command. The array 
 * is terminated with NULL.
 *
 * @param rule  A pointer to the rule.
 * @return      A pointer to an array containing a command, and its arguments.
 */
char **rule_cmd(rule *rule);


/**
 * Free the memory of a structure of the type makefile. This will also 
 * deallocate the memory for rules returned by makefile_rule.
 *
 * @param make  A pointer to the structue of type makefile to be deallocated.
 */
void makefile_del(makefile *make);

#endif
