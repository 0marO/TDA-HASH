#include "src/hash.h"
#include "pa2m.h"

#include "src/estructuras_y_auxiliares.h"

#include <stdlib.h>
#include <string.h>

void pruebas_de_crear_un_hash_abierto()
{	
	size_t capacidad = 2;
	hash_t *hash = hash_crear(capacidad);
	pa2m_afirmar(hash != NULL, "Se crea un hash y es distinto de NULL.");

	hash_destruir(hash);

	capacidad = 6969;
	hash = hash_crear(capacidad);
	pa2m_afirmar(hash != NULL, "Se crea un hash con otra capacidad y es distinto de NULL.");

	hash_destruir(hash);
}

void pruebas_de_insertar_elementos_en_un_hash_abierto()
{
	pa2m_afirmar(hash_insertar(NULL, "AC123BD", "Codigo de Omar", NULL) == NULL,
		     "No se puede insertar un par con hash NULL.");

	size_t capacidad = 3;
	hash_t *hash = hash_crear(capacidad);

	pa2m_afirmar(hash_insertar(hash, NULL, "Codigo de Omar", NULL) == NULL,
		     "No se puede insertar un par con clave NULL.");
	pa2m_afirmar(hash_cantidad(hash) == 0, "La cantidad de elementos en el hash es 0");

	void *anterior = NULL;

	pa2m_afirmar(hash_insertar(hash, "AC123BD", NULL, &anterior) != NULL && anterior == NULL,
		     "Se puede insertar un par al hash vacio con valor NULL. el anterior es NULL.");

	hash_quitar(hash,"AC123BD");

	pa2m_afirmar(hash_insertar(hash, "AC123BD", "Codigo de Omar", &anterior) != NULL && anterior == NULL,
		     "se inserto un par al hash vacio. el anterior es NULL.");
	pa2m_afirmar(hash_cantidad(hash) == 1, "La cantidad de elementos en el hash es 1");

	pa2m_afirmar(hash_insertar(hash, "AC123BD", "Codigo de Rodrigo", &anterior) != NULL &&
			 strcmp("Codigo de Omar", (char *)anterior) == 0,
		     "se inserto un par al hash con la MISMA CLAVE. El anterior es igual al valor ingresado previamente.");
	pa2m_afirmar(hash_cantidad(hash) == 1, "La cantidad de elementos en el hash es 1");

	pa2m_afirmar(hash_insertar(hash, "AC123BD", "Codigo de Pablo", &anterior) != NULL &&
			 strcmp("Codigo de Rodrigo", (char *)anterior) == 0,
		     "se inserto un par al hash con la MISMA CLAVE. El anterior es igual al valor ingresado previamente.");
	pa2m_afirmar(hash_cantidad(hash) == 1, "La cantidad de elementos en el hash es 1");

	anterior = NULL;
	pa2m_afirmar(hash_insertar(hash, "OPQ976", "Codigo de Pablo", &anterior) != NULL && anterior == NULL,
		     "se inserto un par al hash con DIFERENTE clave y mismo valor. El anterior es NULL.");
	pa2m_afirmar(hash_cantidad(hash) == 2, "La cantidad de elementos en el hash es 2");

	pa2m_afirmar(hash_insertar(hash, "DZE443", "Codigo de Pablo", &anterior) != NULL && anterior == NULL,
		     "se inserto un par al hash con DIFERENTE clave y mismo valor. El anterior es NULL.");
	pa2m_afirmar(hash_cantidad(hash) == 3, "La cantidad de elementos en el hash es 3");

	pa2m_afirmar(hash_insertar(hash, "BD123AC", "Codigo de Pablo", &anterior) != NULL && anterior == NULL,
		     "se inserto un par al hash con DIFERENTE clave y mismo valor. El anterior es NULL.");
	pa2m_afirmar(hash_cantidad(hash) == 4, "La cantidad de elementos en el hash es 4");

	pa2m_afirmar(hash_insertar(hash, "BD123AC", "Codigo de Rogelio", &anterior) != NULL &&
			 strcmp("Codigo de Pablo", (char *)anterior) == 0,
		     "se inserto un par al hash con la MISMA CLAVE. El anterior es igual al valor ingresado previamente.");
	pa2m_afirmar(hash_cantidad(hash) == 4, "La cantidad de elementos en el hash SIGUE SIENDO 4");

	// Prueba de caja blanca
	pa2m_afirmar(hash->capacidad != 3, "Al insertar 4 elementos, la capacidad inicial de 3 cambió (rehash).");

	pa2m_afirmar(hash_insertar(hash, "AC123BD", NULL, &anterior) != NULL && anterior != NULL,
		     "Se puede insertar un par al hash con valor NULL y clave repetida. el anterior NO ES NULL.");

	pa2m_afirmar(hash_insertar(hash, "BD968AC", NULL, &anterior) != NULL && anterior == NULL,
		     "Se puede insertar un par nuevo al hash  con valor NULL y clave no repetida. el anterior es NULL.");

	pa2m_afirmar(hash_insertar(hash, "BD968AC", "pepe", &anterior) != NULL && anterior == NULL,
		     "Se puede insertar un par al hash  con valor NO NULL y clave repetida con valor previo NULL. el anterior es NULL.");


	hash_destruir_todo(hash, NULL);
}

