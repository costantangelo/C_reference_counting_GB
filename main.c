/*
 * Piccolo esempio di refcounting GB per Alba CubeSat.
 */


#include <stdio.h> // <- printf(),
#include <stdlib.h> // <- malloc(), free(),

// Definiamo un nuovo tipo mediante una struct.
// Una struct minimale che raccoglie (int value) e (unsigned int refcount), ques'ultimo sarà necessario per il gb.
typedef struct {
    int value;
    unsigned int refcount;
    //Nota: ambo i campi sono interi da 4 byte l'uno, dunque non vi è padding da minimizzare.
} IntObject;

// Definiamo una sorta di costruttore per tali tipi.
// Se si trattasse di una struct come le altre allora potremmo creare
// l'istanza new_int_obj banalmente nello stack frame di new_int_object(...)
// per poi returnare una sua copia e ritrovarcela nello stack frame del main, semplice.
// Ma noi vogliamo in principio allocare tale istanza di IntObject nell'heap, per implementare poi il gb.
// Dobbiamo dunque accedere all'heap con malloc() e fare restituire al costruttore il mem addr dell'istanza così creata.
IntObject *new_int_object(int given_value) {

    IntObject *new_int_obj = malloc(sizeof(IntObject));

    new_int_obj -> value = given_value;
    new_int_obj -> refcount = 1;
    // Operatore di dereference per i typedef [Struct].

    return new_int_obj; // <- Mem addr della memoria allocata con malloc
}

void print_int_obj(IntObject *given_int_obj) {
    printf("-------------\n");
    if (given_int_obj == NULL) {
        printf("isNULL");
    }else {
        printf("%p has value : %d\n", given_int_obj, given_int_obj -> value);
        printf("%p has ref count : %d\n", given_int_obj, given_int_obj -> refcount);
    }
}

void update_value(IntObject *given_int_obj, int new_value) {
    given_int_obj -> value = new_value;
}

void retain(IntObject *given_int_obj) {
    given_int_obj -> refcount += 1;
}

void release(IntObject **given_int_obj) {

    if (given_int_obj == NULL || *given_int_obj == NULL)
        return;

    IntObject *p = *given_int_obj; //RICORDIAMO: p CONTIENE UNA COPIA DELL'ADDR DATO DA *given_int_obj

    if (p -> refcount == 0) //Ricordiamo che -> esegue una dereferenziazione implicita
        return;

    p->refcount -= 1;

    if (p->refcount == 0) {
        free(p);
        *given_int_obj = NULL;
    }
}

int main(void) {

    IntObject *x = new_int_object(55);
    print_int_obj(x);
    //Il successo della stampa simboleggia che l'oggetto è
    //"sopravvissuto" alla rimozione dello stack frame di new_int_obj().

    update_value(x,77);
    print_int_obj(x);

    retain(x);
    print_int_obj(x);

    release(&x);
    release(&x);
    print_int_obj(x); //isNull

    return 0;
}








