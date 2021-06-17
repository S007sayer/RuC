/*
 *	Copyright 2021 Andrey Terekhov, Victor Y. Fadeev, Dmitrii Davladov
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */

#include "tree.h"


static inline bool is_negative(const item_t value)
{
	return value >> (8 * sizeof(item_t) - 1);
}

static inline void vector_swap(vector *const vec, size_t fst, size_t snd)
{
	const item_t temp = vector_get(vec, fst);
	vector_set(vec, fst, vector_get(vec, snd));
	vector_set(vec, snd, temp);
}


static inline size_t ref_get_next(const node *const nd)
{
	return nd->index - 2;
}

static inline size_t ref_get_amount(const node *const nd)
{
	return nd->index + 1 + (size_t)vector_get(nd->tree, nd->index);
}

static inline size_t ref_get_children(const node *const nd)
{
	return nd->index + 2 + (size_t)vector_get(nd->tree, nd->index);
}

static inline int ref_set_next(const node *const nd, const item_t value)
{
	return vector_set(nd->tree, ref_get_next(nd), value);
}

static inline int ref_set_amount(const node *const nd, const item_t value)
{
	return vector_set(nd->tree, ref_get_amount(nd), value);
}
static inline int ref_set_children(const node *const nd, const item_t value)
{
	return vector_set(nd->tree, ref_get_children(nd), value);
}

static inline node node_broken()
{
	node nd = { NULL, SIZE_MAX };
	return nd;
}


static node node_search_parent(const node *const nd, size_t *const number)
{
	if (!node_is_correct(nd) || nd->index == 0)
	{
		return node_broken();
	}

	*number = 1;
	item_t index = vector_get(nd->tree, ref_get_next(nd));
	while (!is_negative(index) && index != 0)
	{
		index = vector_get(nd->tree, (size_t)index - 2);
		(*number)++;
	}

	node parent = { nd->tree, (size_t)(~index + 1) };
	*number = node_get_amount(&parent) - *number;
	return parent;
}


/*
 *	 __     __   __     ______   ______     ______     ______   ______     ______     ______
 *	/\ \   /\ "-.\ \   /\__  _\ /\  ___\   /\  == \   /\  ___\ /\  __ \   /\  ___\   /\  ___\
 *	\ \ \  \ \ \-.  \  \/_/\ \/ \ \  __\   \ \  __<   \ \  __\ \ \  __ \  \ \ \____  \ \  __\
 *	 \ \_\  \ \_\\"\_\    \ \_\  \ \_____\  \ \_\ \_\  \ \_\    \ \_\ \_\  \ \_____\  \ \_____\
 *	  \/_/   \/_/ \/_/     \/_/   \/_____/   \/_/ /_/   \/_/     \/_/\/_/   \/_____/   \/_____/
 */


node node_get_root(vector *const tree)
{
	const size_t size = vector_size(tree);
	if (size == 0)
	{
		vector_add(tree, 0);
		vector_add(tree, 0);
		vector_add(tree, 5);
	}
	else if (size == SIZE_MAX || size < 3 || vector_get(tree, 0) < 0)
	{
		return node_broken();
	}

	node nd = { tree, 0 };
	return nd;
}

node node_get_child(const node *const nd, const size_t index)
{
	if (!node_is_correct(nd) || index >= node_get_amount(nd))
	{
		return node_broken();
	}

	size_t child_index = (size_t)vector_get(nd->tree, ref_get_children(nd));
	for (size_t i = 0; i < index; i++)
	{
		child_index = (size_t)vector_get(nd->tree, child_index - 2);
	}

	node child = { nd->tree, child_index };
	return child;
}

node node_get_parent(const node *const nd)
{
	return node_search_parent(nd, NULL);
}


item_t node_get_type(const node *const nd)
{
	return node_is_correct(nd) && nd->index != 0 ? vector_get(nd->tree, nd->index - 1) : ITEM_MAX;
}

size_t node_get_argc(const node *const nd)
{
	return node_is_correct(nd) ? (size_t)vector_get(nd->tree, nd->index) : 0;
}

item_t node_get_arg(const node *const nd, const size_t index)
{
	return index < node_get_argc(nd) ? vector_get(nd->tree, nd->index + 1 + index) : ITEM_MAX;
}

size_t node_get_amount(const node *const nd)
{
	return node_is_correct(nd) ? (size_t)vector_get(nd->tree, ref_get_amount(nd)) : 0;
}


node node_get_next(const node *const nd)
{
	if (!node_is_correct(nd))
	{
		return node_broken();
	}

	node next = { nd->tree, (size_t)vector_get(nd->tree, ref_get_children(nd)) };

	if (node_get_amount(nd) == 0)
	{
		item_t index = vector_get(nd->tree, ref_get_next(nd));
		while (is_negative(index))
		{
			// Get next reference from parent
			index = vector_get(nd->tree, (size_t)(~index + 1) - 2);
		}

		next.index = (size_t)index;
	}

	return next.index != 0 ? next : node_broken();
}

