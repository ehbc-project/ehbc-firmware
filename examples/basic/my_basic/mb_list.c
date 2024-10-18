/*
** This source file is part of MY-BASIC
**
** For the latest info, see https://github.com/paladin-t/my_basic/
**
** Copyright (C) 2011 - 2024 Tony Wang
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifdef _MSC_VER
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS
#	endif /* _CRT_SECURE_NO_WARNINGS */
#endif /* _MSC_VER */

#include "my_basic.h"
#include "mb_internal.h"


/** List operations */

int _ls_cmp_data(void* node, void* info) {
	_ls_node_t* n = (_ls_node_t*)node;

	return (n->data == info) ? 0 : 1;
}

int _ls_cmp_extra(void* node, void* info) {
	_ls_node_t* n = (_ls_node_t*)node;

	return (n->extra == info) ? 0 : 1;
}

int _ls_cmp_extra_object(void* node, void* info) {
	_ls_node_t* n = (_ls_node_t*)node;

	return _ht_cmp_object(n->extra, info);
}

int _ls_cmp_extra_string(void* node, void* info) {
	_ls_node_t* n = (_ls_node_t*)node;
	char* s1 = (char*)n->extra;
	char* s2 = (char*)info;

	return strcmp(s1, s2);
}

#ifdef MB_ENABLE_MODULE
int _ls_cmp_module_func(void* node, void* info) {
	_module_func_t* m = (_module_func_t*)node;
	mb_interpreter_t* s = (mb_interpreter_t*)info;

	return strcmp(m->module, s->with_module);
}
#endif /* MB_ENABLE_MODULE */

_ls_node_t* _ls_create_node(void* data) {
	_ls_node_t* result = 0;

	result = (_ls_node_t*)mb_malloc(sizeof(_ls_node_t));
	mb_assert(result);
	memset(result, 0, sizeof(_ls_node_t));
	result->data = data;

	return result;
}

_ls_node_t* _ls_create(void) {
	_ls_node_t* result = 0;

	result = _ls_create_node(0);

	return result;
}

_ls_node_t* _ls_find(_ls_node_t* list, void* data, _ls_compare_t cmp, int* idx) {
	_ls_node_t* result = 0;

	mb_assert(list && data && cmp);

	if(idx) *idx = 0;

	list = list->next;
	while(list) {
		if(!cmp(list->data, data)) {
			result = list;

			break;
		}
		list = list->next;
		if(idx) ++*idx;
	}

	return result;
}

_ls_node_t* _ls_back(_ls_node_t* node) {
	_ls_node_t* result = node;

	result = result->prev;

	return result;
}

_ls_node_t* _ls_pushback(_ls_node_t* list, void* data) {
	_ls_node_t* result = 0;
	_ls_node_t* tmp = 0;

	mb_assert(list);

	result = _ls_create_node(data);

	tmp = _ls_back(list);
	if(!tmp)
		tmp = list;
	tmp->next = result;
	result->prev = tmp;
	list->prev = result;
	list->data = (char*)list->data + 1;

	return result;
}

void* _ls_popback(_ls_node_t* list) {
	void* result = 0;
	_ls_node_t* tmp = 0;

	mb_assert(list);

	tmp = _ls_back(list);
	if(tmp) {
		result = tmp->data;
		if(list != tmp->prev)
			list->prev = tmp->prev;
		else
			list->prev = 0;
		tmp->prev->next = 0;
		safe_free(tmp);
		list->data = (char*)list->data - 1;
	}

	return result;
}

_ls_node_t* _ls_front(_ls_node_t* node) {
	_ls_node_t* result = node;

	result = result->next;

	return result;
}

void* _ls_popfront(_ls_node_t* list) {
	void* result = 0;
	_ls_node_t* tmp = 0;

	mb_assert(list);

	tmp = _ls_front(list);
	if(tmp) {
		result = tmp->data;
		list->next = tmp->next;
		if(tmp->next)
			tmp->next->prev = list;
		if(!list->next)
			list->prev = 0;
		tmp->prev = tmp->next = 0;
		safe_free(tmp);
		list->data = (char*)list->data - 1;
	}

	return result;
}

_ls_node_t* _ls_insert_at(_ls_node_t* list, int index, void* data) {
	_ls_node_t* result = 0;
	_ls_node_t* tmp = 0;

	mb_assert(list);

	tmp = list->next;
	while(tmp && index) {
		tmp = tmp->next;
		--index;
	}
	if(!tmp) {
		if(index == 0)
			result = _ls_pushback(list, data);
	} else {
		result = _ls_create_node(data);
		tmp->prev->next = result;
		result->prev = tmp->prev;
		result->next = tmp;
		tmp->prev = result;
	}
	list->data = (char*)list->data + 1;

	return result;
}

