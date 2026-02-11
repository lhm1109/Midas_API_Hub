#ifndef __DB_ST_DT_DB_H__
#define __DB_ST_DT_DB_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DB_Type.h"

#pragma pack(1)

// Global Function
extern __declspec(dllexport) void ConvertStrChar(CString& strSource, char chDestin[], int nChar);
extern __declspec(dllexport) void ConvertCharStr(char chSource[], CString& strDestin, int nChar);

#define MAX_MESSAGE_LENGTH      256
// ===== // DB_ALL

// unit
#include "DB_ST_DT_UNIT.h"

//Update Message
enum DBUpdateMessageId
{
	D_UPDATE_DEFAULT=0,
	D_UPDATE_DBALL,
	D_UPDATE_BUFFER_BEFORE,
	D_UPDATE_BUFFER_AFTER,
	D_UPDATE_UNIT,
	D_UPDATE_SEL_ADD,
	D_UPDATE_SEL_DEL,
	D_UPDATE_SEL_PRE,
	D_UPDATE_SEL_POST,
	D_UPDATE_EXE_START,
	D_UPDATE_EXE_END,
	D_UPDATE_DISPLAYOPTION,
	D_UPDATE_REFRESH,
	D_UPDATE_REMOVE_STEEL,
	D_UPDATE_REMOVE_DAMP,
	D_UPDATE_REMOVE_REINFOCE,
	D_UPDATE_REMOVE_ALU,
	D_UPDATE_REMOVE_CONCRETE,
	D_UPDATE_REMOVE_FOOTING,
	D_UPDATE_REMOVE_SRC,
	D_UPDATE_REMOVE_PUSHOVER,
	D_UPDATE_REMOVE_INFLGRID,      // 격자모델 영향선 결과 삭제
	D_UPDATE_REMOVE_GRIDMODEL,     // 격자모델 해석 결과 삭제
	D_UPDATE_REMOVE_PCDESIGN,      // PC Design 결과 삭제
	D_UPDATE_REMOVE_IMPDGNFORCE,   // Import Design Force 삭제
	D_UPDATE_REMOVE_LOADRATING,    // Bridge Load Rating 결과 삭제
	D_UPDATE_REMOVE_PLATEGIRDER,   // Plate Girder Design 결과 삭제
	D_UPDATE_REMOVE_CHRC_BEAMDESIGN,//CH RC design(Beam)
	D_UPDATE_REMOVE_CHRC_COLUMNDESIGN,// CH RC design(Column)
	D_UPDATE_REMOVE_CHRC_SEISMICDESIGN,// CH RC design(seismic)
	D_UPDATE_DIVPAT_MODE,          // Pattern Divide시 점선표시
	D_UPDATE_DIVPAT_PARAM,         // Pattern Divide시 점선표시
	D_UPDATE_PRESSURE_MODE,        // Pressure Load 입력시 점선표시
	D_UPDATE_PRESSURE_PARAM,       // Pressure Load 입력시 점선표시
	D_UPDATE_LOADCASE_CHANGED,
	D_UPDATE_STAGE_CHANGED,        // Current Stage 변경
	D_UPDATE_STAGE_DBALL,          // stage db가 변경되었을 때
	D_UPDATE_POST_STAGE_CHANGED,   // 후처리 모드에서 stage 변경되었을 때
	D_UPDATE_POST_STEP_CHANGED,    // 후처리 모드에서 step만 변경되었을 때
	D_UPDATE_BEFORE_SEL_PRE,       // Stage 모드 후처리에서 전처리로 넘어가기 전
	D_UPDATE_DB_OPEN,              // Document 열릴 때(OnNewDocument는 안 날림)
	D_UPDATE_DB_CLOSE,             // Document 닫힐 때
	D_UPDATE_PREF_CHANGED,         // Preference 변경할 때(Dialog에서 날림)
	D_UPDATE_PCDESIGN_START,       // PC 설계 시작시 
	D_UPDATE_LOADRATING_START,     // Bridge Load Rating 설계 시작시
	D_UPDATE_PLATEGIRDER_START,   // Plate Girder Design 결과 시작
	D_UPDATE_CHRCBeamDESIGN_START,       // CH RC design(Beam)
	D_UPDATE_CHRCColumnDESIGN_START,       // CH RC design(Column) 
	D_UPDATE_CHRCSEISMICDESIGN_START,//CH RC seismic design 
	D_UPDATE_DGN_FORCE_CHANGED,    // 설계 부재력이 변경되었을 때
	D_UPDATE_STYP_TYPE_CHANGED,     // Styp의 type이 변경되었을 때
	D_UPDATE_DGN_BEAM_DESIGN_RESULT_REMOVED,   // 보 설계결과가 삭제된 경우(EC2:04 capacity design시만 사용)   PMS:3956-Seungjun-20100105 EC8:04 Seismic Design. 
	D_UPDATE_DGN_COLUMN_DESIGN_RESULT_REMOVED, // 기둥 설계결과가 삭제된 경우(EC2:04 capacity design시만 사용) PMS:3956-Seungjun-20100105 EC8:04 Seismic Design. 
	D_UPDATE_DGN_WALL_DESIGN_RESULT_REMOVED,   // 벽체 설계결과가 삭제된 경우(EC2:04 capacity design시만 사용) PMS:3956-Seungjun-20100105 EC8:04 Seismic Design.  
	D_UPDATE_REMOVE_CRACK,
	D_UPDATE_REMOVE_JUD,
	D_UPDATE_REMOVE_STL_ORTH_DECK,
	D_UPDATE_STL_ORTH_DECK_START,
	D_UPDATE_STL_COMP_GIRDER_START, //Steel Composite Girder Design 설계 시작
	D_UPDATE_NOTIFY,			          // 일반적인 목적으로 사용(Notify 하는 용도)
	D_UPDATE_REMOVE_STL_LOADRATING, // Steel Load Rating 결과 삭제
	D_UPDATE_STL_LOADRATING_START,  // Steel Load Rating 설계 시작시
	D_UPDATE_REMOVE_RC_LOADRATING,  // RC Load Rating 결과 삭제
	D_UPDATE_RC_LOADRATING_START,   // RC Load Rating 설계 시작시
	D_UPDATE_REMOVE_BRIDGE_SEISEVAL,// Bridge Seismic Evaluation 결과 삭제
    D_UPDATE_REMOVE_BRIDGE_SEISEVAL_BEARING,
	D_UPDATE_REMOVE_BRIDGE_SEISEVAL_JP,// 일본 Bridge Seismic Evaluation 결과 삭제
	
