// RunLength.cpp: implementation of the LZW class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "RunLength.h"

#define GetDWORD(buf,bit,mask) ((*(unsigned long*)(((unsigned char*)buf)+((bit)>>3)))>>((bit)&7)&(mask))
#define GetWORD(buf,bit,mask) ((*(unsigned short*)(((unsigned char*)buf)+((bit)>>3)))>>((bit)&7)&(mask))

int GetBitCount(int n)
{
	int nBitCount = 0;
	while(n)
		n >>= 1, nBitCount++;
	return nBitCount;
}

int BinarySearch(void* pValue, int nVlaueSize, void* pArray, int nCount)
{
	int nIndex, nResult, nStart = 0, nEnd = nCount-1;
	while(nStart <= nEnd)
	{
		nIndex = (nEnd+nStart)/2;
		if((nResult = memcmp((unsigned char*)pArray+nIndex*nVlaueSize, pValue, nVlaueSize)) == 0)
			return nIndex;
		if(nResult > 0)
			nEnd = nIndex-1;
		else
			nStart = nIndex+1;
	}
	return -1;
}

bool CompressRunLength(unsigned char *pSrc, int nSrcLen, unsigned char *&pDes, int &nDesLen, int nBitsPerSample, void* nRuns, int nRunCount, int nRunSize)
{
	pDes = (unsigned char*)malloc(nSrcLen*2);
	memset(pDes, 0, nSrcLen*2);

	nDesLen = sizeof(unsigned long);
	*(unsigned long*)pDes = nSrcLen;							// save source length
	*(pDes+nDesLen++) = nBitsPerSample;					// save bits per sample
	*(pDes+nDesLen++) = nRunCount;						// save runs count
	*(pDes+nDesLen++) = nRunSize;						// save run bytes
	memcpy(pDes+nDesLen, nRuns, nRunCount*nRunSize);	// save runs
	nDesLen += nRunCount*nRunSize;
	nDesLen <<= 3; // bytes to bits
	if(nRunCount == 0)
		nRunCount = 256, nRunSize = 1, nRuns = NULL;

	int nBitsPerTypeIndex = GetBitCount(nRunCount-1);
	int nMaxRunLength = (1 << nBitsPerSample)-1, nRunLength, nRunIndex, nByte = 0;
	// loop in the source buffer
	while(nByte < nSrcLen)
		if((nRuns && (nRunIndex = BinarySearch(pSrc+nByte, nRunSize, nRuns, nRunCount)) != -1 &&
			memcmp(pSrc+nByte+nRunSize, (unsigned char*)nRuns+nRunIndex*nRunSize, nRunSize) == 0) ||
			(!nRuns && (nRunIndex = *(pSrc+nByte)) == *(pSrc+nByte+1)))
		{	// set bit to 1 to indicate type found
			*(pDes+(nDesLen>>3)) |= 1 << (nDesLen&7);
			*(unsigned long*)(pDes+(++nDesLen>>3)) |= nRunIndex << (nDesLen&7);
			nDesLen += nBitsPerTypeIndex;
			// skip the two repeated runs
			nByte += nRunSize*2;
			// get run length - 2 (without the two repeated runs)
			nRunLength = 0;
			while(nRunLength < nMaxRunLength && nByte < nSrcLen && 
				((nRuns && memcmp(pSrc+nByte, (unsigned char*)nRuns+nRunIndex*nRunSize, nRunSize) == 0) || (!nRuns && (unsigned char)nRunIndex == *(pSrc+nByte))))
				nRunLength++, nByte += nRunSize;
			// save run length and increment destination length by bits per sample
			*(unsigned long*)(pDes+(nDesLen>>3)) |= nRunLength << (nDesLen&7);
			nDesLen += nBitsPerSample;
		}
		else	// copy one byte
			*(unsigned short*)(pDes+(++nDesLen>>3)) |= *(pSrc+nByte++) << (nDesLen&7), nDesLen += 8;
	nDesLen = (nDesLen+7)/8;	// bits to bytes
	pDes = (unsigned char*)realloc(pDes, nDesLen);

	return true;
}

bool DecompressRunLength(unsigned char *pSrc, int nSrcLen, unsigned char *&pDes, int &nDesLen)
{
	if(nSrcLen == 0)
		return true;
	
	// allocate destination buffer
	nDesLen = *(unsigned long*)pSrc;
	pDes = (unsigned char*)malloc(nDesLen);
	memset(pDes, 0, nDesLen);
	
	// copy compression information
	int nSrcIndex = sizeof(unsigned long);
	int nBitsPerSample = *(pSrc+nSrcIndex++);
	int nRunCount = *(pSrc+nSrcIndex++);
	int nRunSize = *(pSrc+nSrcIndex++);
	void* nRuns = pSrc+nSrcIndex;
	nSrcIndex += nRunSize*nRunCount;
	nSrcIndex <<= 3; // bytes to bits
	if(nRunCount == 0)
		nRunCount = 256, nRunSize = 1, nRuns = NULL;
	
	int nBitsPerTypeIndex = GetBitCount(nRunCount-1);
	int nMaxTypeIndex = (1 << nBitsPerTypeIndex)-1;
	int nMaxRunLength = (1 << nBitsPerSample)-1;
	int nDesIndex = 0, nRunLength, nRunIndex, nRun, nByte;

	nSrcLen <<= 3; // bytes to bits
	while(nSrcIndex < nSrcLen-8)
		if((*(pSrc+(nSrcIndex>>3)) >> (nSrcIndex++&7)) & 1)
		{
			nRunIndex = GetDWORD(pSrc, nSrcIndex, nMaxTypeIndex), nSrcIndex += nBitsPerTypeIndex;
			nRunLength = GetDWORD(pSrc, nSrcIndex, nMaxRunLength)+2, nSrcIndex += nBitsPerSample;
			for(nRun = 0; nRun < nRunLength; nRun++)
				for(nByte = 0; nByte < nRunSize; nByte++, nDesIndex += 8)
					*(unsigned short*)(pDes+(nDesIndex>>3)) |= nRuns ? GetWORD(nRuns+nRunSize*nRunIndex, nByte<<3, 0xff) : (unsigned char)nRunIndex;
		}
		else	// copy one byte
			*(unsigned short*)(pDes+(nDesIndex>>3)) |=  GetWORD(pSrc, nSrcIndex, 0xff), nDesIndex += 8, nSrcIndex += 8;

	return true;
}
