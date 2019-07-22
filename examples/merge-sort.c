#include <assert.h>
#include <stdlib.h>
#include "list.h"
#include <stdio.h>
#include "common.h"

static uint16_t values[256];
static void list_merge(struct list_head *head,
                       struct list_head *left,
                       struct list_head *right)
{
    struct list_head *tmp;
    while (1) {
        if (list_empty(left)) {
            list_splice_tail(right, head);
            return;
        } else if (list_empty(right)) {
            list_splice_tail(left, head);
            return;
        }
        if (container_of(left->next, struct listitem, list)->i <=
            container_of(right->next, struct listitem, list)->i) {
            tmp = left->next;
        } else {
            tmp = right->next;
        }
        list_del(tmp);
        list_add_tail(tmp, head);
    }
}

static void list_mergesort(struct list_head *head)
{
    struct list_head list_left, list_right;
    if (list_empty(head) || list_is_singular(head))
        return;

    INIT_LIST_HEAD(&list_left);
    INIT_LIST_HEAD(&list_right);
    int list_num = 0;
    struct list_head *node1, *node2;
    list_for_each_safe (node1, node2, head) {
        list_num++;
    }
    // printf ("list_num = %d", list_num);
    int left = list_num / 2;
    int right = list_num - left;

    size_t i;
    for (i = 0; i < left; i++) {
        list_move_tail(head->next, &list_left);
    }
    for (i = 0; i < right; i++) {
        list_move_tail(head->next, &list_right);
    }



    list_mergesort(&list_left);
    list_mergesort(&list_right);
    list_merge(head, &list_left, &list_right);
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));
    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        // printf ("%d\n", item->i);
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    list_mergesort(&testlist);

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}
