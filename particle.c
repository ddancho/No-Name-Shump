#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

void particleLayerUpdate(ParticleLayer *head, Vector2 position, float dt){
    ParticleLayer *temp = NULL;
	ParticleLayer *current = head;

	while(current != NULL){
		temp = current->next;
		if(current->particle.update(&current->particle, position, dt) == true)
			current = temp;
		else
			current = current->next;
	}
}

void particleLayerDraw(ParticleLayer *head){
    ParticleLayer *current = head;

    while(current != NULL){
        if(current->particle.isDraw == true)
            current->particle.draw(&current->particle);
        current = current->next;
    }
}

void particleLayerDestroy(ParticleLayer **head){
    ParticleLayer *current = *head;

    while(current != NULL){
        current->particle.destroy(&current->particle);
        *head = current;
        current = current->next;
        free(*head);
    }

    *head = NULL;
}

Particle * particleCreate(ParticleLayer **head, float id){
    ParticleLayer *temp = NULL;
	ParticleLayer *current = *head;

	temp = malloc(sizeof(ParticleLayer));
	if(temp == NULL)
		fprintf(stderr, "particle malloc error...\n");

	temp->next = NULL;
	temp->particle.id = id;

	if(current != NULL){
		while(current->next != NULL)
			current = current->next;
		current->next = temp;
	}
	else{
		*head = temp;
	}

	return &temp->particle;
}

void particleDestroy(ParticleLayer **head, float id){
    ParticleLayer *prev = NULL;
	ParticleLayer *current = *head;

	while(current != NULL){
		if(current->particle.id == id){
			if(prev != NULL)
				prev->next = current->next;
			else if(prev == NULL)
				*head = current->next;
            current->particle.destroy(&current->particle);
			free(current);
			break;
		}
		else{
			prev = current;
			current = current->next;
		}
	}
}

Particle* getParticleByID(ParticleLayer *head, float id){
    ParticleLayer *current = head;
    Particle *temp = NULL;

    while(current != NULL){
        if(current->particle.id == id){
            temp = &current->particle;
            break;
        }
        current = current->next;
    }

    return temp;
}


























