// testDiVAImageBuffer.cpp : Test dxe la clase Defines the entry point for the console application.
//

#include <stdio.h>

#include "DiVACaptureAVI.h"
#include "DiVADisplay.h"
#include "DiVAImageBuffer.h"

long tamBuffer = 10;
long waitTimeout = 100;
int politicaPut = BUFFERC_PUT_DISCARD;

int main(int argc, char* argv[])
{
	int ret;
	DiVACaptureAVI *capturadoraAvi = 0;
	DiVAImageBuffer *buffer = 0;

	printf("-----------------------\n");
	printf("Test de DiVAImageBuffer\n");
	printf("-----------------------\n");

	char strEntrada[100] = "" ;
	long entrada = -1;

	printf("Tamaño de buffer (%d por defecto): ", tamBuffer);
	fscanf(stdin, "%s", strEntrada);
	if (strEntrada[0] != '\n')
	{
		entrada = atol(strEntrada);
		if (entrada > 0)
			tamBuffer = entrada;
	}
	
	printf("Timeout (%d por defecto, -1=INF): ", waitTimeout);
	fscanf(stdin, "%s", strEntrada);
	if (strEntrada[0] != '\n')
	{
		entrada = atol(strEntrada);
		if (entrada > 0)
			waitTimeout = entrada;
		else if (entrada == -1)
			waitTimeout = 0;
	}
/*
	printf("Politica para put (%d por defecto):\n", politicaPut);
	printf("%d - BUFFERC_PUT_DISCARD\n", BUFFERC_PUT_DISCARD);
	printf("%d - BUFFERC_PUT_WAIT\n", BUFFERC_PUT_WAIT);
	printf("%d - BUFFERC_PUT_REPLACE\n", BUFFERC_PUT_REPLACE);

	fscanf(stdin, "%s", strEntrada);
	if (strEntrada[0] != '\n')
	{
		entrada = atol(strEntrada);
		if ((entrada >= BUFFERC_PUT_DISCARD) && (entrada <= BUFFERC_PUT_REPLACE))
			politicaPut = entrada;
	}
*/
	printf("Inicializando...");

	capturadoraAvi = new DiVACaptureAVI();
//	capturadoraAvi->init("../../data/objects.avi");
	capturadoraAvi->init();

	printf("Capturadora AVI inicializada.\n\t Archivo: '%s'\n\t Num frames: %d\n", capturadoraAvi->getFileName(), capturadoraAvi->getNumFrames());

	DiVAImage *imagenIn = capturadoraAvi->getSampleFrame()->clone();
	DiVAImage *imagenOut1 = capturadoraAvi->getSampleFrame()->clone();
	DiVAImage *imagenOut2 = capturadoraAvi->getSampleFrame()->clone();

	// Crear e inicializar buffer
	buffer = new DiVAImageBuffer(tamBuffer, imagenIn);
	buffer->setWaitTimeout(waitTimeout);
//	buffer->setPoliticaPut(politicaPut);
	long idCliente1 = buffer->registrarConsumidor();
	long idCliente2 = buffer->registrarConsumidor();

// Display multiple: 3 filas, 1 columna
DiVADisplay *display = new DiVADisplay("testDiVAImageBuffer", imagenIn->getHeight(), imagenIn->getWidth(),
			   3, 1, 10, 10, true, 1.0, true);

	long curIdFrame1 = 0;
	long curIdFrame2 = 0;

	char tecla = '\n';

	printf("----------------------------------------------\n");
	printf("\nPRUEBA DE DiVAImageBuffer\n");
	printf("0 - Leer frame e insertarlo en buffer\n");
	printf("1 - Coger nuevo frame del buffer (cliente 1)\n");
	printf("2 - Coger nuevo frame del buffer (cliente 2)\n");
	printf("b - Ver estado del buffer\n");
	printf("q - Salir\n");
	printf("----------------------------------------------\n");

	while ((tecla != 'q') && (tecla != 'Q'))
	{
		tecla = getchar();

		if (tecla != '\n')
			printf("TECLA: '%c'\n", tecla); 

		if (tecla == '0')
		{
			ret = capturadoraAvi->getNextFrame(imagenIn);
			if (ret == 0)
			{
				printf("Recogida imagen %d, con timestamp %f\n", imagenIn->getId(), imagenIn->getTimeStamp());
				ret = buffer->put(imagenIn);
				if (ret == BUFFERC_OK)
				{
					printf("Frame %d insertado en el buffer. %d elementos ocupados, %d libres\n", imagenIn->getId(), buffer->getOcupados(), buffer->getLibres());
					display->insertImage(imagenIn, 1, 1, "last frame in");
					display->windowVisible();
				}
				else 
					printf("Problema al insertar en el buffer. Retorno: %d\n", ret);
			}
			else if (ret == 1)
				printf("Ya no quedan frames. Retorno: %d\n", ret);
			else 
				printf("Error al leer frame. Retorno: %d\n", ret);
		}
		else if (tecla == '1')
		{
			ret = buffer->get(imagenOut1, curIdFrame1, idCliente1);
			if (ret == BUFFERC_OK)
			{
				printf("Frame %d sacado del buffer. %d elementos ocupados, %d libres\n", imagenOut1->getId(), buffer->getOcupados(), buffer->getLibres());

if (curIdFrame1 == imagenOut1->getId())
	curIdFrame1++;
else
	curIdFrame1 = imagenOut1->getId() + 1;

				display->insertImage(imagenOut1, 2, 1, "last frame out-1");
				display->windowVisible();
			}
			else
			{
				printf("Error en el buffer: ");
				buffer->printError(ret);
			}
		}
		else if (tecla == '2')
		{
			ret = buffer->get(imagenOut2, curIdFrame2, idCliente2);
			if (ret == BUFFERC_OK)
			{
				printf("Frame %d sacado del buffer. %d elementos ocupados, %d libres\n", imagenOut2->getId(), buffer->getOcupados(), buffer->getLibres());

if (curIdFrame2 == imagenOut2->getId())
	curIdFrame2++;
else
	curIdFrame2 = imagenOut2->getId() + 1;

				display->insertImage(imagenOut2, 3, 1, "last frame out-2");
				display->windowVisible();
			}
			else
			{
				printf("Error en el buffer: ");
				buffer->printError(ret);
			}
		}

		else if ((tecla == 'b') || (tecla == 'B'))
		{
			printf("ESTADO DEL BUFFER\n");
			printf("\tTamaño: %d\n", tamBuffer);
			printf("\tOcupados: %d\n", buffer->getOcupados());
			printf("\tLibres: %d\n", buffer->getLibres());
			printf("\tTimeout: %d\n", buffer->getWaitTimeout());
			printf("\tPolitica put: %d\n", buffer->getPoliticaPut());
		}

		else if (tecla != '\n')
		{
			printf("----------------------------------------------\n");
			printf("\nPRUEBA DE DiVAImageBuffer\n");
			printf("0 - Leer frame e insertarlo en buffer\n");
			printf("1 - Coger nuevo frame del buffer (cliente 1)\n");
			printf("2 - Coger nuevo frame del buffer (cliente 2)\n");
			printf("b - Ver estado del buffer\n");
			printf("q - Salir\n");
			printf("----------------------------------------------\n");
		}
	}

	printf("Liberando...");

	delete display;

	buffer->eliminarConsumidor(idCliente1);
	buffer->eliminarConsumidor(idCliente2);
	delete buffer;

	delete imagenIn;
	delete imagenOut1;
	delete imagenOut2;

	delete capturadoraAvi;

	printf("OK\n");


	return 0;
}



