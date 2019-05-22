/*
 * protocolos.c
 *
 *  Created on: 13 abr. 2019
 *      Author: utnso
 */


#include "protocolos.h"

int get_tamanio_paquete_select(t_paquete_select* paquete_select){
	return paquete_select->nombre_tabla->size + sizeof(uint16_t)+ sizeof(t_operacion)+sizeof(int);
}

int get_tamanio_paquete_create(t_paquete_create* paquete_create){
	return paquete_create->nombre_tabla->size + sizeof(uint16_t)+ sizeof(t_operacion)+sizeof(int)+sizeof(t_consistencia)+sizeof(long);
}

int get_tamanio_paquete_insert(t_paquete_insert* paquete_insert){
	return paquete_insert->nombre_tabla->size+ paquete_insert->valor->size+ 2*sizeof(int)+ sizeof(t_operacion)+sizeof(uint16_t)+sizeof(long);
}

int get_tamanio_paquete_add(t_paquete_add* paquete_add){
	return sizeof(t_operacion) +sizeof(uint16_t) + sizeof(t_consistencia);
}

int get_tamanio_paquete_describe_drop(t_paquete_drop_describe* paquete_drop_describe){
	return sizeof(t_operacion) + sizeof(int) + paquete_drop_describe->nombre_tabla->size;
}


//Creacion de paquetes
t_paquete_add* crear_paquete_add(t_instruccion_lql instruccion){
	t_paquete_add* paquete_add = malloc(sizeof(t_paquete_add));
	paquete_add->codigo_operacion = instruccion.operacion;
	paquete_add->num_memoria = instruccion.parametros.ADD.numero_memoria;
	paquete_add->consistencia = instruccion.parametros.ADD.consistencia;
	return paquete_add;
}

t_paquete_select* crear_paquete_select(t_instruccion_lql instruccion){
	t_paquete_select* paquete_select = malloc(sizeof(t_paquete_select));
	paquete_select->codigo_operacion = instruccion.operacion;
	paquete_select->key = instruccion.parametros.SELECT.key;
	char* nombre_tabla = instruccion.parametros.SELECT.tabla;
	paquete_select->nombre_tabla = malloc(sizeof(t_buffer));
	paquete_select->nombre_tabla->size = string_size(nombre_tabla);
	paquete_select->nombre_tabla->palabra = malloc(paquete_select->nombre_tabla->size);
	memcpy(paquete_select->nombre_tabla->palabra, nombre_tabla, paquete_select->nombre_tabla->size);


	return paquete_select;
}

t_paquete_create* crear_paquete_create(t_instruccion_lql instruccion) {
	t_paquete_create* paquete_create = malloc(sizeof(t_paquete_create));
	paquete_create->codigo_operacion = instruccion.operacion;
	paquete_create->consistencia = instruccion.parametros.CREATE.consistencia;
	paquete_create->num_particiones = instruccion.parametros.CREATE.num_particiones;
	paquete_create->tiempo_compac = instruccion.parametros.CREATE.compactacion_time;
	char* nombre_tabla = instruccion.parametros.CREATE.tabla;
	paquete_create->nombre_tabla = malloc(sizeof(t_buffer));
	paquete_create->nombre_tabla->size = string_size(nombre_tabla);
	paquete_create->nombre_tabla->palabra = malloc(paquete_create->nombre_tabla->size);
	memcpy(paquete_create->nombre_tabla->palabra, nombre_tabla, paquete_create->nombre_tabla->size);
	return paquete_create;
}

t_paquete_insert* crear_paquete_insert(t_instruccion_lql instruccion){
	t_paquete_insert* paquete_insert = malloc(sizeof(t_paquete_insert));
	paquete_insert->codigo_operacion = instruccion.operacion;

	paquete_insert->nombre_tabla = malloc(sizeof(t_buffer));
	char* nombre_tabla = instruccion.parametros.INSERT.tabla;
	paquete_insert->nombre_tabla->size = string_size(nombre_tabla);
	paquete_insert->nombre_tabla->palabra = malloc(paquete_insert->nombre_tabla->size);
	memcpy(paquete_insert->nombre_tabla->palabra, nombre_tabla, paquete_insert->nombre_tabla->size);

	paquete_insert->key = instruccion.parametros.INSERT.key;

	paquete_insert->valor = malloc(sizeof(t_buffer));
	char* valor_ingresar = instruccion.parametros.INSERT.value;
	paquete_insert->valor->size = string_size(valor_ingresar);
	paquete_insert->valor->palabra = malloc(paquete_insert->valor->size);
	memcpy(paquete_insert->valor->palabra, valor_ingresar, paquete_insert->valor->size);

	paquete_insert->timestamp=instruccion.parametros.INSERT.timestamp;
	return paquete_insert;
}