	// 특별한 경우 이외에는 추가하지 말고 D_UPDATE_NOTIFY를 활용하세요.
};

// UpdateHint
// UpdateHint
struct T_UPDATE_HINT : public CObject
{
	BOOL bDirectDrawLabel;
	unsigned int nCommand;
	CString csNotifyCmd;		// D_UPDATE_NOTIFY 일때 사용
	void Initialize()
	{
		bDirectDrawLabel=FALSE;
		nCommand=0;
		csNotifyCmd.Empty();
	}
};

// Timer
#define D_TIMER_AUTOSAVE        1
#define D_TIMER_ANALYSIS        2
#define D_TIMER_ANALSTOP        3
#define D_TIMER_CHECKLOCK       4
#define D_TIMER_CHECKLOCK_RETRY 5
#define D_TIMER_CHECKCLIENT     6    // Web-Client Timer
#define D_TIMER_CLIENT_REWRITE  7    // Web-Client Fast Timer

//하중조건 정의
#define D_LOADCASE_NONE                 99999
#define D_LOADCASE_STATIC               1
#define D_LOADCASE_EIGEN                2
#define D_LOADCASE_BUCKLING             3
#define D_LOADCASE_SPECTRUM             4
#define D_LOADCASE_HISTORY              5
#define D_LOADCASE_LINELANE             6
#define D_LOADCASE_SURFACELANE          7
#define D_LOADCASE_MOVING               8
#define D_LOADCASE_SETTLE               9
#define D_LOADCASE_COMB_GENERAL        10
#define D_LOADCASE_COMB_STEEL          11
#define D_LOADCASE_COMB_CONCRETE       12
#define D_LOADCASE_COMB_FDN            13
#define D_LOADCASE_COMB_SRC            14
#define D_LOADCASE_ENVELOPE            15
#define D_LOADCASE_HYDRATION           16
#define D_LOADCASE_PUSHOVER            17
#define D_LOADCASE_STAGE               18
#define D_LOADCASE_ECCENSPEC           19
#define D_LOADCASE_INFLGRID            20
#define D_LOADCASE_GRIDMODEL           21
#define D_LOADCASE_THEX                22 // 동해석 관련(후처리에서만 생기는 Case)
#define D_LOADCASE_THAM                23 // 동해석 관련(후처리에서만 생기는 Case)
#define D_LOADCASE_THPO                24 // 동해석 관련(후처리에서만 생기는 Case)
#define D_LOADCASE_CRANE               25 // 중국 gen crane 하중
#define D_LOADCASE_COMB_ALUMINUM       26 // 국내 Aluminum
#define D_LOADCASE_COMB_STLCOMP        27
#define D_LOADCASE_COMB_CFSTEEL        28
#define D_LOADCASE_LINELANE_OP         29
#define D_LOADCASE_SURFACELANE_OP      30
#define D_LOADCASE_COMB_SEISMIC        31
#define D_LOADCASE_COMB_TIMEHISTORY    32
#define D_LOADCASE_COMB_LINEAR_EVAL    33
#define D_LOADCASE_SPECTRUM_ND         34
#define D_LOADCASE_ECCENSPEC_ND        35
#define D_LOADCASE_CB_JSONFILE         36

