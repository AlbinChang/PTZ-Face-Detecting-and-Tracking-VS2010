/*
 *-----------------------------------------------------------------------------
 *       Name				CGDef.h
 *       Purpose			Constant definition
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2009/03/13
 *
 *
 *       Description
 *		 This header file defines constant and external struct type
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(CG_CGDEF)         //
#define CG_CGDEF

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif
   	
typedef HANDLE HCG;

typedef enum tagCGTYPE { 
	CG300TYPE   = 0, 
	CG400TYPE   = 1,
	CG410TYPE   = 2,
	VT121TYPE   = 3,
	VT1CARDTYPE = 4
} CGTYPE;


typedef enum tagVIDEO_FORMAT {
	YUV422		= 0,
	RGB888		= 1,
	RGB565		= 2,
	RGB555		= 3,
	RGB8888		= 4,
	ALL8BIT		= 5,
	LIMITED8BIT = 6
} VIDEO_FORMAT;


typedef enum tagVIDEO_STANDARD { 
	PAL		= 0, 
	NTSC	= 1
} VIDEO_STANDARD;


typedef enum tagVIDEO_SCAN { 
	FRAME = 0,	 //帧方式
	FIELD = 1,   //场方式，默认方式
	FIELD1 = 2,	 //场方式1 (add by lm 20090313)
} VIDEO_SCAN;


typedef enum tagMIRROR_DIRECTION { 
	HOR_DIR		= 0,  
	VERT_DIR	= 1	
} MIRROR_DIRECTION;


typedef enum tagVIDEO_ADJUST_PARAM {
	BRIGHTNESS		= 1,
	CONTRAST		= 2,
	HUE				= 3,
	SATURATION		= 4
} VIDEO_ADJUST_PARAM;


typedef enum tagCRY_OSC { 
	CRY_OSC_35M = 0, 
	CRY_OSC_28M = 1	
} CRY_OSC;

typedef enum tagEXT_VIDEO_OUTPUT {
	EXT_VOUT_MODE				= 0,
	EXT_VOUT_SOURCE				= 1,
	EXT_VOUT_CYCLE_LENGTH		= 2,
	EXT_VOUT_CYCLE_SOURCE		= 3
} EXT_VIDEO_OUTPUT;


typedef enum tagBOARD_INFO { 
	BOARD_TYPE		    	 = 0, 
	BOARD_SERIALNUM	    	 = 1,
	BOARD_SUB_DEVICE	     = 2,

	BOARD_FIRMWARE_RELEASE   = 3,
	BOARD_DEVICES            = 4,
	BOARD_CRY_OSC_TYPE       = 5,
	BOARD_VIDEO_SOURCE_VALID = 6,
	BOARD_HW_DEVICENUM       = 7
} BOARD_INFO;


typedef enum tagLUMA_PARAM {
	LUMA_CHROMA_TRAP	= 0
} LUMA_PARAM;


typedef enum tagGAMMA_CORR { 
	NO_GAMMACORR	= 0, 
	Y_GAMMACORR		= 1,
	UV_GAMMACORR	= 2,
	YUV_GAMMACORR	= 3
} GAMMA_CORR;


typedef enum tagVIDEO_SOURCE_TYPE {	
	COMPOSITE_VIDEO		= 0,
	S_VIDEO  			= 1,
	COMPONENT_VIDEO		= 2
} VIDEO_SOURCE_TYPE;


typedef enum tagINTERCHANGE_TYPE {	
	HOR_ODD_COLUMN		= 0,
	HOR_EVEN_COLUMN		= 1,
	VERT_ODD_ROW  		= 2,
	VERT_EVEN_ROW		= 3,
	VERT_DOUBLE			= 4		
} INTERCHANGE_TYPE;


typedef enum tagCGSTATUS { 
	CG_OK							= 0, 
	CG_ALLOCATE_DEVICE_MEMORY_ERROR	= -1, 
	CG_LOCK_DEVICE_MEMORY_ERROR		= -2,
	CG_ACCESS_DEVICE_MEMORY_DENIED	= -3,	
	CG_PARAMETER_INVALID			= -4,
	CG_NOT_SUPPORT_INTERFACE		= -5,
	CG_OPEN_DRIVER_FAILED			= -6,
	CG_CARD_HANDLE_INVALID			= -7,
	CG_NO_CARD_FOUND				= -8,
	CG_HW_MAP_MEMORY_ERROR			= -9,
	CG_NOT_ENOUGH_SYSTEM_MEMORY		= -10,
	CG_HW_INIT_ERROR				= -11,
	CG_PARAMETER_OUT_OF_BOUND		= -12,
	CG_HW_INIT_I2C_ERROR			= -13,
	CG_HW_INIT_AD_ERROR				= -14,
	CG_HW_BOARD_TYPE_ERROR			= -15,
	CG_SNAP_SHOT_TIME_OUT			= -16,
	CG_INIT_DDRAW_ERROR				= -17,
	CG_IN_WORK						= -18,
	CG_NOT_START_CAPTURE			= -19,
	CG_NOT_START_CAPTURE_EX			= -20,
	CG_NOT_START_SNAP     			= -21,
	CG_NOT_START_SNAP_EX			= -22,
	CG_NOT_OPEN_SNAP_EX			    = -23,
	CG_VIDEO_FORMAT_INVALID			= -24,
	CG_NOT_SUPPORT_CARD_TYPE		= -25,
	CG_FILE_CREATE_ERROR			= -26,
	CG_FILE_INVALID					= -27,
	CG_VGA_BASE_INVALID				= -28,
	CG_VIDEO_WINDOW_INVALID			= -29,
	CG_INTERNAL_ERROR				= -30	
} CGSTATUS;


typedef unsigned ( __stdcall * PTHREAD_START)( void * );


#define CG_SUCCESS(status) ( (status) == CG_OK )

#ifdef _DEBUG
#define CG_VERIFY(status) \
		if((status) != CG_OK){ \
			::MessageBox(NULL, CGGetErrorString(status), \
						 "Error", MB_ICONWARNING | MB_OK);   \
		}     
#else
#define CG_VERIFY(status) (status)
#endif


#define CG_MESSAGE(status) \
		if((status) != CG_OK){ \
		::MessageBox(NULL, CGGetErrorString(status), \
		"Error message", MB_ICONWARNING | MB_OK);   \
		}    


typedef struct tagVIDEO_SOURCE {	
	VIDEO_SOURCE_TYPE type;
	int nIndex;	
} VIDEO_SOURCE;


typedef struct tagCHECK_PARAM {	
	BYTE byParam1;
	BYTE byParam2;
	BYTE byParam3;
	BYTE byParam4;	
} CHECK_PARAM;


typedef struct tagSANP_INFO {
	HCG	hcg;
	int nDevice;
	int nNumber;
	void *pParam;	
} SNAP_INFO;


typedef int (CALLBACK* SNAP_PROC)(SNAP_INFO *pInfo);


typedef enum tagCG_COMMAND_CODE {
	CMD_RESERVED0		        = 0x00,
	CMD_RESERVED1	            = 0x01,
	CMD_RESERVED2		        = 0x02,
	CMD_QUERY_CARD_OPEN_COUNT	= 0x10		
} CG_COMMAND_CODE;


typedef struct tagCMD_QUERY_CARD_OPEN_COUNT_PARAM {
	DWORD dwCount;
} CMD_QUERY_CARD_OPEN_COUNT_PARAM;


typedef struct tagCMD_RESERVED0_PARAM {
	BYTE  *pPrevParam;			
	DWORD PrevParamLen;			
	BYTE  *pParam;				
	DWORD ParamLen;		
} CMD_RESERVED0_PARAM;


typedef struct tagCMD_RESERVED1_PARAM {
	BYTE  *pParam;				
	DWORD ParamLen;				
	DWORD DataOffset;			
	BYTE  *pData;				
	DWORD DataLen;		
} CMD_RESERVED1_PARAM;



/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif

