#ifndef __MB_INTERNAL_H__
#define __MB_INTERNAL_H__

#include "config.h"
#include "my_basic.h"

#if defined ARDUINO && !defined MB_CP_ARDUINO
#	define MB_CP_ARDUINO
#endif /* ARDUINO && !MB_CP_ARDUINO */
#ifdef MB_CP_ARDUINO
#	ifndef MB_DISABLE_LOAD_FILE
#		define MB_DISABLE_LOAD_FILE
#	endif /* MB_DISABLE_LOAD_FILE */
#	ifndef MB_MANUAL_REAL_FORMATTING
#		define MB_MANUAL_REAL_FORMATTING
#	endif /* MB_MANUAL_REAL_FORMATTING */
#endif /* MB_CP_ARDUINO */
#ifdef MB_CP_VC
#	include <conio.h>
#	include <malloc.h>
#	include <Windows.h>
#else /* MB_CP_VC */
#	include <stdint.h>
#endif /* MB_CP_VC */
#ifndef MB_CP_ARDUINO
#	include <memory.h>
#endif /* MB_CP_ARDUINO */
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef MB_CP_VC
#	pragma warning(push)
#	pragma warning(disable : 4127)
#	pragma warning(disable : 4214)
#	pragma warning(disable : 4305)
#	pragma warning(disable : 4309)
#	pragma warning(disable : 4805)
#	pragma warning(disable : 4996)
#endif /* MB_CP_VC */

#ifdef MB_CP_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wunused-function"
#	pragma clang diagnostic ignored "-Wunused-variable"
#endif /* MB_CP_CLANG */

#ifdef MB_CP_BORLANDC
#	pragma warn -8004
#	pragma warn -8008
#	pragma warn -8012
#endif /* MB_CP_BORLANDC */

#ifdef MB_COMPACT_MODE
#	pragma pack(1)
#endif /* MB_COMPACT_MODE */

/*
** {========================================================
** Data type declarations
*/

/** Macros */

/* Define as 1 to create hash table nodes lazily, 0 obligingly */
#ifndef _LAZY_HASH_TABLE
#	define _LAZY_HASH_TABLE 1
#endif /* _LAZY_HASH_TABLE */

/* Define as 1 to treat warning as error, 0 just leave it */
#ifndef _WARNING_AS_ERROR
#	define _WARNING_AS_ERROR 0
#endif /* _WARNING_AS_ERROR */

/* Define as 1 to automatically raise error during popping argument, 0 just return an error result */
#ifndef _SIMPLE_ARG_ERROR
#	define _SIMPLE_ARG_ERROR 0
#endif /* _SIMPLE_ARG_ERROR */

/* Define as 1 to use a comma to PRINT a new line, 0 to use a semicolon */
#ifndef _COMMA_AS_NEWLINE
#	define _COMMA_AS_NEWLINE 0
#endif /* _COMMA_AS_NEWLINE */

/* Define as 1 to enable multiline statement */
#ifndef _MULTILINE_STATEMENT
#	define _MULTILINE_STATEMENT 1
#endif /* _MULTILINE_STATEMENT */

/* Hash table size */
#ifndef _HT_ARRAY_SIZE_DEFAULT
#	define _HT_ARRAY_SIZE_TINY 1
#	define _HT_ARRAY_SIZE_SMALL 193
#	define _HT_ARRAY_SIZE_MID 1543
#	define _HT_ARRAY_SIZE_BIG 12289
#	define _HT_ARRAY_SIZE_DEFAULT _HT_ARRAY_SIZE_SMALL
#endif /* _HT_ARRAY_SIZE_DEFAULT */

/* Max length of a single symbol */
#ifndef _SINGLE_SYMBOL_MAX_LENGTH
#	define _SINGLE_SYMBOL_MAX_LENGTH 128
#endif /* _SINGLE_SYMBOL_MAX_LENGTH */

/* Buffer length of some string operations */
#ifndef _INPUT_MAX_LENGTH
#	define _INPUT_MAX_LENGTH 256
#endif /* _INPUT_MAX_LENGTH */
#ifndef _TEMP_FORMAT_MAX_LENGTH
#	define _TEMP_FORMAT_MAX_LENGTH 32
#endif /* _TEMP_FORMAT_MAX_LENGTH */
#ifndef _LAMBDA_NAME_MAX_LENGTH
#	define _LAMBDA_NAME_MAX_LENGTH 32
#endif /* _LAMBDA_NAME_MAX_LENGTH */

/* Helper */
#ifdef MB_COMPACT_MODE
#	define _PACK1 : 1
#	define _PACK2 : 2
#	define _PACK8 : 8
#else /* MB_COMPACT_MODE */
#	define _PACK1
#	define _PACK2
#	define _PACK8
#endif /* MB_COMPACT_MODE */

#ifndef _UNALIGNED_ARG
#	if defined MB_CP_VC && defined MB_OS_WIN64
#		ifdef MB_COMPACT_MODE
#			define _UNALIGNED_ARG __unaligned
#		else /* MB_COMPACT_MODE */
#			define _UNALIGNED_ARG
#		endif /* MB_COMPACT_MODE */
#	else
#		define _UNALIGNED_ARG
#	endif
#endif /* _UNALIGNED_ARG */

#ifndef sgn
#	define sgn(__v) ((__v) ? ((__v) > 0 ? 1 : -1) : 0)
#endif /* sgn */

#ifndef islower
#	define islower(__c) ((__c) >= 'a' && (__c) <= 'z')
#endif /* islower */
#ifndef toupper
#	define toupper(__c) (islower(__c) ? ((__c) - 'a' + 'A') : (__c))
#endif /* toupper */

#ifndef countof
#	define countof(__a) (sizeof(__a) / sizeof(*(__a)))
#endif /* countof */

#ifndef _mb_check_exit
#	define _mb_check_exit(__expr, __exit) do { if((__expr) != MB_FUNC_OK) goto __exit; } while(0)
#endif /* _mb_check_exit */
#ifndef _mb_check_mark_exit
#	define _mb_check_mark_exit(__expr, __result, __exit) do { __result = (__expr); if(__result != MB_FUNC_OK) goto __exit; } while(0)
#endif /* _mb_check_mark_exit */

/** Collections */

/* Collection functors */
#define _OP_RESULT_NORMAL 0
#define _OP_RESULT_DEL_NODE -1

typedef int (* _common_compare_t)(void*, void*);
typedef int (* _common_operation_t)(void*, void*);

/* List */
typedef _common_compare_t _ls_compare_t;
typedef _common_operation_t _ls_operation_t;

typedef struct _ls_node_t {
	void* data;
	struct _ls_node_t* prev;
	struct _ls_node_t* next;
	void* extra;
} _ls_node_t;

/* Dictionary */
typedef unsigned (* _ht_hash_t)(void*, void*);
typedef _common_compare_t _ht_compare_t;
typedef _common_operation_t _ht_operation_t;

typedef struct _ht_node_t {
	_ls_operation_t free_extra;
	_ht_compare_t compare;
	_ht_hash_t hash;
	unsigned array_size;
	unsigned count;
	_ls_node_t** array;
} _ht_node_t;

/** Normal enum/struct/union/const, etc. */

#ifdef MB_ENABLE_FULL_ERROR
/* Error description text */
extern MBCONST const char* const _ERR_DESC[];
#endif /* MB_ENABLE_FULL_ERROR */

/* Data type */
typedef enum _data_e {
	_DT_INVALID = -1,
	_DT_NIL = 0,
	_DT_UNKNOWN,
	_DT_INT,
	_DT_REAL,
	_DT_STRING,
	_DT_TYPE,
	_DT_USERTYPE,
#ifdef MB_ENABLE_USERTYPE_REF
	_DT_USERTYPE_REF,
#endif /* MB_ENABLE_USERTYPE_REF */
	_DT_FUNC,
	_DT_VAR,
	_DT_ARRAY,
#ifdef MB_ENABLE_COLLECTION_LIB
	_DT_LIST,
	_DT_LIST_IT,
	_DT_DICT,
	_DT_DICT_IT,
#endif /* MB_ENABLE_COLLECTION_LIB */
	_DT_LABEL, /* Label type, used for GOTO, GOSUB statement */
#ifdef MB_ENABLE_CLASS
	_DT_CLASS, /* Object instance */
#endif /* MB_ENABLE_CLASS */
	_DT_ROUTINE, /* User defined sub routine in script */
#ifdef MB_ENABLE_LAMBDA
	_DT_OUTER_SCOPE,
#endif /* MB_ENABLE_LAMBDA */
	_DT_SEP, /* Separator */
#ifdef MB_ENABLE_SOURCE_TRACE
	_DT_PREV_IMPORT,
	_DT_POST_IMPORT,
#endif /* MB_ENABLE_SOURCE_TRACE */
	_DT_EOS /* End of statement */
} _data_e;

#ifdef MB_ENABLE_COLLECTION_LIB
#	define _HAS_REF_OBJ_LOCK
#endif /* MB_ENABLE_COLLECTION_LIB */

#ifdef _HAS_REF_OBJ_LOCK
typedef short _lock_t;
#endif /* _HAS_REF_OBJ_LOCK */

struct _ref_t;

typedef void (* _unref_func_t)(struct _ref_t*, void*);

#define _NONE_REF 1

#ifndef _ref_count_t
typedef unsigned _ref_count_t;
#endif /* _ref_count_t */

/* The reference structure should be always at the head of an object */
typedef struct _ref_t {
	_ref_count_t* count;
	_ref_count_t* weak_count;
	_unref_func_t on_unref;
	_data_e type _PACK8;
	struct mb_interpreter_t* s;
} _ref_t;

typedef struct _gc_t {
	_ht_node_t* table;
	_ht_node_t* recursive_table;
	_ht_node_t* collected_table;
	_ht_node_t* valid_table;
	unsigned char collecting;
	bool_t disabled _PACK1;
} _gc_t;

#ifdef MB_ENABLE_USERTYPE_REF
typedef struct _calculation_operator_info_t {
	mb_meta_operator_t is;
	mb_meta_operator_t add;
	mb_meta_operator_t sub;
	mb_meta_operator_t mul;
	mb_meta_operator_t div;
	mb_meta_operator_t neg;
} _calculation_operator_info_t;

typedef struct _usertype_ref_t {
	_ref_t ref;
	void* usertype;
	mb_dtor_func_t dtor;
	mb_clone_func_t clone;
	mb_hash_func_t hash;
	mb_cmp_func_t cmp;
	mb_fmt_func_t fmt;
#ifdef MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF
	mb_alive_value_checker_t alive_checker;
#endif /* MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF */
	_calculation_operator_info_t* calc_operators;
	mb_meta_func_t coll_func;
	mb_meta_func_t generic_func;
} _usertype_ref_t;
#endif /* MB_ENABLE_USERTYPE_REF */

typedef struct _func_t {
	char* name;
	mb_func_t pointer;
} _func_t;

#define _PATHING_NONE 0
#define _PATHING_NORMAL 1
#define _PATHING_UNKNOWN_FOR_NOT_FOUND 2
#define _PATHING_UPVALUE 3

#define _PN(__b) ((!!(__b)) ? (_PATHING_NORMAL) : (_PATHING_NONE))
#define _PU(__b) ((!!(__b)) ? (_PATHING_UNKNOWN_FOR_NOT_FOUND) : (_PATHING_NONE))

typedef struct _var_t {
	char* name;
	struct _object_t* data;
#ifdef MB_ENABLE_CLASS
	unsigned char pathing _PACK2;
	bool_t is_me _PACK1;
#endif /* MB_ENABLE_CLASS */
} _var_t;