void pruebas_de_quitar_elementos_del_hash_abierto()
{

	pa2m_afirmar(hash_quitar(NULL, "BD123AC") == NULL,
		     "No se puede quitar un elemento con hash NULL.");

	size_t capacidad = 3;
	hash_t *hash = hash_crear(capacidad);

	pa2m_afirmar(hash_quitar(NULL, "BD123AC") == NULL,
		     "No se puede quitar un elemento cuya clave no existe en el hash.");

	pa2m_afirmar(hash_cantidad(hash) == 0, "La cantidad de elementos en el hash es 0");

	void *anterior = NULL;
	hash_insertar(hash, "BD123AC", "Codigo de Rogelio", &anterior);

	pa2m_afirmar(hash_cantidad(hash) == 1, "La cantidad de elementos en el hash ahora es 1");

	char *resultado = (char *)hash_quitar(hash, "BD123AC");
	pa2m_afirmar(strcmp(resultado, "Codigo de Rogelio") == 0,
		     "Se busca quitar el unico elemento del hash con su clave. La op. es existosa y devuelve el elemento.");

	pa2m_afirmar(hash_cantidad(hash) == 0, "La cantidad de elementos en el hash vuelve a ser 0.");

	hash_insertar(hash, "AC123BD", "Codigo de Rogelio", &anterior);
	hash_insertar(hash, "OPQ976", "Codigo de Fer", &anterior);
	hash_insertar(hash, "A421ACB", "Codigo de Pepe", &anterior);
	hash_insertar(hash, "AA442CD", "Codigo de Mati", &anterior);
	hash_insertar(hash, "QDM443", "Codigo de Lea", &anterior);

	pa2m_afirmar(hash_cantidad(hash) == 5,
		     "La cantidad de elementos ahora es 5. (si o si hubo rehash)");

	resultado = (char *)hash_quitar(hash, "A421ACB");
	pa2m_afirmar(strcmp(resultado, "Codigo de Pepe") == 0,
		     "Se busca quitar un elemento del hash con su clave. La op. es existosa y devuelve el elemento.");

	pa2m_afirmar(hash_cantidad(hash) == 4,
		     "La cantidad de elementos ahora es 4.");

	pa2m_afirmar(hash_quitar(hash, "A421ACB") == NULL,
		     "Se busca quitar un elemento con una clave ya sacada. como la clave no está devuelve NULL.");

	pa2m_afirmar(hash_cantidad(hash) == 4,
		     "La cantidad de elementos sigue siendo 4.");

	resultado = (char *)hash_quitar(hash, "NO EXISTE");
	pa2m_afirmar(resultado == NULL,
		     "Se busca quitar un el. cuya clave no existe en el hash. No lo encuantra y devuelve NULL.");

	resultado = (char *)hash_quitar(hash, "AC123BD");
	pa2m_afirmar(strcmp(resultado, "Codigo de Rogelio") == 0,
		     "Se busca quitar un elemento del hash con su clave. La op. es existosa y devuelve el elemento.");

	pa2m_afirmar(hash_cantidad(hash) == 3,
		     "La cantidad de elementos ahora es 3.");

	hash_quitar(hash, "OPQ976");
	hash_quitar(hash, "AA442CD");
	hash_quitar(hash, "QDM443");

	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Se quitaron todos los elementos y ahora la cantidad es 0.");

	hash_destruir(hash);
}