#define D_SELECTLOAD_STATIC             0x000000001     // 0000 0000 0000 0000 0000 0000 0000 0000 0001
#define D_SELECTLOAD_EIGEN              0x000000002     // 0000 0000 0000 0000 0000 0000 0000 0000 0010
#define D_SELECTLOAD_BUCKLING           0x000000004     // 0000 0000 0000 0000 0000 0000 0000 0000 0100
#define D_SELECTLOAD_SPECTRUM           0x000000008     // 0000 0000 0000 0000 0000 0000 0000 0000 1000
#define D_SELECTLOAD_HISTORY            0x000000010     // 0000 0000 0000 0000 0000 0000 0000 0001 0000
#define D_SELECTLOAD_LINELANE           0x000000020     // 0000 0000 0000 0000 0000 0000 0000 0010 0000
#define D_SELECTLOAD_SURFACELANE        0x000000040     // 0000 0000 0000 0000 0000 0000 0000 0100 0000
#define D_SELECTLOAD_MOVING             0x000000080     // 0000 0000 0000 0000 0000 0000 0000 1000 0000
#define D_SELECTLOAD_SETTLE             0x000000100     // 0000 0000 0000 0000 0000 0000 0001 0000 0000
#define D_SELECTLOAD_HYDRATION          0x000000200     // 0000 0000 0000 0000 0000 0000 0010 0000 0000
#define D_SELECTLOAD_PUSHOVER           0x000000400     // 0000 0000 0000 0000 0000 0000 0100 0000 0000
#define D_SELECTLOAD_STAGE              0x000000800     // 0000 0000 0000 0000 0000 0000 1000 0000 0000
#define D_SELECTLOAD_ECCENSPEC          0x000001000     // 0000 0000 0000 0000 0000 0001 0000 0000 0000
#define D_SELECTLOAD_SPECTRUM_ND		0x000002000     // 0000 0000 0000 0000 0000 0010 0000 0000 0000
#define D_SELECTLOAD_ECCENSPEC_ND       0x000004000     // 0000 0000 0000 0000 0000 0100 0000 0000 0000
#define D_SELECTLOAD_COMB_GENERAL       0x000010000     // 0000 0000 0000 0000 0001 0000 0000 0000 0000
#define D_SELECTLOAD_COMB_STEEL         0x000020000     // 0000 0000 0000 0000 0010 0000 0000 0000 0000
#define D_SELECTLOAD_COMB_CFSTEEL       0x000008000     // 0000 0000 0000 0000 0000 1000 0000 0000 0000
#define D_SELECTLOAD_COMB_CONCRETE      0x000040000     // 0000 0000 0000 0000 0100 0000 0000 0000 0000
#define D_SELECTLOAD_COMB_FDN           0x000080000     // 0000 0000 0000 0000 1000 0000 0000 0000 0000
#define D_SELECTLOAD_COMB_SRC           0x000100000     // 0000 0000 0000 0001 0000 0000 0000 0000 0000
#define D_SELECTLOAD_ENVELOPE           0x000200000     // 0000 0000 0000 0010 0000 0000 0000 0000 0000
#define D_SELECTLOAD_INFLGRID           0x000400000     // 0000 0000 0000 0100 0000 0000 0000 0000 0000
#define D_SELECTLOAD_GRIDMODEL          0x000800000     // 0000 0000 0000 1000 0000 0000 0000 0000 0000
#define D_SELECTLOAD_THEX               0x001000000     // 0000 0000 0001 0000 0000 0000 0000 0000 0000  // ALL에 포함안됨 
#define D_SELECTLOAD_THAM               0x002000000     // 0000 0000 0010 0000 0000 0000 0000 0000 0000  // ALL에 포함안됨 
#define D_SELECTLOAD_THPO               0x004000000     // 0000 0000 0100 0000 0000 0000 0000 0000 0000  // ALL에 포함안됨 
#define D_SELECTLOAD_CRANE              0x008000000     // 0000 0000 1000 0000 0000 0000 0000 0000 0000
#define D_SELECTLOAD_MAXMINFLAG         0x010000000     // 0000 0001 0000 0000 0000 0000 0000 0000 0000
#define D_SELECTLOAD_MAXMINONLY         0x020000000     // 0000 0010 0000 0000 0000 0000 0000 0000 0000
#define D_SELECTLOAD_COMB_ALUMINUM      0x040000000     // 0000 0100 0000 0000 0000 0000 0000 0000 0000
#define D_SELECTLOAD_COMB_STLCOMP       0x080000000     // 0000 1000 0000 0000 0000 0000 0000 0000 0000
#define D_SELECTLOAD_COMB_SEISMIC       0x100000000     // 0001 0000 0000 0000 0000 0000 0000 0000 0000
#define D_SELECTLOAD_COMB_LINEAR_EVAL   0x200000000     // 0010 0000 0000 0000 0000 0000 0000 0000 0000
																					
#define D_SELECTLOAD_FORCOMB            0x008807999     // 0000 0000 1000 1000 0000 0111 1001 1001 1001
#define D_SELECTLOAD_FORALL             0x1D89FFF99     // 0001 1101 1000 1001 1111 1111 1111 1001 1001
#define D_SELECTLOAD_ALLWITHENVELOPE    0x3D8BFFF99     // 0011 1101 1000 1011 1111 1111 1111 1001 1001

//축 정의
#define D_AXIS_GX       1
#define D_AXIS_GY       2
#define D_AXIS_GZ       3
#define D_AXIS_UX       4
#define D_AXIS_UY       5
#define D_AXIS_UZ       6
struct T_AXIS_VECTOR    // 회전축 또는 직선의 정의
{
	double dblOrgX;     // 회전축/직선상의 1점의 X좌표
	double dblOrgY;     // 회전축/직선상의 1점의 X좌표
	double dblOrgZ;     // 회전축/직선상의 1점의 X좌표
	double dblVecX;     // 회전축/직선의 X 방향벡터
	double dblVecY;     // 회전축/직선의 Y 방향벡터
	double dblVecZ;     // 회전축/직선의 Z 방향벡터
};

// 요소 종류
#define TRUSS_EL        1
#define BEAM_EL         2
#define PLSTRS_EL       3
#define PLATE_EL        4
#define WALL_EL         5
#define PANEL_EL        18
#define BDSSLAB_EL      19
#define PLSTRN_EL       6
#define SDSSLAB_EL      20
#define AXISYM_EL       7
#define SOLID_EL        8
#define TRTENS_EL       9
#define TRCOMP_EL       10
#define WALLOPEN_EL     29  // 벽체 개구부 선택용 정보. 2014-01-27. by hsshim
#define MAX_ELTYP_NUM   30

