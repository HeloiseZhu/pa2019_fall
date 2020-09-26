#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

extern uint32_t look_up_symtab(char *sym, bool *success);

enum
{
	NOTYPE = 256,
	EQ,
	NUM,
	REG,
	SYMB,
	
	/* TODO: Add more token types */
	DEREF,
	NEQ,
	NUMH,
	AND,
	OR,
	NEG,
	BE,
	AE
};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"\\+", '+'},
	{"-", '-'},
	{"\\*", '*'},
	{"/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	
	{"0[Xx][0-9a-fA-F]{1,29}", NUMH},
	{"[0-9]{1,31}", NUM},
	{"\\$e[a-d]x", REG},
	{"\\$e[ds]i", REG},
	{"\\$e[bsi]p", REG},
	{"[_A-Za-z][_A-Za-z0-9]{0,30}", SYMB},
	
	{"<=", BE},
	{">=", AE},
	{"!=", NEQ},
	{"==", EQ},
	{"!", '!'},
	{"&&", AND},
	{"&", '&'},
	{"\\|{2}", OR},
	{"\\|", '|'}
	
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type)
				{
				case NOTYPE: break;
				case NUM:
					memcpy(tokens[nr_token].str, substr_start, substr_len);
				case SYMB: 
					memcpy(tokens[nr_token].str, substr_start, substr_len);
				case REG: 
					memcpy(tokens[nr_token].str, substr_start, substr_len);
				case NUMH:
					for(int j = 0; j < strlen(tokens[nr_token].str); j++) {
						if(tokens[nr_token].str[j] >= 'A' && tokens[nr_token].str[j] <= 'F')
							tokens[nr_token].str[j] += 0x20;
					}
					memcpy(tokens[nr_token].str, substr_start, substr_len);
				default:
					tokens[nr_token].type = rules[i].token_type;
					nr_token++;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}


int op_priority(int op_type) {
	switch(op_type) {
	case '(': return 1;
	case ')': return 1;
	case '!': return 2;
	case NEG: return 2;
	case DEREF: return 2;
	case '*': return 3;
	case '/': return 3;
	case '+': return 4;
	case '-': return 4;
	case BE:  return 6;
	case AE:  return 6;
	case EQ:  return 7;
	case NEQ: return 7;
	case '&': return 8;
	case '|': return 10;
	case AND: return 11;
	case OR:  return 12;
	default:
		printf("Undefined operator(%d) in function op_priority!\n", op_type);
		return 0;
	}
}


int dominant_oprator(int s, int e) {
	int left = 0, pos = s, cpri = 0;
	for(int i = s; i <= e; i++) {
		if(tokens[i].type == '(') {
			left++;
			i++;
			while(i <= e) {
				if(tokens[i].type == '(')
					left++;
				else if(tokens[i].type == ')')
					left--;
				if(left == 0) break;
				i++;
			}
			//if(i > e) break;
		}
		else if(tokens[i].type == NUM || tokens[i].type == NUMH || tokens[i].type == REG || tokens[i].type == SYMB) continue;
		else{
			int tpri = op_priority(tokens[i].type);
			if(tpri > cpri) {
				pos = i;
				cpri = tpri;
			}
		}
	}
	printf("In function dominant_oprator, s == %d, e == %d, find oprator pos: %d\n", s, e, pos);
	return pos;
}

bool check_parentheses(int s, int e, bool *suc)
{
	/* TODO */
	printf("In function check_parentheses, s == %d, e == %d\n", s, e);
	
	bool flg = true;
	int left = 0;
	if(tokens[s].type != '(' || tokens[e].type != ')')
		flg = false;
	for(int i = s; i <= e; i++) {
		if(tokens[i].type == '(')
			left++;
		else if(tokens[i].type == ')') {
			if(left <= 0) {
				*suc = false;
				return false;
			}
			else {
				left--;
				if(left == 0 && i != e)
					flg = false;
			}
		}
	}
	if(left != 0) {
		*suc = false;
		return false;
	}
	return flg;
}

uint32_t eval(int s, int e, bool *success)
{
	if(s > e) {
		/* Bad expression */
		*success = false;
		return 0;
	}
	else if(s == e) {
		/* Single token.
		 * For now this token should be a number.
		 * Return the value of number. 
		*/
		if(tokens[s].type == NUM){
			return atoi(tokens[s].str);
		}
		else if(tokens[s].type == NUMH){
			int len = strlen(tokens[s].str);
			int res = 0;
			for(int i = 2; i< len; i++){
				if(tokens[s].str[i] >= 'a' && tokens[s].str[i] <= 'f') {
					res = 16 * res + (tokens[s].str[i] - 'a') + 10;
				}
				else {
					res = 16 * res + (tokens[s].str[i] - '0');
				}
			}
			return res;
		}
		else if(tokens[s].type == REG) {
			bool suc;
			char reg_name[4] = {tokens[s].str[1], tokens[s].str[2], tokens[s].str[3], '\0'};
			uint32_t reg_val = get_reg_val(reg_name, &suc);
			if(suc == false) {
				//printf("Invalid register name:%s.\n", reg_name);
				*success = false;
				return 0;
			}
			else
				return reg_val;
		}
		else if(tokens[s].type == SYMB) {
			uint32_t addr = look_up_symtab(tokens[s].str, success);
			if(*success == false)
				return 0;
			else
				return addr;
		}
		else {
			printf("Invalid single token.\n");
			*success = false;
			return 0;
		}
	}
	else {
		bool suc = true;
		if(check_parentheses(s, e, &suc) == true) {
			return eval(s + 1, e - 1, success);
		}
		else {
			if(suc == false) {
				*success = false;
				return 0;
			}
			else {
				printf("In function eval, s == %d, e == %d\n", s, e);
				uint32_t val1, val2;
				int op = dominant_oprator(s, e);
				val1 = eval(s, op - 1, success);
				val2 = eval(op + 1, e, success);
				*success = true;
				switch(tokens[op].type) {
				case '+':	return val1 + val2;
				case '-':	return val1 - val2;
				case '*':	return val1 * val2;
				case '/':
					if(val2 == 0) {
						*success = false;
						return 0;
					}
					else
						return val1 / val2;
				case BE: 	return val1 <= val2;
				case AE: 	return val1 >= val2;
				case EQ:  	return val1 == val2;
				case NEQ: 	return val1 != val2;
				case '!':	return !val2;
				case AND:	return val1 && val2;
				case '&':	return val1 & val2;
				case OR:	return val1 || val2;
				case '|':	return val1 | val2;
				case DEREF: return vaddr_read(val2, SREG_CS, 4);
				case NEG:   return -val2;
				default:
					printf("Undefined operator(id:%d) in function eval!\n", op);
					*success = false;
					return 0;
				}
				
			}
		}
	}
}


uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
	/*
	printf("\nPlease implement expr at expr.c\n");
	assert(0);
	*/
	
	/* TODO: Implement code to evaluate the expression. */
	for(int i = 0; i < nr_token; i++){
		if(tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != NUM && tokens[i - 1].type != NUMH && tokens[i - 1].type != ')'))){
			tokens[i].type = DEREF;
		}
		if(tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != NUM && tokens[i - 1].type != NUMH && tokens[i - 1].type != ')'))){
			tokens[i].type = NEG;
		}
	}
	
	return eval(0, nr_token - 1, success);
}