typedef struct _array_t {
#ifdef MB_ENABLE_ARRAY_REF
	_ref_t ref;
#endif /* MB_ENABLE_ARRAY_REF */
	char* name;
	_data_e type;
#ifndef MB_SIMPLE_ARRAY
	_data_e* types;
#endif /* MB_SIMPLE_ARRAY */
	void* raw;
	unsigned count;
	unsigned char dimension_count;
	unsigned dimensions[MB_MAX_DIMENSION_COUNT];
} _array_t;

#ifdef MB_ENABLE_COLLECTION_LIB
typedef struct _array_helper_t {
	struct mb_interpreter_t* s;
	_array_t* array;
	int index;
} _array_helper_t;
#endif /* MB_ENABLE_COLLECTION_LIB */

#ifdef MB_ENABLE_COLLECTION_LIB
typedef struct _list_t {
	_ref_t ref;
	_lock_t lock;
	_ls_node_t* list;
	_ls_node_t* cached_node;
	int cached_index;
	int_t count;
	int_t* range_begin;
} _list_t;

typedef struct _list_it_t {
	_ref_t weak_ref;
	_list_t* list;
	bool_t locking _PACK1;
	union {
		_ls_node_t* node;
		int_t ranging;
	} curr;
} _list_it_t;

typedef struct _dict_t {
	_ref_t ref;
	_lock_t lock;
	_ht_node_t* dict;
} _dict_t;

#define _INVALID_DICT_IT ((_ls_node_t*)(intptr_t)~0)

typedef struct _dict_it_t {
	_ref_t weak_ref;
	_dict_t* dict;
	bool_t locking _PACK1;
	unsigned curr_bucket;
	_ls_node_t* curr_node;
} _dict_it_t;

typedef struct _keys_helper_t {
	mb_value_t* keys;
	int size;
	int index;
} _keys_helper_t;
#endif /* MB_ENABLE_COLLECTION_LIB */

typedef struct _label_t {
	char* name;
	_ls_node_t* node;
} _label_t;

#ifdef MB_ENABLE_CLASS
#define _META_LIST_MAX_DEPTH UINT_MAX

#define _CLASS_ME "ME"

#define _CLASS_HASH_FUNC "HASH"
#define _CLASS_COMPARE_FUNC "COMPARE"
#define _CLASS_TO_STRING_FUNC "TO_STRING"

#define _CLASS_OVERRIDE_FMT "_%s"

typedef struct _class_t {
	_ref_t ref;
	char* name;
	struct _class_t* created_from;
	_ls_node_t* meta_list;
	struct _running_context_t* scope;
	struct _routine_t* hash;
	struct _routine_t* compare;
	void* userdata;
} _class_t;
#endif /* MB_ENABLE_CLASS */

#ifdef MB_ENABLE_LAMBDA
typedef struct _running_context_ref_t {
	_ref_t ref;
	struct _running_context_ref_t* prev;
	struct _running_context_t* scope;
} _running_context_ref_t;

typedef struct _upvalue_scope_tuple_t {
	struct mb_interpreter_t* s;
#ifdef MB_ENABLE_CLASS
	_class_t* instance;
#endif /* MB_ENABLE_CLASS */
	struct _running_context_t* scope;
	_running_context_ref_t* outer_scope;
	struct _lambda_t* lambda;
	_ht_node_t* filled;
} _upvalue_scope_tuple_t;

typedef struct _lambda_t {
	_ref_t ref;
	struct _running_context_t* scope;
	_ls_node_t* parameters;
	_running_context_ref_t* outer_scope;
	struct _running_context_t* overlapped;
	_ht_node_t* upvalues;
	_ls_node_t* entry;
	_ls_node_t* end;
} _lambda_t;
#endif /* MB_ENABLE_LAMBDA */

typedef struct _routine_t {
	union {
		struct {
			struct _running_context_t* scope;
			_ls_node_t* parameters;
			_ls_node_t* entry;
		} basic;
#ifdef MB_ENABLE_LAMBDA
		_lambda_t lambda;
#endif /* MB_ENABLE_LAMBDA */
		struct {
			mb_routine_func_t entry;
		} native;
	} func;
	char* name;
#ifdef MB_ENABLE_SOURCE_TRACE
	char* source_file;
#endif /* MB_ENABLE_SOURCE_TRACE */
#ifdef MB_ENABLE_CLASS
	_class_t* instance;
#endif /* MB_ENABLE_CLASS */
	bool_t is_cloned _PACK1;
	mb_routine_type_e type;
} _routine_t;

#ifdef MB_ENABLE_SOURCE_TRACE
typedef struct _import_info_t {
	char* source_file;
} _import_info_t;
#endif /* MB_ENABLE_SOURCE_TRACE */

typedef union _raw_u { mb_data_e e; char c; int_t i; real_t r; void* p; mb_val_bytes_t b; } _raw_u;

typedef unsigned char _raw_t[sizeof(_raw_u)];

typedef struct _object_t {
	_data_e type;
	union {
		int_t integer;
		real_t float_point;
		char* string;
		mb_data_e type;
		void* usertype;
#ifdef MB_ENABLE_USERTYPE_REF
		_usertype_ref_t* usertype_ref;
#endif /* MB_ENABLE_USERTYPE_REF */
		_func_t* func;
		_var_t* variable;
		_array_t* array;
#ifdef MB_ENABLE_COLLECTION_LIB
		_list_t* list;
		_list_it_t* list_it;
		_dict_t* dict;
		_dict_it_t* dict_it;
#endif /* MB_ENABLE_COLLECTION_LIB */
		_label_t* label;
#ifdef MB_ENABLE_CLASS
		_class_t* instance;
#endif /* MB_ENABLE_CLASS */
		_routine_t* routine;
		char separator;
#ifdef MB_ENABLE_SOURCE_TRACE
		_import_info_t* import_info;
#endif /* MB_ENABLE_SOURCE_TRACE */
		mb_val_bytes_t bytes;
		void* pointer;
		_raw_t raw;
	} data;
	bool_t is_ref _PACK1;
#ifdef MB_PREFER_SPEED
	bool_t is_const _PACK1;
#endif /* MB_PREFER_SPEED */
#ifdef MB_ENABLE_SOURCE_TRACE
	int source_pos;
	unsigned short source_row;
	unsigned short source_col;
#else /* MB_ENABLE_SOURCE_TRACE */
	char source_pos _PACK1;
#endif /* MB_ENABLE_SOURCE_TRACE */
} _object_t;

#ifdef MB_ENABLE_MODULE
typedef struct _module_func_t {
	char* module;
	mb_func_t func;
} _module_func_t;
#endif /* MB_ENABLE_MODULE */

typedef struct _dynamic_buffer_t {
	char bytes[_TEMP_FORMAT_MAX_LENGTH];
	union {
		char* charp;
#if defined MB_CP_VC && defined MB_ENABLE_UNICODE
		wchar_t* wcharp;
#endif /* MB_CP_VC && MB_ENABLE_UNICODE */
	} pointer;
	size_t size;
} _dynamic_buffer_t;

#define _MB_MEM_TAG_SIZE (sizeof(mb_mem_tag_t))

extern MBAPI size_t MB_SIZEOF_4BYTES;
extern MBAPI size_t MB_SIZEOF_8BYTES;
extern MBAPI size_t MB_SIZEOF_32BYTES;
extern MBAPI size_t MB_SIZEOF_64BYTES;
extern MBAPI size_t MB_SIZEOF_128BYTES;
extern MBAPI size_t MB_SIZEOF_256BYTES;
extern MBAPI size_t MB_SIZEOF_512BYTES;
#ifdef MB_ENABLE_ALLOC_STAT
extern MBAPI size_t MB_SIZEOF_INT;
extern MBAPI size_t MB_SIZEOF_PTR;
extern MBAPI size_t MB_SIZEOF_LSN;
extern MBAPI size_t MB_SIZEOF_HTN;
extern MBAPI size_t MB_SIZEOF_HTA;
extern MBAPI size_t MB_SIZEOF_OBJ;
#ifdef MB_ENABLE_USERTYPE_REF
extern MBAPI size_t MB_SIZEOF_UTR;
#endif /* MB_ENABLE_USERTYPE_REF */
extern MBAPI size_t MB_SIZEOF_FUN;
extern MBAPI size_t MB_SIZEOF_VAR;
extern MBAPI size_t MB_SIZEOF_ARR;
#ifdef MB_ENABLE_COLLECTION_LIB
extern MBAPI size_t MB_SIZEOF_LST;
extern MBAPI size_t MB_SIZEOF_DCT;
#endif /* MB_ENABLE_COLLECTION_LIB */
extern MBAPI size_t MB_SIZEOF_LBL;
#ifdef MB_ENABLE_CLASS
extern MBAPI size_t MB_SIZEOF_CLS;
#endif /* MB_ENABLE_CLASS */
extern MBAPI size_t MB_SIZEOF_RTN;
#else /* MB_ENABLE_ALLOC_STAT */
extern MBAPI size_t MB_SIZEOF_INT;
extern MBAPI size_t MB_SIZEOF_PTR;
extern MBAPI size_t MB_SIZEOF_LSN;
extern MBAPI size_t MB_SIZEOF_HTN;
extern MBAPI size_t MB_SIZEOF_HTA;
extern MBAPI size_t MB_SIZEOF_OBJ;
#ifdef MB_ENABLE_USERTYPE_REF
extern MBAPI size_t MB_SIZEOF_UTR;
#endif /* MB_ENABLE_USERTYPE_REF */
extern MBAPI size_t MB_SIZEOF_FUN;
extern MBAPI size_t MB_SIZEOF_VAR;
extern MBAPI size_t MB_SIZEOF_ARR;
#ifdef MB_ENABLE_COLLECTION_LIB
extern MBAPI size_t MB_SIZEOF_LST;
extern MBAPI size_t MB_SIZEOF_DCT;
#endif /* MB_ENABLE_COLLECTION_LIB */
extern MBAPI size_t MB_SIZEOF_LBL;
#ifdef MB_ENABLE_CLASS
extern MBAPI size_t MB_SIZEOF_CLS;
#endif /* MB_ENABLE_CLASS */
extern MBAPI size_t MB_SIZEOF_RTN;
#endif /* MB_ENABLE_ALLOC_STAT */

#ifndef _CONST_PART1
#	ifdef MB_PREFER_SPEED
#		define _CONST_PART1 false, false,
#	else /* MB_PREFER_SPEED */
#		define _CONST_PART1 false,
#	endif /* MB_PREFER_SPEED */
#endif /* _CONST_PART1 */
#ifndef _CONST_PART2
#	ifdef MB_ENABLE_SOURCE_TRACE
#		define _CONST_PART2 0, 0, 0
#	else /* MB_ENABLE_SOURCE_TRACE */
#		define _CONST_PART2 0
#	endif /* MB_ENABLE_SOURCE_TRACE */
#endif /* _CONST_PART2 */
#ifndef _CONST_TAIL
#	define _CONST_TAIL _CONST_PART1 _CONST_PART2
#endif /* _CONST_TAIL */

extern const _object_t _OBJ_INT_UNIT;
extern const _object_t _OBJ_INT_ZERO;
#define _MAKE_NIL(__o) do { memset((__o), 0, sizeof(_object_t)); (__o)->type = _DT_NIL; } while(0)

extern _object_t* _OBJ_BOOL_TRUE;
extern _object_t* _OBJ_BOOL_FALSE;

#ifdef MB_ENABLE_CLASS
extern const _object_t _OBJ_UNKNOWN;
extern const _ls_node_t _LS_NODE_UNKNOWN;
#endif /* MB_ENABLE_CLASS */