t_paquete_drop_describe* crear_paquete_drop_describe(t_instruccion_lql instruccion){
	t_paquete_drop_describe* paquete = malloc(sizeof(t_paquete_drop_describe));
	paquete->codigo_operacion = instruccion.operacion;
	paquete->nombre_tabla = malloc(sizeof(t_buffer));
	char* nombre_tabla = instruccion.parametros.DROP.tabla;
	paquete->nombre_tabla->size = string_size(nombre_tabla);
	paquete->nombre_tabla->palabra = malloc(paquete->nombre_tabla->size);
	memcpy(paquete->nombre_tabla->palabra, nombre_tabla, paquete->nombre_tabla->size);

	return paquete;
}

//Serializacion de paquetes
char* serializar_paquete_select(t_paquete_select* paquete_select, int bytes)
{
	char * serializado = malloc(bytes);
	int desplazamiento = 0;

	memcpy(serializado + desplazamiento, &(paquete_select->codigo_operacion), sizeof(t_operacion));
	desplazamiento+= sizeof(t_operacion);
	memcpy(serializado + desplazamiento, &(paquete_select->nombre_tabla->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serializado + desplazamiento, paquete_select->nombre_tabla->palabra, paquete_select->nombre_tabla->size);
	desplazamiento+= paquete_select->nombre_tabla->size;
	memcpy(serializado + desplazamiento, &(paquete_select->key), sizeof(uint16_t));
		desplazamiento+= sizeof(uint16_t);
	return serializado;
}

char* serializar_paquete_create(t_paquete_create* paquete_create, int bytes){
	char * serializado = malloc(bytes);
	int desplazamiento = 0;

	memcpy(serializado + desplazamiento, &(paquete_create->codigo_operacion), sizeof(t_operacion));
	desplazamiento+= sizeof(t_operacion);
	memcpy(serializado + desplazamiento, &(paquete_create->nombre_tabla->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serializado + desplazamiento, paquete_create->nombre_tabla->palabra, paquete_create->nombre_tabla->size);
	desplazamiento+= paquete_create->nombre_tabla->size;
	memcpy(serializado + desplazamiento, &(paquete_create->consistencia), sizeof(t_consistencia));
	desplazamiento+= sizeof(t_consistencia);
	memcpy(serializado + desplazamiento, &(paquete_create->num_particiones), sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);
	memcpy(serializado + desplazamiento, &(paquete_create->tiempo_compac), sizeof(long));
	desplazamiento+= sizeof(long);
	return serializado;
}

char* serializar_paquete_insert(t_paquete_insert* paquete_insert, int bytes){
	char* serializado = malloc(bytes);
	int desplazamiento = 0;
	memcpy(serializado + desplazamiento, &(paquete_insert->codigo_operacion), sizeof(t_operacion));
	desplazamiento+= sizeof(t_operacion);
	memcpy(serializado + desplazamiento, &(paquete_insert->nombre_tabla->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serializado + desplazamiento, paquete_insert->nombre_tabla->palabra, paquete_insert->nombre_tabla->size);
	desplazamiento+= paquete_insert->nombre_tabla->size;
	memcpy(serializado + desplazamiento, &(paquete_insert->key), sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);
	memcpy(serializado + desplazamiento, &(paquete_insert->valor->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serializado + desplazamiento, paquete_insert->valor->palabra, paquete_insert->valor->size);
	desplazamiento+= paquete_insert->valor->size;
	memcpy(serializado + desplazamiento, &(paquete_insert->timestamp), sizeof(long));
	desplazamiento+= sizeof(long);

	return serializado;
}

char* serializar_paquete_add(t_paquete_add* paquete_add, int bytes){
	char* serializado = malloc(bytes);
	int desplazamiento = 0;
	memcpy(serializado + desplazamiento, &(paquete_add->codigo_operacion), sizeof(t_operacion));
	desplazamiento+= sizeof(t_operacion);
	memcpy(serializado + desplazamiento, &(paquete_add->num_memoria), sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);
	memcpy(serializado + desplazamiento, &(paquete_add->consistencia), sizeof(t_consistencia));
	desplazamiento+= sizeof(t_consistencia);
	return serializado;
}

char* serialiazar_paquete_drop_describe(t_paquete_drop_describe* paquete, int bytes){
	char* serializado = malloc(bytes);
	int desplazamiento = 0;
	memcpy(serializado + desplazamiento, &(paquete->codigo_operacion), sizeof(t_operacion));
	desplazamiento+= sizeof(t_operacion);
	memcpy(serializado + desplazamiento, &(paquete->nombre_tabla->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serializado + desplazamiento, paquete->nombre_tabla->palabra, paquete->nombre_tabla->size);
	desplazamiento+= paquete->nombre_tabla->size;
	return serializado;
}


//Desearializacion de paquetes

t_paquete_select* deserializar_select (int socket_cliente){
	t_paquete_select* consulta_select = malloc(sizeof(t_paquete_select));
	consulta_select->codigo_operacion = SELECT;
	consulta_select->nombre_tabla = malloc(sizeof(t_buffer));
	recv(socket_cliente,  &(consulta_select->nombre_tabla->size), sizeof(int), MSG_WAITALL);
	int size_nombre_tabla = consulta_select->nombre_tabla->size;
	char * nombre_tabla = malloc(size_nombre_tabla);
	consulta_select->nombre_tabla->palabra = malloc(consulta_select->nombre_tabla->size);
	recv(socket_cliente, nombre_tabla, size_nombre_tabla, MSG_WAITALL);
	strcpy(consulta_select->nombre_tabla->palabra ,nombre_tabla);
	recv(socket_cliente,  &(consulta_select->key), sizeof(uint16_t), MSG_WAITALL);

	return consulta_select;//Acordarse de hacer un free despues de usarse
}

t_paquete_create* deserializar_create (int socket_cliente){
	t_paquete_create* consulta_create = malloc(sizeof(t_paquete_create));
	consulta_create->codigo_operacion=CREATE;
	consulta_create->nombre_tabla = malloc(sizeof(t_buffer));
	recv(socket_cliente,  &(consulta_create->nombre_tabla->size), sizeof(int), MSG_WAITALL);
	int size_nombre_tabla = consulta_create->nombre_tabla->size;
	char * nombre_tabla = malloc(size_nombre_tabla);
	consulta_create->nombre_tabla->palabra = malloc(consulta_create->nombre_tabla->size);
	recv(socket_cliente, nombre_tabla, size_nombre_tabla, MSG_WAITALL);
	memcpy(consulta_create->nombre_tabla->palabra ,nombre_tabla, size_nombre_tabla );
	recv(socket_cliente,  &(consulta_create->consistencia), sizeof(t_consistencia), MSG_WAITALL);
	recv(socket_cliente,  &(consulta_create->num_particiones), sizeof(uint16_t), MSG_WAITALL);
	recv(socket_cliente,  &(consulta_create->tiempo_compac), sizeof(long), MSG_WAITALL);

	return consulta_create;//Acordarse de hacer un free despues de usarse
}

t_paquete_insert* deserealizar_insert(int socket_cliente) {
	t_paquete_insert* consulta_insert = malloc(sizeof(t_paquete_insert));
	consulta_insert->codigo_operacion = INSERT;

	consulta_insert->nombre_tabla = malloc(sizeof(t_buffer));
	recv(socket_cliente,  &(consulta_insert->nombre_tabla->size), sizeof(int), MSG_WAITALL);
	int size_nombre_tabla = consulta_insert->nombre_tabla->size;
	char * nombre_tabla = malloc(size_nombre_tabla);
	consulta_insert->nombre_tabla->palabra = malloc(consulta_insert->nombre_tabla->size);
	recv(socket_cliente, nombre_tabla, size_nombre_tabla, MSG_WAITALL);
	memcpy(consulta_insert->nombre_tabla->palabra ,nombre_tabla, size_nombre_tabla );

	recv(socket_cliente, &(consulta_insert->key), sizeof(uint16_t), MSG_WAITALL);

	consulta_insert->valor = malloc(sizeof(t_buffer));
	recv(socket_cliente,  &(consulta_insert->valor->size), sizeof(int), MSG_WAITALL);
	int size_valor = consulta_insert->valor->size;
	char * valor = malloc(size_valor);
	consulta_insert->valor->palabra = malloc(consulta_insert->valor->size);
	recv(socket_cliente, valor, size_valor, MSG_WAITALL);
	memcpy(consulta_insert->valor->palabra ,valor, size_valor );

	recv(socket_cliente,  &(consulta_insert->timestamp), sizeof(long), MSG_WAITALL);
	return consulta_insert;//Acordarse de hacer un free despues de usarse
}

t_paquete_add* desearilizar_add(int socket_cliente){
	t_paquete_add* consulta_add = malloc(sizeof(t_paquete_add));
	consulta_add->codigo_operacion = ADD;
	recv(socket_cliente,  &(consulta_add->num_memoria), sizeof(uint16_t), MSG_WAITALL);
	recv(socket_cliente,  &(consulta_add->consistencia), sizeof(t_consistencia), MSG_WAITALL);

	return consulta_add;//Acordarse de hacer un free despues de usarse
}

t_paquete_drop_describe* deserealizar_drop_describe(int socket_cliente) {
	t_paquete_drop_describe* consulta = malloc(sizeof(t_paquete_drop_describe));
	consulta->nombre_tabla = malloc(sizeof(t_buffer));
	recv(socket_cliente,  &(consulta->nombre_tabla->size), sizeof(int), MSG_WAITALL);
	int size_nombre_tabla = consulta->nombre_tabla->size;
	char * nombre_tabla = malloc(size_nombre_tabla);
	consulta->nombre_tabla->palabra = malloc(consulta->nombre_tabla->size);
	recv(socket_cliente, nombre_tabla, size_nombre_tabla, MSG_WAITALL);
	memcpy(consulta->nombre_tabla->palabra ,nombre_tabla, size_nombre_tabla );

	return consulta;//Acordarse de hacer un free despues de usarse
}

/*struct tablaMemoriaGossip crearTablaGossip(){
	struct tablaMemoriaGossip* elementoCreado = malloc(sizeof(struct tablaMemoriaGossip));
	//-- CREAR PRIMER ELEMENTO(MEMORIA A, LA PRIEMRA QUE SE LEVANTA, HAY QUE PASAR POR PARAMETROS LOS DATOS DE LA MISMA--//
	return *elementoCreado;
}*/

struct tablaMemoriaGossip crearElementoParaTablaDeGossip(struct memoriaGossip memoria){ // hay que pasar por parametro un struct con los datos de la nueva memoria
	struct memoriaGossip* memogossip = malloc(sizeof(struct memoriaGossip));
	struct tablaMemoriaGossip* elementoCreado = malloc(sizeof(struct tablaMemoriaGossip));
	// memogossip.descriptorMemoria = tabla.descriptorMemoria(seteo descriptor de la nueva memoria)
	// memogossip.IP = tabla.IP(seteo ip de la nueva memoria)
	// memogossip.PUERTO = tabla.PUERTO(seteo puerto de la nueva memoria)
	// memogossip.estado= tabla.estado(seteo estado)
	// memogossip.siguiente=null;
	elementoCreado->memoria = *memogossip;
	elementoCreado->siguiente= NULL;
	return *elementoCreado;
}

void agregarMemoriaALaTablaGossip(struct tablaMemoriaGossip* tabla, struct tablaMemoriaGossip* elementoAAgregar){
	if(tabla->siguiente == NULL){
		tabla->siguiente = elementoAAgregar;
	} else { agregarMemoriaALaTablaGossip(tabla->siguiente,elementoAAgregar); };
}

