// this file is partly copyed and modified from
// https://github.com/torvalds/linux/blob/master/include/linux/container_of.h
// and
// https://github.com/torvalds/linux/blob/master/include/linux/list.h

#pragma once
#include <stddef.h>

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member)                           \
    ({                                                            \
        void *__mptr = (void *)(ptr);                             \
        ((type *)(__mptr - offsetof(type, member)));              \
    })

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

struct list_head {
    struct list_head *next, *prev;
};

static inline void list_init(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

// add ne to front
static inline void list_add(struct list_head *ne, struct list_head *head) {
    struct list_head *next = head->next;
    next->prev = ne;
    ne->next = next;
    ne->prev = head;
    head->next = ne;
}

static inline void list_del(struct list_head *entry) {
    struct list_head *prev = entry->prev;
    struct list_head *next = entry->next;
    next->prev = prev;
    prev->next = next;
}