#define _VAR_ARGS_STR "..."

#define _IS_VAR_ARGS(__v) ((__v) == &_VAR_ARGS)

extern const _var_t _VAR_ARGS;

/* Parsing context */
#define _CLASS_STATE_NONE 0
#define _CLASS_STATE_PROC 1

extern const char _MULTI_LINE_COMMENT_PREFIX[3];
extern const char _MULTI_LINE_COMMENT_POSTFIX[3];

extern const char _UNICODE_OPEN_QUOTE[3];
extern const char _UNICODE_CLOSE_QUOTE[3];

typedef enum _parsing_state_e {
	_PS_NORMAL,
	_PS_STRING,
	_PS_COMMENT,
	_PS_MULTI_LINE_COMMENT
} _parsing_state_e;

typedef enum _symbol_state_e {
	_SS_IDENTIFIER,
	_SS_OPERATOR
} _symbol_state_e;

#define _ROUTINE_STATE_NONE 0
#define _ROUTINE_STATE_DEF 1
#define _ROUTINE_STATE_PARAMS 2

typedef struct _parsing_context_t {
	_ls_node_t* imported;
	char current_char;
	char current_symbol[_SINGLE_SYMBOL_MAX_LENGTH + 1];
	int current_symbol_nonius;
	int current_symbol_contains_accessor;
	_object_t* last_symbol;
	int multi_line_comment_count;
	_parsing_state_e parsing_state;
	_symbol_state_e symbol_state;
#ifdef MB_ENABLE_CLASS
	unsigned short class_state;
#endif /* MB_ENABLE_CLASS */
	unsigned short routine_state;
	unsigned short routine_params_state;
	int parsing_pos;
	unsigned short parsing_row;
	unsigned short parsing_col;
} _parsing_context_t;

/* Running context */
#define _SCOPE_META_ROOT (1 << 0)
#define _SCOPE_META_REF (1 << 1)

#define _INFINITY_CALC_DEPTH -1

typedef struct _running_context_t {
	struct _running_context_t* prev;
	unsigned meta;
	_ht_node_t* var_dict;
	struct _running_context_t* ref;
	_var_t* next_loop_var;
	mb_value_t intermediate_value;
	int calc_depth;
#ifdef MB_ENABLE_LAMBDA
	_ls_node_t* refered_lambdas;
#endif /* MB_ENABLE_LAMBDA */
} _running_context_t;

/* Expression processing utilities */
typedef struct _tuple3_t {
	void* e1;
	void* e2;
	void* e3;
} _tuple3_t;

/* Interpreter tag */
#define _JMP_NIL 0x00
#define _JMP_INS 0x01
#define _JMP_STR 0x02

#define _NO_EAT_COMMA 2

typedef struct mb_interpreter_t {
	/** Fundamental */
#ifdef MB_ENABLE_FORK
	struct mb_interpreter_t* forked_from;
	_running_context_t* forked_context;
	_ls_node_t* all_forked;
#endif /* MB_ENABLE_FORK */
	bool_t valid _PACK1;
	void* userdata;
	_ls_node_t* ast;
	/** Memory management */
	_gc_t gc;
	_ls_node_t* edge_destroy_objects;
	_ls_node_t* lazy_destroy_objects;
	/** Scripting interface and module */
	_ht_node_t* local_func_dict;
	_ht_node_t* global_func_dict;
#ifdef MB_ENABLE_MODULE
	_ht_node_t* module_func_dict;
	char* with_module;
	_ls_node_t* using_modules;
#endif /* MB_ENABLE_MODULE */
	/** Parsing and interpreting */
	char* source_file;
	_parsing_context_t* parsing_context;
	_running_context_t* running_context;
	int run_count;
	bool_t has_run _PACK1;
	_ls_node_t* var_args;
#ifdef MB_ENABLE_USERTYPE_REF
	_object_t* usertype_ref_ahead;
#endif /* MB_ENABLE_USERTYPE_REF */
	unsigned char jump_set;
#ifdef MB_ENABLE_CLASS
	_class_t* last_instance;
	bool_t calling _PACK1;
#endif /* MB_ENABLE_CLASS */
	_routine_t* last_routine;
	_ls_node_t* sub_stack;
	_ls_node_t* suspent_point;
	int schedule_suspend_tag;
	int_t no_eat_comma_mark;
	_ls_node_t* skip_to_eoi;
	_ls_node_t* in_neg_expr;
#ifdef MB_ENABLE_STACK_TRACE
	_ls_node_t* stack_frames;
#endif /* MB_ENABLE_STACK_TRACE */
#if _MULTILINE_STATEMENT
	_ls_node_t* multiline_enabled;
#endif /* _MULTILINE_STATEMENT */
	/** Error handling */
	bool_t handled_error _PACK1;
	mb_error_e last_error;
	char* last_error_file;
	int last_error_pos;
	unsigned short last_error_row;
	unsigned short last_error_col;
	/** Handlers */
	mb_alive_checker_t alive_check_handler;
	mb_debug_stepped_handler_t debug_prev_stepped_handler;
	mb_debug_stepped_handler_t debug_post_stepped_handler;
	mb_error_handler_t error_handler;
	mb_print_func_t printer;
	mb_input_func_t inputer;
	mb_import_handler_t import_handler;
} mb_interpreter_t;

/* Operations */
extern const char _PRECEDE_TABLE[20][20]; /* Operator priority table */

extern _object_t* _exp_assign;

#define _copy_bytes(__l, __r) do { memcpy((__l), (__r), sizeof(mb_val_bytes_t)); } while(0)

#define _set_real_with_hex(__r, __i) \
	do { \
		if(sizeof(__r) == sizeof(unsigned char)) { \
			union { unsigned char i; real_t r; } __u; \
			__u.i = (unsigned char)__i; \
			__r = __u.r; \
		} else if(sizeof(__r) == sizeof(unsigned short)) { \
			union { unsigned short i; real_t r; } __u; \
			__u.i = (unsigned short)__i; \
			__r = __u.r; \
		} else if(sizeof(__r) == sizeof(unsigned)) { \
			union { unsigned i; real_t r; } __u; \
			__u.i = __i; \
			__r = __u.r; \
		} else if(sizeof(__r) == sizeof(unsigned long)) { \
			union { unsigned long i; real_t r; } __u; \
			__u.i = __i; \
			__r = __u.r; \
		} else if(sizeof(__r) == sizeof(unsigned long long)) { \
			union { unsigned long long i; real_t r; } __u; \
			__u.i = __i; \
			__r = __u.r; \
		} else { \
			mb_assert(0 && "Invalid real number precision."); \
		} \
	} while(0)

#if MB_CONVERT_TO_INT_LEVEL == MB_CONVERT_TO_INT_LEVEL_NONE
#	define _convert_to_int_if_posible(__o) do { ((void)(__o)); } while(0)
#else /* MB_CONVERT_TO_INT_LEVEL == MB_CONVERT_TO_INT_LEVEL_NONE */
#	define _convert_to_int_if_posible(__o) \
		do { \
			if((__o)->type == _DT_REAL && (real_t)(int_t)(__o)->data.float_point == (__o)->data.float_point) { \
				(__o)->type = _DT_INT; \
				(__o)->data.integer = (int_t)(__o)->data.float_point; \
			} \
		} while(0)
#endif /* MB_CONVERT_TO_INT_LEVEL == MB_CONVERT_TO_INT_LEVEL_NONE */

#define _instruct_head(__tuple) \
	_object_t opndv1; \
	_object_t opndv2; \
	_tuple3_t* tpptr = (_tuple3_t*)(*__tuple); \
	_object_t* opnd1 = (_object_t*)(tpptr->e1); \
	_object_t* opnd2 = (_object_t*)(tpptr->e2); \
	_object_t* val = (_object_t*)(tpptr->e3);
#define _instruct_common(__tuple) \
	_instruct_head(__tuple) \
	opndv1.type = (opnd1->type == _DT_INT || (opnd1->type == _DT_VAR && opnd1->data.variable->data->type == _DT_INT)) ? \
		_DT_INT : _DT_REAL; \
	opndv1.data = opnd1->type == _DT_VAR ? opnd1->data.variable->data->data : opnd1->data; \
	opndv2.type = (opnd2->type == _DT_INT || (opnd2->type == _DT_VAR && opnd2->data.variable->data->type == _DT_INT)) ? \
		_DT_INT : _DT_REAL; \
	opndv2.data = opnd2->type == _DT_VAR ? opnd2->data.variable->data->data : opnd2->data;
#define _instruct_fun_num_num(__optr, __tuple) \
	do { \
		_instruct_common(__tuple) \
		if(opndv1.type == _DT_INT && opndv2.type == _DT_INT) { \
			val->type = _DT_REAL; \
			val->data.float_point = (real_t)__optr((real_t)opndv1.data.integer, (real_t)opndv2.data.integer); \
		} else { \
			val->type = _DT_REAL; \
			val->data.float_point = (real_t)__optr( \
				opndv1.type == _DT_INT ? opndv1.data.integer : opndv1.data.float_point, \
				opndv2.type == _DT_INT ? opndv2.data.integer : opndv2.data.float_point); \
		} \
		_convert_to_int_if_posible(val); \
	} while(0)
#define _instruct_bool_op_bool(__optr, __tuple) \
	do { \
		_instruct_common(__tuple) \
		if(opndv1.type == _DT_NIL) { opndv1.type = _DT_INT; opndv1.data.integer = 0; } \
		else if(opndv1.type != _DT_INT && opndv1.type != _DT_REAL) { opndv1.type = _DT_INT; opndv1.data.integer = 1; } \
		if(opndv2.type == _DT_NIL) { opndv2.type = _DT_INT; opndv2.data.integer = 0; } \
		else if(opndv2.type != _DT_INT && opndv2.type != _DT_REAL) { opndv2.type = _DT_INT; opndv2.data.integer = 1; } \
		if(opndv1.type == _DT_INT && opndv2.type == _DT_INT) { \
			if((real_t)(opndv1.data.integer __optr opndv2.data.integer) == ((real_t)opndv1.data.integer __optr (real_t)opndv2.data.integer)) { \
				val->type = _DT_INT; \
				val->data.integer = opndv1.data.integer __optr opndv2.data.integer; \
			} else { \
				val->type = _DT_REAL; \
				val->data.float_point = (real_t)((real_t)opndv1.data.integer __optr (real_t)opndv2.data.integer); \
			} \
		} else { \
			val->type = _DT_REAL; \
			val->data.float_point = (real_t) \
				((opndv1.type == _DT_INT ? opndv1.data.integer : opndv1.data.float_point) __optr \
				(opndv2.type == _DT_INT ? opndv2.data.integer : opndv2.data.float_point)); \
		} \
		_convert_to_int_if_posible(val); \
	} while(0)
#define _instruct_int_op_int(__optr, __tuple) \
	do { \
		_instruct_common(__tuple) \
		if(opndv1.type == _DT_INT && opndv2.type == _DT_INT) { \
			val->type = _DT_INT; \
			val->data.integer = opndv1.data.integer __optr opndv2.data.integer; \
		} else { \
			val->type = _DT_INT; \
			val->data.integer = \
				((opndv1.type == _DT_INT ? opndv1.data.integer : (int_t)(opndv1.data.float_point)) __optr \
				(opndv2.type == _DT_INT ? opndv2.data.integer : (int_t)(opndv2.data.float_point))); \
		} \
	} while(0)
