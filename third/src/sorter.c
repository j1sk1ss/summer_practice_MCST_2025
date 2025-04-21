#include "../include/sorter.h"


static int _desc_compare(const void* a, const void* b) {
    return (*(value_t**)b)->tmp_int - (*(value_t**)a)->tmp_int;
}

static int _asc_compare(const void* a, const void* b) {
    return (*(value_t**)a)->tmp_int - (*(value_t**)b)->tmp_int;
}

/*
We split linked list by slow-fast pointers. 
When slow go by 1, fast goes by 2. 
*/
static int _split_linked_list(value_t* src, value_t** front, value_t** back) {
    value_t *fast = src->next, *slow = src;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    *front = src;
    *back = slow->next;
    slow->next = NULL;
    return 1;
}

/*
Recombine two neighbor elements by comparing with cmp function.
As a reference of changing cmp type was qsort, were we pass cmp function as arg. 
*/
value_t* _merge_linked_lists(value_t* a, value_t* b, int (*cmp)(const void*, const void*)) {
    value_t* result = NULL;
    if (!a) return b;
    else if (!b) return a;

    if (cmp(&a, &b) <= 0) {
        result = a;
        result->next = _merge_linked_lists(a->next, b, cmp);
    }
    else {
        result = b;
        result->next = _merge_linked_lists(a, b->next, cmp);
    }

    return result;
}

static int _list_merge_sort(value_t** h_ref, int (*cmp)(const void*, const void*)) {
    value_t *head = *h_ref, *a, *b;
    if (!head || !head->next) return 0;
    
    _split_linked_list(head, &a, &b);
    _list_merge_sort(&a, cmp);
    _list_merge_sort(&b, cmp);
 
    *h_ref = _merge_linked_lists(a, b, cmp);
    return 1;
}

/*
As base for sorting alghoritm is choosed merge sort. 
Merge sort can be applied for data structures with low random element access, like in flatten array.
That't why here we using merge sort for linked lists.
*/
int sorter_sort(commander_result_t* res) {
    if (!res || !res->h) return 0;
    _list_merge_sort(&res->h, res->sort_type == 1 ? _asc_compare : _desc_compare);
    return 1;
}