int node_set_next(node *const nd)
{
	node next = node_get_next(nd);
	if (!node_is_correct(&next))
	{
		return -1;
	}

	*nd = next;
	return 0;
}


node node_add_child(const node *const nd, const item_t type)
{
	if (!node_is_correct(nd))
	{
		return node_broken();
	}

	vector_add(nd->tree, ~(item_t)nd->index + 1);
	vector_add(nd->tree, type);
	node child = { nd->tree, vector_add(nd->tree, 0) };
	vector_add(nd->tree, 0);
	vector_add(nd->tree, 0);

	const size_t amount = node_get_amount(nd);
	ref_set_amount(nd, amount + 1);

	if (amount == 0)
	{
		ref_set_children(nd, child.index);
	}
	else
	{
		node prev = node_get_child(nd, amount - 1);
		ref_set_next(&prev, child.index);
	}

	return child;
}

int node_set_type(const node *const nd, const item_t type)
{
	if (!node_is_correct(nd))
	{
		return -1;
	}

	if (nd->index == 0)
	{
		return -2;
	}

	return vector_set(nd->tree, nd->index - 1, type);
}

int node_add_arg(const node *const nd, const item_t arg)
{
	if (!node_is_correct(nd))
	{
		return -1;
	}

	if (node_get_amount(nd) != 0)
	{
		return -2;
	}

	ref_set_amount(nd, arg);

	vector_add(nd->tree, 0);
	vector_set(nd->tree, nd->index, vector_get(nd->tree, nd->index) + 1);
	return 0;
}

int node_set_arg(const node *const nd, const size_t index, const item_t arg)
{
	if (!node_is_correct(nd) || index >= node_get_argc(nd))
	{
		return -1;
	}

	return vector_set(nd->tree, nd->index + 1 + index, arg);
}


int node_copy(node *const dest, const node *const src)
{
	if (!node_is_correct(src) || dest == NULL)
	{
		return -1;
	}

	*dest = *src;
	return 0;
}

size_t node_save(const node *const nd)
{
	return node_is_correct(nd)
		? nd->index
		: SIZE_MAX;
}

node node_load(vector *const tree, const size_t index)
{
	if (!vector_is_correct(tree) || vector_get(tree, index) >= (item_t)(vector_size(tree) - index - 2))
	{
		return node_broken();
	}

	node nd = { tree, index };
	return nd;
}

int node_order(const node *const fst, const node *const snd)
{
	if (node_swap(fst, snd))
	{
		return -1;
	}

	vector_swap(fst->tree, ref_get_amount(fst), ref_get_amount(snd));
	vector_swap(fst->tree, ref_get_children(fst), ref_get_children(snd));

	return 0;
}

int node_swap(const node *const fst, const node *const snd)
{
	size_t fst_index = 0;
	node fst_prev = node_search_parent(fst, &fst_index);

	size_t snd_index = 0;
	node snd_prev = node_search_parent(snd, &snd_index);

	if (!node_is_correct(&fst_prev) || !node_is_correct(&snd_prev) || fst->tree != snd->tree)
	{
		return -1;
	}

	vector *const tree = fst->tree;
	size_t reference;

	if (fst_index == 0)
	{
		reference = ref_get_children(&fst_prev);
	}
	else
	{
		fst_prev = node_get_child(&fst_prev, fst_index - 1);
		reference = ref_get_next(&fst_prev);
	}

	if (snd_index == 0)
	{
		vector_swap(tree, reference, ref_get_children(&snd_prev));
	}
	else
	{
		snd_prev = node_get_child(&snd_prev, snd_index - 1);
		vector_swap(tree, reference, ref_get_next(&snd_prev));
	}

	vector_swap(tree, ref_get_next(fst), ref_get_next(snd));
	return 0;
}

int node_remove(const node *const nd, const size_t index)
{
	if (!node_is_correct(nd) || index >= node_get_amount(nd))
	{
		return -1;
	}

	node child;
	if (index == 0)
	{
		child = node_get_child(nd, index);
		ref_set_amount(nd, (item_t)node_get_amount(nd) - 1);

		if (node_get_amount(nd) != 0)
		{
			ref_set_children(nd, vector_get(nd->tree, ref_get_next(&child)));
		}
	}
	else
	{
		child = node_get_child(nd, index - 1);
		const size_t reference = (size_t)vector_get(nd->tree, ref_get_next(&child));
		ref_set_next(&child, vector_get(nd->tree, reference - 2));

		child.index = reference;
		ref_set_amount(nd, (item_t)node_get_amount(nd) - 1);
	}

	if (node_get_amount(&child) == 0 && ref_get_children(&child) == vector_size(nd->tree) - 1)
	{
		vector_resize(nd->tree, ref_get_next(&child));
	}

	return 0;
}

bool node_is_correct(const node *const nd)
{
	return nd != NULL && vector_is_correct(nd->tree);
}