unsigned _ls_remove(_ls_node_t* list, _ls_node_t* node, _ls_operation_t op) {
	unsigned result = 0;

	mb_assert(list && node);

	if(node->prev)
		node->prev->next = node->next;
	if(node->next)
		node->next->prev = node->prev;
	if(list->prev == node)
		list->prev = node->prev;
	if(list->prev == list)
		list->prev = 0;
	if(op)
		op(node->data, node->extra);
	safe_free(node);
	++result;
	list->data = (char*)list->data - 1;

	return result;
}

unsigned _ls_try_remove(_ls_node_t* list, void* info, _ls_compare_t cmp, _ls_operation_t op) {
	unsigned result = 0;
	_ls_node_t* tmp = 0;

	mb_assert(list && cmp);

	tmp = list->next;
	while(tmp) {
		if(cmp(tmp, info) == 0) {
			if(tmp->prev)
				tmp->prev->next = tmp->next;
			if(tmp->next)
				tmp->next->prev = tmp->prev;
			if(list->prev == tmp)
				list->prev = tmp->prev;
			if(list->prev == list)
				list->prev = 0;
			if(op)
				op(tmp->data, tmp->extra);
			safe_free(tmp);
			++result;
			list->data = (char*)list->data - 1;

			break;
		}
		tmp = tmp->next;
	}

	return result;
}

unsigned _ls_foreach(_ls_node_t* list, _ls_operation_t op) {
	unsigned idx = 0;
	int opresult = _OP_RESULT_NORMAL;
	_ls_node_t* node = 0;
	_ls_node_t* tmp = 0;

	mb_assert(list && op);

	node = list->next;
	while(node) {
		opresult = op(node->data, node->extra);
		++idx;
		tmp = node;
		node = node->next;

		if(_OP_RESULT_DEL_NODE == opresult) {
			tmp->prev->next = node;
			if(node)
				node->prev = tmp->prev;
			safe_free(tmp);
			list->data = (char*)list->data - 1;
		}
	}

	return idx;
}

_ls_node_t* _ls_sort(_ls_node_t* _UNALIGNED_ARG * list, _ls_compare_t cmp) {
	/* Copyright 2001 Simon Tatham, http://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.c */
	bool_t is_circular = false, is_double = true;
	_ls_node_t* p, * q, * e, * tail, * oldhead;
	int insize, nmerges, psize, qsize, i;
	_ls_node_t* lst = 0;

	mb_assert(list && *list && cmp);

	lst = *list;
	if(lst) lst = lst->next;

	if(!lst)
		return 0;

	insize = 1;

	while(1) {
		p = lst;
		oldhead = lst;
		lst = 0;
		tail = 0;

		nmerges = 0;

		while(p) {
			nmerges++;
			q = p;
			psize = 0;
			for(i = 0; i < insize; i++) {
				psize++;
				if(is_circular)
					q = (q->next == oldhead ? 0 : q->next);
				else
					q = q->next;
				if(!q)
					break;
			}

			qsize = insize;

			while(psize > 0 || (qsize > 0 && q)) {
				if(psize == 0) {
					e = q; q = q->next; qsize--;
					if(is_circular && q == oldhead) q = 0;
				} else if(qsize == 0 || !q) {
					e = p; p = p->next; psize--;
					if(is_circular && p == oldhead) p = 0;
				} else if(cmp(p->data, q->data) <= 0) {
					e = p; p = p->next; psize--;
					if(is_circular && p == oldhead) p = 0;
				} else {
					e = q; q = q->next; qsize--;
					if(is_circular && q == oldhead) q = 0;
				}

				if(tail)
					tail->next = e;
				else
					lst = e;
				if(is_double)
					e->prev = tail;
				tail = e;
			}

			p = q;
		}
		if(is_circular) {
			tail->next = lst;
			if(is_double)
				lst->prev = tail;
		} else {
			tail->next = 0;
		}

		if(nmerges <= 1) {
			(*list)->next = lst;
			(*list)->prev = tail;

			lst->prev = *list;

			return *list;
		}

		insize *= 2;
	}
}

unsigned _ls_count(_ls_node_t* list) {
	union { void* p; unsigned u; } tmp;

	mb_assert(list);

	tmp.p = list->data;

	return tmp.u;
}

bool_t _ls_empty(_ls_node_t* list) {
	bool_t result = false;

	mb_assert(list);

	result = 0 == list->next;

	return result;
}

void _ls_clear(_ls_node_t* list) {
	_ls_node_t* tmp = 0;

	mb_assert(list);

	list->data = 0;

	tmp = list;
	list = list->next;
	tmp->next = 0;
	tmp->prev = 0;

	while(list) {
		tmp = list;
		list = list->next;
		safe_free(tmp);
	}
}

void _ls_destroy(_ls_node_t* list) {
	_ls_clear(list);
	safe_free(list);
}

int _ls_free_extra(void* data, void* extra) {
	int result = _OP_RESULT_DEL_NODE;
	mb_unrefvar(data);

	mb_assert(extra);

	safe_free(extra);

	return result;
}