#define _instruct_num_op_num(__optr, __tuple) \
	do { \
		_instruct_common(__tuple) \
		if(opndv1.type == _DT_INT && opndv2.type == _DT_INT) { \
			if((real_t)(opndv1.data.integer __optr opndv2.data.integer) == ((real_t)opndv1.data.integer __optr (real_t)opndv2.data.integer)) { \
				val->type = _DT_INT; \
				val->data.integer = opndv1.data.integer __optr opndv2.data.integer; \
			} else { \
				val->type = _DT_REAL; \
				val->data.float_point = (real_t)((real_t)opndv1.data.integer __optr (real_t)opndv2.data.integer); \
			} \
		} else { \
			val->type = _DT_REAL; \
			val->data.float_point = (real_t) \
				((opndv1.type == _DT_INT ? opndv1.data.integer : opndv1.data.float_point) __optr \
				(opndv2.type == _DT_INT ? opndv2.data.integer : opndv2.data.float_point)); \
		} \
		_convert_to_int_if_posible(val); \
	} while(0)
#define _instruct_obj_op_obj(__optr, __tuple) \
	do { \
		_instruct_head(__tuple); \
		opndv1.type = opnd1->type == _DT_VAR ? opnd1->data.variable->data->type : opnd1->type; \
		opndv1.data = opnd1->type == _DT_VAR ? opnd1->data.variable->data->data : opnd1->data; \
		opndv2.type = opnd2->type == _DT_VAR ? opnd2->data.variable->data->type : opnd2->type; \
		opndv2.data = opnd2->type == _DT_VAR ? opnd2->data.variable->data->data : opnd2->data; \
		val->type = _DT_INT; \
		if(opndv1.type == opndv2.type) { \
			val->data.integer = (int_t)(mb_memcmp(&opndv1.data, &opndv2.data, sizeof(_raw_t)) __optr 0); \
		} else { \
			val->data.integer = (int_t)(opndv1.type __optr opndv2.type); \
		} \
	} while(0)
#define _instruct_connect_strings(__tuple) \
	do { \
		char* _str1 = 0; \
		char* _str2 = 0; \
		_tuple3_t* tpptr = (_tuple3_t*)(*__tuple); \
		_object_t* opnd1 = (_object_t*)(tpptr->e1); \
		_object_t* opnd2 = (_object_t*)(tpptr->e2); \
		_object_t* val = (_object_t*)(tpptr->e3); \
		val->type = _DT_STRING; \
		if(val->data.string) { \
			safe_free(val->data.string); \
		} \
		_str1 = _extract_string(opnd1); \
		_str2 = _extract_string(opnd2); \
		val->data.string = (char*)mb_malloc(strlen(_str1) + strlen(_str2) + 1); \
		memset(val->data.string, 0, strlen(_str1) + strlen(_str2) + 1); \
		strcat(val->data.string, _str1); \
		strcat(val->data.string, _str2); \
	} while(0)
#define _instruct_compare_strings(__optr, __tuple) \
	do { \
		char* _str1 = 0; \
		char* _str2 = 0; \
		_tuple3_t* tpptr = (_tuple3_t*)(*__tuple); \
		_object_t* opnd1 = (_object_t*)(tpptr->e1); \
		_object_t* opnd2 = (_object_t*)(tpptr->e2); \
		_object_t* val = (_object_t*)(tpptr->e3); \
		val->type = _DT_INT; \
		_str1 = _extract_string(opnd1); \
		_str2 = _extract_string(opnd2); \
		val->data.integer = strcmp(_str1, _str2) __optr 0; \
	} while(0)
#ifdef MB_ENABLE_USERTYPE_REF
#	if !defined _instruct_obj_meta_obj
#		define _instruct_obj_meta_obj(__s, __tuple, __optr, __result, __exit) \
			do { \
				_tuple3_t* tpptr = (_tuple3_t*)(*__tuple); \
				_object_t* opnd1 = (_object_t*)(tpptr->e1); \
				_object_t* opnd2 = (_object_t*)(tpptr->e2); \
				_object_t* retval = (_object_t*)(tpptr->e3); \
				if(opnd1->type == _DT_VAR) opnd1 = opnd1->data.variable->data; \
				if(opnd2->type == _DT_VAR) opnd2 = opnd2->data.variable->data; \
				{ \
					mb_value_t vfst, vscd; \
					mb_value_t ret; \
					mb_make_nil(vfst); \
					mb_make_nil(vscd); \
					mb_make_nil(ret); \
					if(opnd1->type == _DT_USERTYPE_REF && opnd1->data.usertype_ref->calc_operators && opnd1->data.usertype_ref->calc_operators->__optr) { \
						_internal_object_to_public_value(opnd1, &vfst); \
						_internal_object_to_public_value(opnd2, &vscd); \
						__result = opnd1->data.usertype_ref->calc_operators->__optr((__s), (__tuple), &vfst, &vscd, &ret); \
						_public_value_to_internal_object(&ret, retval); \
						goto __exit; \
					} else if(opnd2->type == _DT_USERTYPE_REF && opnd2->data.usertype_ref->calc_operators && opnd2->data.usertype_ref->calc_operators->__optr) { \
						_internal_object_to_public_value(opnd1, &vfst); \
						_internal_object_to_public_value(opnd2, &vscd); \
						__result = opnd2->data.usertype_ref->calc_operators->__optr((__s), (__tuple), &vfst, &vscd, &ret); \
						_public_value_to_internal_object(&ret, retval); \
						goto __exit; \
					} \
				} \
			} while(0)
#	endif /* _instruct_obj_meta_obj */
#endif /* MB_ENABLE_USERTYPE_REF */
#ifndef _instruct_obj_meta_obj
#	define _instruct_obj_meta_obj(__s, __tuple, __optr, __result, __exit) do { ((void)(__s)); ((void)(__tuple)); ((void)(__result)); } while(0)
#endif /* _instruct_obj_meta_obj */
#define _proc_div_by_zero(__s, __tuple, __cast, __exit, __result, __kind) \
	do { \
		_instruct_common(__tuple) \
		if((opndv2.type == _DT_INT && opndv2.data.integer == 0) || (opndv2.type == _DT_REAL && (__cast)opndv2.data.float_point == 0)) { \
			if((opndv1.type == _DT_INT && opndv1.data.integer == 0) || (opndv1.type == _DT_REAL && opndv1.data.float_point == 0)) { \
				val->type = _DT_REAL; \
				_set_real_with_hex(val->data.float_point, MB_FNAN); \
			} else { \
				val->type = _DT_REAL; \
				_set_real_with_hex(val->data.float_point, MB_FINF); \
			} \
			_handle_error_on_obj((__s), __kind, (__s)->source_file, ((__tuple) && *(__tuple)) ? ((_object_t*)(((_tuple3_t*)(*(__tuple)))->e1)) : 0, MB_FUNC_WARNING, __exit, __result); \
		} \
	} while(0)

#define _set_tuple3_result(__l, __r) \
	do { \
		_object_t* val = (_object_t*)(((_tuple3_t*)(*(__l)))->e3); \
		val->type = _DT_INT; \
		val->data.integer = __r; \
	} while(0)

#define _math_calculate_fun_real(__s, __l, __a, __f, __exit, __result) \
	do { \
		switch((__a).type) { \
		case MB_DT_INT: \
			(__a).value.float_point = (real_t)__f((real_t)(__a).value.integer); \
			(__a).type = MB_DT_REAL; \
			break; \
		case MB_DT_REAL: \
			(__a).value.float_point = (real_t)__f((__a).value.float_point); \
			break; \
		default: \
			_handle_error_on_obj(__s, SE_RN_NUMBER_EXPECTED, (__s)->source_file, ((__l) && *(__l)) ? ((_object_t*)(((_tuple3_t*)(*(__l)))->e1)) : 0, MB_FUNC_WARNING, __exit, __result); \
			break; \
		} \
		mb_convert_to_int_if_posible(__a); \
	} while(0)

#define _using_jump_set_of_instructional(__s, __obj, __exit, __result) \
	do { \
		if((__s)->jump_set & (~_JMP_INS)) { \
			_handle_error_on_obj(__s, SE_RN_CANNOT_MIX_INSTRUCTIONAL_AND_STRUCTURED, (__s)->source_file, DON(__obj), MB_FUNC_ERR, __exit, __result); \
		} else { \
			(__s)->jump_set |= _JMP_INS; \
		} \
	} while(0)
#define _using_jump_set_of_structured(__s, __obj, __exit, __result) \
	do { \
		if((__s)->jump_set & (~_JMP_STR)) { \
			_handle_error_on_obj(__s, SE_RN_CANNOT_MIX_INSTRUCTIONAL_AND_STRUCTURED, (__s)->source_file, DON(__obj), MB_FUNC_ERR, __exit, __result); \
		} else { \
			(__s)->jump_set |= _JMP_STR; \
		} \
	} while(0)

/* ========================================================} */

/*
** {========================================================
** Private function declarations
*/

/** List operations */

int _ls_cmp_data(void* node, void* info);
int _ls_cmp_extra(void* node, void* info);
int _ls_cmp_extra_object(void* node, void* info);
int _ls_cmp_extra_string(void* node, void* info);
#ifdef MB_ENABLE_MODULE
int _ls_cmp_module_func(void* node, void* info);
#endif /* MB_ENABLE_MODULE */

_ls_node_t* _ls_create_node(void* data);
_ls_node_t* _ls_create(void);
_ls_node_t* _ls_find(_ls_node_t* list, void* data, _ls_compare_t cmp, int* idx);
_ls_node_t* _ls_back(_ls_node_t* node);
_ls_node_t* _ls_pushback(_ls_node_t* list, void* data);
void* _ls_popback(_ls_node_t* list);
_ls_node_t* _ls_front(_ls_node_t* node);
void* _ls_popfront(_ls_node_t* list);
_ls_node_t* _ls_insert_at(_ls_node_t* list, int index, void* data);
unsigned _ls_remove(_ls_node_t* list, _ls_node_t* node, _ls_operation_t op);
unsigned _ls_try_remove(_ls_node_t* list, void* info, _ls_compare_t cmp, _ls_operation_t op);
unsigned _ls_foreach(_ls_node_t* list, _ls_operation_t op);
_ls_node_t* _ls_sort(_ls_node_t* _UNALIGNED_ARG * list, _ls_compare_t cmp);
unsigned _ls_count(_ls_node_t* list);
bool_t _ls_empty(_ls_node_t* list);
void _ls_clear(_ls_node_t* list);
void _ls_destroy(_ls_node_t* list);
int _ls_free_extra(void* data, void* extra);
#define _LS_FOREACH(L, O, P, E) \
	do { \
		_ls_node_t* __lst = L; \
		int __opresult = _OP_RESULT_NORMAL; \
		_ls_node_t* __tmp = 0; \
		mb_assert(L); \
		__lst = __lst->next; \
		while(__lst) { \
			if(P != 0) { \
				P(__lst->data, __lst->extra, E); \
			} \
			if(O != 0) { \
				__opresult = O(__lst->data, __lst->extra); \
			} \
			__tmp = __lst; \
			__lst = __lst->next; \
			if(_OP_RESULT_DEL_NODE == __opresult) { \
				__tmp->prev->next = __lst; \
				if(__lst) { \
					__lst->prev = __tmp->prev; \
				} \
				safe_free(__tmp); \
				(L)->data = (char*)(L)->data - 1; \
			} \
		} \
	} while(0)

/** Dictionary operations */

unsigned _ht_hash_object(void* ht, void* d);
unsigned _ht_hash_string(void* ht, void* d);
unsigned _ht_hash_intptr(void* ht, void* d);
unsigned _ht_hash_ref(void* ht, void* d);

int _ht_cmp_object(void* d1, void* d2);
int _ht_cmp_string(void* d1, void* d2);
int _ht_cmp_intptr(void* d1, void* d2);
int _ht_cmp_ref(void* d1, void* d2);

