#include <string.h>
#include <stdlib.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
const size_t CAPACIDAD_MINIMA = 3;

struct par{
	const char *clave;
	void *valor;
	struct par *siguiente;
};

struct hash{
	struct par **tabla;
	size_t capacidad;
	size_t cantidad;
};

hash_t *hash_crear(size_t capacidad)
{	
	hash_t *hash = calloc(1, sizeof(hash_t)); 
	if(!hash)
		return NULL;
	if(capacidad<=CAPACIDAD_MINIMA)
		hash->capacidad = CAPACIDAD_MINIMA;
	else
		hash->capacidad = capacidad;

	hash->tabla = calloc(hash->capacidad, sizeof(struct par*));
	
	if(!hash->tabla){
		free(hash);
		return NULL;
	}

	return hash;
	
}

hash_t *rehashear(hash_t *hash){
	hash_t *hash_nuevo = hash_crear(hash->capacidad * 2);
	if(!hash_nuevo){
		hash_destruir(hash);
		return NULL;
	}
		
	for(int i=0; i<hash->capacidad; i++){
		struct par *actual = hash->tabla[i];
		while(actual){
			hash_insertar(hash, actual->clave, actual->valor,
		      NULL);
			actual = actual->siguiente;
		}
	}
	free(hash);
	return hash_nuevo;
}

int funcion_hash(const char *clave)
{
	int clave_hash = 0;
	for(int i=0; i<strlen(clave);i++){
		clave_hash += clave[i];
	}
	return clave_hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;
	*anterior = NULL;
	float factor_carga = (float)hash->cantidad / (float)hash->capacidad ;
	if(factor_carga > FACTOR_CARGA_MAXIMO){
		hash = rehashear(hash);
	}

	int posicion = funcion_hash(clave)% (int)hash->capacidad;
	struct par *actual = hash->tabla[posicion];
	while(actual){
		if(actual->clave == clave){
			*anterior = actual->valor;
			actual->valor = elemento;
			return hash;
		}
		actual = actual->siguiente;
	}

	struct par *nuevo_par = malloc(sizeof(struct par));
	nuevo_par->clave = clave;
	nuevo_par->valor = elemento;
	nuevo_par->siguiente = hash->tabla[posicion];
	hash->tabla[posicion] = nuevo_par;
	hash->cantidad++;
	return hash;
}

struct par *buscar_par_anterior(struct par *actual, const char *clave)
{
	if(!actual)
		return NULL;
	
	if(actual->siguiente->clave == clave)
		return actual;

	return buscar_par_anterior(actual->siguiente, clave);
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash )
		return NULL;

	int posicion = funcion_hash(clave) % (int)hash->capacidad;
	struct par *actual = hash->tabla[posicion];
	struct par *par_anterior = buscar_par_anterior(actual, clave);
	
	if(!par_anterior)
		return NULL;
	
	struct par *par_auxiliar = par_anterior->siguiente;
	void *elemento_eliminado = par_auxiliar->valor;
	par_anterior->siguiente =  par_auxiliar->siguiente;

	free(par_auxiliar);
	hash->cantidad--;

	return elemento_eliminado;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash)
		return NULL;

	int posicion = funcion_hash(clave) % (int)hash->capacidad;
	struct par *actual = hash->tabla[posicion];
	while(actual){
		if(actual->clave == clave){
			return actual->valor;
		}
		actual = actual->siguiente;
	}
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !hash_obtener(hash, clave))
		return false;

	return true;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if(!hash)
		return;
	for(int i=0; i<hash->capacidad; i++){
		struct par *actual = hash->tabla[i];
		while(actual){
			struct par *auxiliar = actual;
			actual = actual->siguiente;
			free(auxiliar);
		}
	}
	free(hash->tabla);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash || !destructor)
		return;

	for(int i=0; i<hash->capacidad; i++){
		struct par *actual = hash->tabla[i];
		while(actual){
			struct par *auxiliar = actual;
			actual = actual->siguiente;
			destructor(auxiliar->valor);
			free(auxiliar);
		}
	}
	free(hash->tabla);
	free(hash);

}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;

	return n;
}
