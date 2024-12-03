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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"


/* ------------------------------- Constants ------------------------------- */

#define MAX_RULES 256
#define MAX_LINE 1024
#define MAX_PREREQ 32
#define MAX_CMD 32

/* ------------------------------ Structures ------------------------------- */

struct makefile {
	struct rule *rules;
};

struct rule {
	char *target;
	char **prereq;
	char **cmd;
	rule *next;
};


/* ------------------ Declarations of internal functions ------------------ */

static rule *parse_rule(FILE *fp, bool *err);
static char *extract_target(char **p, char *buf, FILE *fp, bool *err);
static char *parse_prereqs(char **p, char **prereq, size_t *n_prereq);
static char *advance_until_cmd(char *buf, FILE *fp);
static size_t parse_cmd(char **cmd, char **p);
static rule *create_rule(char *target, char **prereq, char **cmd);
static char **dupe_str_array(size_t n, char **a);
static char *next_line(char buf[MAX_LINE], FILE *fp);
static char *parse_word(char **p, char *delim);
static void skipwhite(char **p);
static bool expect(char **p, char c);
static bool is_blank_line(const char *s);
static void free_arr(char **arr);
static void del_rules(struct rule *rules);
static void err0(bool *err);
static void err1(char *target, bool *err);
static void err2(char *prereq[], size_t n_prereq, char *target, bool *err);


/* -------------------------- External functions -------------------------- */

makefile *parse_makefile(FILE *fp)
{
	makefile *m = malloc(sizeof *m);
	rule **tailp = &m->rules;

	bool err = false;
	while ((*tailp = parse_rule(fp, &err)) != NULL) {
		tailp = &(*tailp)->next;
	}
	*tailp = NULL;

	if (m->rules == NULL || err) {
		makefile_del(m);
		return NULL;
	}

	return m;
}


const char *makefile_default_target(makefile *m)
{
	return m->rules->target;
}


rule *makefile_rule(makefile *m, const char *target)
{
	rule *i = m->rules;
	while (i != NULL){
		if (strcmp(i->target, target) == 0) {
			return i;
		}
		i = i->next;
	}

	return NULL;
}


const char **rule_prereq(rule *rule)
{
	return (const char **)rule->prereq;
}


char **rule_cmd(rule *rule)
{
	return rule->cmd;
}


void makefile_del(makefile *make)
{
	del_rules(make->rules);
	free(make);
}


/* -------------------------- Internal functions -------------------------- */

/**
 * Parse a rule.
 *
 * @param fp    File to read from.
 * @param err   Pointer to flag which gets set to true on error.
 * @return      A parsed rule or NULL.
 */
static rule *parse_rule(FILE *fp, bool *err)
{
	// Buffer variables
	char buf[MAX_LINE];
	char *p;

	// Variables to fill
	char *prereq[MAX_PREREQ];
	size_t n_prereq;
	char *cmd[MAX_CMD];
	
	char *target = extract_target(&p, buf, fp, err);
	if (target == NULL) {
		return NULL;
	}

	p = parse_prereqs(&p, prereq, &n_prereq);
	if(p == NULL)
	{
		err2(prereq, n_prereq, target, err);
		return NULL;
	}

	p = advance_until_cmd(buf, fp);
	if(p == NULL)
	{
		err2(prereq, n_prereq, target, err);
		return NULL;
	}

	size_t n_words = parse_cmd(cmd, &p);

	rule *r = create_rule(target, dupe_str_array(n_prereq, prereq), 
	                      dupe_str_array(n_words, cmd));

	return r;
}