// ===== // DB_NODE_ATTRIBUTE
// Copy Node Attributes
struct T_NODE_ATTR
{
	BOOL bCopyCons;
	BOOL bCopyNspr;
	BOOL bCopyGspr;
	BOOL bCopySkew;
	BOOL bCopyDrls;
	BOOL bCopyElnk;
	BOOL bCopyNlnk;
	BOOL bCopyRigd;
	BOOL bCopyMcon;
	BOOL bCopyCnld;
	BOOL bCopySdsp;
	BOOL bCopyNtmp;
	BOOL bCopyNmas;
	BOOL bCopyCldr;
};
// ===== // DB_ELEM_ATTRIBUTE
// Copy Elem Attribues
struct T_ELEM_ATTR
{
	BOOL bCopyFrls;
	BOOL bCopyPrls;
	BOOL bCopyOffs;
	BOOL bCopyBmld;
	BOOL bCopyFmld;
	BOOL bCopyPres;
	BOOL bCopyPrst;
	BOOL bCopyPtns;
	BOOL bCopyEtmp;
	BOOL bCopyBtmp;
	BOOL bCopyGtmp;
	BOOL bCopyIfgs;
	BOOL bCopyDesn;
	BOOL bCopyHydr;
	BOOL bCopySsps;
};
// ===== // DB_BUILDING_ATTRIBUTE
// Copy Building Attribues
struct T_BLDG_ATTR
{
	BOOL bCopyFbla;
};
// ===== // DB_LOAD_ATTRIBUTE
// Copy Load Attribues
struct T_LOAD_ATTR
{
	T_LOAD_ATTR()
	{
		Initialize();
	}
	BOOL bCopyBodf;
	BOOL bCopyNbof;
	BOOL bCopyCnld;
	BOOL bCopySdsp;
	BOOL bCopyBmld;
	BOOL bCopyFmld;
	BOOL bCopyFbld;
	BOOL bCopyPres;
	BOOL bCopyPrst;
	BOOL bCopyPtns;
	BOOL bCopyStmp;
	BOOL bCopyNtmp;
	BOOL bCopyEtmp;
	BOOL bCopyBtmp;
	BOOL bCopyGtmp;
	BOOL bCopyWind;
	BOOL bCopyWnat;
	BOOL bCopySeis;
	BOOL bCopyIfgs;
	BOOL bCopyPnld;
	BOOL bCopyTdpl;
	BOOL bCopyIpcr;
	void Initialize()
	{
		bCopyBodf=FALSE;
		bCopyNbof=FALSE;
		bCopyCnld=FALSE;
		bCopySdsp=FALSE;
		bCopyBmld=FALSE;
		bCopyFmld=FALSE;
		bCopyFbld=FALSE;
		bCopyPres=FALSE;
		bCopyPrst=FALSE;
		bCopyPtns=FALSE;
		bCopyStmp=FALSE;
		bCopyNtmp=FALSE;
		bCopyEtmp=FALSE;
		bCopyBtmp=FALSE;
		bCopyGtmp=FALSE;
		bCopyWind=FALSE;
		bCopyWnat=FALSE;
		bCopySeis=FALSE;
		bCopyIfgs=FALSE;
		bCopyPnld=FALSE;
		bCopyTdpl=FALSE;
		bCopyIpcr=FALSE;
	}
};

#define CMDTYPE_FREE                    0
#define CMDTYPE_REMOVE_ANALYSIS         1
#define CMDTYPE_REMOVE_DESIGN           2
#define CMDTYPE_REMOVE_STEEL            3
#define CMDTYPE_REMOVE_CONCRETE         4
#define CMDTYPE_REMOVE_FDN              5
#define CMDTYPE_REMOVE_SRC              6
#define CMDTYPE_REMOVE_PUSHOVER         7
#define CMDTYPE_REMOVE_GRIDMODEL        8
#define CMDTYPE_REMOVE_PCDESIGN         9
#define CMDTYPE_REMOVE_IMPDGNFORCE      10
#define CMDTYPE_REMOVE_LOADRATING       11
#define CMDTYPE_REMOVE_PLATEGIRDER      12
#define CMDTYPE_REMOVE_CONC_COLUMN_KR   13
#define CMDTYPE_REMOVE_CONC_AND_COLKR   14
#define CMDTYPE_MODIFY_PSCDESIGN        15
#define CMDTYPE_REMOVE_CRACK            16
#define CMDTYPE_REMOVE_JUD              17
#define CMDTYPE_REMOVE_JFD              18
#define CMDTYPE_REMOVE_ALUMINUM         19

#define CMDTYPE_REMOVE_STL_LOADRATING   21
#define CMDTYPE_REMOVE_STL_ORTHDECK     22
#define CMDTYPE_REMOVE_RC_LOADRATING    23
#define CMDTYPE_REMOVE_CFSTEEL          24

#define CMDTYPE_REMOVE_BRDG_SEIS_EVAL   25
#define CMDTYPE_REMOVE_REINFORCE		26
#define CMDTYPE_REMOVE_BRDG_SEIS_EVAL_BEARING   27
#define CMDTYPE_REMOVE_POST_TABLE            	29
#define CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP 31

// NX Mec
#define CMDTYPE_REMOVE_STATIC         32
#define CMDTYPE_REMOVE_TIME_HISTORY    33
#define CMDTYPE_REMOVE_LINEAR_TIME_HISTORY    34
#define CMDTYPE_REMOVE_NONLINEAR_TIME_HISTORY    35
#define CMDTYPE_REMOVE_MOVING    36
#define CMDTYPE_REMOVE_SETTLEMENT    37
#define CMDTYPE_REMOVE_SPECTRUM    38
#define CMDTYPE_REMOVE_NONLINEAR_TIME_PUSHOVER    39
#define CMDTYPE_REMOVE_STAGPOST         40

