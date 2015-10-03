/*
 *-----------------------------------------------------------------------------
 *       Name				FuncApi.h
 *       Purpose			
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2002/05/06
 *
 *
 *       Description
 *
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(CG_FUNCAPI)         //
#define CG_FUNCAPI

#include "..\inc\CGVideo.h"


#define WIDTH_BYTES(bits) (((bits) + 31) / 32 * 4)
#define PIXEL_BYTES(bits) (((bits) + 7) / 8 )
#define PIXEL_BITS(bits) (((bits) + 7) / 8 * 8)


#define RECT_WIDTH(lpRect) ((lpRect)->right - (lpRect)->left)
#define RECT_HEIGHT(lpRect) ((lpRect)->bottom - (lpRect)->top)


VIDEO_FORMAT RgbToVideoFormat(int nBitCount);
int VideoFormatToRgb(VIDEO_FORMAT format);

void SetRectangleMask(HCG hcg, int nLeft, int nTop, int nWidth, int nHeight, BOOL bEnable);


int VideoSourceToIndex(VIDEO_SOURCE *pSource);
BOOL IndexToVideoSource(int nIndex, VIDEO_SOURCE *pSource);

BOOL GetClientWindow(RECT *pClient, RECT *pParent, int nIndex, int nMax);
BOOL GetOutputWindow(RECT *pOutput, RECT *pClient, int nMinX, int nMinY, int nMaxX, int nMaxY, int nMaxWidth, int nMaxHeigth, int nModeX, int nModeY);

BOOL OpenBMPFile(LPCSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer);
BOOL SaveBMPFile(LPCSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer);


#endif