_ht_node_t* _ht_create(unsigned size, _ht_compare_t cmp, _ht_hash_t hs, _ls_operation_t freeextra);
_ls_node_t* _ht_find(_ht_node_t* ht, void* key);
unsigned _ht_set_or_insert(_ht_node_t* ht, void* key, void* value);
unsigned _ht_remove(_ht_node_t* ht, void* key, _ls_compare_t cmp);
unsigned _ht_foreach(_ht_node_t* ht, _ht_operation_t op);
unsigned _ht_count(_ht_node_t* ht);
void _ht_clear(_ht_node_t* ht);
void _ht_destroy(_ht_node_t* ht);
int _ht_remove_existing(void* data, void* extra, _ht_node_t* ht);
#define _HT_FOREACH(H, O, P, E) \
	do { \
		_ls_node_t* __bucket = 0; \
		unsigned __ul = 0; \
		if((H)->array) { \
			for(__ul = 0; __ul < (H)->array_size; ++__ul) { \
				__bucket = (H)->array[__ul]; \
				if(__bucket) { \
					_LS_FOREACH(__bucket, O, P, E); \
				} \
			} \
		} \
	} while(0)

/** Memory manipulations */

void _init_dynamic_buffer(_dynamic_buffer_t* buf);
void _dispose_dynamic_buffer(_dynamic_buffer_t* buf);
size_t _countof_dynamic_buffer(_dynamic_buffer_t* buf, size_t es);
void _resize_dynamic_buffer(_dynamic_buffer_t* buf, size_t es, size_t c);

#define _INIT_BUF(b) do { _init_dynamic_buffer(&(b)); } while(0)
#define _DISPOSE_BUF(b) do { _dispose_dynamic_buffer(&(b)); } while(0)
#define _CHARS_OF_BUF(b) (_countof_dynamic_buffer((&b), sizeof(char)))
#define _RESIZE_CHAR_BUF(b, c) do { _resize_dynamic_buffer(&(b), sizeof(char), (c)); } while(0)
#define _HEAP_CHAR_BUF(b) (((b).pointer.charp != (b).bytes) ? ((b).pointer.charp) : (mb_memdup((b).pointer.charp, (unsigned)(b).size)))
#define _CHAR_BUF_PTR(b) ((b).pointer.charp)
#if defined MB_CP_VC && defined MB_ENABLE_UNICODE
#define _WCHARS_OF_BUF(b) (_countof_dynamic_buffer((&b), sizeof(wchar_t)))
#define _RESIZE_WCHAR_BUF(b, c) do { _resize_dynamic_buffer(&(b), sizeof(wchar_t), (c)); } while(0)
#define _WCHAR_BUF_PTR(b) ((b).pointer.wcharp)
#endif /* MB_CP_VC && MB_ENABLE_UNICODE */

#define _MB_CHECK_MEM_TAG_SIZE(y, s) ((y)(mb_mem_tag_t)(s) == (s))
#define _MB_WRITE_MEM_TAG_SIZE(t, s) (*((mb_mem_tag_t*)((char*)(t) - _MB_MEM_TAG_SIZE)) = (mb_mem_tag_t)(s))
#define _MB_READ_MEM_TAG_SIZE(t) (*((mb_mem_tag_t*)((char*)(t) - _MB_MEM_TAG_SIZE)))

#ifdef MB_ENABLE_ALLOC_STAT
extern volatile size_t _mb_allocated;
#else /* MB_ENABLE_ALLOC_STAT */
extern const size_t _mb_allocated;
#endif /* MB_ENABLE_ALLOC_STAT */

extern mb_memory_allocate_func_t _mb_allocate_func;
extern mb_memory_free_func_t _mb_free_func;

void* mb_malloc(size_t s);
void mb_free(void* p);

int mb_memcmp(void* l, void* r, size_t s);
size_t mb_memtest(void* p, size_t s);

char* mb_strdup(const char* p, size_t s);
char* mb_strupr(char* s);

#define safe_free(__p) do { if(__p) { mb_free(__p); __p = 0; } else { mb_assert(0 && "Memory already released."); } } while(0)

bool_t mb_is_little_endian(void);

/** Unicode handling */

#if defined MB_CP_VC && defined MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING
int mb_bytes_to_wchar(const char* sz, wchar_t** out, size_t size);
int mb_bytes_to_wchar_ansi(const char* sz, wchar_t** out, size_t size);
int mb_wchar_to_bytes(const wchar_t* sz, char** out, size_t size);
#endif /* MB_CP_VC && MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING */

int mb_uu_getbom(const char** ch);
#ifdef MB_ENABLE_UNICODE
int mb_uu_ischar(const char* ch);
int mb_uu_strlen(const char* ch);
int mb_uu_substr(const char* ch, int begin, int count, char** o);
#endif /* MB_ENABLE_UNICODE */

/** Expression processing */

#ifndef _ONCALC /* Uprootable stub */
#	define _ONCALC(__s, __tuple, __optr, __result, __exit) do { ((void)(__s)); ((void)(__tuple)); ((void)(__result)); } while(0)
#endif /* _ONCALC */
#ifndef _ONNEG /* Uprootable stub */
#	define _ONNEG(__s, __l, __arg, __result, __exit) do { ((void)(__s)); ((void)(__l)); ((void)(__arg)); ((void)(__result)); } while(0)
#endif /* _ONNEG */

#ifndef _ONCOND /* Uprootable stub */
#	define _ONCOND(__s, __o, __v) do { ((void)(__s)); ((void)(__o)); ((void)(__v)); } while(0)
#endif /* _ONCOND */

bool_t _is_operator(mb_func_t op);
bool_t _is_flow(mb_func_t op);
bool_t _is_unary(mb_func_t op);
bool_t _is_binary(mb_func_t op);
char _get_priority(mb_func_t op1, mb_func_t op2);
int _get_priority_index(mb_func_t op);
_object_t* _operate_operand(mb_interpreter_t* s, _object_t* optr, _object_t* opnd1, _object_t* opnd2, int* status);
bool_t _is_expression_terminal(mb_interpreter_t* s, _object_t* obj);
bool_t _is_unexpected_calc_type(mb_interpreter_t* s, _object_t* obj);
bool_t _is_referenced_calc_type(mb_interpreter_t* s, _object_t* obj);
int _calc_expression(mb_interpreter_t* s, _ls_node_t** l, _object_t** val);

#ifndef _PREVCALL /* Uprootable stub */
#	define _PREVCALL(__s, __l, __r) do { ((void)(__s)); ((void)(__l)); ((void)(__r)); } while(0)
#endif /* _PREVCALL */
#ifndef _POSTCALL /* Uprootable stub */
#	define _POSTCALL(__s, __l, __r) do { ((void)(__s)); ((void)(__l)); ((void)(__r)); } while(0)
#endif /* _POSTCALL */

_ls_node_t* _push_var_args(mb_interpreter_t* s);
void _pop_var_args(mb_interpreter_t* s, _ls_node_t* last_var_args);
int _pop_arg(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, unsigned* ia, _routine_t* r, mb_pop_routine_arg_func_t pop_arg, _ls_node_t* args, mb_value_t* arg);
int _proc_args(mb_interpreter_t* s, _ls_node_t** l, _running_context_t* running, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg, bool_t proc_ref, _ls_node_t* args);
int _eval_routine(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg);
int _eval_script_routine(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg);
#ifdef MB_ENABLE_LAMBDA
int _eval_lambda_routine(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg);
#endif /* MB_ENABLE_LAMBDA */
int _eval_native_routine(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg);
int _has_routine_lex_arg(mb_interpreter_t* s, void** l, mb_value_t* va, unsigned ca, unsigned* ia, void* r);
int _pop_routine_lex_arg(mb_interpreter_t* s, void** l, mb_value_t* va, unsigned ca, unsigned* ia, void* r, mb_value_t* val);
int _has_routine_fun_arg(mb_interpreter_t* s, void** l, mb_value_t* va, unsigned ca, unsigned* ia, void* r);
int _pop_routine_fun_arg(mb_interpreter_t* s, void** l, mb_value_t* va, unsigned ca, unsigned* ia, void* r, mb_value_t* val);
bool_t _is_print_terminal(mb_interpreter_t* s, _object_t* obj);
mb_meta_status_e _try_overridden(mb_interpreter_t* s, void** l, mb_value_t* d, const char* f, mb_meta_func_e t);

/** Handlers */

#if _WARNING_AS_ERROR
#	define _handle_error_now(__s, __err, __f, __result) \
		do { \
			_set_current_error((__s), (__err), (__f)); \
			if((__s)->error_handler) { \
				if((__s)->handled_error) \
					break; \
				(__s)->handled_error = true; \
				((__s)->error_handler)((__s), (__s)->last_error, (char*)mb_get_error_desc((__s)->last_error), \
					(__s)->last_error_file, \
					(__s)->parsing_context && !(__s)->run_count ? (__s)->parsing_context->parsing_pos : (__s)->last_error_pos, \
					(__s)->parsing_context && !(__s)->run_count ? (__s)->parsing_context->parsing_row : (__s)->last_error_row, \
					(__s)->parsing_context && !(__s)->run_count ? (__s)->parsing_context->parsing_col : (__s)->last_error_col, \
					(__result)); \
			} \
		} while(0)
#	define _handle_error_at_pos(__s, __err, __f, __pos, __row, __col, __ret, __exit, __result) \
		do { \
			if(_set_current_error((__s), (__err), (__f))) { \
				_set_error_pos((__s), (__pos), (__row), (__col)); \
				__result = (__ret); \
			} \
			goto __exit; \
		} while(0)
#else /* _WARNING_AS_ERROR */
#	define _handle_error_now(__s, __err, __f, __result) \
		do { \
			_set_current_error((__s), (__err), (__f)); \
			if((__s)->error_handler) { \
				if((__s)->handled_error) \
					break; \
				(__s)->handled_error = true; \
				((__s)->error_handler)((__s), (__s)->last_error, (char*)mb_get_error_desc((__s)->last_error), \
					(__s)->last_error_file, \
					(__s)->parsing_context && !(__s)->run_count ? (__s)->parsing_context->parsing_pos : (__s)->last_error_pos, \
					(__s)->parsing_context && !(__s)->run_count ? (__s)->parsing_context->parsing_row : (__s)->last_error_row, \
					(__s)->parsing_context && !(__s)->run_count ? (__s)->parsing_context->parsing_col : (__s)->last_error_col, \
					(__result)); \
			} \
			if((__result) == MB_FUNC_WARNING) { \
				(__s)->last_error = SE_NO_ERR; \
				(__s)->last_error_file = 0; \
				(__s)->handled_error = false; \
			} \
		} while(0)
#	define _handle_error_at_pos(__s, __err, __f, __pos, __row, __col, __ret, __exit, __result) \
		do { \
			if(_set_current_error((__s), (__err), (__f))) { \
				_set_error_pos((__s), (__pos), (__row), (__col)); \
				if((__ret) != MB_FUNC_WARNING) { \
					__result = (__ret); \
				} \
			} \
			goto __exit; \
		} while(0)
#endif /* _WARNING_AS_ERROR */
#ifdef MB_ENABLE_SOURCE_TRACE
#	define _handle_error_on_obj(__s, __err, __f, __obj, __ret, __exit, __result) \
		do { \
			if(__obj) { \
				_handle_error_at_pos((__s), (__err), (__f), (__obj)->source_pos, (__obj)->source_row, (__obj)->source_col, (__ret), __exit, (__result)); \
			} else { \
				_handle_error_at_pos((__s), (__err), (__f), 0, 0, 0, (__ret), __exit, (__result)); \
			} \
		} while(0)
