#ifndef __MB_LIB_CORE_H__
#define __MB_LIB_CORE_H__

#include "config.h"
#include "mb_internal.h"
#include "my_basic.h"


_MB_DECLARE_LIBRARY(core)

/* Identifier */
#define _CORE_ID_ADD        "+"
#define _CORE_ID_MIN        "-"
#define _CORE_ID_MUL        "*"
#define _CORE_ID_DIV        "/"
#define _CORE_ID_MOD        "MOD"
#define _CORE_ID_POW        "^"
#define _CORE_ID_LBRA       "("
#define _CORE_ID_RBRA       ")"
#define _CORE_ID_NEG        0
#define _CORE_ID_EQU        "="
#define _CORE_ID_LT         "<"
#define _CORE_ID_GT         ">"
#define _CORE_ID_LTEQ       "<="
#define _CORE_ID_GTEQ       ">="
#define _CORE_ID_NEQ        "<>"
#define _CORE_ID_AND        "AND"
#define _CORE_ID_OR         "OR"
#define _CORE_ID_NOT        "NOT"
#define _CORE_ID_IS         "IS"
#define _CORE_ID_LET        "LET"
#define _CORE_ID_DIM        "DIM"
#define _CORE_ID_IF         "IF"
#define _CORE_ID_THEN       "THEN"
#define _CORE_ID_ELSEIF     "ELSEIF"
#define _CORE_ID_ELSE       "ELSE"
#define _CORE_ID_ENDIF      "ENDIF"
#define _CORE_ID_FOR        "FOR"
#define _CORE_ID_IN         "IN"
#define _CORE_ID_TO         "TO"
#define _CORE_ID_STEP       "STEP"
#define _CORE_ID_NEXT       "NEXT"
#define _CORE_ID_WHILE      "WHILE"
#define _CORE_ID_WEND       "WEND"
#define _CORE_ID_DO         "DO"
#define _CORE_ID_UNTIL      "UNTIL"
#define _CORE_ID_EXIT       "EXIT"
#define _CORE_ID_GOTO       "GOTO"
#define _CORE_ID_GOSUB      "GOSUB"
#define _CORE_ID_RETURN     "RETURN"
#define _CORE_ID_CALL       "CALL"
#define _CORE_ID_DEF        "DEF"
#define _CORE_ID_ENDDEF     "ENDDEF"
#define _CORE_ID_VARARGS    "VARARGS"
#define _CORE_ID_CLASS      "CLASS"
#define _CORE_ID_ENDCLASS   "ENDCLASS"
#define _CORE_ID_NEW        "NEW"
#define _CORE_ID_VAR        "VAR"
#define _CORE_ID_REFLECT    "REFLECT"
#define _CORE_ID_LAMBDA     "LAMBDA"
#define _CORE_ID_MEM        "MEM"
#define _CORE_ID_TYPE       "TYPE"
#define _CORE_ID_IMPORT     "IMPORT"
#define _CORE_ID_END        "END"

/* Library function & statement */

int _core_dummy_assign(mb_interpreter_t* s, void** l);
int _core_add(mb_interpreter_t* s, void** l);
int _core_min(mb_interpreter_t* s, void** l);
int _core_mul(mb_interpreter_t* s, void** l);
int _core_div(mb_interpreter_t* s, void** l);
int _core_mod(mb_interpreter_t* s, void** l);
int _core_pow(mb_interpreter_t* s, void** l);
int _core_open_bracket(mb_interpreter_t* s, void** l);
int _core_close_bracket(mb_interpreter_t* s, void** l);
int _core_neg(mb_interpreter_t* s, void** l);
int _core_equal(mb_interpreter_t* s, void** l);
int _core_less(mb_interpreter_t* s, void** l);
int _core_greater(mb_interpreter_t* s, void** l);
int _core_less_equal(mb_interpreter_t* s, void** l);
int _core_greater_equal(mb_interpreter_t* s, void** l);
int _core_not_equal(mb_interpreter_t* s, void** l);
int _core_and(mb_interpreter_t* s, void** l);
int _core_or(mb_interpreter_t* s, void** l);
int _core_not(mb_interpreter_t* s, void** l);
int _core_is(mb_interpreter_t* s, void** l);
int _core_let(mb_interpreter_t* s, void** l);
int _core_dim(mb_interpreter_t* s, void** l);
int _core_if(mb_interpreter_t* s, void** l);
int _core_then(mb_interpreter_t* s, void** l);
int _core_elseif(mb_interpreter_t* s, void** l);
int _core_else(mb_interpreter_t* s, void** l);
int _core_endif(mb_interpreter_t* s, void** l);
int _core_for(mb_interpreter_t* s, void** l);
#ifdef MB_ENABLE_COLLECTION_LIB
int _core_in(mb_interpreter_t* s, void** l);
#endif /* MB_ENABLE_COLLECTION_LIB */
int _core_to(mb_interpreter_t* s, void** l);
int _core_step(mb_interpreter_t* s, void** l);
int _core_next(mb_interpreter_t* s, void** l);
int _core_while(mb_interpreter_t* s, void** l);
int _core_wend(mb_interpreter_t* s, void** l);
int _core_do(mb_interpreter_t* s, void** l);
int _core_until(mb_interpreter_t* s, void** l);
int _core_exit(mb_interpreter_t* s, void** l);
int _core_goto(mb_interpreter_t* s, void** l);
int _core_gosub(mb_interpreter_t* s, void** l);
int _core_return(mb_interpreter_t* s, void** l);
int _core_call(mb_interpreter_t* s, void** l);
int _core_def(mb_interpreter_t* s, void** l);
int _core_enddef(mb_interpreter_t* s, void** l);
int _core_args(mb_interpreter_t* s, void** l);
#ifdef MB_ENABLE_CLASS
int _core_class(mb_interpreter_t* s, void** l);
int _core_endclass(mb_interpreter_t* s, void** l);
int _core_new(mb_interpreter_t* s, void** l);
int _core_var(mb_interpreter_t* s, void** l);
int _core_reflect(mb_interpreter_t* s, void** l);
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_LAMBDA
int _core_lambda(mb_interpreter_t* s, void** l);
#endif /* MB_ENABLE_LAMBDA */
#ifdef MB_ENABLE_ALLOC_STAT
int _core_mem(mb_interpreter_t* s, void** l);
#endif /* MB_ENABLE_ALLOC_STAT */
int _core_type(mb_interpreter_t* s, void** l);
int _core_import(mb_interpreter_t* s, void** l);
int _core_end(mb_interpreter_t* s, void** l);

#endif // __MB_LIB_CORE_H__