// 1D, 2D, 3D 좌표
struct T_1COOR
{
	double x;       // L
	void Initialize()
	{
		x=0.;
	}
};
struct T_1COOR_UNIT
{
	T_1COOR_UNIT()
	{
		nCount=1;
		nUnit[0]=D_UNITSYS_BASE_LENGTH;
		x=nUnit[0];
	}
	int x;
	int nCount;
	int nUnit[1];
};
struct T_2COOR
{
	double x;       // L
	double y;       // L
	void Initialize()
	{
		x=0.;
		y=0.;
	}
};
struct T_2COOR_UNIT
{
	T_2COOR_UNIT()
	{
		nCount=2;
		for(int i=0; i<nCount; i++)nUnit[i]=D_UNITSYS_BASE_LENGTH;
		x=nUnit[0];
		y=nUnit[1];
	}
	int x;
	int y;
	int nCount;
	int nUnit[2];
};
struct T_3COOR
{
	double x;       // L
	double y;       // L
	double z;       // L
	void Initialize()
	{
		x=0.;
		y=0.;
		z=0.;
	}
	T_3COOR& Set(double x, double y, double z=0.0)
	{ this->x = x; this->y = y; this->z = z; return *this; }
};
struct T_3COOR_UNIT
{
	T_3COOR_UNIT()
	{
		nCount=3;
		for(int i=0; i<nCount; i++)nUnit[i]=D_UNITSYS_BASE_LENGTH;
		x=nUnit[0];
		y=nUnit[1];
		z=nUnit[2];
	}
	int x;
	int y;
	int z;
	int nCount;
	int nUnit[3];
};

struct T_KEY2
{
	UINT k1;
	UINT k2;
};

struct UINT2
{
	UINT2() : i1(0), i2(0) {}
	UINT2(UINT num1, UINT num2) { i1 = num1; i2 = num2; }
	operator unsigned long() { return (i1 << 16) + (i2); }
	BOOL operator == (UINT2 u2) const { return i1 == u2.i1 && i2 == u2.i2; }
	UINT i1;
	UINT i2;
};

struct T_VAR_VXD    // 가변길이 데이타의 변수 선언용 구조체
{
	UINT nCount; // number of data
	UINT rCount; // reverse of nCount
	void SetCount(UINT nNewCount) { nCount = nNewCount; rCount = ~nCount; }
};

struct T_DATA_VXD   // 가변 길이 데이타의 데이타 저장용 구조체
{
	T_DATA_VXD() { pData = 0; nSize = 0; rSize = ~nSize; nPos = 0; }
	~T_DATA_VXD() { if (pData) delete pData; }
	void SetSize(UINT nNewSize) 
	{
		ResetPos();
		if (nNewSize != 0 && nSize == nNewSize) 
		{
			nPos += sizeof(nSize)+sizeof(rSize);
			return;
		}
		if (pData) { delete pData; pData = 0; }
		nSize = nNewSize; rSize = ~nSize;
		pData = new unsigned char[GetTotalSize()];
		Write(&nSize, sizeof(nSize));
		Write(&rSize, sizeof(rSize));
	}
	UINT GetTotalSize() 
	{
		if (nSize <= 0) return sizeof(UINT)*2;  // consider nSize, rSize
		return ((nSize-1)/4+1)*4 + sizeof(UINT)*2; 
	}
	void ResetPos() { nPos = 0; }
	BOOL PrepareRead() 
	{ 
		ResetPos(); 
		if (!Read(&nSize, sizeof(nSize))) return FALSE;
		if (!Read(&rSize, sizeof(rSize))) return FALSE;
		if (nSize != ~rSize) return FALSE;
		return TRUE;
	}
	BOOL Write(void* pVar, UINT nBytes)
	{
		if (nBytes <= 0) return TRUE;
		if (nPos + nBytes > nSize + sizeof(UINT)*2) return FALSE;
		memcpy(pData+nPos, pVar, nBytes); nPos += nBytes;
		return TRUE;
	}
	BOOL Read(void* pVar, UINT nBytes)
	{
		if (nBytes <= 0) return TRUE;
		if (nPos + nBytes > nSize + sizeof(UINT)*2) return FALSE;
		memcpy(pVar, pData+nPos, nBytes); nPos += nBytes;
		return TRUE;
	}
	UINT nSize; // total number of bytes
	UINT rSize;
	UINT nPos;
	unsigned char* pData;
};


#define T_UDRD_KEY unsigned int
struct T_UDRD_INDEX 
{
	int Index;
	int nCommandType;
	CString strCmd;
};
struct T_UDRD_BUFFER
{
	int Index;
	int nCmd;
	int nKey;
};