#else /* MB_ENABLE_SOURCE_TRACE */
#	define _handle_error_on_obj(__s, __err, __f, __obj, __ret, __exit, __result) \
		do { ((void)(__obj)); _handle_error_at_pos((__s), (__err), (__f), 0, 0, 0, (__ret), __exit, (__result)); } while(0)
#endif /* MB_ENABLE_SOURCE_TRACE */

#define _OUTTER_SCOPE(__s) ((__s)->prev ? (__s)->prev : (__s))

bool_t _set_current_error(mb_interpreter_t* s, mb_error_e err, char* f);

mb_print_func_t _get_printer(mb_interpreter_t* s);
mb_input_func_t _get_inputer(mb_interpreter_t* s);

int _standard_printer(mb_interpreter_t* s, const char* fmt, ...);

void _print_string(mb_interpreter_t* s, _object_t* obj);

/** Parsing helpers */

char* _load_file(mb_interpreter_t* s, const char* f, const char* prefix, bool_t importing);
void _end_of_file(_parsing_context_t* context);

#define _ZERO_CHAR '\0'
#define _NEWLINE_CHAR '\n'
#define _RETURN_CHAR '\r'
#define _STRING_POSTFIX_CHAR '$'
#define _DUMMY_ASSIGN_CHAR "#"
#define _INVALID_CLASS_CHAR 0x18
#define _INVALID_ROUTINE_CHAR 0x1B

#define _REMARK_STR "REM"

bool_t _is_blank_char(char c);
bool_t _is_eof_char(char c);
bool_t _is_newline_char(char c);
bool_t _is_separator_char(char c);
bool_t _is_bracket_char(char c);
bool_t _is_quotation_char(char c);
bool_t _is_comment_char(char c);
bool_t _is_accessor_char(char c);
bool_t _is_numeric_char(char c);
bool_t _is_identifier_char(char c);
bool_t _is_operator_char(char c);
bool_t _is_exponential_char(char c);
bool_t _is_using_at_char(char c);
bool_t _is_exponent_prefix(char* s, int begin, int end);

int _append_char_to_symbol(mb_interpreter_t* s, char c);
#ifdef MB_ENABLE_UNICODE_ID
int _append_uu_char_to_symbol(mb_interpreter_t* s, const char* str, int n);
#endif /* MB_ENABLE_UNICODE_ID */
int _cut_symbol(mb_interpreter_t* s, int pos, unsigned short row, unsigned short col);
int _append_symbol(mb_interpreter_t* s, char* sym, bool_t* delsym, int pos, unsigned short row, unsigned short col);
int _create_symbol(mb_interpreter_t* s, _ls_node_t* l, char* sym, _object_t** obj, _ls_node_t*** asgn, bool_t* delsym);
_data_e _get_symbol_type(mb_interpreter_t* s, char* sym, _raw_t* value);
int _parse_char(mb_interpreter_t* s, const char* str, int n, int pos, unsigned short row, unsigned short col);
void _set_error_pos(mb_interpreter_t* s, int pos, unsigned short row, unsigned short col);
char* _prev_import(mb_interpreter_t* s, char* lf, int* pos, unsigned short* row, unsigned short* col);
char* _post_import(mb_interpreter_t* s, char* lf, int* pos, unsigned short* row, unsigned short* col);

/** Object processors */

#define DON(__o) ((__o) ? ((_object_t*)((__o)->data)) : 0)
#define DON2(__a) (((__a) && *(__a)) ? (_object_t*)((*((_ls_node_t**)(__a)))->data) : 0)
#define TON(__t) (((__t) && *(__t)) ? (_object_t*)(((_tuple3_t*)(*(__t)))->e1) : 0)

#define _IS_EOS(__o) (__o && ((_object_t*)(__o))->type == _DT_EOS)
#define _IS_SEP(__o, __c) (((_object_t*)(__o))->type == _DT_SEP && ((_object_t*)(__o))->data.separator == __c)
#define _IS_FUNC(__o, __f) (((_object_t*)(__o))->type == _DT_FUNC && ((_object_t*)(__o))->data.func->pointer == __f)
#define _IS_UNARY_FUNC(__o) (((_object_t*)(__o))->type == _DT_FUNC && _is_unary(((_object_t*)(__o))->data.func->pointer))
#define _IS_VAR(__o) ((__o) && ((_object_t*)(__o))->type == _DT_VAR)
#ifdef MB_ENABLE_COLLECTION_LIB
#	define _IS_LIST(__o) ((__o) && ((_object_t*)(__o))->type == _DT_LIST)
#	define _IS_DICT(__o) ((__o) && ((_object_t*)(__o))->type == _DT_DICT)
#	define _IS_COLL(__o) (_IS_LIST(__o) || _IS_DICT(__o))
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
#	define _IS_CLASS(__o) ((__o) && ((_object_t*)(__o))->type == _DT_CLASS)
#	define _GET_CLASS(__o) ((!__o) ? 0 : (_IS_CLASS(__o) ? (__o) : (_IS_VAR(__o) && _IS_CLASS((__o)->data.variable->data) ? (__o)->data.variable->data : 0)))
#	define _IS_ME(__v) (!!(__v)->is_me)
#else /* MB_ENABLE_CLASS */
#	define _IS_ME(__v) false
#endif /* MB_ENABLE_CLASS */
#define _IS_ROUTINE(__o) ((__o) && ((_object_t*)(__o))->type == _DT_ROUTINE)
#define _GET_ROUTINE(__o) ((!__o) ? 0 : (_IS_ROUTINE(__o) ? (__o) : (_IS_VAR(__o) && _IS_ROUTINE((__o)->data.variable->data) ? (__o)->data.variable->data : 0)))

#ifdef MB_ENABLE_USERTYPE_REF
#	define _REF_USERTYPE_REF(__o) \
		case _DT_USERTYPE_REF: \
			_ref(&(__o)->data.usertype_ref->ref, (__o)->data.usertype_ref); \
			break;
#	define _UNREF_USERTYPE_REF(__o) \
		case _DT_USERTYPE_REF: \
			_unref(&(__o)->data.usertype_ref->ref, (__o)->data.usertype_ref); \
			break;
#	define _ADDGC_USERTYPE_REF(__o, __g) \
		case _DT_USERTYPE_REF: \
			_gc_add(&(__o)->data.usertype_ref->ref, (__o)->data.usertype_ref, (__g)); \
			break;
#else /* MB_ENABLE_USERTYPE_REF */
#	define _REF_USERTYPE_REF(__o) { (void)(__o); }
#	define _UNREF_USERTYPE_REF(__o) { (void)(__o); }
#	define _ADDGC_USERTYPE_REF(__o, __g) { (void)(__o); (void)(__g); }
#endif /* MB_ENABLE_USERTYPE_REF */
#ifdef MB_ENABLE_ARRAY_REF
#	define _REF_ARRAY(__o) \
		case _DT_ARRAY: \
			if(!(__o)->is_ref) \
				_ref(&(__o)->data.array->ref, (__o)->data.array); \
			break;
#	define _UNREF_ARRAY(__o) \
		case _DT_ARRAY: \
			if(!(__o)->is_ref) \
				_unref(&(__o)->data.array->ref, (__o)->data.array); \
			break;
#	define _ADDGC_ARRAY(__o, __g) \
		case _DT_ARRAY: \
			if(!(__o)->is_ref) \
				_gc_add(&(__o)->data.array->ref, (__o)->data.array, (__g)); \
			break;
#else /* MB_ENABLE_ARRAY_REF */
#	define _REF_ARRAY(__o) case _DT_ARRAY: { (void)(__o); } break;
#	define _UNREF_ARRAY(__o) case _DT_ARRAY: { (void)(__o); } break;
#	define _ADDGC_ARRAY(__o, __g) case _DT_ARRAY: { (void)(__o); (void)(__g); } break;
#	define _DESTROY_ARRAY(__o) \
		case _DT_ARRAY: \
			if(!(__o)->is_ref) \
				_destroy_array((__o)->data.array); \
			break;
#endif /* MB_ENABLE_ARRAY_REF */
#ifdef MB_ENABLE_COLLECTION_LIB
#	define _REF_COLL(__o) \
		case _DT_LIST: \
			_ref(&(__o)->data.list->ref, (__o)->data.list); \
			break; \
		case _DT_DICT: \
			_ref(&(__o)->data.dict->ref, (__o)->data.dict); \
			break;
#	define _UNREF_COLL(__o) \
		case _DT_LIST: \
			_unref(&(__o)->data.list->ref, (__o)->data.list); \
			break; \
		case _DT_DICT: \
			_unref(&(__o)->data.dict->ref, (__o)->data.dict); \
			break;
#	define _ADDGC_COLL(__o, __g) \
		case _DT_LIST: \
			_gc_add(&(__o)->data.list->ref, (__o)->data.list, (__g)); \
			break; \
		case _DT_DICT: \
			_gc_add(&(__o)->data.dict->ref, (__o)->data.dict, (__g)); \
			break;
#	define _UNREF_COLL_IT(__o) \
		case _DT_LIST_IT: \
			_destroy_list_it((__o)->data.list_it); \
			break; \
		case _DT_DICT_IT: \
			_destroy_dict_it((__o)->data.dict_it); \
			break;
#	define _ADDGC_COLL_IT(__o, __g) \
		case _DT_LIST_IT: \
			_destroy_list_it((__o)->data.list_it); \
			break; \
		case _DT_DICT_IT: \
			_destroy_dict_it((__o)->data.dict_it); \
			break;
#else /* MB_ENABLE_COLLECTION_LIB */
#	define _REF_COLL(__o) { (void)(__o); }
#	define _UNREF_COLL(__o) { (void)(__o); }
#	define _ADDGC_COLL(__o, __g) { (void)(__o); (void)(__g); }
#	define _UNREF_COLL_IT(__o) { (void)(__o); }
#	define _ADDGC_COLL_IT(__o, __g) { (void)(__o); (void)(__g); }
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
#	define _REF_CLASS(__o) \
		case _DT_CLASS: \
			_ref(&(__o)->data.instance->ref, (__o)->data.instance); \
			break;
#	define _UNREF_CLASS(__o) \
		case _DT_CLASS: \
			if(!(__o)->is_ref) \
				_unref(&(__o)->data.instance->ref, (__o)->data.instance); \
			break;
#	define _ADDGC_CLASS(__o, __g) \
		case _DT_CLASS: \
			if(!(__o)->is_ref) \
				_gc_add(&(__o)->data.instance->ref, (__o)->data.instance, (__g)); \
			break;
#else /* MB_ENABLE_CLASS */
#	define _REF_CLASS(__o) { (void)(__o); }
#	define _UNREF_CLASS(__o) { (void)(__o); }
#	define _ADDGC_CLASS(__o, __g) { (void)(__o); (void)(__g); }
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_LAMBDA
#	define _REF_ROUTINE(__o) \
		case _DT_ROUTINE: \
			if(!(__o)->is_ref && (__o)->data.routine->type == MB_RT_LAMBDA) \
				_ref(&(__o)->data.routine->func.lambda.ref, (__o)->data.routine); \
			break;
#	define _UNREF_ROUTINE(__o) \
		case _DT_ROUTINE: \
			if(!(__o)->is_ref && (__o)->data.routine->type == MB_RT_LAMBDA) \
				_unref(&(__o)->data.routine->func.lambda.ref, (__o)->data.routine); \
			else if(!(__o)->is_ref && (__o)->data.routine->type != MB_RT_LAMBDA) \
				_destroy_routine(0, (__o)->data.routine); \
			break;
