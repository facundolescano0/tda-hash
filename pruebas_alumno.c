#include "pa2m.h"
#include "src/hash.h"
#include <stdlib.h>
#include <string.h>

const char *clave1 = "A";
const char *clave2 = "B";
const char *clave3 = "C";
const char *clave4 = "AB";
const char *clave5 = "BB";
const char *clave6 = "CB";
const char *clave7 = "CC";

bool funcion_iteradora(const char *clave, void *valor, void *aux)
{
	if (valor == aux) {
		return false;
	}
	return true;
}

void destructor(void *elemento)
{
	return;
}

void prueba_creacion()
{
	hash_t *hash1 = hash_crear(0);
	pa2m_afirmar(hash1 != NULL,
		     "Se crea hash con capacidad menor a 3 y destruyo");
	hash_destruir(hash1);
	hash_t *hash2 = hash_crear(4);
	pa2m_afirmar(hash2 != NULL,
		     "Se crea hash con capacidad mayor a 3 y destruyo todo");
	hash_destruir_todo(hash2, &destructor);
	hash_t *hash3 = hash_crear(4);
	pa2m_afirmar(
		hash3 != NULL,
		"Se crea hash con capacidad mayor a 3 y destruyo con destructor NULL");
	hash_destruir_todo(hash3, NULL);
}

void prueba_hash_vacio()
{
	void *aux = NULL;
	hash_t *hash = hash_crear(5);
	pa2m_afirmar(hash_cantidad(hash) == 0, "Hash no tiene elementos");
	pa2m_afirmar(hash_quitar(hash, clave1) == NULL,
		     "No se puedo quitar en un hash vacio");
	pa2m_afirmar(
		hash_obtener(hash, clave1) == NULL,
		"No se puede obtener el elemento de una clave en un hash vacio");
	pa2m_afirmar(hash_contiene(hash, clave1) == false,
		     "El hash no contiene elementos");
	pa2m_afirmar(hash_con_cada_clave(hash, funcion_iteradora, aux) == 0,
		     "No se puede iterar un hash vacio");
	hash_destruir(hash);
}

void prueba_hash_nulo()
{
	void *aux = NULL;
	hash_t *hash = NULL;
	pa2m_afirmar(hash_cantidad(hash) == 0, "Hash nulo no tiene elementos");
	pa2m_afirmar(hash_quitar(hash, clave1) == NULL,
		     "No se puedo quitar en un hash nulo");
	pa2m_afirmar(
		hash_obtener(hash, clave1) == NULL,
		"No se puede obtener el elemento de una clave en un hash nulo");
	pa2m_afirmar(hash_contiene(hash, clave1) == false,
		     "El hash no contiene elementos");
	pa2m_afirmar(hash_con_cada_clave(hash, funcion_iteradora, aux) == 0,
		     "No se puede iterar un hash nulo");
	hash_destruir(hash);
}

void prueba_inserccion()
{
	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;
	int elemento4 = 4;
	int elemento5 = 5;
	int elemento6 = 6;
	void *elemento = &elemento1;
	void **anterior = &elemento;

	hash_t *hash = hash_crear(0);
	pa2m_afirmar(hash_insertar(hash, NULL, &elemento3, anterior) == NULL,
		     "No se puede insertar par con clave nula");
	pa2m_afirmar(hash_insertar(hash, clave2, NULL, anterior) != NULL,
		     "Se inserta un elemento NULL corectamente");
	pa2m_afirmar(hash_insertar(hash, clave1, &elemento2, anterior) != NULL,
		     "Se inserta un elemento con clave valida");
	pa2m_afirmar(hash_insertar(hash, clave1, &elemento1, anterior) != NULL,
		     "Se inserta un elemento con clave repetida");
	pa2m_afirmar(*anterior == &elemento2,
		     "El elemento anterior es el correcto");
	pa2m_afirmar(hash_insertar(hash, clave2, &elemento2, anterior) != NULL,
		     "Se inserta un elemento con clave repetida");
	pa2m_afirmar(*anterior == NULL, "El elemento anterior es el correcto");
	hash_insertar(hash, clave3, &elemento3, anterior);
	hash_insertar(hash, clave4, &elemento4, anterior);
	hash_insertar(hash, clave5, &elemento5, anterior);
	hash_insertar(hash, clave6, &elemento6, anterior);
	pa2m_afirmar(hash_cantidad(hash) == 6,
		     "Se insertan varias elementos y causa rehash");

	hash_destruir(hash);
}