void pruebas_de_buscar_y_obtener_elementos_del_hash_abierto()
{

	pa2m_afirmar(hash_contiene(NULL, "AC123BD") == false,
		     "Se busca si un hash NULL contiene un el. con determinada clave. El resultado es falso.");

	pa2m_afirmar(hash_obtener(NULL, "AC123BD") == NULL,
		     "Se busca obtener de un hash NULL un el. con determinada clave. El resultado es NULL.");

	size_t capacidad = 3;
	hash_t *hash = hash_crear(capacidad);

	pa2m_afirmar(hash_contiene(hash, "AC123BD") == false,
		     "Se busca si un hash contiene un el. con determinada clave no insertada. El resultado es falso.");

	pa2m_afirmar(hash_obtener(hash, "AC123BD") == NULL,
		     "Se busca obtener de un hash un el. con determinada clave no insertada. El resultado es NULL.");

	void *anterior = NULL;

	hash_insertar(hash, "AC123BD", "Codigo de Rogelio", &anterior);
	hash_insertar(hash, "OPQ976", "Codigo de Fer", &anterior);
	hash_insertar(hash, "A421ACB", "Codigo de Pepe", &anterior);
	hash_insertar(hash, "AA442CD", "Codigo de Mati", &anterior);
	hash_insertar(hash, "QDM443", "Codigo de Lea", &anterior);

	pa2m_afirmar(hash_contiene(hash, "AC123BD") == true,
		     "Se busca si un hash contiene un el. con determinada clave insertada previamente. El resultado es true.");
	pa2m_afirmar(hash_contiene(hash, "OPQ976") == true,
		     "Se busca si un hash contiene un el. con determinada clave insertada previamente. El resultado es true.");
	pa2m_afirmar(hash_contiene(hash, "A421ACB") == true,
		     "Se busca si un hash contiene un el. con determinada clave insertada previamente. El resultado es true.");
	pa2m_afirmar(hash_contiene(hash, "AA442CD") == true,
		     "Se busca si un hash contiene un el. con determinada clave insertada previamente. El resultado es true.");

	pa2m_afirmar(strcmp((char *)hash_obtener(hash, "AC123BD"), "Codigo de Rogelio") == 0,
		     "Se busca obtener de un hash un el. con determinada clave insertada previamente. El resultado es ese el.");

	pa2m_afirmar(strcmp((char *)hash_obtener(hash, "A421ACB"), "Codigo de Pepe") == 0,
		     "Se busca obtener de un hash un el. con determinada clave insertada previamente. El resultado es ese el.");

	hash_insertar(hash, "QDM443", "CODIGO CAMBIADO", &anterior);

	pa2m_afirmar(strcmp((char *)hash_obtener(hash, "QDM443"), "CODIGO CAMBIADO") == 0,
		     "Se inserto clave repetida con otro valor. Al obtener esa clave, el valor devuelto es el nuevo.");

	pa2m_afirmar(hash_contiene(hash, "DZE443") == false,
		     "Se busca si un hash contiene un el. con determinada clave no insertada. El resultado es falso.");

	pa2m_afirmar(hash_obtener(hash, "DZE443") == NULL,
		     "Se busca obtener de un hash un el. con determinada clave no insertada. El resultado es NULL.");

	hash_destruir(hash);
}

void modificar_string(void *elemento)
{
	if (!elemento)
		return;
	strcpy(elemento, "MODIFICADO");
}

void pruebas_de_destruir_todo_un_hash_creado_junto_con_sus_elementos()
{
	size_t capacidad = 3;
	hash_t *hash = hash_crear(capacidad);

	void *anterior = NULL;

	char *valor = malloc(sizeof(char) * 16);
	if(!valor)
		return;
	strcpy(valor, "Codigo de Pepe");

	char *valor2 = malloc(sizeof(char) * 16);
	if(!valor2){
		free(valor);
		return;
	}
	strcpy(valor2, "Codigo de Mati");

	char *valor3 = malloc(sizeof(char) * 14);
	if(!valor3){
		free(valor);
		free(valor2);
		return;
	}
	strcpy(valor3, "Codigo de Lea");

	char *valor4 = malloc(sizeof(char) * 19);
	if(!valor4){
		free(valor);
		free(valor2);
		free(valor3);
		return;
	}
	strcpy(valor4, "Codigo de Rogelio");

	hash_insertar(hash, "A421ACB", valor, &anterior);
	hash_insertar(hash, "AA442CD", valor2, &anterior);
	hash_insertar(hash, "QDM443", valor3, &anterior);
	hash_insertar(hash, "AC123BD", valor4, &anterior);

	hash_destruir_todo(hash, &modificar_string);

	pa2m_afirmar(strcmp(valor, "MODIFICADO") == 0,
		     "Al destruir el hash, se aplico correctamente la funcion destructora con el elemento.");
	pa2m_afirmar(strcmp(valor2, "MODIFICADO") == 0,
		     "Al destruir el hash, se aplico correctamente la funcion destructora con el elemento.");
	pa2m_afirmar(strcmp(valor3, "MODIFICADO") == 0,
		     "Al destruir el hash, se aplico correctamente la funcion destructora con el elemento.");
	pa2m_afirmar(strcmp(valor4, "MODIFICADO") == 0,
		     "Al destruir el hash, se aplico correctamente la funcion destructora con el elemento.");

	free(valor);
	free(valor2);
	free(valor3);
	free(valor4);
}

