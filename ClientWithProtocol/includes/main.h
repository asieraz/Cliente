/*
 * main.h
 *
 *  Created on: 20 may 2025
 *      Author: j.fernandezde
 */

#ifndef INCLUDES_MAIN_H_
#define INCLUDES_MAIN_H_

#include <string>

// Conecta al servidor (host, puerto)
bool conectar_al_servidor(const std::string& host, int puerto);

// Inicia el ciclo de comunicación (menú, selección, respuesta)
void iniciar_cliente();



#endif /* INCLUDES_MAIN_H_ */