/**
 * Extract target from next line in fp, updates p to point to the first 
 * non-blank character after ':' in buf. Also reads in a full line to buf from fp.
 * 
 * @param p   Pointer that keeps info about the current place in line.
 * @param buf Buffer that should be filled with one line from fp.
 * @param fp  File pointer from where the next line should be red.
 * @param err Pointer to bool that keeps track if error occured.
 * @return    Target if line is as expected, NULL if error.
*/
static char *extract_target(char **p, char *buf, FILE *fp, bool *err)
{
	// read line with target and prerequisites
	if ((*p = next_line(buf, fp)) == NULL) {
		return NULL;
	}
	
	// line cannot begin with whitespace
	if (isspace(**p))
	{
		err0(err);
		return NULL;
	}

	char *target = parse_word(p, ":");

	skipwhite(p);

	if (!expect(p, ':'))
	{
		err1(target, err);
		return NULL;
	}

	skipwhite(p);

	return target;
}


/**
 * Parse prerequisites and andvance p to end of line
 * 
 * @param prereq    Array to fill with prerequisites, should be previously 
 *                  allocated.
 * @param n_prereq  Pointer to number of prerequisites that is filled with 
 *                  number of prerequisites.
 * @param p         Pointer to place in string that is updated to end of line.
 * @return          Pointer to end of line in buffer, NULL if error. 
*/
static char *parse_prereqs(char **p, char **prereq, size_t *n_prereq)
{
	*n_prereq = 0; 
	while (*n_prereq < MAX_PREREQ
			&& (prereq[*n_prereq] = parse_word(p, "")) != NULL) {
		(*n_prereq)++;
		skipwhite(p);
	}

	if (!expect(p, '\n'))
	{
		return NULL;
	}

	return *p; 
}


/**
 * Advance until start of a command by reaing next row in fp.
 * 
 * @param buf   Pointer to the buffer to fill, must be allocated before.
 * @param fp    File pointer to the file that should be red .
 * @return      Pointer to place in buf where command starts, NULL if error.
*/
static char *advance_until_cmd(char *buf, FILE *fp)
{
	char *p;
	if ((p = next_line(buf, fp)) == NULL)
	{
		return NULL;
	}

	// command has to begin with tab
	if (!expect(&p, '\t'))
	{
		return NULL;
	}

	skipwhite(&p);

	return p;
}


/**
 * Parse a command and insert words into **cmd.
 * 
 * @param cmd     Array of words in a command that is previous allocated.
 * @param p       Pointer to current adress in the line to parse.
 * @return        Number of words that is parsed in command, ie the length of the array cmd. 
*/
static size_t parse_cmd(char **cmd, char **p)
{
	size_t n_words = 0;
	while (n_words < MAX_CMD && (cmd[n_words] = parse_word(p, "")) != NULL) {
		n_words++;
		skipwhite(p);
	}

	return n_words;
}


/**
 * Creates a rule given a target, a prereq string and a cmd_str.
 * 
 * @param target	Target in makefile.
 * @param prereq	Pointer to array with prerequisites.
 * @param cmd		Pointer to commands to run.
 * @return 			A rule that is allocated.
 * @note 			Rule must be freed when not needed anymore.
*/
static rule *create_rule(char *target, char **prereq, char **cmd)
{
	rule *r = malloc(sizeof *r);
	r->target = target;
	r->prereq = prereq;
	r->cmd = cmd;

	return r;
}


/**
 * Duplicate an array of strings.
 *
 * @param n     Size of array to duplicate.
 * @param a     Array to duplicate.
 * @return      NULL-terminated array which should be freed using free.
 */
static char **dupe_str_array(size_t n, char **a)
{
	char **ret = malloc((n + 1) * sizeof *ret);

	for (size_t i = 0; i < n; i++) {
		ret[i] = a[i];
	}
	ret[n] = NULL;

	return ret;
}


/**
 * Fills buf with the next line from fp. Returns buf if a line was read and
 * NULL otherwise.
 * 
 * @param buf   The buffer with the contents of the next line.
 * @param fp    The file to read.
 * @return      The buffer.
 */