void prueba_obtener_contener()
{
	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;
	int elemento4 = 4;
	int elemento5 = 5;
	int elemento6 = 6;
	void *elemento7 = NULL;
	const char *clave_sin_insertar = "ABC";
	void *elemento = &elemento1;
	void **anterior = &elemento;

	hash_t *hash = hash_crear(6);

	hash_insertar(hash, clave1, &elemento1, anterior);
	hash_insertar(hash, clave2, &elemento2, anterior);
	hash_insertar(hash, clave3, &elemento3, anterior);
	hash_insertar(hash, clave4, &elemento4, anterior);
	hash_insertar(hash, clave5, &elemento5, anterior);
	hash_insertar(hash, clave6, &elemento6, anterior);
	hash_insertar(hash, clave7, elemento7, anterior);

	pa2m_afirmar(!hash_contiene(hash, NULL),
		     "Hash no contiene elementos con clave nula");
	pa2m_afirmar(hash_obtener(hash, NULL) == NULL,
		     "No se puede obtener un elemento con clave nula");
	pa2m_afirmar(!hash_contiene(hash, clave_sin_insertar),
		     "Hash no contiene una clave que no esta");
	pa2m_afirmar(hash_obtener(hash, clave_sin_insertar) == NULL,
		     "No se puede obtener una clave que no esta");
	pa2m_afirmar(hash_contiene(hash, clave7),
		     "Hash contiene elemento NULL");
	pa2m_afirmar(hash_obtener(hash, clave7) == NULL,
		     "Se obtiene elemento NULL");
	hash_quitar(hash, clave7);
	pa2m_afirmar(!hash_contiene(hash, clave7),
		     "Se quita elemento y no lo contiene");
	pa2m_afirmar(hash_obtener(hash, clave7) == NULL,
		     "Se quita elemento y no se lo obtiene");
	pa2m_nuevo_grupo("Pruebas de obtener y contener varios elementos");
	pa2m_afirmar(hash_contiene(hash, clave1),
		     "Hash contiene primer elemento");
	pa2m_afirmar(hash_obtener(hash, clave1) == &elemento1,
		     "Se obtiene primer elemento");
	pa2m_afirmar(hash_contiene(hash, clave2),
		     "Hash contiene segundo elemento");
	pa2m_afirmar(hash_obtener(hash, clave2) == &elemento2,
		     "Se obtiene segundo elemento");
	pa2m_afirmar(hash_contiene(hash, clave3),
		     "Hash contiene tercer elemento");
	pa2m_afirmar(hash_obtener(hash, clave3) == &elemento3,
		     "Se obtiene tercero elemento");
	pa2m_afirmar(hash_contiene(hash, clave4),
		     "Hash contiene cuarto elemento");
	pa2m_afirmar(hash_obtener(hash, clave4) == &elemento4,
		     "Se obtiene cuarto elemento");
	pa2m_afirmar(hash_contiene(hash, clave5),
		     "Hash contiene quinto elemento");
	pa2m_afirmar(hash_obtener(hash, clave5) == &elemento5,
		     "Se obtiene quinto elemento");
	pa2m_afirmar(hash_contiene(hash, clave6),
		     "Hash contiene sexto elemento");
	pa2m_afirmar(hash_obtener(hash, clave6) == &elemento6,
		     "Se obtiene sexto elemento");

	hash_destruir(hash);
}

void prueba_eliminacion()
{
	int elemento1 = 1;
	void *elemento = &elemento1;
	void **anterior = &elemento;
	hash_t *hash = hash_crear(0);
	hash_insertar(hash, clave1, &elemento1, anterior);
	hash_insertar(hash, clave2, NULL, anterior);
	pa2m_afirmar(hash_quitar(hash, NULL) == NULL,
		     "No se puede quitar una clave nula");
	pa2m_afirmar(hash_quitar(hash, clave2) == NULL,
		     "No se puede quitar un elemento que no existe");
	pa2m_afirmar(hash_quitar(hash, clave2) == NULL,
		     "Se quita un elemento NULL con clave valida");
	pa2m_afirmar(hash_quitar(hash, clave1) == &elemento1,
		     "Se quita un elemento con clave valida");
	pa2m_afirmar(hash_quitar(hash, clave1) == NULL,
		     "No se puede quitar el mismo elemento denuevo");
	hash_destruir(hash);
}

void prueba_iterador_interno()
{
	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;
	int elemento4 = 4;
	int elemento5 = 5;
	int elemento6 = 6;
	void *elemento7 = NULL;
	int elemento8 = 11;

	void *elemento = &elemento1;
	void **anterior = &elemento;
	void *aux_nulo = NULL;
	void *aux = &elemento8;
	void *aux_corte = &elemento4;
	hash_t *hash = hash_crear(6);

	hash_insertar(hash, clave1, &elemento1, anterior);
	hash_insertar(hash, clave2, &elemento2, anterior);
	hash_insertar(hash, clave3, &elemento3, anterior);
	hash_insertar(hash, clave4, &elemento4, anterior);
	hash_insertar(hash, clave5, &elemento5, anterior);
	hash_insertar(hash, clave6, &elemento6, anterior);
	hash_insertar(hash, clave7, elemento7, anterior);
	pa2m_afirmar(hash_con_cada_clave(hash, NULL, aux) == 0,
		     "No se puede iterar con una funcion nula");
	pa2m_afirmar(hash_con_cada_clave(hash, funcion_iteradora, aux_corte) !=
			     7,
		     "La iteracion se detiene correctamente");
	pa2m_afirmar(hash_con_cada_clave(hash, funcion_iteradora, aux_nulo) ==
			     7,
		     "Se puede iterar con aux nulo");
	pa2m_afirmar(hash_con_cada_clave(hash, funcion_iteradora, aux) == 7,
		     "Se iteran todos los elementos");

	hash_destruir_todo(hash, destructor);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creacion");
	prueba_creacion();
	pa2m_nuevo_grupo("Pruebas de hash vacio");
	prueba_hash_vacio();
	pa2m_nuevo_grupo("Pruebas de hash nulo");
	prueba_hash_nulo();
	pa2m_nuevo_grupo("Pruebas de inserccion");
	prueba_inserccion();
	pa2m_nuevo_grupo("Pruebas bordes de obtener y contener");
	prueba_obtener_contener();
	pa2m_nuevo_grupo("Pruebas de eliminacion");
	prueba_eliminacion();
	pa2m_nuevo_grupo("Pruebas de iterador interno");
	prueba_iterador_interno();
	return pa2m_mostrar_reporte();
}