// ===== // DB_LIST
struct T_LIST_DATA
{
	int nCmd;
	int nKey;
};
struct T_LIST_UDRD_D 
{
	int key;
};
#define DBDataList CList<T_LIST_DATA, T_LIST_DATA&>
#define NodeList DBDataList
#define ElemList DBDataList
#define MatlList DBDataList
#define SectList DBDataList
#define ThikList DBDataList
#define StorList DBDataList
#define StldList DBDataList
#define FbldList DBDataList
#define ElnkList DBDataList
#define NlnkList DBDataList
#define FipaList DBDataList
#define FssfList DBDataList
#define PsltList DBDataList
//#define WndcList DBDataList
//#define SescList DBDataList
#define ThisList DBDataList
#define GstpList DBDataList
#define NllpList DBDataList
#define AsgbList DBDataList
#define DmgbList DBDataList
#define SmptList DBDataList
#define GrupList DBDataList
#define SecvList DBDataList
#define SevoList DBDataList
#define VsecList DBDataList
#define VbemList DBDataList
#define SdomList DBDataList
#define BngrList DBDataList
#define LdgrList DBDataList
#define TdgrList DBDataList
#define PoldList DBDataList
#define PolcList DBDataList
#define PofcList DBDataList
#define PhgtList DBDataList // MNET:2762-GSJI-20070518
#define IethList DBDataList
#define P1ltList DBDataList
#define MtcsList DBDataList
#define BlcfList DBDataList
#define SlbsList DBDataList
#define FgrpList DBDataList
#define FounList DBDataList
#define GrndList DBDataList
#define PileList DBDataList
#define LagrList DBDataList
#define LaplList DBDataList
#define WvepList DBDataList
#define WdprList DBDataList
#define WdfcList DBDataList
#define ArprList DBDataList
#define EpstList DBDataList
#define EpseList DBDataList
#define PfslList DBDataList
#define PfsbList DBDataList
#define PfamList DBDataList
#define RfstList DBDataList
#define HASHSIZELISTNODE 10007
#define HASHSIZELISTELEM 10007
#define HASHSIZELISTMATL 10007
#define HASHSIZELISTSECT 10007
#define HASHSIZELISTTHIK 10007
#define HASHSIZELISTSTOR 101
#define HASHSIZELISTSTLD 10007
#define HASHSIZELISTWOPN 101
#define HASHSIZELISTFBLD 1007
#define HASHSIZELISTELNK 101
#define HASHSIZELISTNLNK 101
//#define HASHSIZELISTWNDC 11
//#define HASHSIZELISTSESC 11
#define HASHSIZELISTTHIS 10007
#define HASHSIZELISTGSTP 101
#define HASHSIZELISTNLLP 101
#define HASHSIZELISTASGB 101
#define HASHSIZELISTDMGB 101
#define HASHSIZELISTSMPT 11
#define HASHSIZELISTGRUP 101
#define HASHSIZELISTVSEC 101
#define HASHSIZELISTVBEM 101
#define HASHSIZELISTSDOM 101
#define HASHSIZELISTBNGR 101
#define HASHSIZELISTLDGR 101
#define HASHSIZELISTTDGR 101
#define HASHSIZELISTPOLD 11
#define HASHSIZELISTPOLC 11
#define HASHSIZELISTIETH 10007
#define HASHSIZELISTCLASS 10007
#define HASHSIZELISTP1LT 101
#define HASHSIZELISTMTCS 101
#define HASHSIZELISTBLCF 101
#define HASHSIZELISTSLBS 101
#define HASHSIZELISTFGRP 11
#define HASHSIZELISTLAGR 101
#define HASHSIZELISTLAPL 101
#define HASHSIZELISTWVEP 101
#define HASHSIZELISTWDPR 101
#define HASHSIZELISTWDFC 101
#define HASHSIZELISTARPR 101
#define HASHSIZELISTEPST 101
#define HASHSIZELISTEPSE 101
#define HASHSIZELISTRFST 10007

