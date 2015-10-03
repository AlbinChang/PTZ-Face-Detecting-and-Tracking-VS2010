/*
 *-----------------------------------------------------------------------------
 *       Name				CGVideoEx.h
 *       Purpose			
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2003/05/06
 *
 *
 *       Description
 *		 
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(CG_VIDEO_EX)         //
#define CG_VIDEO_EX

#include "CGDef.h"

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif



CGSTATUS __stdcall CGCaptureEx(HCG hcg, HWND hwnd, BOOL bEnable);

CGSTATUS __stdcall CGOpenSnapEx(HCG hcg, SNAP_PROC lpSnapFunc, void *pParam);
CGSTATUS __stdcall CGCloseSnapEx(HCG hcg);
CGSTATUS __stdcall CGStartSnapEx(HCG hcg, DWORD dwMemOffset, BOOL bInterline, WORD wSum);
CGSTATUS __stdcall CGStopSnapEx(HCG hcg);


CGSTATUS __stdcall CGDataTransform(BYTE *pDestBuf, BYTE *pSrcBuf, int nWidth, int nHeight, int nBitCount, BOOL bVerFlip);
CGSTATUS __stdcall CGDataInterchange(BYTE *pDestBuf, BYTE *pSrcBuf, int nWidth, int nHeight, int nBitCount, INTERCHANGE_TYPE type);

CGSTATUS __stdcall CGGetDisplayBits(HDC hdc, int nLeft, int nTop, int nWidth, int nHeight, int *pBitCount, BYTE *pImageBuffer);


/* extren "C" { */
#ifdef __cplusplus
}
#endif

#endif