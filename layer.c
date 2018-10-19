#include <stdio.h>
#include <stdlib.h>

#include "layer.h"

void layerUpdate(Layer *head, float dt){
    Layer *temp = NULL;
	Layer *current = head;

	while(current != NULL){
		temp = current->next;
		if(current->entity.update(&current->entity, dt) == true)
			current = temp;
		else
			current = current->next;
	}
}

void layerDraw(Layer *head){
    Layer *current = head;

    while(current != NULL){
        current->entity.draw(&current->entity);
        current = current->next;
    }
}

void layerDestroy(Layer **head){
    Layer *current = *head;

    while(current != NULL){
        current->entity.destroy(&current->entity);
        *head = current;
        current = current->next;
        free(*head);
    }

    *head = NULL;
}

unsigned int getLayerSize(Layer *head){
	Layer *current = head;
	unsigned int i = 0;

	while(current != NULL){
		i++;
		current = current->next;
	}

	return i;
}

Entity* getEntityByIndex(Layer *head, unsigned int index){
	Layer *current = head;
	Entity *temp = NULL;
	unsigned int i = 0;

	while(current != NULL){
		if(i == index){
            temp = &current->entity;
			break;
		}
		i++;
		current = current->next;
	}

	return temp;
}

Entity* getEntityById(Layer *head, float id){
	Layer *current = head;
	Entity *temp = NULL;

	while(current != NULL){
		if(current->entity.id == id){
            temp = &current->entity;
			break;
		}
		current = current->next;
	}

	return temp;
}

Entity * entityCreate(Layer **head, float id, EntityType entityType){
    Layer *temp = NULL;
	Layer *current = *head;

	temp = malloc(sizeof(Layer));
	if(temp == NULL){
		fprintf(stderr, "malloc error...\n");
		return NULL;
	}

	temp->next = NULL;
	temp->entity.id = id;
	temp->entity.entityType = entityType;

	if(current != NULL){
        temp->next = *head;
        *head = temp;
	}
	else
        *head = temp;

	return &temp->entity;
}

void entityDestroy(Layer **head, float id){
    Layer *prev = NULL;
	Layer *current = *head;

    while(current->entity.id != id){
        if(current->next == NULL){
            fprintf(stderr, "entity %f nout found...\n", id);
            return;
        }
        prev = current;
        current = current->next;
    }

    if(current == *head)
        *head = current->next;
    else
        prev->next = current->next;

    free(current);
}
