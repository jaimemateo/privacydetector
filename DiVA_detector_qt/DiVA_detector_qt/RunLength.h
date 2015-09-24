// RunLength.h
//
//////////////////////////////////////////////////////////////////////

bool CompressRunLength(unsigned char *pSrc, int nSrcLen, unsigned char *&pDes, int &nDesLen, int nBitsPerSample, void* nRuns, int nRunCount, int nRunSize);
bool DecompressRunLength(unsigned char *pSrc, int nSrcLen, unsigned char *&pDes, int &nDesLen);