#include "DB_ST_DT_CMD.h"       // LT_????_CMD, UR_????_ADD, UR_????_DEL, ...
//#include "DB_ST_DT_PREF.h"      // preference
#include "DB_ST_DT_NODE.h"      // node
#include "DB_ST_DT_ELEM.h"      // element
#include "DB_ST_DT_GRUP.h"      // grup, bngr, ldgr, tdgr, vsec
#include "DB_ST_DT_CASE.h"      // stld, lcom, envl
#include "DB_ST_DT_MATL.h"      // material
#include "DB_ST_DT_MATD_REIN.h"	// Material Design - Reinforcement
#include "DB_ST_DT_SECT.h"      // section
#include "DB_ST_DT_SSM_REIN.h"  // Section Reinforcement
#include "DB_ST_DT_THIK.h"      // thickness
#include "DB_ST_DT_BLDG.h"      // building, story
#include "DB_ST_DT_BNDR.h"      // boundary
#include "DB_ST_DT_ASGB.h"      // Seismic Isolator
#include "DB_ST_DT_DMGB.h"      // Damper GB
#include "DB_ST_DT_APPRAISAL.h" // Appraisal
#include "DB_ST_DT_SP.h"
#include "DB_ST_DT_SP14SeisMembParams.h"
#include "DB_ST_DT_SP16SafetyMembParams.h"
#include "DB_ST_DT_SP16CombinedSectParams.h"
#include "DB_ST_DT_MASS.h"      // mass
#include "DB_ST_DT_STLD.h"      // static load
#include "DB_ST_DT_WIND.h"      // wind load
#include "DB_ST_DT_WNAT.h"      // wnat load
#include "DB_ST_DT_SEIS.h"      // seismic load
#include "DB_ST_DT_SEIS_CVL.h"  // seismic load Civil
#include "DB_ST_DT_ERPR.h"      // Earth Pressure
#include "DB_ST_DT_SPEC.h"      // response spectrum
#include "DB_ST_DT_TIME.h"      // time history
#include "DB_ST_DT_MOVE.h"      // moving load
#include "DB_ST_DT_SPLD.h"      // special load(initial force, settlement, erection sequence, composite bridge)
#include "DB_ST_DT_WVLD.h"	    
#include "DB_ST_DT_HYDR.h"      // hydration
#include "DB_ST_DT_NONL.h"      // non linear
#include "DB_ST_DT_POVR.h"      // push over
#include "DB_ST_DT_POCV.h"      // push over curve
#include "DB_ST_DT_ITHA.h"      // Inelastic Time History Analysis
#include "DB_ST_DT_STAG.h"      // stage
#include "DB_ST_DT_MHTP.h"      // Material high temperature properties
#include "DB_ST_DT_GMOD.h"      // grid model
#include "DB_ST_DT_ANAL.h"      // analysis(actl, pdel, buck)
#include "DB_ST_DT_POST.h"      // cutl, ulfc, ulfd
#include "DB_ST_DT_ETC.h"       // pjcf, styp, gdln, npln, nucs
#include "DB_ST_DT_TRLN.h"      // TDat, TSET, gdln, npln, nucs
#include "DB_ST_DT_DY2E.h"      // Dyna2E
#include "DB_ST_DT_TOWER.h"     // mbtp
#include "DB_ST_DT_FIGR.h"      // user defined figure
#include "DB_ST_DT_MTCS.h"      // Material Coordinates System
#include "DB_ST_DT_SSM.h"       // Section Manager
#include "DB_ST_DN_JFD.h"       // Foundation
#include "DB_ST_DT_WOPN.h"      // Wall Opening
#include "DB_ST_DT_WDPR.h"      // Wind Pressure added by yskim 자동 풍하중 계산. [4802]
#include "DB_ST_DN_RESULT_RATING.h"  
#include "DB_ST_DN_DESIGN.h"  
#include "DB_PFSL.h"
#include "DB_PFSB.h"
#include "DB_PFAM.h"
#include "DB_ST_DT_SIPA.h"		// seismic investigation Civil JP
#include "DB_ST_DT_SIGR.h"		// seismic investigation Civil JP
#include "DB_ST_DT_SIMD.h"		// seismic investigation Civil JP
#include "DB_ST_DT_SIRD.h"		// seismic investigation Civil JP
#include "DB_ST_DT_SIBD.h"		// seismic investigation Civil JP
#include "DB_ST_DT_SIDP.h"		// seismic investigation Civil JP
#include "DB_ST_DT_SISR.h"		// seismic investigation Civil JP
#include "DB_ST_DT_SIET.h"		// seismic investigation Civil JP
#include "DB_ST_DT_SIFA.h"		// seismic investigation Civil JP
#include "DB_ST_DT_PRIS.h"		// result printout Civil JP
#include "DB_ST_DT_PROS.h"		// result printout Civil JP
#include "DB_ST_DT_PMIS.h"		// Model image output Civil JP
#include "DB_ST_DT_PMOS.h"		// Model image output Civil JP


struct T_COLOR_D
{
	COLORREF WireFrame;
	COLORREF HiddenFill;
	COLORREF HiddenEdge;
	BOOL bBlending;
	double dBlendingFactor;
	void Initialize()
	{
		WireFrame=COLORREF(RGB(0, 0, 0));
		HiddenFill=COLORREF(RGB(0, 0, 0));
		HiddenEdge=COLORREF(RGB(0, 0, 0));
		bBlending=FALSE;
		dBlendingFactor=0.5;
	}
};

#define T_WALL_KEY unsigned int
union T_WALL_K
{
	T_WALL_KEY keymap;
	struct
	{
		unsigned int wallid : 16;   // 0-65535
		unsigned int storid : 16;   // 0-65535
	}key;
};
struct T_WALL_D
{
	CArray<T_ELEM_K, T_ELEM_K>arElemKey;
	double xc;
	double yc;
	double ux;
	double uy;
	double dLength;
	double dThickness;
	double dHeight;
	BOOL bAllConc;
	BOOL bLineWall;
	BOOL bSameThickness;
	BOOL bSameThikId;
	T_ELEM_K RootElemKey;
	void Initialize()
	{
		arElemKey.RemoveAll();
		xc=0.;
		yc=0.;
		ux=0.;
		uy=0.;
		dLength=0.;
		dThickness=0.;
		dHeight=0.;
		bAllConc=FALSE;
		bLineWall=FALSE;
		bSameThickness=FALSE;
		bSameThikId=FALSE;
		RootElemKey = 0;
	}
	T_WALL_D()
	{
	}
	T_WALL_D(T_WALL_D& src)
	{
		*this = src;
	}
	T_WALL_D& operator=(T_WALL_D& src)
	{
		arElemKey.Copy(src.arElemKey);
		xc=src.xc;
		yc=src.yc;
		ux=src.ux;
		uy=src.uy;
		dLength=src.dLength;
		dThickness=src.dThickness;
		dHeight=src.dHeight;
		bAllConc=src.bAllConc;
		bLineWall=src.bLineWall;
		bSameThickness=src.bSameThickness;
		bSameThikId=src.bSameThikId;
		RootElemKey = src.RootElemKey;
		return *this;
	}
};
struct T_WALL_UDRD_D
{
	T_WALL_K key;
	T_WALL_D data;
};
struct T_WALL_UNIT
{
	T_WALL_UNIT()
	{
		arElemKey=D_UNITSYS_NONE;
		xc=D_UNITSYS_BASE_LENGTH;
		yc=D_UNITSYS_BASE_LENGTH;
		ux=D_UNITSYS_NONE;
		uy=D_UNITSYS_NONE;
		dLength=D_UNITSYS_BASE_LENGTH;
		dThickness=D_UNITSYS_BASE_LENGTH;
		dHeight=D_UNITSYS_BASE_LENGTH;
		bAllConc=D_UNITSYS_NONE;
		bLineWall=D_UNITSYS_NONE;
		bSameThickness=D_UNITSYS_NONE;
		bSameThikId=D_UNITSYS_NONE;
	}
	int arElemKey;
	int xc;
	int yc;
	int ux;
	int uy;
	int dLength;
	int dThickness;
	int dHeight;
	int bAllConc;
	int bLineWall;
	int bSameThickness;
	int bSameThikId;
};
#define HASHSIZEWALL HASHSIZEELEM

