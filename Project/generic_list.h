#ifndef structuresHeader
#define structuresHeader

#include "defs.h"

typedef struct _node node;
typedef struct _list list;
typedef struct _vector vector;

node *create_node(item new_item, node *next_node, int item_size_);
item get_node_item(node *got_node);
void set_item_as(node *got_node, void (*setter)(item, item), item setting);
node *get_next_node(node *got_node);
void set_next_node(node *got_node, node *next_node);
void free_node(node *got_node, void (*free_item)(item));

list *create_list(int item_size);
node *get_head(list *got_list);
void set_head(list *got_list, node *got_head);
void push_item_to_list(list *got_list, item new_item);
size_t get_list_size(list *got_list);
void print_list(list *got_list, void (*print_item)(item));
void free_list(list *got_list, void (*free_item)(item));

void decrement_list_size(list *got_list);

#endif