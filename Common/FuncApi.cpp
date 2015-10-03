// Funcapi.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "math.h"
#include "funcapi.h"



VIDEO_FORMAT RgbToVideoFormat(int nBitCount)
{
	VIDEO_FORMAT format;

	switch(nBitCount) {
	case 8:
		format = LIMITED8BIT;
		break;
	case 15:
		format = RGB555;
		break;
	case 16:
		format = RGB565;
		break;
	case 24:
		format = RGB888;
		break;
	case 32:
		format = RGB8888;
		break;
	default:
		format = (VIDEO_FORMAT)-1;
		break;
	}
	
	return format;
}



int VideoFormatToRgb(VIDEO_FORMAT format)
{
	int nBits = 0;

	switch(format) {
	case YUV422:
		nBits = 16;
		break;
	case RGB555:
		nBits = 15;
		break;
	case RGB565:
		nBits = 16;
		break;
	case RGB888:
		nBits = 24;
		break;
	case RGB8888:
		nBits = 32;
		break;
	case ALL8BIT:
	case LIMITED8BIT:
		nBits = 8;
		break;
	default:
		nBits = 0;
	}

	return nBits;
}





void SetRectangleMask(HCG hcg, int nLeft, int nTop, int nWidth, int nHeight, BOOL bEnable)
{
	int nRight = 0, nBottom = 0;

	nRight	= nLeft + nWidth;
	nBottom	= nTop + nHeight;

	for(int i = nLeft; i < nRight; i++){
		for(int j = nTop; j < nBottom; j++){
			CGSetPixelMask(hcg, i, j, bEnable);
		}
	}
}





int VideoSourceToIndex(VIDEO_SOURCE *pSource)
{
	int nSource = -1;
	
	switch(pSource->type) {
	case COMPOSITE_VIDEO:
		nSource = pSource->nIndex;
		break;
	case S_VIDEO:
		nSource	= pSource->nIndex + 6;
		break;
	case COMPONENT_VIDEO:
		nSource = pSource->nIndex + 9;
		break;
	default:
		nSource = -1;
		break;
	}
	
	return nSource;
}




BOOL IndexToVideoSource(int nIndex, VIDEO_SOURCE *pSource)
{
	BOOL bRVal = TRUE;

	if (nIndex < 6) {
		pSource->type		= COMPOSITE_VIDEO;
		pSource->nIndex	    = nIndex;
	}
	else if (nIndex < 9) {
		pSource->type		= S_VIDEO;
		pSource->nIndex     = nIndex - 6;
	}
	else if (nIndex < 11) {
		pSource->type		= COMPONENT_VIDEO;
		pSource->nIndex 	= nIndex - 9;
	}
	else{
		bRVal = FALSE;
	}

	return bRVal;
}




BOOL GetClientWindow(RECT *pClient, RECT *pParent, int nIndex, int nMax)
{
	BOOL bRVal = TRUE;
	int nCols = 0, nRows = 0;
	int nWidth = 0, nHeigth = 0;

	if (nIndex > nMax || nIndex < 0) {
		return bRVal;
	}
	
	nRows	= (int)(sqrt((float)nMax) + 0.5);
	nCols	= (nMax + nRows - 1) / nRows;

	nWidth	= (pParent->right - pParent->left) / nCols - 1;
	nHeigth	= (pParent->bottom - pParent->top) / nRows - 1;

	pClient->left	= pParent->left + (nIndex % nCols) * (nWidth + 1);
	pClient->top	= pParent->top + (nIndex / nCols) * (nHeigth + 1);
	pClient->right	= pClient->left + nWidth;
	pClient->bottom	= pClient->top + nHeigth;

	return bRVal;
}



