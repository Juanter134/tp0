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
        log_error(logger, "Error al leer el archivo de configuración");
	}
	
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	if(config_has_property (config, "PUERTO")){
		puerto = config_get_string_value(config, "PUERTO");
		log_info(logger,"El valor de PUERTO es: %s", puerto);
	}
	if(config_has_property (config, "IP")){
		ip = config_get_string_value(config, "IP");
		log_info(logger,"El valor de IP es: %s", ip);
	}
	// Loggeamos el valor de config

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	bool res = realizar_handshake(conexion, logger);
	if(res){
		enviar_mensaje(valor, conexion);
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

	while (true){
		leido = readline("> ");
		log_info(logger, leido);

		if (!strcmp(leido, "")) {
			free(leido);
			break;
		}

		free(leido);
	}

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
	char* leido;
	t_paquete* paquete = crear_paquete();

	while (true) {
		leido = readline("> ");

		if (!strcmp(leido, "")) {
			free(leido);
			break;
		}

		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
	}

	enviar_paquete(paquete, conexion); 
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	config_destroy(config); 
	log_destroy(logger);
	close(conexion);
}
