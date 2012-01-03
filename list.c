#include "list.h"
#include <stdlib.h>
#include <string.h>


list_p create_list(){
	list_p list = (list_p) malloc(sizeof(struct list));
	list->length = 0;
	list->first = NULL;
	list->last = NULL;
	list->destructor = free;
	return list;
}

list_iter_p list_iterator(list_p list, char init){
	list_iter_p iter = (list_iter_p)malloc(sizeof(struct list_iter));
	if(init==FRONT){
		iter->current = list->first;
	}
	else if(init==BACK){
		iter->current = list->last;
	}
	else return NULL;
	iter->started = 0;
	return iter;
}

void list_add(list_p list, void* data, int size){
	nodeptr node = (nodeptr)malloc(sizeof(struct linked_node));
	node->data = malloc(size);
	memcpy(node->data, data, size);
	
	if(list->first==NULL){
		node->prev = NULL;
		node->next = NULL;
		list->first = node;
		list->last = node;
	}
	else{
		list->last->next = node;
		node->prev = list->last;
		node->next = NULL;
		list->last = node;
	}
	list->length++;
}

void* list_current(list_iter_p iter){
	if(iter->started&&iter->current!=NULL)
		return iter->current->data;
	return NULL;
}

void* list_next(list_iter_p iter){
	if(!iter->started&&iter->current!=NULL){
		iter->started=1;
		return iter->current->data;
	}
	if(iter->current!=NULL){
		iter->current = iter->current->next;
		return list_current(iter);
	}
	return NULL;
}

void* list_prev(list_iter_p iter){
	if(!iter->started&&iter->current!=NULL){
		iter->started=1;
		return iter->current->data;
	}
	if(iter->current!=NULL){
		iter->current = iter->current->prev;
		return list_current(iter);
	}
	return NULL;
}

void* list_first(list_p list){
	return list->first->data;
}

void* list_last(list_p list){
	return list->last->data;
}

void* list_pop(list_p list){
	nodeptr last = list->last;
	if(last == NULL) return NULL;
	list->last = last->prev;
	void* data = last->data;
	last->prev->next = NULL;
	free(last);
	return data;
}

void* list_poll(list_p list){
	nodeptr first = list->first;
	if(first == NULL) return NULL;
	list->first = first->next;
	void* data = first->data;
	first->next->prev = NULL;
	free(first);
	return data;
}

void list_remove(list_p list, char end){
	void * data;
	if(end == FRONT)
		data = list_poll(list);
	else if (end == BACK)
		data = list_pop(list);
	else return;
	list->destructor(data);
}

void destroy_list(list_p list){
	nodeptr cur = list->first;
	nodeptr next;
	while(cur!=NULL){
		next = cur->next;
		list->destructor(cur->data);
		free(cur);
		cur = next;
	}
	free(list);
}

