/**
* \file DiVACrono.h
* \author Juan Carlos San Miguel Avedillo
* \date 02-02-07
* \version 1.2
* \brief Class to take task times
* 
* This file is the header of the DiVACrono class.
* It is based on previous implementation of DiVACrono (by V.Valdes)
*
*/	


//class description
/**
 * \class DiVACrono
 * \brief To take task times
 *
 * Write detailed description here.
 */

#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#ifndef DiVACrono_HPP
#define DiVACrono_HPP


class DiVACrono {
	
	
public:	

	///Initial Time
	double inicio;

	///Final Time
	double fin;

	///Default constructor
	DiVACrono();
	///Default destructor
	~DiVACrono();

	///Method to start to take time
	void start();
	
	///Method to stop taking time
	double stop();

	///Method to start to take time and print a message
	double stop(char *cadena);
};
#endif