bool cambiar_valor_del_par(const char *clave, void *valor, void *aux)
{
	if (aux != NULL && strcmp((char *)valor, (char *)aux) == 0){
		strcpy((char *)valor, "MODIFICADO");
		return false;
	}

	return true;
}

void pruebas_del_iterador_interno_con_una_funcion_provista_por_el_usuario()
{
	size_t capacidad = 3;
	hash_t *hash = hash_crear(capacidad);



	void *anterior = NULL;

	hash_insertar(hash, "AC123BD", "Codigo de Rogelio", &anterior);
	hash_insertar(hash, "OPQ976", "Codigo de Fer", &anterior);
	hash_insertar(hash, "A421ACB", "Codigo de Pepe", &anterior);
	hash_insertar(hash, "AA442CD", "Codigo de Mati", &anterior);

	char *valor = malloc(sizeof(char) * 20);
	if(!valor)
		return;
	strcpy(valor, "CODIGO INICIAL");
	hash_insertar(hash, "QDM443", valor, &anterior);


	size_t invocaciones = hash_con_cada_clave(hash, &cambiar_valor_del_par, NULL);
	pa2m_afirmar(invocaciones == 5,
		     "Se invoca el iterador interno y las invocaciones son iguales a la cant de elementos.");


	invocaciones = hash_con_cada_clave(hash, &cambiar_valor_del_par, valor);

	pa2m_afirmar(strcmp(valor, "MODIFICADO") == 0,
		     "Se comprueba que la funcion del usuario tuvo efecto con éxito.");

	hash_destruir(hash);
	free(valor);
}

static char *rand_string(char *str, size_t tamanio)
{
	const char caracteres[] = "ABCDEFGHIJK1234567890...";
	if (tamanio) {
		--tamanio;
		for (size_t n = 0; n < tamanio; n++) {
			int posicion = rand() % (int) (sizeof caracteres - 1);
			str[n] = caracteres[posicion];
		}
		str[tamanio] = '\0';
	}	
    	return str;
}

void destruir_string(void *elemento)
{
	if (!elemento)
		return;
	free(elemento);
}

void pruebas_de_insertar_muchos_elementos()
{
	size_t capacidad = 3;
	hash_t *hash = hash_crear(capacidad);

	int inserciones = 300000;
	char *string = malloc(sizeof(char)*6);
	void* anterior = NULL;
	for (int i = 0; i < inserciones; i++){
		hash_insertar(hash,(const char *)rand_string(string, 6), "hola", anterior );
	}
	pa2m_afirmar(hash_cantidad(hash) > 200000, "Se insertaron al menos 200k de elementos.");

	pa2m_afirmar(strcmp((char *)hash_obtener(hash, string), "hola") == 0,
	 "luego de insertar aprox. 200k de elementos, probamos obtener un elemento y la operación es exitosa");
	
	free(string);

	hash_destruir(hash);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de HASH ABIERTO con direccionamiento cerrado.");

	pa2m_nuevo_grupo("Pruebas de crear hash.");
	pruebas_de_crear_un_hash_abierto();

	pa2m_nuevo_grupo("Pruebas de INSERTAR elementos en el hash.");
	pruebas_de_insertar_elementos_en_un_hash_abierto();

	pa2m_nuevo_grupo("Pruebas de QUITAR elementos del hash.");
	pruebas_de_quitar_elementos_del_hash_abierto();

	pa2m_nuevo_grupo("Pruebas de BUSCAR elementos del hash.(contener y obtener)");
	pruebas_de_buscar_y_obtener_elementos_del_hash_abierto();

	pa2m_nuevo_grupo("Pruebas de destruir todo.");
	pruebas_de_destruir_todo_un_hash_creado_junto_con_sus_elementos();

	pa2m_nuevo_grupo("Pruebas de iterador interno.");
	pruebas_del_iterador_interno_con_una_funcion_provista_por_el_usuario();

	pa2m_nuevo_grupo("Pruebas con muchas inserciones.");
	pruebas_de_insertar_muchos_elementos();

	return pa2m_mostrar_reporte();
}