struct T_FREEEDGE
{
	T_ELEM_K KeyElem;
	T_NODE_K No1;
	T_NODE_K No2;
	int No1Index;
	int No2Index;
};
struct T_FREEFACE
{
	T_ELEM_K KeyElem;
	int nFace;   // 면의 번호
	int nNode;   // 면을 구성하는 절점수
	T_NODE_K Node[4];  // 면을 구성하는 절점번호
	int NodeIndex[4];  // 면을 구성하는 절점의 배열 인덱스
	void Initialize()
	{
		KeyElem=0;
		nFace=0;
		nNode=0;
		for(int i=0; i<4; i++)
		{
			Node[i]=0;
			NodeIndex[i]=0;
		}
	}
};
struct T_MENU_STATE
{
	BOOL bIncludeEnable;
	BOOL bEnable;
	BOOL bIncludeCheck;
	BOOL bCheck;

	void Enable(BOOL bEnable) 
	{
		bIncludeEnable = TRUE;
		this->bEnable = bEnable;
	}
	void SetCheck(BOOL bCheck)
	{
		bIncludeCheck = TRUE;
		this->bCheck = bCheck;
	}
	void Initialize()
	{
		bIncludeEnable = FALSE;
		bEnable = FALSE;
		bIncludeCheck = FALSE;
		bCheck = FALSE;
	}
};

// GUID - pjcf
#define T_GUPJ_K T_PJCF_K
typedef CArray<T_GUPJ_K, T_GUPJ_K> T_GUPJ_K_LIST;

struct T_GUPJ_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUPJ_D() { Initialize(); }
};

struct T_GUPJ_UDRD_D
{
	T_GUPJ_K key;
	T_GUPJ_D data;
};
#define HASHSIZEGUPJ HASHSIZEPJCF

// GUID - stor
#define T_GUST_K T_STOR_K
typedef CArray<T_GUST_K, T_GUST_K> T_GUST_K_LIST;

struct T_GUST_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUST_D() { Initialize(); }
};

struct T_GUST_UDRD_D
{
	T_GUST_K key;
	T_GUST_D data;
};
#define HASHSIZEGUST HASHSIZESTOR

// GUID - NPLN
#define T_GUNP_K T_NPLN_K
typedef CArray<T_GUNP_K, T_GUNP_K> T_GUNP_K_LIST;

struct T_GUNP_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUNP_D() { Initialize(); }
};

struct T_GUNP_UDRD_D
{
	T_GUNP_K key;
	T_GUNP_D data;
};
#define HASHSIZEGUNP HASHSIZENPLN

// GUID - matl
#define T_GUMT_K T_MATL_K
typedef CArray<T_GUMT_K, T_GUMT_K> T_GUMT_K_LIST;

struct T_GUMT_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUMT_D() { Initialize(); }
};

struct T_GUMT_UDRD_D
{
	T_GUMT_K key;
	T_GUMT_D data;
};
#define HASHSIZEGUMT HASHSIZEMATL

// GUID - sect
#define T_GUSC_K T_SECT_K
typedef CArray<T_GUSC_K, T_GUSC_K> T_GUSC_K_LIST;

struct T_GUSC_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUSC_D() { Initialize(); }
};

struct T_GUSC_UDRD_D
{
	T_GUSC_K key;
	T_GUSC_D data;
};
#define HASHSIZEGUSC HASHSIZESECT

// GUID - thik
#define T_GUTK_K T_THIK_K
typedef CArray<T_GUTK_K, T_GUTK_K> T_GUTK_K_LIST;

struct T_GUTK_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUTK_D() { Initialize(); }
};

struct T_GUTK_UDRD_D
{
	T_GUTK_K key;
	T_GUTK_D data;
};
#define HASHSIZEGUTK HASHSIZETHIK

// GUID - memb
#define T_GUMB_K T_MEMB_K
typedef CArray<T_GUMB_K, T_GUMB_K> T_GUMB_K_LIST;

struct T_GUMB_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUMB_D() { Initialize(); }
};

struct T_GUMB_UDRD_D
{
	T_GUMB_K key;
	T_GUMB_D data;
};
#define HASHSIZEGUMB HASHSIZEMEMB

// GUID - node
#define T_GUNO_K T_NODE_K
typedef CArray<T_GUNO_K, T_GUNO_K> T_GUNO_K_LIST;

struct T_GUNO_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUNO_D() { Initialize(); }
};

struct T_GUNO_UDRD_D
{
	T_GUNO_K key;
	T_GUNO_D data;
};
#define HASHSIZEGUNO HASHSIZENODE

// GUID - elem
#define T_GUEL_K T_ELEM_K
typedef CArray<T_GUEL_K, T_GUEL_K> T_GUEL_K_LIST;

struct T_GUEL_D
{
	GUID guid;

	void Initialize() { CoCreateGuid(&guid); }
	T_GUEL_D() { Initialize(); }
};

struct T_GUEL_UDRD_D
{
	T_GUEL_K key;
	T_GUEL_D data;
};
#define HASHSIZEGUEL HASHSIZEELEM

#pragma pack()

#include "DB_ST_DN.h"
#include "DB_ST_RT.h"
#include "DB_ST_DT_REINFORCE.h"	// Reinforce CH

#endif