#	define _ADDGC_ROUTINE(__o, __g, __r) \
		case _DT_ROUTINE: \
			if(!(__o)->is_ref && (__o)->data.routine->type == MB_RT_LAMBDA) \
				_gc_add(&(__o)->data.routine->func.lambda.ref, (__o)->data.routine, (__g)); \
			else if((__r) && !(__o)->is_ref && (__o)->data.routine->type != MB_RT_LAMBDA) \
				_dispose_object(__o); \
			break;
#	define _COLL_ROUTINE(__o) \
		do { \
			if((__o)->type == _DT_ROUTINE && (__o)->data.routine->type != MB_RT_LAMBDA) \
				(__o)->is_ref = true; \
		} while(0)
#else /* MB_ENABLE_LAMBDA */
#	define _REF_ROUTINE(__o) case _DT_ROUTINE: { (void)(__o); } break;
#	define _UNREF_ROUTINE(__o) case _DT_ROUTINE: { (void)(__o); } break;
#	define _ADDGC_ROUTINE(__o, __g, __r) \
		case _DT_ROUTINE: \
			((void)(__g)); \
			((void)(__r)); \
			_dispose_object(__o); \
			break;
#	define _COLL_ROUTINE(__o) \
		do { \
			if((__o)->type == _DT_ROUTINE) \
				(__o)->is_ref = true; \
		} while(0)
#endif /* MB_ENABLE_LAMBDA */
#define _ADDGC_STRING(__o) \
	case _DT_STRING: \
		_dispose_object(__o); \
		break;
#define _REF(__o) \
	switch((__o)->type) { \
	_REF_USERTYPE_REF(__o) \
	_REF_ARRAY(__o) \
	_REF_COLL(__o) \
	_REF_CLASS(__o) \
	_REF_ROUTINE(__o) \
	default: break; \
	}
#define _UNREF(__o) \
	switch((__o)->type) { \
	_UNREF_USERTYPE_REF(__o) \
	_UNREF_ARRAY(__o) \
	_UNREF_COLL(__o) \
	_UNREF_CLASS(__o) \
	_UNREF_ROUTINE(__o) \
	default: break; \
	}
#define _ADDGC(__o, __g, __r) \
	if(!(__o)->data.pointer || !_ht_find((__g)->collected_table, (__o)->data.pointer)) { \
		switch((__o)->type) { \
		_ADDGC_USERTYPE_REF(__o, __g) \
		_ADDGC_ARRAY(__o, __g) \
		_ADDGC_COLL(__o, __g) \
		_ADDGC_COLL_IT(__o, __g) \
		_ADDGC_CLASS(__o, __g) \
		_ADDGC_ROUTINE(__o, __g, __r) \
		_ADDGC_STRING(__o) \
		default: break; \
		} \
	}
#ifndef _GCNOW /* Uprootable stub */
#	define _GCNOW(__s) (!!(__s))
#endif /* _GCNOW */
#ifndef _PREPAREGC /* Uprootable stub */
#	define _PREPAREGC(__s, __g) do { ((void)(__s)); ((void)(__g)); } while(0)
#endif /* _PREPAREGC */
#ifndef _PREVGC /* Uprootable stub */
#	define _PREVGC(__s, __g) do { ((void)(__s)); ((void)(__g)); } while(0)
#endif /* _PREVGC */
#ifndef _POSTGC /* Uprootable stub */
#	define _POSTGC(__s, __g) do { ((void)(__s)); ((void)(__g)); } while(0)
#endif /* _POSTGC */

int_t _get_size_of(_data_e type);
bool_t _try_get_value(_object_t* obj, mb_value_u* val, _data_e expected);

bool_t _is_nil(void* obj);
bool_t _is_number(void* obj);
bool_t _is_string(void* obj);
char* _extract_string(_object_t* obj);
#ifdef MB_MANUAL_REAL_FORMATTING
void _real_to_str(real_t r, char* str, size_t size, size_t afterpoint);
#endif /* MB_MANUAL_REAL_FORMATTING */
void _real_to_str_std(real_t r, char* str, size_t size);
#ifndef mb_realtostr
#	define mb_realtostr(__r, __s, __z) _real_to_str_std((__r), (__s), (__z))
#endif /* mb_realtostr */

#ifdef _HAS_REF_OBJ_LOCK
bool_t _lock_ref_object(_lock_t* lk, _ref_t* ref, void* obj);
bool_t _unlock_ref_object(_lock_t* lk, _ref_t* ref, void* obj);
bool_t _write_on_ref_object(_lock_t* lk, _ref_t* ref, void* obj);
#endif /* _HAS_REF_OBJ_LOCK */

bool_t _is_ref(_object_t* obj);
_ref_count_t _ref(_ref_t* ref, void* data);
bool_t _unref(_ref_t* ref, void* data);
_ref_count_t _weak_ref(_ref_t* ref, void* data, _ref_t* weak);
bool_t _weak_unref(_ref_t* weak);
void _create_ref(_ref_t* ref, _unref_func_t dtor, _data_e t, mb_interpreter_t* s);
void _destroy_ref(_ref_t* ref);

void _gc_add(_ref_t* ref, void* data, _gc_t* gc);
unsigned _gc_remove(_ref_t* ref, void* data, _gc_t* gc);
int _gc_add_reachable(void* data, void* extra, void* h);
int _gc_add_reachable_both(void* data, void* extra, void* h);
#ifdef MB_ENABLE_FORK
int _gc_get_reachable_in_forked(void* data, void* extra, _ht_node_t* valid);
#endif /* MB_ENABLE_FORK */
void _gc_get_reachable(mb_interpreter_t* s, _ht_node_t* ht, _running_context_t* end);
void _gc_alive_marker(mb_interpreter_t* s, void* h, mb_value_t val);
int _gc_destroy_garbage_in_list(void* data, void* extra, _gc_t* gc);
int _gc_destroy_garbage_in_dict(void* data, void* extra, _gc_t* gc);
#ifdef MB_ENABLE_CLASS
int _gc_destroy_garbage_in_class(void* data, void* extra, _gc_t* gc);
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_LAMBDA
int _gc_destroy_garbage_in_lambda(void* data, void* extra, _gc_t* gc);
void _gc_destroy_garbage_in_outer_scope(_running_context_ref_t* p, _gc_t* gc);
#endif /* MB_ENABLE_LAMBDA */
int _gc_destroy_garbage(void* data, void* extra, _gc_t* gc);
#ifdef MB_ENABLE_CLASS
int _gc_destroy_garbage_class(void* data, void* extra, _gc_t* gc);
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_CLASS
int _gc_destroy_garbage_outer_scope(void* data, void* extra, _gc_t* gc);
#endif /* MB_ENABLE_CLASS */
void _gc_swap_tables(mb_interpreter_t* s);
void _gc_try_trigger(mb_interpreter_t* s);
void _gc_collect_garbage(mb_interpreter_t* s, int depth);

#ifdef MB_ENABLE_USERTYPE_REF
_usertype_ref_t* _create_usertype_ref(mb_interpreter_t* s, void* val, mb_dtor_func_t un, mb_clone_func_t cl, mb_hash_func_t hs, mb_cmp_func_t cp, mb_fmt_func_t ft);
void _destroy_usertype_ref(_usertype_ref_t* c);
void _unref_usertype_ref(_ref_t* ref, void* data);
void _clone_usertype_ref(_usertype_ref_t* src, _object_t* tgt);
bool_t _try_call_func_on_usertype_ref(mb_interpreter_t* s, _ls_node_t** ast, _object_t* obj, _ls_node_t* pathed, int* ret);
#endif /* MB_ENABLE_USERTYPE_REF */

_array_t* _create_array(mb_interpreter_t* s, const char* n, _data_e t);
void _destroy_array(_array_t* arr);
void _init_array(_array_t* arr);
_array_t* _clone_array(mb_interpreter_t* s, _array_t* arr);
int _get_array_pos(mb_interpreter_t* s, _array_t* arr, int* d, int c);
int _get_array_index(mb_interpreter_t* s, _ls_node_t** l, _object_t* c, unsigned* index, bool_t* literally);
bool_t _get_array_elem(mb_interpreter_t* s, _array_t* arr, unsigned index, mb_value_u* val, _data_e* type);
int _set_array_elem(mb_interpreter_t* s, _ls_node_t* ast, _array_t* arr, unsigned index, mb_value_u* val, _data_e* type);
void _clear_array(_array_t* arr);
bool_t _is_array(void* obj);
#ifdef MB_ENABLE_ARRAY_REF
void _unref_array(_ref_t* ref, void* data);
#endif /* MB_ENABLE_ARRAY_REF */

