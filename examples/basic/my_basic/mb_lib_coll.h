#ifndef __MB_LIB_COLL_H__
#define __MB_LIB_COLL_H__

#include "config.h"
#include "mb_internal.h"
#include "my_basic.h"

#ifdef MB_ENABLE_COLLECTION_LIB

_MB_DECLARE_LIBRARY(coll)

/* Identifier */
#define _COLL_ID_LIST "LIST"
#define _COLL_ID_DICT "DICT"
#define _COLL_ID_PUSH "PUSH"
#define _COLL_ID_POP "POP"
#define _COLL_ID_BACK "BACK"
#define _COLL_ID_INSERT "INSERT"
#define _COLL_ID_SORT "SORT"
#define _COLL_ID_EXISTS "EXISTS"
#define _COLL_ID_INDEX_OF "INDEX_OF"
#define _COLL_ID_REMOVE "REMOVE"
#define _COLL_ID_CLEAR "CLEAR"
#define _COLL_ID_CLONE "CLONE"
#define _COLL_ID_TO_ARRAY "TO_ARRAY"
#define _COLL_ID_ITERATOR "ITERATOR"
#define _COLL_ID_MOVE_NEXT "MOVE_NEXT"

/* Library function & statement */
int _coll_list(mb_interpreter_t* s, void** l);
int _coll_dict(mb_interpreter_t* s, void** l);
int _coll_push(mb_interpreter_t* s, void** l);
int _coll_pop(mb_interpreter_t* s, void** l);
int _coll_back(mb_interpreter_t* s, void** l);
int _coll_insert(mb_interpreter_t* s, void** l);
int _coll_sort(mb_interpreter_t* s, void** l);
int _coll_exists(mb_interpreter_t* s, void** l);
int _coll_index_of(mb_interpreter_t* s, void** l);
int _coll_remove(mb_interpreter_t* s, void** l);
int _coll_clear(mb_interpreter_t* s, void** l);
int _coll_clone(mb_interpreter_t* s, void** l);
int _coll_to_array(mb_interpreter_t* s, void** l);
int _coll_iterator(mb_interpreter_t* s, void** l);
int _coll_move_next(mb_interpreter_t* s, void** l);

/* Internal function */
int _execute_ranged_for_loop(mb_interpreter_t* s, _ls_node_t** l, _var_t* var_loop);

_list_t* _create_list(mb_interpreter_t* s);
void _destroy_list(_list_t* c);
_dict_t* _create_dict(mb_interpreter_t* s);
void _destroy_dict(_dict_t* c);
_list_it_t* _create_list_it(_list_t* coll, bool_t lock);
bool_t _destroy_list_it(_list_it_t* it);
_list_it_t* _move_list_it_next(_list_it_t* it);
_dict_it_t* _create_dict_it(_dict_t* coll, bool_t lock);
bool_t _destroy_dict_it(_dict_it_t* it);
_dict_it_t* _move_dict_it_next(_dict_it_t* it);
void _unref_list(_ref_t* ref, void* data);
void _unref_dict(_ref_t* ref, void* data);
bool_t _push_list(_list_t* coll, mb_value_t* val, _object_t* oarg);
bool_t _pop_list(_list_t* coll, mb_value_t* val, mb_interpreter_t* s);
bool_t _insert_list(_list_t* coll, int_t idx, mb_value_t* val, _object_t** oval);
bool_t _set_list(_list_t* coll, int_t idx, mb_value_t* val, _object_t** oval);
bool_t _remove_at_list(_list_t* coll, int_t idx);
_ls_node_t* _node_at_list(_list_t* coll, int index);
bool_t _at_list(_list_t* coll, int_t idx, mb_value_t* oval);
bool_t _find_list(_list_t* coll, mb_value_t* val, int* idx);
void _clear_list(_list_t* coll);
void _sort_list(_list_t* coll);
void _invalidate_list_cache(_list_t* coll);
void _fill_ranged(_list_t* coll);
bool_t _set_dict(_dict_t* coll, mb_value_t* key, mb_value_t* val, _object_t* okey, _object_t* oval);
bool_t _remove_dict(_dict_t* coll, mb_value_t* key);
bool_t _find_dict(_dict_t* coll, mb_value_t* val, mb_value_t* oval);
void _clear_dict(_dict_t* coll);
bool_t _invalid_list_it(_list_it_t* it);
bool_t _invalid_dict_it(_dict_it_t* it);
bool_t _assign_with_it(_object_t* tgt, _object_t* src);
bool_t _try_purge_it(mb_interpreter_t* s, mb_value_t* val, _object_t* obj);
int _clone_to_list(void* data, void* extra, _list_t* coll);
int _clone_to_dict(void* data, void* extra, _dict_t* coll);
int _copy_list_to_array(void* data, void* extra, _array_helper_t* h);
int _copy_keys_to_value_array(void* data, void* extra, _keys_helper_t* h);
#endif /* MB_ENABLE_COLLECTION_LIB */

MBAPI int mb_init_coll(struct mb_interpreter_t* s, void** l, mb_value_t* coll);
MBAPI int mb_get_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, mb_value_t idx, mb_value_t* val);
MBAPI int mb_set_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, mb_value_t idx, mb_value_t val);
MBAPI int mb_remove_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, mb_value_t idx);
MBAPI int mb_count_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, int* c);
MBAPI int mb_keys_of_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, mb_value_t* keys, int c);

#endif // __MB_LIB_COLL_H__