BOOL GetOutputWindow(RECT *pOutput, RECT *pClient, int nMinX, int nMinY, int nMaxX, int nMaxY, int nMaxWidth, int nMaxHeigth, int nModeX, int nModeY)
{
	BOOL bRVal = TRUE;
	int nMode = 0;

	CopyMemory(pOutput, pClient, sizeof(RECT));


	if (pClient->left < nMinX) {
		pOutput->left = nMinX;
	}
	if (pClient->right < nMinX) {
		pOutput->right = nMinX;
	}
	if ((pClient->right > nMaxX ) || 
		((pClient->right - pClient->left) > nMaxWidth)) {
		pOutput->right = pOutput->left + nMaxWidth;
	}
	
	if (pClient->top < nMinY) {
		pOutput->top = nMinY;
	}
	if (pClient->bottom < nMinY) {
		pOutput->bottom = nMinY;
	}
	if ((pClient->bottom > nMaxY ) || 
		((pClient->bottom - pClient->top) > nMaxHeigth)) {
		pOutput->bottom = pOutput->top + nMaxHeigth;
	}


	nMode = pOutput->left % nModeX;
	if (nMode) {
		pOutput->left += (nModeX - nMode);
	}

	nMode = pOutput->top % nModeY;
	if (nMode) {
		pOutput->top += (nModeY - nMode);
	}
	
	nMode = pOutput->right % nModeX;
	if (nMode) {
		pOutput->right -= nMode;
	}
	
	nMode = pOutput->bottom % nModeY;
	if (nMode) {
		pOutput->bottom -= nMode;
	}

	return bRVal;
}





BOOL OpenBMPFile(LPCSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer)
{
	BOOL bRVal = TRUE;
	DWORD dwBytesRead = 0;
	DWORD dwSize = 0;
	BITMAPFILEHEADER bfh;

	HANDLE hFile = ::CreateFile(lpFileName,
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL
								);
	if (hFile == INVALID_HANDLE_VALUE) {
		bRVal = FALSE;
	}
	else{
		dwSize = sizeof(BITMAPFILEHEADER);
		::ReadFile(hFile, &bfh, dwSize, &dwBytesRead, NULL );
		if (bfh.bfType != ((WORD)'M' << 8 | 'B')) {
			bRVal = FALSE;
		}
		else{
			dwSize = sizeof(BITMAPINFOHEADER);
			::ReadFile(hFile, pBmpInfo, dwSize, &dwBytesRead, NULL);
			if (pBmpInfo->bmiHeader.biBitCount <= 8) {
				dwSize = bfh.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
				ReadFile(hFile,
					(char *)pBmpInfo + sizeof(BITMAPINFOHEADER),
					dwSize,
					&dwBytesRead,
					NULL
					);
			}
			
			dwSize = WIDTH_BYTES(pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biBitCount) *
					pBmpInfo->bmiHeader.biHeight;
			
			ReadFile(hFile, pImageBuffer, dwSize, &dwBytesRead, NULL);
		}
		
		CloseHandle(hFile);
	}
	
	return bRVal;
}




BOOL SaveBMPFile(LPCSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer)
{
	BOOL bRVal				= TRUE;
	DWORD dwBytesRead		= 0;
	DWORD dwSize			= 0;
	BITMAPFILEHEADER bfh	= {0};
	int nTable				= 0;
	DWORD dwImageSize		= 0;

	if (pBmpInfo->bmiHeader.biBitCount > 8) {
		nTable = 0;
	}
	else{
		nTable = 256;
	}

	dwImageSize =  WIDTH_BYTES(pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biBitCount) * 
					pBmpInfo->bmiHeader.biHeight;
	
	if (dwImageSize <= 0) {
		bRVal = FALSE;
	}
	else{
		bfh.bfType		= (WORD)'M' << 8 | 'B';
		bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
		bfh.bfSize		= bfh.bfOffBits + dwImageSize;			
		
		HANDLE hFile = ::CreateFile(lpFileName,
			GENERIC_WRITE ,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE) {
			bRVal = FALSE;
		}
		else{
			dwSize = sizeof(BITMAPFILEHEADER);
			::WriteFile(hFile, &bfh, dwSize, &dwBytesRead, NULL );
			
			dwSize = sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
			::WriteFile(hFile, pBmpInfo, dwSize, &dwBytesRead, NULL );
			
			dwSize = dwImageSize;
			WriteFile(hFile, pImageBuffer, dwSize, &dwBytesRead, NULL );
			
			CloseHandle(hFile);
		}
	}
	
	return bRVal;
}