#ifdef MB_ENABLE_CLASS
typedef int (* _class_scope_walker_t)(void*, void*, void*);
typedef bool_t (* _class_meta_walker_t)(_class_t*, void*, void*);
void _init_class(mb_interpreter_t* s, _class_t* instance, char* n);
void _begin_class(mb_interpreter_t* s);
bool_t _end_class(mb_interpreter_t* s);
void _unref_class(_ref_t* ref, void* data);
void _destroy_class(_class_t* c);
bool_t _traverse_class(_class_t* c, _class_scope_walker_t scope_walker, _class_meta_walker_t meta_walker, unsigned meta_depth, bool_t meta_walk_on_self, void* extra_data, void* ret);
bool_t _link_meta_class(mb_interpreter_t* s, _class_t* derived, _class_t* base);
void _unlink_meta_class(mb_interpreter_t* s, _class_t* derived);
int _unlink_meta_instance(void* data, void* extra, _class_t* derived);
int _clone_clsss_field(void* data, void* extra, void* n);
bool_t _clone_class_meta_link(_class_t* meta, void* n, void* ret);
int _search_class_meta_function(mb_interpreter_t* s, _class_t* instance, const char* n, _routine_t* _UNALIGNED_ARG * f);
int _search_class_hash_and_compare_functions(mb_interpreter_t* s, _class_t* instance);
bool_t _is_a_class(_class_t* instance, void* m, void* ret);
bool_t _add_class_meta_reachable(_class_t* meta, void* ht, void* ret);
#ifdef MB_ENABLE_COLLECTION_LIB
int _reflect_class_field(void* data, void* extra, void* d);
#endif /* MB_ENABLE_COLLECTION_LIB */
int _format_class_to_string(mb_interpreter_t* s, void** l, _class_t* instance, _object_t* out, bool_t* got_tostr);
_class_t* _reflect_string_to_class(mb_interpreter_t* s, const char* n, mb_value_t* arg);
bool_t _is_valid_class_accessor_following_routine(mb_interpreter_t* s, _var_t* var, _ls_node_t* ast, _ls_node_t** out);
#endif /* MB_ENABLE_CLASS */
void _init_routine(mb_interpreter_t* s, _routine_t* routine, char* n, mb_routine_func_t f);
int _begin_routine(mb_interpreter_t* s);
bool_t _end_routine(mb_interpreter_t* s);
void _begin_routine_definition(mb_interpreter_t* s);
void _begin_routine_parameter_list(mb_interpreter_t* s);
void _end_routine_parameter_list(mb_interpreter_t* s);
_object_t* _duplicate_parameter(void* data, void* extra, _running_context_t* running);
_routine_t* _clone_routine(_routine_t* sub, void* c, bool_t toupval);
#ifdef MB_ENABLE_LAMBDA
_running_context_t* _init_lambda(mb_interpreter_t* s, _routine_t* routine);
void _unref_routine(_ref_t* ref, void* data);
void _destroy_routine(mb_interpreter_t* s, _routine_t* r);
void _mark_upvalue(mb_interpreter_t* s, _lambda_t* lambda, _object_t* obj, const char* n);
void _try_mark_upvalue(mb_interpreter_t* s, _routine_t* r, _object_t* obj);
_running_context_ref_t* _create_outer_scope(mb_interpreter_t* s);
void _unref_outer_scope(_ref_t* ref, void* data);
void _destroy_outer_scope(_running_context_ref_t* p);
int _do_nothing_on_ht_for_lambda(void* data, void* extra);
int _fill_with_upvalue(void* data, void* extra, _upvalue_scope_tuple_t* tuple);
int _remove_filled_upvalue(void* data, void* extra, _ht_node_t* ht);
int _fill_outer_scope(void* data, void* extra, _upvalue_scope_tuple_t* tuple);
int _remove_this_lambda_from_upvalue(void* data, void* extra, _routine_t* routine);
_running_context_t* _link_lambda_scope_chain(mb_interpreter_t* s, _lambda_t* lambda, bool_t weak);
_running_context_t* _unlink_lambda_scope_chain(mb_interpreter_t* s, _lambda_t* lambda, bool_t weak);
bool_t _is_valid_lambda_body_node(mb_interpreter_t* s, _lambda_t* lambda, _object_t* obj);
#endif /* MB_ENABLE_LAMBDA */
#ifdef MB_ENABLE_CLASS
_running_context_t* _reference_scope_by_class(mb_interpreter_t* s, _running_context_t* p, _class_t* c);
_running_context_t* _push_scope_by_class(mb_interpreter_t* s, _running_context_t* p);
_ls_node_t* _search_identifier_in_class(mb_interpreter_t* s, _class_t* instance, const char* n, _ht_node_t** ht, _running_context_t** sp);
#endif /* MB_ENABLE_CLASS */
_running_context_t* _reference_scope_by_routine(mb_interpreter_t* s, _running_context_t* p, _routine_t* r);
_running_context_t* _push_weak_scope_by_routine(mb_interpreter_t* s, _running_context_t* p, _routine_t* r);
_running_context_t* _push_scope_by_routine(mb_interpreter_t* s, _running_context_t* p);
void _destroy_scope(mb_interpreter_t* s, _running_context_t* p);
_running_context_t* _pop_weak_scope(mb_interpreter_t* s, _running_context_t* p);
_running_context_t* _pop_scope(mb_interpreter_t* s, bool_t tidy);
void _out_of_scope(mb_interpreter_t* s, _running_context_t* running, void* instance, _routine_t* routine, bool_t lose);
_running_context_t* _find_scope(mb_interpreter_t* s, _running_context_t* p);
_running_context_t* _get_root_scope(_running_context_t* scope);
#ifdef MB_ENABLE_LAMBDA
_running_context_ref_t* _get_root_ref_scope(_running_context_ref_t* scope);
#endif /* MB_ENABLE_LAMBDA */
_running_context_t* _get_scope_to_add_routine(mb_interpreter_t* s);
_ls_node_t* _search_identifier_in_scope_chain(mb_interpreter_t* s, _running_context_t* scope, const char* n, int fp, _ht_node_t** ht, _running_context_t** sp);
_array_t* _search_array_in_scope_chain(mb_interpreter_t* s, _array_t* i, _object_t** o);
_var_t* _search_var_in_scope_chain(mb_interpreter_t* s, _var_t* i, _object_t** o);
_ls_node_t* _search_identifier_accessor(mb_interpreter_t* s, _running_context_t* scope, const char* n, _ht_node_t** ht, _running_context_t** sp, bool_t unknown_for_not_found);

_var_t* _create_var(_object_t** oobj, const char* n, size_t ns, bool_t dup_name);
int _retrieve_var(void* data, void* extra, void* t);
_object_t* _create_object(void);
int _clone_object(mb_interpreter_t* s, _object_t* obj, _object_t* tgt, bool_t toupval, bool_t deep);
int _dispose_object(_object_t* obj);
int _destroy_object(void* data, void* extra);
int _destroy_object_with_extra(void* data, void* extra);
int _destroy_object_not_compile_time(void* data, void* extra);
int _destroy_object_capsule_only(void* data, void* extra);
int _do_nothing_on_object(void* data, void* extra);
int _lose_object(void* data, void* extra, _running_context_t* running);
int _remove_source_object(void* data, void* extra);
int _destroy_memory(void* data, void* extra);
int _compare_numbers(const _object_t* first, const _object_t* second);
bool_t _is_internal_object(_object_t* obj);
_data_e _public_type_to_internal_type(mb_data_e t);
mb_data_e _internal_type_to_public_type(_data_e t);
int _public_value_to_internal_object(mb_value_t* pbl, _object_t* itn);
int _internal_object_to_public_value(_object_t* itn, mb_value_t* pbl);
int _create_internal_object_from_public_value(mb_value_t* pbl, _object_t** itn);
int _compare_public_value_and_internal_object(mb_value_t* pbl, _object_t* itn);
void _try_clear_intermediate_value(void* data, void* extra, mb_interpreter_t* s);
void _remove_if_exists(void* data, void* extra, _ls_node_t* ls);
void _destroy_var_arg(void* data, void* extra, _gc_t* gc);
void _destroy_edge_objects(mb_interpreter_t* s);
void _mark_edge_destroy_string(mb_interpreter_t* s, char* ch);
void _destroy_lazy_objects(mb_interpreter_t* s);
void _mark_lazy_destroy_string(mb_interpreter_t* s, char* ch);
void _assign_public_value(mb_interpreter_t* s, mb_value_t* tgt, mb_value_t* src, bool_t pit);
void _swap_public_value(mb_value_t* tgt, mb_value_t* src);
int _clear_scope_chain(mb_interpreter_t* s);
int _dispose_scope_chain(mb_interpreter_t* s);
void _tidy_scope_chain(mb_interpreter_t* s);
void _collect_intermediate_value_in_scope(_running_context_t* running, void* data);
#ifdef MB_ENABLE_FORK
void _collect_intermediate_value_in_scope_chain(void* data, void* extra, void* d);
#endif /* MB_ENABLE_FORK */
void _collect_intermediate_value(_ref_t* ref, void* data);
void _mark_dangling_intermediate_value(mb_interpreter_t* s, _running_context_t* running);
_object_t* _eval_var_in_print(mb_interpreter_t* s, _object_t** val_ptr, _ls_node_t** ast, _object_t* obj);

/** Interpretation */

int _prev_stepped(mb_interpreter_t* s, _ls_node_t* ast);
int _post_stepped(mb_interpreter_t* s, _ls_node_t* ast);
int _execute_statement(mb_interpreter_t* s, _ls_node_t** l, bool_t force_next);
int _common_end_looping(mb_interpreter_t* s, _ls_node_t** l);
int _common_keep_looping(mb_interpreter_t* s, _ls_node_t** l, _var_t* var_loop);
int _execute_normal_for_loop(mb_interpreter_t* s, _ls_node_t** l, _var_t* var_loop);
int _skip_to(mb_interpreter_t* s, _ls_node_t** l, mb_func_t f, _data_e t);
bool_t _skip_single_line_struct(_ls_node_t** ast, mb_func_t func);
int _skip_if_chunk(mb_interpreter_t* s, _ls_node_t** l);
int _skip_struct(mb_interpreter_t* s, _ls_node_t** l, mb_func_t open_func, mb_func_t post_open_func, mb_func_t close_func);
bool_t _multiline_statement(mb_interpreter_t* s);

_running_context_t* _create_running_context(bool_t create_var_dict);
_parsing_context_t* _reset_parsing_context(_parsing_context_t* context);
void _destroy_parsing_context(_parsing_context_t* _UNALIGNED_ARG * context);

/** Interface processors */

#ifdef MB_ENABLE_MODULE
_module_func_t* _create_module_func(mb_interpreter_t* s, mb_func_t f);
int _ls_destroy_module_func(void* data, void* extra);
int _ht_destroy_module_func_list(void* data, void* extra);
#endif /* MB_ENABLE_MODULE */
char* _generate_func_name(mb_interpreter_t* s, char* n, bool_t with_mod);
int _register_func(mb_interpreter_t* s, char* n, mb_func_t f, bool_t local);
int _remove_func(mb_interpreter_t* s, char* n, bool_t local);
_ls_node_t* _find_func(mb_interpreter_t* s, char* n, bool_t* mod);

/** Library helpers */

#define _MB_DECLARE_LIBRARY(__l) \
	int _mb_open_##__l##_lib(mb_interpreter_t* s); \
	int _mb_close_##__l##_lib(mb_interpreter_t* s);

#define _MB_DEFINE_LIBRARY(__l) \
	int _mb_open_##__l##_lib(mb_interpreter_t *s) { \
		int result = 0; \
		int i = 0; \
		mb_assert(s); \
		for (i = 0; i < countof(_##__l##_libs); ++i) \
			result += _register_func(s, _##__l##_libs[i].name, _##__l##_libs[i].pointer, true); \
		return result; \
	} \
	int _mb_close_##__l##_lib(mb_interpreter_t *s) { \
		int result = 0; \
		int i = 0; \
		mb_assert(s); \
		for (i = 0; i < countof(_##__l##_libs); ++i) \
			result += _remove_func(s, _##__l##_libs[i].name, true); \
		return result; \
	}

#define _MB_OPEN_LIBRARY(__l, __s) _mb_open_##__l##_lib(__s)
#define _MB_CLOSE_LIBRARY(__l, __s) _mb_close_##__l##_lib(__s)

/* ========================================================} */


/*
** {========================================================
** Lib declarations
*/

/** Macro */

#ifdef MB_CP_VC
#	if MB_CP_VC < 1300
#		define MB_FUNC 0
#	else /* MB_CP_VC < 1300 */
#		define MB_FUNC __FUNCTION__
#	endif /* MB_CP_VC < 1300 */
#elif defined MB_CP_BORLANDC
#	define MB_FUNC __FUNC__
#elif defined MB_CP_PELLESC
#	define MB_FUNC __func__
#else /* MB_CP_VC */
#	define MB_FUNC __FUNCTION__
#endif /* MB_CP_VC */

#ifdef MB_CP_VC
#	if MB_CP_VC < 1300
#		define _do_nothing(__s, __l, __exit, __result) \
			do { \
				_ls_node_t* ast = 0; static int i = 0; ++i; \
				printf("Unaccessable function called %d times.\n", i); \
				ast = (_ls_node_t*)(*(__l)); \
				_handle_error_on_obj((__s), SE_RN_REACHED_TO_WRONG_FUNCTION, (__s)->source_file, DON(ast), MB_FUNC_ERR, __exit, (__result)); \
			} while(0)
#	endif /* MB_CP_VC < 1300 */
#endif /* MB_CP_VC */
#ifndef _do_nothing
#	define _do_nothing(__s, __l, __exit, __result) \
		do { \
			_ls_node_t* ast = (_ls_node_t*)(*(__l)); \
			_handle_error_on_obj((__s), SE_RN_REACHED_TO_WRONG_FUNCTION, (char*)MB_FUNC, DON(ast), MB_FUNC_ERR, __exit, (__result)); \
		} while(0);
#endif /* _do_nothing */

/* ========================================================} */


#ifdef MB_COMPACT_MODE
#	pragma pack()
#endif /* MB_COMPACT_MODE */

#ifdef MB_CP_BORLANDC
#	pragma warn .8004
#	pragma warn .8008
#	pragma warn .8012
#endif /* MB_CP_BORLANDC */

#ifdef MB_CP_CLANG
#	pragma clang diagnostic pop
#endif /* MB_CP_CLANG */

#ifdef MB_CP_VC
#	pragma warning(pop)
#endif /* MB_CP_VC */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif // __MB_INTERNAL_H__
