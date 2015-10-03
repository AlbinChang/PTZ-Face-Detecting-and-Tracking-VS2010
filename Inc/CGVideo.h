/*
 *-----------------------------------------------------------------------------
 *       Name				CGVideo.h
 *       Purpose			
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2008/07/16
 *
 *
 *       Description
 *
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(CG_CGVIDEO)         //
#define CG_CGVIDEO

#include "CGDef.h"

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif
   	
CGSTATUS __stdcall BeginCGCard(int nDevice, HCG *pHandle);
CGSTATUS __stdcall EndCGCard(HCG hcg);

CGSTATUS __stdcall CGCapture(HCG hcg, BOOL bEnable);

CGSTATUS __stdcall CGCaptureShot(HCG hcg);

CGSTATUS __stdcall CGSetInputWindow(HCG hcg, int nStartX, int nStartY,	int nWidth, int nHeight);
CGSTATUS __stdcall CGSetOutputWindow(HCG hcg, int nStartX, int nStartY, int nWidth, int nHeight);

CGSTATUS __stdcall CGSetVideoFormat(HCG hcg, VIDEO_FORMAT format);

CGSTATUS __stdcall CGSetVideoStandard(HCG hcg, VIDEO_STANDARD mode);

CGSTATUS __stdcall CGSetDelay(HCG hcg, int nXDelay, int nYDelay);

CGSTATUS __stdcall CGSetScanMode(HCG hcg, VIDEO_SCAN mode);

CGSTATUS __stdcall CGSetVideoSource(HCG hcg, VIDEO_SOURCE source);

CGSTATUS __stdcall CGAdjustVideo(HCG hcg, VIDEO_ADJUST_PARAM param, BYTE byValue);

CGSTATUS __stdcall CGWaitEvenVSync(HCG hcg);
CGSTATUS __stdcall CGWaitOddVSync(HCG hcg);

CGSTATUS __stdcall CGGetStaticMem(DWORD *pPages);
CGSTATUS __stdcall CGSetStaticMem(DWORD dwPages);

CGSTATUS __stdcall CGStaticMemLock(DWORD dwStartOffset, DWORD dwLength, HANDLE *pHandle, PVOID *ppLineAddr);
CGSTATUS __stdcall CGStaticMemUnlock(HANDLE handle);


CGSTATUS __stdcall CGSnapShot( HCG hcg, DWORD dwMemOffset, WORD wIntervSyncs, BOOL bInterline, WORD wSum);
CGSTATUS __stdcall CGStartSnap( HCG hcg, DWORD dwMemOffset, BOOL bInterline, WORD wSum);
CGSTATUS __stdcall CGGetSnappingNumber( HCG hcg, int *pNumber);
CGSTATUS __stdcall CGStopSnap( HCG hcg);

CGSTATUS __stdcall CGEnableMask(HCG hcg, BOOL bEnable);
CGSTATUS __stdcall CGSetPixelMask(HCG hcg, int x, int y, BOOL bEnable);

CGSTATUS __stdcall CGGetCardType(HCG hcg, CGTYPE *pType);
CGSTATUS __stdcall CGGetCardTotal(int *pNumber);

CGSTATUS __stdcall CGLoadCFGFile(HCG hcg, char *filename);

char * __stdcall CGGetErrorString(CGSTATUS status);

CGSTATUS __stdcall CGCommand( HCG hcg, CG_COMMAND_CODE CommandCode, void *pContext, int *pLength);


/************************ CG300Util *************************************/
CGSTATUS __stdcall CGVideoPresent(HCG hcg, BOOL *pStatus);
CGSTATUS __stdcall CGSetPLLFrequency(HCG hcg, BYTE byValue);
CGSTATUS __stdcall CGSelectCryOSC(HCG hcg, CRY_OSC mode);


/************************ CG400Util *************************************/
CGSTATUS __stdcall CGEnableVideoMirror(HCG hcg, MIRROR_DIRECTION dir, BOOL bEnable);
CGSTATUS __stdcall CGSetVideoExtOutput(HCG hcg, EXT_VIDEO_OUTPUT mode, int nValue);
CGSTATUS __stdcall CGWaitVSync(HCG hcg);
CGSTATUS __stdcall CGGetBoardInfo(HCG hcg, BOARD_INFO mode, char *pInfo);
CGSTATUS __stdcall CGCheckBoard(HCG hcg, CHECK_PARAM Param, BOOL *pStatus);
CGSTATUS __stdcall CGLumaControl(HCG hcg, LUMA_PARAM Param, BYTE byValue);
CGSTATUS __stdcall CGGammaCorrControl(HCG hcg, GAMMA_CORR param);
CGSTATUS __stdcall CGSetGammaCorrCoef(HCG hcg, BYTE *pBuffer);


/************************ CG410Util *************************************/
CGSTATUS __stdcall CGReadSCMParam(HCG hcg, BYTE *pBuffer);
CGSTATUS __stdcall CGWriteSCMParam(HCG hcg, BYTE *pBuffer, BYTE byEntries);



/* extren "C" { */
#ifdef __cplusplus
}
#endif

#endif