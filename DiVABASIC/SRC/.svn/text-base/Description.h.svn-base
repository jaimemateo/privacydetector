//file description
/**
* \file Description.h
* \author Alvaro Garcia Martin
* \date 15-11-09
* \version 1.2
* \brief Class to define descriptors for of char strings
* 
* This file is the header of the Description class.
*
*/	


//class description
/**
 * \class Description
 * \brief Describes any kind of description to send or receive
 *
 */

#if !defined(DESCRIPTION_H_INCLUDED)
#define DESCRIPTION_H_INCLUDED
class Description  
{
public:
	Description(int size);
	~Description();
	int setBuffer(char* data, int size);
	int getBufferCopied(char** pbuffercopy);
	char* getBuffer(){return _buffer;};
	int getSize(){return _usedSize;};
	long getId(){return idImage;};
	long setId(long id){idImage = id; return idImage;};
	int copy(Description *desc);
// Indicate private attributes
private:
	///image identification
	long idImage;
	///image timestamp
	long timestamp;
	///image
	char *_buffer;
	int _totalSize;
	int _usedSize;
	
};
#endif