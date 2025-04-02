#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if(config_has_property (config, "CLAVE")){
		valor = config_get_string_value(config, "CLAVE");
		log_info(logger,"El valor de CLAVE es: %s", valor);
    } else {
        log_error(logger, "Error al leer el archivo de configuración o clave inexistente");
	}
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	bool res = realizar_handshake(conexion, logger);
	if(res){
		size_t bytes;

		int32_t result;

		bytes = send(conexion, valor, strnlen(valor, 10) + 1, 0);
		bytes = recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

		if(result == 0){
			log_info(logger, "Mensaje enviado con éxito");
		}
		else {
			log_error(logger, "Error al enviar el mensaje");
		}
	}

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create ("tp0.log", "nuevo_logger", true, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL) {
        printf("Error al crear el logger\n");
    }

	log_info (nuevo_logger, "Hola! Soy un log");

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = 	config_create ("cliente.config");

	if (nuevo_config == NULL) abort();

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	while (true){
		leido = readline("> ");
		log_info(logger, leido);

		if (!strcmp(leido, "")) {
			free(leido);
			break;
		}

		free(leido);
	}

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	
	// ¡No te olvides de liberar las lineas antes de regresar!
	
}

bool realizar_handshake(int conexion, t_log* logger){
	size_t bytes;

	int32_t handshake = 1;
	int32_t result;

	bytes = send(conexion, &handshake, sizeof(int32_t), 0);
	bytes = recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

	if(result == 0){
		log_info(logger, "Handshake realizado con éxito");
		return true;
	}
	else {
		log_error(logger, "Error al realizar el handshake");
		return false;
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	config_destroy(config); 
	log_destroy(logger);
	free(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
