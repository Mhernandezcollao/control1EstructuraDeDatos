#define _XOPEN_SOURCE 500 //Habilita funciones de  la biblioteca (strdup) en linux.

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

/*
 * Estructura que guarda los datos de la taba.
 */
struct entrada_s {
	char *clave;
	char *valor;
	struct entrada_s *sgte;
};
typedef struct entrada_s entrada_t;

/*
 * Estructura  de la hashtable.
 */
struct hashtable_s {
	int largo;
	struct entrada_s **tabla;	
};
typedef struct hashtable_s hashtable_t;


/*
 * Función que permite crear una nueva tabla.
 */
hashtable_t *crearHT( int largo ) {

	hashtable_t *hashtable = NULL;
	int i;

	if( largo < 1 ) return NULL;

	if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( hashtable->tabla = malloc( sizeof( entrada_t * ) * largo ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < largo; i++ ) {
		hashtable->tabla[i] = NULL;
	}

	hashtable->largo = largo;

	return hashtable;	
}

/*
 * Función que crear una cadena para una tabla.
 */
int hashHT( hashtable_t *hashtable, char *clave ) {

	unsigned long int hashval;
	int i = 0;

	while( hashval < ULONG_MAX && i < strlen( clave ) ) {
		hashval = hashval << 8;
		hashval += clave[ i ];
		i++;
	}

	return hashval % hashtable->largo;
}

/* Create a key-value pair. */
entrada_t *nuevoPar( char *clave, char *valor ) {
	entrada_t *newpar;

	if( ( newpar = malloc( sizeof( entrada_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( newpar->clave = strdup( clave ) ) == NULL ) {
		return NULL;
	}

	if( ( newpar->valor = strdup( valor ) ) == NULL ) {
		return NULL;
	}

	newpar->sgte = NULL;

	return newpar;
}


/*
 * Función que inserta un par clave-valor en una tabla hash.
 */
void insertar( hashtable_t *hashtable, char *clave, char *valor ) {
	int com = 0;
	entrada_t *newpar = NULL;
	entrada_t *sgte = NULL;
	entrada_t *ul = NULL;

	com = hashHT( hashtable, clave );

	sgte = hashtable->tabla[ com ];

	while( sgte != NULL && sgte->clave != NULL && strcmp( clave, sgte->clave ) > 0 ) {
		ul = sgte;
		sgte = sgte->sgte;
	}

	// Reemplazar cadena
	if( sgte != NULL && sgte->clave != NULL && strcmp( clave, sgte->clave ) == 0 ) {

		free( sgte->valor );
		sgte->valor = strdup( valor );

	// De lo contrarios se crea otro par
	} else {
		newpar = nuevoPar( clave, valor );

		if( sgte == hashtable->tabla[ com ] ) {
			newpar->sgte = sgte;
			hashtable->tabla[ com ] = newpar;
	
		} else if ( sgte == NULL ) {
			ul->sgte = newpar;

		} else  {
			newpar->sgte = sgte;
			ul->sgte = newpar;
		}
	}
}

/*
 * Función que recupere un par clave-valor de una tabla.
 */
char *recuperar( hashtable_t *hashtable, char *clave ) {
	int com = 0;
	entrada_t *par;

	com = hashHT( hashtable, clave );

	par = hashtable->tabla[ com ];
	while( par != NULL && par->clave != NULL && strcmp( clave, par->clave ) > 0 ) {
		par = par->sgte;
	}

	if( par == NULL || par->valor == NULL || strcmp( clave, par->clave ) != 0 ) {
		return NULL;

	} else {
		return par->valor;
	}
	
}


int main( int argc, char **argv ) {

	hashtable_t *hashtable = crearHT( 65536 );

	insertar( hashtable, "123", "lucy" );
	insertar( hashtable, "456", "ruben" );
	insertar( hashtable, "789", "saray" );
	insertar( hashtable, "321", "may" );

	printf( "%s\n", recuperar( hashtable, "123" ) );
	printf( "%s\n", recuperar( hashtable, "456" ) );
	printf( "%s\n", recuperar( hashtable, "789" ) );
	printf( "%s\n", recuperar( hashtable, "321" ) );

	return 0;
}
