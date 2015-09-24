#include "Description.h"
#include <stdlib.h>
#include <string.h>
Description::Description(int size)
{
	_totalSize = size;
	_usedSize = 0;
	_buffer =(char*)malloc(size);
	memset(_buffer,0,size);
}

Description::~Description()
{
	free((void*) _buffer);
}
int Description::setBuffer(char* data, int size)
{
	_usedSize = size;
	memcpy(_buffer,data,_usedSize);
	return _usedSize;
}
int Description::getBufferCopied(char** pbuffercopy)
{
	char* buffercopy = *pbuffercopy;
	memcpy(buffercopy,_buffer,_usedSize);
	return _usedSize;
}

int Description::copy(Description *desc)
{

	this->setId(desc->getId());
	this->setBuffer(desc->getBuffer(), desc->getSize());

	return desc->getSize();
}