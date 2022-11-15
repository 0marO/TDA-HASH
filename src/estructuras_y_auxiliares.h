#ifndef __ESTRUCTURAS_Y_AUXILIARES_H__
#define __ESTRUCTURAS_Y_AUXILIARES_H__

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CAPACIDAD_MINIMA 3
#define FACTOR_DE_CARGA 0.75
#define FACTOR_DE_AUMENTO 2

typedef struct par{
	char *clave;
	void *elemento;
	struct par *siguiente;
} par_t;

struct hash {
	par_t **tabla;
	size_t capacidad;
	size_t ocupados;
};

/*
 * Calcula la el código numérico correspondiente a la clave string recibida.
 * Siempre dará el milsmo resultado para la misma string.
 */
unsigned long funcion_hash(const char *str);

/*
 * Funcion que recibe una clave string y un hash inicializado y devuelve un entero positivo, 
 * que es una posición de la tabla del hash parámetro.
 */
size_t obtener_posicion(const char *clave, hash_t *hash);

/*
 * Funcion que recibe una clave y un valor. Con éstos crea una estructura de par. Devuelve un puntero a 
 * la estructura creada.
 */
par_t *crear_par(const char *clave, void *elemento);

/*
 * Actualiza o crea un par en la posición de la tabla de hash recibida por parámetro.
 * para esto itera sobre la cadena de pares existentes.
 */
bool actualizar_o_crear_par(hash_t *hash, size_t posicion, const char *clave, void *elemento, 
	void **anterior);


/* función que recibe un hash existente. Se encarga de duplicar la capacidad de la tabla de hash
 * y de transpasar los pares exitentes en el hash parámetro a la nueva tabla.
 * devuelve el hash parámetro con la tabla actualizada.
 */
hash_t *rehash(hash_t *hash);


/* Función que duplica el string recibido por parámetro, reservando memoria dinámica.
 * Devuelve un puntero a la string duplicada.
 */
char *copiar_string(const char *string);

/*
 * Recorre la cadena de pares del par recibido por parámetro y elimina el par buscado si se encuentra.
*/
par_t *buscar_y_quitar_rec(par_t *par_actual, void **elemento, const char *referencia, bool *flag);

/* Recorre una cadena de pares hasta encontrar la referencia o hasta que no haya pares.
*/
void buscar_elemento(par_t *par_actual, void **elemento,  const char *referencia, bool *flag);

/* Función recursiva. Función auxiliar a hash_destruir_todo. Recibe un par y libera su clave. Luego recorre
* recursivamente los pares hasta que no haya mas. luego se libera cada par, aprovechando el backtraking.
*/
void destruir_pares(par_t *par_actual, void (*destructor)(void *));


/* Función auxiliar a hash_con_cada_clave. Recibe un par y recorre
*  los pares hasta que no haya mas, aplicando la función del usuario.
* Si esta ultima devuelve False se corta el recorrido de pares.
*/
void recorrer_pares_rec(par_t *par_actual, bool (*f)(const char *clave, void *valor, void *aux),
	size_t *invocaciones, bool *seguir, void *aux);



#endif /* __ESTRUCTURAS_Y_AUXILIARES_H__ */