static char *next_line(char buf[MAX_LINE], FILE *fp)
{
	do {
		if (fgets(buf, MAX_LINE, fp) == NULL) {
			return NULL;
		}
	} while (is_blank_line(buf));

	return buf;
}


/**
 * Parse a word and update p to point to the first character after the word.
 * The word is delimited by whitespace and any character in delim. The
 * returned string should be freed using free.
 * 
 * @param p     A pointer to the first character after the word.
 * @param delim A string of delimeters.
 * @return      A string with the word.
 */
static char *parse_word(char **p, char *delim)
{
	size_t n = 0;
	while (!isspace((*p)[n]) && strchr(delim, (*p)[n]) == NULL) {
		n++;
	}

	if (n == 0) {
		return NULL;
	}

	char *word = strndup(*p, n);
	*p += n;

	return word;
}


/**
 * Advance pointer to the next character which is not a space, stops at
 * newline.
 * 
 * @param p     The pointer to a character.
 */
static void skipwhite(char **p)
{
	while (isspace(**p) && **p != '\n') {
		(*p)++;
	}
}


/**
 * Check that the character pointed to by p is c, and increment p if it is.
 * 
 * @param p    The pointer to a character.
 * @param c    The character to compare with.
 * @return     True if equal, false otherwise.
 */
static bool expect(char **p, char c)
{
	if (**p != c) {
		return false;
	}

	(*p)++;

	return true;
}


/**
 * Check if line is blank.
 * 
 * @param s    The string (line) to check.
 * @return     True if line is blank, false otherwise.
 */
static bool is_blank_line(const char *s) 
{
	size_t i = 0;
	while (s[i] != '\0'){
		if (!isspace(s[i])) {
			return false;
		}
		i++;
	}

	return true;
}

/**
 * Free an array until NULL found
 * @param arr   Array to free
*/
static void free_arr(char **arr)
{
	size_t i = 0;
	while (arr[i] != NULL){
		free(arr[i]);
		i++;
	}
}

/**
 * Recursively delete a list of rules.
 * 
 * @param rules   The rules to delete.
 */
static void del_rules(struct rule *rules) 
{
	if (rules == NULL) {
		return;
	}

	free(rules->target);

	free_arr(rules->prereq);	
	free(rules->prereq);

	free_arr(rules->cmd);
	free(rules->cmd);

	del_rules(rules->next);

	free(rules);
}


/* ------------------------ Internal error handling ------------------------ */

/**
 * Sets err to true, lowest order of error cleanup
 * 
 * @note There exists 3 levels of error cleanup, err0, err1, and err2. 
 * When err2 is called, it means that err2 is performed, then err1, and lastly err0.
 * If err0 is called, only err0 is executed.
 * @param err Pointer to bool with error
 * @return
*/
static void err0(bool *err)
{
	*err = true;
}

/**
 * Middle order of cleanup, frees target and calls err0()
 * 
 * @note There exists 3 levels of error cleanup, err0, err1, and err2. 
 * When err2 is called, it means that err2 is performed, then err1, and lastly err0.
 * If err0 is called, only err0 is executed.
 * @param target    Allocated target to free
 * @param err       Pointer to bool that tells if error occured
 * @return
*/
static void err1(char *target, bool *err)
{
	free(target);
	err0(err);
}

/**
 * Highest order of cleanup, frees prereqs and calls err1()
 * 
 * @note There exists 3 levels of error cleanup, err0, err1, and err2. 
 * When err2 is called, it means that err2 is performed, then err1, and lastly err0.
 * If err0 is called, only err0 is executed.
 * @param prereq    Allocated array of prereqs to free
 * @param n_prereq  Number of elements in prereq to free
 * @param target    Allocated target to free
 * @param err       Pointer to bool that tells if error occured
 * @return
*/
static void err2(char *prereq[], size_t n_prereq, char *target, bool *err)
{
	for (size_t i = 0; i < n_prereq; i++) {
		free(prereq[i]);
	}
	err1(target, err);
}

