#include "hash.h"
#include "estructuras_y_auxiliares.h"

hash_t *hash_crear(size_t capacidad)
{
	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash)
		return NULL;
	hash->capacidad = capacidad;
	hash->tabla = calloc(capacidad, sizeof(par_t*));
	if (!hash->tabla){
		free(hash);
		return NULL;
	}

	return hash;
}

unsigned long funcion_hash(const char *str)
{
	unsigned long valor = 5381;
	unsigned long c;

	while ((c = (unsigned long)*str++))
		valor = ((valor << 5) + valor) + c;

	return valor;
}

size_t obtener_posicion(const char *clave, hash_t *hash)
{
	unsigned long  valor_hash = funcion_hash(clave);
	return (size_t) valor_hash % hash->capacidad;
}

par_t *crear_par(const char *clave, void *elemento)
{	
	if (!clave)
		return NULL;
	par_t *par_nuevo = calloc(1,sizeof(par_t));
	if (!par_nuevo)
		return NULL;
	par_nuevo->clave = (char *)clave;
	par_nuevo->elemento = elemento;
	return par_nuevo;
}

bool actualizar_o_crear_par(hash_t *hash, size_t posicion, const char *clave, void *elemento, 
	void **anterior)
{
	par_t *par_actual = hash->tabla[posicion];
	while (par_actual && strcmp(par_actual->clave, clave) != 0){
		par_actual = par_actual->siguiente;
	}
	
	if (par_actual){
		if (anterior != NULL)
			*anterior = par_actual->elemento;
		par_actual->elemento = elemento;
		return true;
	}

	par_actual = crear_par(copiar_string(clave), elemento);
	if (!par_actual)
		return false;
	if (anterior != NULL)
		*anterior = NULL;
	par_actual->siguiente = hash->tabla[posicion];
	hash->tabla[posicion] = par_actual;
	
	hash->ocupados += 1;
	return true;
}

void transpasar_pares(hash_t *hash_viejo, hash_t *hash_redimensionado, size_t pos){
	
	par_t *par_a_transpasar = hash_viejo->tabla[pos];
	
	while (par_a_transpasar){
		size_t nueva_pos = obtener_posicion(par_a_transpasar->clave, hash_redimensionado);
		par_t *siguiente = par_a_transpasar->siguiente;

		par_t *par_aux = hash_redimensionado->tabla[nueva_pos];
		hash_redimensionado->tabla[nueva_pos] = par_a_transpasar;
		par_a_transpasar->siguiente = par_aux;

		hash_redimensionado->ocupados += 1;
		par_a_transpasar = siguiente;
	}
}

hash_t *rehash(hash_t *hash)
{
	hash_t hash_aux = *hash;

	hash->tabla = calloc(hash->capacidad * FACTOR_DE_AUMENTO , sizeof(par_t *));
	if(!hash->tabla)
		return NULL;

	hash->capacidad *= FACTOR_DE_AUMENTO;
	hash->ocupados = 0;

	for (size_t i = 0; i < hash_aux.capacidad; i++){
		transpasar_pares(&hash_aux, hash, i);
	}

	free(hash_aux.tabla);
	return hash;
} 

char *copiar_string(const char *string)
{
	if (!string)
		return NULL;

	char *ptr = malloc(strlen(string) + 1);
	if (!ptr)
		return NULL;

	strcpy(ptr, string);
	return ptr;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	if ( (float)(hash->ocupados+1) / (float)hash->capacidad > FACTOR_DE_CARGA)
		hash = rehash(hash);

	size_t posicion = obtener_posicion(clave, hash);
	
	if (!hash->tabla[posicion]){
		par_t *par = crear_par(copiar_string(clave), elemento);
		if (!par)
			return NULL;
		if (anterior != NULL)
			*anterior = NULL;
		hash->tabla[posicion] = par;
		hash->ocupados += 1;
		return hash;
	}

	bool resultado = actualizar_o_crear_par(hash, posicion, clave, elemento, anterior);
	if (resultado)
		return hash;
	return NULL;
}

par_t *buscar_y_quitar_rec(par_t *par_actual, void **elemento, const char *referencia, bool *flag)
{	
	if (!par_actual)
		return NULL;	

	if (strcmp(par_actual->clave, referencia) == 0){
		par_t *siguiente = par_actual->siguiente;
		*elemento = par_actual->elemento;
		free(par_actual->clave);
		free(par_actual);
		*flag = true;
		return siguiente;
	}
	par_actual->siguiente = buscar_y_quitar_rec(par_actual->siguiente, elemento, referencia, flag);

	return par_actual;

}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if(!hash || !clave)
		return NULL;
	
	size_t posicion = obtener_posicion(clave, hash);

	if (hash->tabla[posicion] == NULL)
		return NULL;
	
	void *elemento = NULL;
	bool flag = false;
	hash->tabla[posicion] = buscar_y_quitar_rec(hash->tabla[posicion], &elemento, clave, &flag);

	if (flag)
		hash->ocupados -= 1;
	
	return elemento;
}

void buscar_elemento(par_t *par_actual, void **elemento,  const char *referencia, bool *flag)
{
	while (par_actual && strcmp(par_actual->clave, referencia) != 0){
		par_actual = par_actual->siguiente;
	}
	
	if (!par_actual)
		return;

	if (flag)
		*flag = true;
	if (elemento)
		*elemento = par_actual->elemento;
	
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	
	size_t posicion = obtener_posicion(clave, hash);

	void *elemento = NULL;
	buscar_elemento(hash->tabla[posicion], &elemento, clave, NULL);

	return elemento;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	
	size_t posicion = obtener_posicion(clave, hash);

	bool flag = false;
	buscar_elemento(hash->tabla[posicion], NULL, clave, &flag);

	return flag;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->ocupados;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash,NULL);
}

void destruir_pares(par_t *par_actual, void (*destructor)(void *))
{

	while (par_actual){
		par_t *siguiente = par_actual->siguiente;
		if(destructor != NULL)
			destructor(par_actual->elemento);
		free(par_actual->clave);	
		free(par_actual);
		par_actual = siguiente;
	}
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if(!hash)
		return;
	
	size_t i = 0;
	while (i < hash->capacidad){
		if (hash->tabla[i] != NULL)
			destruir_pares(hash->tabla[i], destructor);
		i++;
	}
	free(hash->tabla);
	free(hash);
}

void recorrer_pares_rec(par_t *par_actual, bool (*f)(const char *clave, void *valor, void *aux),
	size_t *invocaciones, bool *seguir, void *aux)
{
	while (par_actual && *seguir){
		*seguir = f(par_actual->clave, par_actual->elemento, aux);
		*invocaciones += 1;
		par_actual = par_actual->siguiente;
	}
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux), void *aux)
{
	if(!f || !hash || hash_cantidad(hash) == 0)
		return 0;

	size_t invocaciones = 0;
	bool seguir = true;
	size_t i = 0;
	while (seguir && i < hash->capacidad){
		if (hash->tabla[i] != NULL)
			recorrer_pares_rec(hash->tabla[i], f, &invocaciones, 
								&seguir, aux);
		i++;
	}
	return invocaciones;
}


