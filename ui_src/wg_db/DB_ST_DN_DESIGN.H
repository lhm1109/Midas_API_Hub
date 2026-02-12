#ifndef __DB_ST_DN_DESIGN_H__
#define __DB_ST_DN_DESIGN_H__

#define MIDAS_SDK

#include "DB_ST_DN.h"
#include "DB_ST_DT_UNIT.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

////////////////////////////////////////////////////////////////
///////////////// Struct for Design Module /////////////////////
////////////////////////////////////////////////////////////////
enum DesignTypeId
{
    D_DESIGN_TYPE_STEEL=0,
    D_DESIGN_TYPE_RC_BEAM_DGN,
    D_DESIGN_TYPE_RC_COLM_DGN,
    D_DESIGN_TYPE_RC_BRCE_DGN,
    D_DESIGN_TYPE_RC_WALL_DGN,
    D_DESIGN_TYPE_PS_BEAM_DGN,
    D_DESIGN_TYPE_RC_BEAM_CHK,
    D_DESIGN_TYPE_RC_COLM_CHK,
    D_DESIGN_TYPE_RC_BRCE_CHK,
    D_DESIGN_TYPE_RC_WALL_CHK,
    D_DESIGN_TYPE_PS_BEAM_CHK,
    D_DESIGN_TYPE_SRC_BEAM,
    D_DESIGN_TYPE_SRC_COLM,
    D_DESIGN_TYPE_END
};

struct T_DATA_GEN
{
    int iCurLengthIdx, iCurForceIdx;
    int iCodeLengthIdx, iCodeForceIdx;

    int iNumStl;
    int iNumBeam, iNumColm, iNumBrce, iNumWall;//for concrete
    int iNumSrcm,  iNumSrbm; //for SRC
    // ����, ���
    int iNumNode, iNumElem;
    int iNumSect, iNumMatS, iNumMatC, iNumMatR;

    int iNumLcom;
    
    void Initialize()
    {
        iCurLengthIdx=iCurForceIdx=0;
        iCodeLengthIdx=iCodeForceIdx=0;
        iNumStl=0;
        iNumBeam=iNumColm=iNumBrce=iNumWall=0;
        iNumNode=iNumElem=0;
        iNumSect=iNumMatS=iNumMatC=iNumMatR=0;
        iNumSrcm=iNumSrbm=0;
        iNumLcom=0;
    }
};

struct T_FORCSTRIP_D
{
    // dLineForce - local force : I, 1/4, Center, 3/4, J
    //              component : Fx, Fy, Fz, Mx, My, Mz
    double dLineForce[5][6];

    void Initialize(double dVal)
    {
        for(int i=0; i<5; i++)
            for(int j=0; j<6; j++)
                dLineForce[i][j] = dVal;
    }
};

// Propeties of Effective Beam
#define T_PREB_K unsigned int
typedef CArray<T_PREB_K, T_PREB_K> T_PREB_K_LIST;

struct T_PREB_D
{
    CString strName;
    CString strDesc;
    double  adWidth[4]; // [0]:I-Left, [1]:I-Right, [2]:J-Left, [3]:J-Right

    void Initialize()
    {
        strName.Empty();
        strDesc.Empty();
        for(int i=0; i<4; i++) adWidth[i] = 0.;
    }
    BOOL operator==(const T_PREB_D& src) const
    {
        if(strName != src.strName) return FALSE;
        if(strDesc != src.strDesc) return FALSE;
        for(int i=0; i<4; i++) if(fabs(adWidth[i] - src.adWidth[i]) > 1.e-8) return FALSE;
        return TRUE;
    }
};
struct T_PREB_D_CH
{
    char    strName[20];
    char    strDesc[80];
    double  adWidth[4]; // [0]:I-Left, [1]:I-Right, [2]:J-Left, [3]:J-Right

    void ConvertToString(T_PREB_D& rData)
    {
        ConvertCharStr(strName, rData.strName, sizeof(strName));
        ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
        for(int i=0; i<4; i++) rData.adWidth[i] = adWidth[i];
    }
    void ConvertToChar(T_PREB_D& rData)
    {
        ConvertStrChar(rData.strName, strName, sizeof(strName));
        ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
        for(int i=0; i<4; i++) adWidth[i] = rData.adWidth[i];
    }
};
struct T_PREB_UDRD_D
{
    T_PREB_K key;
    T_PREB_D data;
};
struct T_PREB_UDRD_D_CH
{
    T_PREB_K key;
    T_PREB_D_CH data;
};
struct T_PREB_UDRD_D_CH_RW
{
    T_PREB_K key;
    char    strName[20];
    char    strDesc[80];
    double  adWidth[4]; // [0]:I-Left, [1]:I-Right, [2]:J-Left, [3]:J-Right

    void GetPreb(T_PREB_K& rKey, T_PREB_D_CH& rData)
    {
        rKey=key;
        memcpy(rData.strName, strName, sizeof(strName));
        memcpy(rData.strDesc, strDesc, sizeof(strDesc));
        for(int i=0; i<4; i++) rData.adWidth[i] = adWidth[i];
    }
    void SetPreb(const T_PREB_K Key, const T_PREB_D_CH& rData)
    {
        key=Key;
        memcpy(strName, rData.strName, sizeof(strName));
        memcpy(strDesc, rData.strDesc, sizeof(strDesc));
        for(int i=0; i<4; i++) adWidth[i] = rData.adWidth[i];
    }
};
struct T_PREB_UNIT
{
    T_PREB_UNIT()
    {
        adWidth = D_UNITSYS_BASE_LENGTH;
    }
    int adWidth;
};
#define HASHSIZEPREB 101

// Propeties of Column strip
#define T_PRCS_K unsigned int
typedef CArray<T_PRCS_K, T_PRCS_K> T_PRCS_K_LIST;

struct T_PRCS_D
{
    CString strName;
    CString strDesc;
    double  adWidth[4]; // [0]:I-Left, [1]:I-Right, [2]:J-Left, [3]:J-Right

    void Initialize()
    {
        strName.Empty();
        strDesc.Empty();
        for(int i=0; i<4; i++) adWidth[i] = 0.;
    }
    BOOL operator==(const T_PRCS_D& src) const
    {
        if(strName != src.strName) return FALSE;
        if(strDesc != src.strDesc) return FALSE;
        for(int i=0; i<4; i++) if(fabs(adWidth[i] - src.adWidth[i]) > 1.e-8) return FALSE;
        return TRUE;
    }
};
struct T_PRCS_D_CH
{
    char    strName[20];
    char    strDesc[80];
    double  adWidth[4]; // [0]:I-Left, [1]:I-Right, [2]:J-Left, [3]:J-Right

    void ConvertToString(T_PRCS_D& rData)
    {
        ConvertCharStr(strName, rData.strName, sizeof(strName));
        ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
        for(int i=0; i<4; i++) rData.adWidth[i] = adWidth[i];
    }
    void ConvertToChar(T_PRCS_D& rData)
    {
        ConvertStrChar(rData.strName, strName, sizeof(strName));
        ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
        for(int i=0; i<4; i++) adWidth[i] = rData.adWidth[i];
    }
};
struct T_PRCS_UDRD_D
{
    T_PRCS_K key;
    T_PRCS_D data;
};
struct T_PRCS_UDRD_D_CH
{
    T_PRCS_K key;
    T_PRCS_D_CH data;
};
struct T_PRCS_UDRD_D_CH_RW
{
    T_PRCS_K key;
    char    strName[20];
    char    strDesc[80];
    double  adWidth[4]; // [0]:I-Left, [1]:I-Right, [2]:J-Left, [3]:J-Right

    void GetPrcs(T_PRCS_K& rKey, T_PRCS_D_CH& rData)
    {
        rKey=key;
        memcpy(rData.strName, strName, sizeof(strName));
        memcpy(rData.strDesc, strDesc, sizeof(strDesc));
        for(int i=0; i<4; i++) rData.adWidth[i] = adWidth[i];
    }
    void SetPrcs(const T_PRCS_K Key, const T_PRCS_D_CH& rData)
    {
        key=Key;
        memcpy(strName, rData.strName, sizeof(strName));
        memcpy(strDesc, rData.strDesc, sizeof(strDesc));
        for(int i=0; i<4; i++) adWidth[i] = rData.adWidth[i];
    }
};
struct T_PRCS_UNIT
{
    T_PRCS_UNIT()
    {
        adWidth = D_UNITSYS_BASE_LENGTH;
    }
    int adWidth;
};
#define HASHSIZEPRCS 101

// Column Strip
#define T_MSTR_K unsigned int
typedef CArray<T_MSTR_K, T_MSTR_K> T_MSTR_K_LIST;

struct T_MSTR_D
{
    T_MEMB_K MembK;
    T_PRCS_K PrcsK;

    void Initialize()
    {
        MembK = 0;
        PrcsK = 0;
    }
    BOOL operator==(const T_MSTR_D& src) const
    {
        if(MembK != src.MembK) return FALSE;
        if(PrcsK != src.PrcsK) return FALSE;
        return TRUE;
    }
};
struct T_MSTR_UDRD_D
{
    T_MSTR_K key;
    T_MSTR_D data;

    void GetMstr(T_MSTR_K& rKey, T_MSTR_D& rData)
    {
        rKey=key;
        rData.MembK = data.MembK;
        rData.PrcsK = data.PrcsK;
    }
    void SetMstr(const T_MSTR_K Key, const T_MSTR_D& rData)
    {
        key=Key;
        data.MembK = rData.MembK;
        data.PrcsK = rData.PrcsK;
    }
};

#define HASHSIZEMSTR 101

// Support Line
#define T_SPTL_K unsigned int
typedef CArray<T_SPTL_K, T_SPTL_K> T_SPTL_K_LIST;

struct T_SPTL_D
{
    UINT StartNodeK;
    UINT EndNodeK;
    CArray<UINT, UINT> aNodeK;
	int nDirection; // 0:X, 1:Y

    void Initialize()
    {
        StartNodeK = 0;
        EndNodeK = 0;
        aNodeK.RemoveAll();
		nDirection = 0;
	}

	T_SPTL_D() { Initialize(); }
	T_SPTL_D(T_SPTL_D& src) { *this = src; }
    BOOL operator==(const T_SPTL_D& src) const
    {
        if (StartNodeK != src.StartNodeK) return FALSE;
        if (EndNodeK != src.EndNodeK) return FALSE;
        if (aNodeK.GetSize() != src.aNodeK.GetSize()) return FALSE;
        for (auto i = 0; i < src.aNodeK.GetSize(); i++)
        {
            if (aNodeK[i] != src.aNodeK[i]) return FALSE;
        }
		if (nDirection != src.nDirection) return FALSE;
        return TRUE;
	}
    T_SPTL_D& operator=(const T_SPTL_D& src)
    {
        StartNodeK = src.StartNodeK;
        EndNodeK = src.EndNodeK;
        aNodeK.Copy(src.aNodeK);
		nDirection = src.nDirection;
        return *this;
	}
};

struct T_SPTL_UDRD_D
{
	T_SPTL_K key;
	T_SPTL_D data;

	void GetSptl(T_SPTL_K& rKey, T_SPTL_D& rData)
	{
		rKey = key;
		rData = data;
	}
	void SetSptl(const T_SPTL_K Key, const T_SPTL_D& rData)
	{
		key = Key;
		data = rData;
	}
};

struct T_SPTL_UDRD_D_CH_RW
{
	T_SPTL_K key;
	UINT StartNodeK;
	UINT EndNodeK;
	int nCountNodeK;
	// CArray는 별도 저장
	int nDirection;

	void GetSptl(T_SPTL_K& rKey, T_SPTL_D& rData)
	{
		rKey = key;
		rData.StartNodeK = StartNodeK;
		rData.EndNodeK = EndNodeK;
		// aNodeK는 별도 처리
		rData.nDirection = nDirection;
	}

	void SetSptl(const T_SPTL_K Key, const T_SPTL_D& rData)
	{
		key = Key;
		StartNodeK = rData.StartNodeK;
		EndNodeK = rData.EndNodeK;
		nCountNodeK = rData.aNodeK.GetSize();
		nDirection = rData.nDirection;
	}
};

struct T_SPTL_UNIT
{
	T_SPTL_UNIT()
	{
		// No unit conversion needed for node keys
	}
};

#define HASHSIZESPTL 1001

// Design Strip Section
#define T_DSPS_K unsigned int
typedef CArray<T_DSPS_K, T_DSPS_K> T_DSPS_K_LIST;

struct T_DSPS_D
{
    UINT Node1;      // start node
    UINT Node2;      // end node
    double dDist;   // distant from start node (Ratio)
    double dLeft;    // arm for left  
    double dRight;  // arm for right
    BOOL bFit;

    void Initialize()
    {
        Node1 = 0;
        Node2 = 0;
        dDist = 0.0;
        dLeft = 0.0;
        dRight = 0.0;
        bFit = FALSE;
    }

    BOOL operator == (const T_DSPS_D& rData) const
    {
        if (Node1 != rData.Node1) return FALSE;
        if (Node2 != rData.Node2) return FALSE;
        if (dDist != rData.dDist) return FALSE;
        if (dLeft != rData.dLeft) return FALSE;
        if (dRight != rData.dRight) return FALSE;
        if (bFit != rData.bFit) return FALSE;
        return TRUE;
    }
};

struct T_DSPS_D_CH
{
    UINT Nod1;
    UINT Nod2;
    double dDist;
    double deft;
    double dRight;
    BOOL bFit;

    void ConvertToString(T_DSPS_D& rData)
    {
        rData.Node1 = Nod1;
        rData.Node2 = Nod2;
        rData.dDist = dDist;
        rData.dLeft = deft;
        rData.dRight = dRight;
        rData.bFit = bFit;
    }

    void ConvertToChar(T_DSPS_D& rData)
    {
        Nod1 = rData.Node1;
        Nod2 = rData.Node2;
        dDist = rData.dDist;
        deft = rData.dLeft;
        dRight = rData.dRight;
        bFit = rData.bFit;
    }
};

struct T_DSPS_UDRD_D
{
    T_DSPS_K key;
    T_DSPS_D data;
};

struct T_DSPS_UDRD_D_CH
{
    T_DSPS_K key;
    T_DSPS_D_CH data;

    void GetDsps(T_DSPS_K& rKey, T_DSPS_D_CH& rData)
    {
        rKey = key;
        rData = data;
    }
    void SetDsps(const T_DSPS_K Key, const T_DSPS_D_CH& rData)
    {
        key = Key;
        data = rData;
    }
};

struct T_DSPS_UNIT
{
    T_DSPS_UNIT()
    {
        Node1 = D_UNITSYS_NONE;
        Node2 = D_UNITSYS_NONE;
        dDist = D_UNITSYS_BASE_LENGTH;
        dLeft = D_UNITSYS_BASE_LENGTH;
        dRight = D_UNITSYS_BASE_LENGTH;
        bFit = D_UNITSYS_NONE;
    }
    int Node1;
    int Node2;
    int dDist;
    int dLeft;
    int dRight;
    int bFit;
};

#define HASHSIZEDSPS 100

// Design Strip
#define T_DSTR_K unsigned int
typedef CArray<T_DSTR_K, T_DSTR_K> T_DSTR_K_LIST;

struct T_DGNSTRIP_SPAN_INFO
{
    UINT nSpanNo;
    double dWidth_StartTL;
    double dWidth_StartBR;
    double dWidth_EndTL;
    double dWidth_EndBR;
    double DgnPointNo;
    CArray<T_DSPS_K, T_DSPS_K> aDspsK;  // Design Section Keys in this span

    void Initialize()
    {
        nSpanNo = 0;
        dWidth_StartTL = 0.0;
        dWidth_StartBR = 0.0;
        dWidth_EndTL = 0.0;
        dWidth_EndBR = 0.0;
        DgnPointNo = 0.0;
        aDspsK.RemoveAll();
    }
    T_DGNSTRIP_SPAN_INFO() { Initialize(); }
    T_DGNSTRIP_SPAN_INFO(const T_DGNSTRIP_SPAN_INFO& src) { *this = src; }
    T_DGNSTRIP_SPAN_INFO& operator=(const T_DGNSTRIP_SPAN_INFO& src)
    {
        nSpanNo = src.nSpanNo;
        dWidth_StartTL = src.dWidth_StartTL;
        dWidth_StartBR = src.dWidth_StartBR;
        dWidth_EndTL = src.dWidth_EndTL;
        dWidth_EndBR = src.dWidth_EndBR;
        DgnPointNo = src.DgnPointNo;
        aDspsK.Copy(src.aDspsK);
        return *this;
    }
};

// File Read/Write용 구조체 (CArray 제외)
struct T_DGNSTRIP_SPAN_INFO_RW
{
    UINT nSpanNo;
    double dWidth_StartTL;
    double dWidth_StartBR;
    double dWidth_EndTL;
    double dWidth_EndBR;
    double DgnPointNo;
    int nCountDspsK;  // aDspsK 배열 크기

    void Initialize()
    {
        nSpanNo = 0;
        dWidth_StartTL = 0.0;
        dWidth_StartBR = 0.0;
        dWidth_EndTL = 0.0;
        dWidth_EndBR = 0.0;
        DgnPointNo = 0.0;
        nCountDspsK = 0;
    }
    T_DGNSTRIP_SPAN_INFO_RW() { Initialize(); }

    void SetFromSpanInfo(const T_DGNSTRIP_SPAN_INFO& src)
    {
        nSpanNo = src.nSpanNo;
        dWidth_StartTL = src.dWidth_StartTL;
        dWidth_StartBR = src.dWidth_StartBR;
        dWidth_EndTL = src.dWidth_EndTL;
        dWidth_EndBR = src.dWidth_EndBR;
        DgnPointNo = src.DgnPointNo;
        nCountDspsK = (int)src.aDspsK.GetSize();
    }

    void GetToSpanInfo(T_DGNSTRIP_SPAN_INFO& dst) const
    {
        dst.nSpanNo = nSpanNo;
        dst.dWidth_StartTL = dWidth_StartTL;
        dst.dWidth_StartBR = dWidth_StartBR;
        dst.dWidth_EndTL = dWidth_EndTL;
        dst.dWidth_EndBR = dWidth_EndBR;
        dst.DgnPointNo = DgnPointNo;
        // aDspsK는 별도로 읽어야 함
    }
};

struct T_DGNSTRIP_SPAN_INFO_UNIT
{
    T_DGNSTRIP_SPAN_INFO_UNIT()
    {
        nSpanNo = D_UNITSYS_NONE;
        dWidth_StartTL = D_UNITSYS_BASE_LENGTH;
        dWidth_StartBR = D_UNITSYS_BASE_LENGTH;
        dWidth_EndTL = D_UNITSYS_BASE_LENGTH;
        dWidth_EndBR = D_UNITSYS_BASE_LENGTH;
        DgnPointNo = D_UNITSYS_NONE;
    }
    int nSpanNo;
    int dWidth_StartTL;
    int dWidth_StartBR;
    int dWidth_EndTL;
    int dWidth_EndBR;
    int DgnPointNo;
};

struct T_DSTR_D
{
    int nArrDgnPoint;
    UINT SptlK;
    int nDgnSection;
    CArray<T_DGNSTRIP_SPAN_INFO, T_DGNSTRIP_SPAN_INFO&> aSpan;

    void Initialize()
    {
        nArrDgnPoint = 0;
        SptlK = 0;
        nDgnSection = 0;
        aSpan.RemoveAll();
    }
    T_DSTR_D() { Initialize(); }
    T_DSTR_D(T_DSTR_D& src) { *this = src; }
    BOOL operator==(const T_DSTR_D& src) const
    {
        if (nArrDgnPoint != src.nArrDgnPoint) return FALSE;
		if (SptlK != src.SptlK) return FALSE;
        if (nDgnSection != src.nDgnSection) return FALSE;
        for (auto i = 0; i < src.aSpan.GetSize(); i++)
        {
            if (aSpan[i].DgnPointNo != src.aSpan[i].DgnPointNo) return FALSE;
            if (aSpan[i].dWidth_StartTL != src.aSpan[i].dWidth_StartTL) return FALSE;
            if (aSpan[i].dWidth_StartBR != src.aSpan[i].dWidth_StartBR) return FALSE;
            if (aSpan[i].dWidth_EndTL != src.aSpan[i].dWidth_EndTL) return FALSE;
            if (aSpan[i].dWidth_EndBR != src.aSpan[i].dWidth_EndBR) return FALSE;
        }
        return TRUE;
    }
    T_DSTR_D& operator=(const T_DSTR_D& src)
    {
        nArrDgnPoint = src.nArrDgnPoint;
		SptlK = src.SptlK;
        nDgnSection = src.nDgnSection;
        aSpan.Copy(src.aSpan);
        return *this;
    }
};

struct T_DSTR_D_CH
{
    int nArrDgnPoint;
	UINT SptlK;
    int nDgnSection;
    CArray<T_DGNSTRIP_SPAN_INFO, T_DGNSTRIP_SPAN_INFO&> aSpan;

    void ConvertToString(T_DSTR_D& rData)
    {
        rData.nArrDgnPoint = nArrDgnPoint;
		rData.SptlK = SptlK;
        rData.nDgnSection = nDgnSection;
        rData.aSpan.Copy(aSpan);
    }
    void ConvertToChar(T_DSTR_D& rData)
    {
        nArrDgnPoint = rData.nArrDgnPoint;
		SptlK = rData.SptlK;
        nDgnSection = rData.nDgnSection;
        aSpan.Copy(rData.aSpan);
    }
};

struct T_DSTR_UDRD_D
{
    T_DSTR_K key;
    T_DSTR_D data;

    void GetDstr(T_DSTR_K& rKey, T_DSTR_D& rData)
    {
        rKey = key;
        rData = data;
    }
    void SetDstr(const T_DSTR_K Key, const T_DSTR_D& rData)
    {
        key = Key;
        data = rData;
    }
};

struct T_DSTR_UDRD_D_CH
{
    T_DSTR_K key;
    T_DSTR_D_CH data;
};

struct T_DSTR_UDRD_D_CH_RW
{
    T_DSTR_K key;
    int nArrDgnPoint;
    UINT SptlK;
    int nDgnSection;
    int nCountSpan;
    // CArray aSpan은 별도 처리

    void GetDstr(T_DSTR_K& rKey, T_DSTR_D& rData)
    {
        rKey = key;
        rData.nArrDgnPoint = nArrDgnPoint;
        rData.SptlK = SptlK;
        rData.nDgnSection = nDgnSection;
        // aSpan은 별도 처리
    }
    void SetDstr(const T_DSTR_K Key, const T_DSTR_D& rData)
    {
        key = Key;
        nArrDgnPoint = rData.nArrDgnPoint;
        SptlK = rData.SptlK;
        nDgnSection = rData.nDgnSection;
        // aSpan은 별도 처리
        nCountSpan = (int)rData.aSpan.GetSize();
    }
};

struct T_DSTR_UNIT
{
    T_DSTR_UNIT()
    {
        nArrDgnPoint = D_UNITSYS_NONE;
        nDgnSection = D_UNITSYS_NONE;
    }
    int nArrDgnPoint;
    int nDgnSection;
};

#define HASHSIZEDSTR 1001

//////////////////////////////////////////////////////////////////////////
struct ElementPass
{
	int nElemId;  // 요소 번호 (Element ID)
	int nSpanId; // Design sprip 내 Span ID

	// 요소 내 시작점 정보
	int nEdgeStart;        // 시작점이 위치한 Edge 번호 (1~4, 0이면 내부 시작)
	double dEdgeStart;     // Edge의 시작점으로부터의 거리 (Edge 방향 거리)
	double dDistStart;     // 선분 시작점(A)으로부터의 누적 거리
	double dx0, dy0, dz0;  // 교차 좌표 (요소 내부 시작점)
	double dt0;            // 선분 파라미터(0~1) : A~B 구간에서의 위치 비율
	double dPositionI;     // 전체 시작점에서의 거리

	// 요소 내 끝점 정보
	int nEdgeEnd;          // 끝점이 위치한 Edge 번호 (1~4, 0이면 내부 시작)
	double dEdgeEnd;   	   // Edge의 시작점으로부터의 거리 (Edge 방향 거리)
	double dDistEnd;       // 시작점(A)으로부터의 누적 거리
	double dx1, dy1, dz1;  // 교차 좌표 (요소 내부 시작점)
	double dt1;		       // 선분 파라미터(0~1) : A~B 구간에서의 위치 비율
	double dPositionJ;     // 전체 시작점에서의 거리
};

struct StripTendon
{
	int nTendonId;                   // Tendon번호 (ID)
	int nElemId;                     // TendonElem 번호 (ID)
	T_NODE_D Cross;                  // Strip 요소와 Tendon의 교차점
	double dCrossRatio;              // Strip 요소 내 교차점 위치 비율 (0-1) 
	double dDistY;                   // Tendon 과 Strip-section의 중심점과의 수평 거리(부호 포함)
	double dDistZ;
};

struct StripVsect
{
    int nSectId;                      // 번호 (ID)
	int nSpanID;                      // Design Strip 내 Span ID
	T_NODE_D CenterNode;              // 시작 A-B 절점기준의 중심 좌표
	T_NODE_D CenterL;                 // CenterNode의 left
	T_NODE_D CenterR;                 // CenterNode의 right
	T_NODE_D Centroid;                // 가상 단면 도심
    T_NODE_D SectVec;                 // 단면에 수직인 vector
	T_NODE_D LineVec;                 // 단면 Line 방향 vector (CenterL -> CenterR)
	T_SECT_STIFFNESS  Stiffness;      // 단면 강성             
	double dLength;                   // 단면 길이 (Line 방향)
	std::vector<ElementPass> vecPasses;  // 단면 통과하는 요소별 정보
	std::vector<StripTendon> vecTendons; // 단면 통과하는 Tendon별 정보
    
    void Initialize()
    {
        nSectId = 0;
        nSpanID = 0;
        CenterNode.Initialize();
        CenterL.Initialize();
        CenterR.Initialize();
        Centroid.Initialize();
        SectVec.Initialize();
        LineVec.Initialize();
        Stiffness.Initialize();
        dLength = 0.0;
        vecPasses.clear();
        vecTendons.clear();
	}
};
//////////////////////////////////////////////////////////////////////////

// Set Tendon Profile
#define T_TDPF_K unsigned int
typedef CArray<T_TDPF_K, T_TDPF_K> T_TDPF_K_LIST;

struct T_TDPF_INFO
{
    CString strBoundaryCondition;
    int     nType;      // e.g. Parabolic, Straight
    double  dYa;   // length
    double  dYb;   // length
    double  dYc;   // length
    double  dYd;   // length
    double  dXa;   // ratio
    double  dXb;   // ratio 
    double  dXc;   // ratio
    double  dXd;   // ratio

    T_TDPF_INFO() { Initialize(); }

    void Initialize()
    {
        strBoundaryCondition.Empty();
        nType = 0;
        dYa = 0.0;
        dYb = 0.0;
        dYc = 0.0;
        dYd = 0.0;
        dXa = 0.0;
        dXb = 0.0;
        dXc = 0.0;
        dXd = 0.0;
    }

    BOOL operator==(const T_TDPF_INFO& src) const
    {
        if (nType != src.nType) return FALSE;
        if (strBoundaryCondition != src.strBoundaryCondition) return FALSE;
        if (dgn::UQ(dYa, src.dYa)) return FALSE;
        if (dgn::UQ(dYb, src.dYb)) return FALSE;
        if (dgn::UQ(dYc, src.dYc)) return FALSE;
        if (dgn::UQ(dYd, src.dYd)) return FALSE;
        if (dgn::UQ(dXa, src.dXa)) return FALSE;
        if (dgn::UQ(dXb, src.dXb)) return FALSE;
        if (dgn::UQ(dXc, src.dXc)) return FALSE;
        if (dgn::UQ(dXd, src.dXd)) return FALSE;
        return TRUE;
    }
};

struct T_TDPF_INFO_CH
{
    char    strBoundaryCondition[80];
    int     nType;
    double  dYa;
    double  dYb;
    double  dYc;
    double  dYd;
    double  dXa;
    double  dXb;
    double  dXc;
    double  dXd;

    void ConvertToString(T_TDPF_INFO& rData)
    {
        ConvertCharStr(strBoundaryCondition, rData.strBoundaryCondition, sizeof(strBoundaryCondition));
        rData.nType = nType;
        rData.dYa = dYa;
        rData.dYb = dYb;
        rData.dYc = dYc;
        rData.dYd = dYd;
        rData.dXa = dXa;
        rData.dXb = dXb;
        rData.dXc = dXc;
        rData.dXd = dXd;
    }

    void ConvertToChar(T_TDPF_INFO& rData)
    {
        ConvertStrChar(rData.strBoundaryCondition, strBoundaryCondition, sizeof(strBoundaryCondition));
        nType = rData.nType;
        dYa = rData.dYa;
        dYb = rData.dYb;
        dYc = rData.dYc;
        dYd = rData.dYd;
        dXa = rData.dXa;
        dXb = rData.dXb;
        dXc = rData.dXc;
        dXd = rData.dXd;
    }
};

typedef CArray<T_TDPF_INFO, T_TDPF_INFO&> T_TDPF_INFO_LIST;

struct T_TDPF_D
{
    CString strName;
    int     nElemType; // 0:Beam, 1:Slab, 2:Wall
    T_TDPF_INFO_LIST aInfo;

    void Initialize()
    {
        strName.Empty();
        nElemType = 0;
        aInfo.RemoveAll();
    }
    T_TDPF_D() { Initialize(); }
    T_TDPF_D(T_TDPF_D& src) { *this = src; }
    BOOL operator==(const T_TDPF_D& src) const
    {
        if (strName != src.strName) return FALSE;
        if (nElemType != src.nElemType) return FALSE;
        if (aInfo.GetSize() != src.aInfo.GetSize()) return FALSE;

        for (int i = 0; i < aInfo.GetSize(); i++)
        {
            if (!(aInfo[i] == src.aInfo[i])) return FALSE;
        }
        return TRUE;
    }
    T_TDPF_D& operator=(const T_TDPF_D& src)
    {
        strName = src.strName;
        nElemType = src.nElemType;
        aInfo.Copy(src.aInfo);
        return *this;
    }
};

struct T_TDPF_D_CH
{
    char strName[20];
    int nElemType;
    CArray<T_TDPF_INFO_CH, T_TDPF_INFO_CH&> aInfo;

    void ConvertToString(T_TDPF_D& rData)
    {
        ConvertCharStr(strName, rData.strName, sizeof(strName));
        rData.nElemType = nElemType;

        rData.aInfo.SetSize(aInfo.GetSize());
        for (int i = 0; i < aInfo.GetSize(); i++)
        {
            aInfo[i].ConvertToString(rData.aInfo[i]);
        }
    }

    void ConvertToChar(T_TDPF_D& rData)
    {
        ConvertStrChar(rData.strName, strName, sizeof(strName));
        nElemType = rData.nElemType;

        aInfo.SetSize(rData.aInfo.GetSize());
        for (int i = 0; i < rData.aInfo.GetSize(); i++)
        {
            aInfo[i].ConvertToChar(rData.aInfo[i]);
        }
    }
};

struct T_TDPF_UDRD_D
{
    T_TDPF_K key;
    T_TDPF_D data;

    void GetTdpf(T_TDPF_K& rKey, T_TDPF_D& rData)
    {
        rKey = key;
        rData = data;
    }
    void SetTdpf(const T_TDPF_K Key, const T_TDPF_D& rData)
    {
        key = Key;
        data = rData;
    }
};

struct T_TDPF_UDRD_D_CH
{
    T_TDPF_K key;
    T_TDPF_D_CH data;
};

struct T_TDPF_UDRD_D_CH_RW
{
    T_TDPF_K key;
    char strName[20];
    int nElemType;
    int nCountInfo;
    // CArray�� ���� - ���� ����

    void GetTdpf(T_TDPF_K& rKey, T_TDPF_D_CH& rData)
    {
        rKey = key;
        memcpy(rData.strName, strName, sizeof(strName));
        rData.nElemType = nElemType;
        // aInfo�� ���� ó��
    }
    void SetTdpf(const T_TDPF_K Key, const T_TDPF_D_CH& rData)
    {
        key = Key;
        memcpy(strName, rData.strName, sizeof(strName));
        nElemType = rData.nElemType;
        // aInfo�� ���� ó��
        nCountInfo = (int)rData.aInfo.GetSize();
    }
};

struct T_TDPF_UNIT
{
    T_TDPF_UNIT()
    {
        strName = D_UNITSYS_NONE;
        nElemType = D_UNITSYS_NONE;
    }
    int strName;
    int nElemType;
};

#define HASHSIZETDPF 1001

// Tendon Stressing
#define T_TDST_K unsigned int
typedef CArray<T_TDST_K, T_TDST_K> T_TDST_K_LIST;

struct T_TDST_D
{
    CString strName;
    BOOL    bApplySame;
    int     nFirstMethod;
    double  dFirstJacking;
    double  dFirstTransfer;
    double  dFirstService;
    double  dFirstJackingRatio;
    double  dFirstTransferRatio;
    double  dFirstServiceRatio;
    BOOL    bFirstAuto;
    int     nLastMethod;
    double  dLastJacking;
    double  dLastTransfer;
    double  dLastService;
    double  dLastJackingRatio;
    double  dLastTransferRatio;
    double  dLastServiceRatio;
    BOOL    bLastAuto;
    double  dDLBalancingRatioMax;
    double  dDLBalancingRatioMin;
    double  dPreCompLevelMax;
    double  dPreCompLevelMin;

    void Initialize()
    {
        strName.Empty();
        bApplySame = false;
        nFirstMethod = 0;
        dFirstJacking = 0.0;
        dFirstTransfer = 0.0;
        dFirstService = 0.0;
        dFirstJackingRatio = 0.0;
        dFirstTransferRatio = 0.0;
        dFirstServiceRatio = 0.0;
        bFirstAuto = false;
        nLastMethod = 0;
        dLastJacking = 0.0;
        dLastTransfer = 0.0;
        dLastService = 0.0;
        dLastJackingRatio = 0.0;
        dLastTransferRatio = 0.0;
        dLastServiceRatio = 0.0;
        bLastAuto = false;
        dDLBalancingRatioMax = 0.0;
        dDLBalancingRatioMin = 0.0;
        dPreCompLevelMax = 0.0;
        dPreCompLevelMin = 0.0;
    }
    T_TDST_D() { Initialize(); }
    T_TDST_D(T_TDST_D& src) { *this = src; }
    BOOL operator==(const T_TDST_D& src) const
    {
        if (strName != src.strName) return FALSE;
        if (bApplySame != src.bApplySame) return FALSE;
        if (nFirstMethod != src.nFirstMethod) return FALSE;
        if (dgn::EQ(dFirstJacking, src.dFirstJacking)) return FALSE;
        if (dgn::EQ(dFirstTransfer, src.dFirstTransfer)) return FALSE;
        if (dgn::EQ(dFirstService, src.dFirstService)) return FALSE;
        if (dgn::EQ(dFirstJackingRatio, src.dFirstJackingRatio)) return FALSE;
        if (dgn::EQ(dFirstTransferRatio, src.dFirstTransferRatio)) return FALSE;
        if (dgn::EQ(dFirstServiceRatio, src.dFirstServiceRatio)) return FALSE;
        if (bFirstAuto != src.bFirstAuto) return FALSE;
        if (nLastMethod != src.nLastMethod) return FALSE;
        if (dgn::EQ(dLastJacking, src.dLastJacking)) return FALSE;
        if (dgn::EQ(dLastTransfer, src.dLastTransfer)) return FALSE;
        if (dgn::EQ(dLastService, src.dLastService)) return FALSE;
        if (dgn::EQ(dLastJackingRatio, src.dLastJackingRatio)) return FALSE;
        if (dgn::EQ(dLastTransferRatio, src.dLastTransferRatio)) return FALSE;
        if (dgn::EQ(dLastServiceRatio, src.dLastServiceRatio)) return FALSE;
        if (bLastAuto != src.bLastAuto) return FALSE;
        if (dgn::EQ(dDLBalancingRatioMax, src.dDLBalancingRatioMax)) return FALSE;
        if (dgn::EQ(dDLBalancingRatioMin, src.dDLBalancingRatioMin)) return FALSE;
        if (dgn::EQ(dPreCompLevelMax, src.dPreCompLevelMax)) return FALSE;
        if (dgn::EQ(dPreCompLevelMin, src.dPreCompLevelMin)) return FALSE;
        return TRUE;
    }
    T_TDST_D& operator=(const T_TDST_D& src)
    {
        strName = src.strName;
        bApplySame = src.bApplySame;
        nFirstMethod = src.nFirstMethod;
        dFirstJacking = src.dFirstJacking;
        dFirstTransfer = src.dFirstTransfer;
        dFirstService = src.dFirstService;
        dFirstJackingRatio = src.dFirstJackingRatio;
        dFirstTransferRatio = src.dFirstTransferRatio;
        dFirstServiceRatio = src.dFirstServiceRatio;
        bFirstAuto = src.bFirstAuto;
        nLastMethod = src.nLastMethod;
        dLastJacking = src.dLastJacking;
        dLastTransfer = src.dLastTransfer;
        dLastService = src.dLastService;
        dLastJackingRatio = src.dLastJackingRatio;
        dLastTransferRatio = src.dLastTransferRatio;
        dLastServiceRatio = src.dLastServiceRatio;
        bLastAuto = src.bLastAuto;
        dDLBalancingRatioMax = src.dDLBalancingRatioMax;
        dDLBalancingRatioMin = src.dDLBalancingRatioMin;
        dPreCompLevelMax = src.dPreCompLevelMax;
        dPreCompLevelMin = src.dPreCompLevelMin;
        return *this;
    }
};

struct T_TDST_D_CH
{
    char strName[20];
    BOOL bApplySame;
    int  nFirstMethod;
    double dFirstJacking;
    double dFirstTransfer;
    double dFirstService;
    double dFirstJackingRatio;
    double dFirstTransferRatio;
    double dFirstServiceRatio;
    BOOL bFirstAuto;
    int  nLastMethod;
    double dLastJacking;
    double dLastTransfer;
    double dLastService;
    double dLastJackingRatio;
    double dLastTransferRatio;
    double dLastServiceRatio;
    BOOL bLastAuto;
    double dDLBalancingRatioMax;
    double dDLBalancingRatioMin;
    double dPreCompLevelMax;
    double dPreCompLevelMin;

    void ConvertToString(T_TDST_D& rData)
    {
        ConvertCharStr(strName, rData.strName, sizeof(strName));
        rData.bApplySame = bApplySame;
        rData.nFirstMethod = nFirstMethod;
        rData.dFirstJacking = dFirstJacking;
        rData.dFirstTransfer = dFirstTransfer;
        rData.dFirstService = dFirstService;
        rData.dFirstJackingRatio = dFirstJackingRatio;
        rData.dFirstTransferRatio = dFirstTransferRatio;
        rData.dFirstServiceRatio = dFirstServiceRatio;
        rData.bFirstAuto = bFirstAuto;
        rData.nLastMethod = nLastMethod;
        rData.dLastJacking = dLastJacking;
        rData.dLastTransfer = dLastTransfer;
        rData.dLastService = dLastService;
        rData.dLastJackingRatio = dLastJackingRatio;
        rData.dLastTransferRatio = dLastTransferRatio;
        rData.dLastServiceRatio = dLastServiceRatio;
        rData.bLastAuto = bLastAuto;
        rData.dDLBalancingRatioMax = dDLBalancingRatioMax;
        rData.dDLBalancingRatioMin = dDLBalancingRatioMin;
        rData.dPreCompLevelMax = dPreCompLevelMax;
        rData.dPreCompLevelMin = dPreCompLevelMin;
    }
    void ConvertToChar(T_TDST_D& rData)
    {
        ConvertStrChar(rData.strName, strName, sizeof(strName));
        bApplySame = rData.bApplySame;
        nFirstMethod = rData.nFirstMethod;
        dFirstJacking = rData.dFirstJacking;
        dFirstTransfer = rData.dFirstTransfer;
        dFirstService = rData.dFirstService;
        dFirstJackingRatio = rData.dFirstJackingRatio;
        dFirstTransferRatio = rData.dFirstTransferRatio;
        dFirstServiceRatio = rData.dFirstServiceRatio;
        bFirstAuto = rData.bFirstAuto;
        nLastMethod = rData.nLastMethod;
        dLastJacking = rData.dLastJacking;
        dLastTransfer = rData.dLastTransfer;
        dLastService = rData.dLastService;
        dLastJackingRatio = rData.dLastJackingRatio;
        dLastTransferRatio = rData.dLastTransferRatio;
        dLastServiceRatio = rData.dLastServiceRatio;
        bLastAuto = rData.bLastAuto;
        dDLBalancingRatioMax = rData.dDLBalancingRatioMax;
        dDLBalancingRatioMin = rData.dDLBalancingRatioMin;
        dPreCompLevelMax = rData.dPreCompLevelMax;
        dPreCompLevelMin = rData.dPreCompLevelMin;
    }
};

struct T_TDST_UDRD_D
{
    T_TDST_K key;
    T_TDST_D data;

    void GetTdst(T_TDST_K& rKey, T_TDST_D& rData)
    {
        rKey = key;
        rData = data;
    }
    void SetTdst(const T_TDST_K Key, const T_TDST_D& rData)
    {
        key = Key;
        data = rData;
    }
};

struct T_TDST_UDRD_D_CH
{
    T_TDST_K key;
    T_TDST_D_CH data;

    void GetTdst(T_TDST_K& rKey, T_TDST_D_CH& rData)
    {
        rKey = key;
        rData = data;
    }
    void SetTdst(const T_TDST_K Key, const T_TDST_D_CH& rData)
    {
        key = Key;
        data = rData;
    }
};

struct T_TDST_UNIT
{
    T_TDST_UNIT()
    {
        strName = D_UNITSYS_NONE;
        bApplySame = D_UNITSYS_NONE;
        nFirstMethod = D_UNITSYS_NONE;
        dFirstJacking = D_UNITSYS_BASE_STRESS;
        dFirstTransfer = D_UNITSYS_BASE_STRESS;
        dFirstService = D_UNITSYS_BASE_STRESS;
        dFirstJackingRatio = D_UNITSYS_NONE;
        dFirstTransferRatio = D_UNITSYS_NONE;
        dFirstServiceRatio = D_UNITSYS_NONE;
        bFirstAuto = D_UNITSYS_NONE;
        nLastMethod = D_UNITSYS_NONE;
        dLastJacking = D_UNITSYS_BASE_STRESS;
        dLastTransfer = D_UNITSYS_BASE_STRESS;
        dLastService = D_UNITSYS_BASE_STRESS;
        dLastJackingRatio = D_UNITSYS_NONE;
        dLastTransferRatio = D_UNITSYS_NONE;
        dLastServiceRatio = D_UNITSYS_NONE;
        bLastAuto = D_UNITSYS_NONE;
        dDLBalancingRatioMax = D_UNITSYS_NONE;
        dDLBalancingRatioMin = D_UNITSYS_NONE;
        dPreCompLevelMax = D_UNITSYS_NONE;
        dPreCompLevelMin = D_UNITSYS_NONE;
    }
    int strName;
    int bApplySame;
    int nFirstMethod;
    int dFirstJacking;
    int dFirstTransfer;
    int dFirstService;
    int dFirstJackingRatio;
    int dFirstTransferRatio;
    int dFirstServiceRatio;
    int bFirstAuto;
    int nLastMethod;
    int dLastJacking;
    int dLastTransfer;
    int dLastService;
    int dLastJackingRatio;
    int dLastTransferRatio;
    int dLastServiceRatio;
    int bLastAuto;
    int dDLBalancingRatioMax;
    int dDLBalancingRatioMin;
    int dPreCompLevelMax;
    int dPreCompLevelMin;
};

#define HASHSIZETDST 1001

// Tendon (Beam)
#define T_TDBM_K unsigned int
typedef CArray<T_TDBM_K, T_TDBM_K> T_TDBM_K_LIST;

struct T_TDBM_D
{
    int nTendonNo;
    T_TDNT_K TdntK;
    T_TDPF_K TdpfK;
    T_TDST_K TdstK;
    int nEndType;
    CArray<UINT, UINT> aElemK;

    void Initialize()
    {
        nTendonNo = 0;
        TdntK = 0;
        TdpfK = 0;
        TdstK = 0;
        nEndType = 0;
        aElemK.RemoveAll();
    }
    T_TDBM_D() { Initialize(); }
    T_TDBM_D(T_TDBM_D& src) { *this = src; }
    BOOL operator==(const T_TDBM_D& src) const
    {
        if (nTendonNo != src.nTendonNo) return FALSE;
        if (TdntK != src.TdntK) return FALSE;
        if (TdpfK != src.TdpfK) return FALSE;
        if (TdstK != src.TdstK) return FALSE;
        if (nEndType != src.nEndType) return FALSE;
        if (aElemK.GetSize() != src.aElemK.GetSize()) return FALSE;
        return TRUE;
    }
    T_TDBM_D& operator=(const T_TDBM_D& src)
    {
        nTendonNo = src.nTendonNo;
        TdntK = src.TdntK;
        TdpfK = src.TdpfK;
        TdstK = src.TdstK;
        nEndType = src.nEndType;
        aElemK.Copy(src.aElemK);
        return *this;
    }
};

struct T_TDBM_D_CH
{
    int nTendonNo;
    UINT TdntK;
    UINT TdpfK;
    UINT TdstK;
    int nEndType;
    CArray<UINT, UINT> aElemK;

    void ConvertToString(T_TDBM_D& rData)
    {
        rData.nTendonNo = nTendonNo;
        rData.TdntK = TdntK;
        rData.TdpfK = TdpfK;
        rData.TdstK = TdstK;
        rData.nEndType = nEndType;
        rData.aElemK.Copy(aElemK);
    }
    void ConvertToChar(T_TDBM_D& rData)
    {
        nTendonNo = rData.nTendonNo;
        TdntK = rData.TdntK;
        TdpfK = rData.TdpfK;
        TdstK = rData.TdstK;
        nEndType = rData.nEndType;
        aElemK.Copy(rData.aElemK);
    }
};

struct T_TDBM_UDRD_D
{
    T_TDBM_K key;
    T_TDBM_D data;

    void GetTdbm(T_TDBM_K& rKey, T_TDBM_D& rData)
    {
        rKey = key;
        rData = data;
    }
    void SetTdbm(const T_TDBM_K Key, const T_TDBM_D& rData)
    {
        key = Key;
        data = rData;
    }
};

struct T_TDBM_UDRD_D_CH
{
    T_TDBM_K key;
    T_TDBM_D_CH data;
};

struct T_TDBM_UDRD_D_CH_RW
{
    T_TDBM_K key;
    int nTendonNo;
    UINT TdntK;
    UINT TdpfK;
    UINT TdstK;
    int nEndType;
    int nCountElemKey;
    // CArray aElemK - 별도 ?�??

    void GetTdbm(T_TDBM_K& rKey, T_TDBM_D& rData)
    {
        rKey = key;
        rData.nTendonNo = nTendonNo;
        rData.TdntK = TdntK;
        rData.TdpfK = TdpfK;
        rData.TdstK = TdstK;
        rData.nEndType = nEndType;
        // aElemK??별도 처리
    }
    void SetTdbm(const T_TDBM_K Key, const T_TDBM_D& rData)
    {
        key = Key;
        nTendonNo = rData.nTendonNo;
        TdntK = rData.TdntK;
        TdpfK = rData.TdpfK;
        TdstK = rData.TdstK;
        nEndType = rData.nEndType;
        // aElemK??별도 처리
        nCountElemKey = (int)rData.aElemK.GetSize();
    }
};

struct T_TDBM_UNIT
{
    T_TDBM_UNIT()
    {
        nTendonNo = D_UNITSYS_NONE;
        TdntK = D_UNITSYS_NONE;
        TdpfK = D_UNITSYS_NONE;
        TdstK = D_UNITSYS_NONE;
        nEndType = D_UNITSYS_NONE;
    }
    int nTendonNo;
    int TdntK;
    int TdpfK;
    int TdstK;
    int nEndType;
};

#define HASHSIZETDBM 1001

#define D_TDBM_NUMBERINGTYPE_SMALLEST   1
#define D_TDBM_NUMBERINGTYPE_LARGEST    2
#define D_TDBM_NUMBERINGTYPE_USERDEFINE 3
struct T_TDBM_CREATE_F
{
    UINT m_nTdbmCreate_NumberingType;
    UINT m_nTdbmCreate_UserNumber;
};


// Tendon Line (Slab) - Numbering
#define D_TDSL_NUMBERINGTYPE_SMALLEST   1
#define D_TDSL_NUMBERINGTYPE_LARGEST    2
#define D_TDSL_NUMBERINGTYPE_USERDEFINE 3
struct T_TDSL_CREATE_F
{
    UINT m_nTdslCreate_NumberingType;
    UINT m_nTdslCreate_UserNumber;
};

// Tendon (Slab)
#define T_TDSL_K unsigned int
typedef CArray<T_TDSL_K, T_TDSL_K> T_TDSL_K_LIST;

struct T_TDSL_D
{
    T_TDNT_K TdntK;
    T_TDPF_K TdpfK;
    T_TDST_K TdstK;
	int nLayoutType;  // 0:Single 1:Banded  2:Distributed
    int nDirection;
    double dBandWidth;
    int nTendonLineNo;
    BOOL bAuto;
    int nArrangement;
    double dArrSpacing;
    int nArrTendonLineNo;
    int nEndType;
    int nDrawType;
    CArray<UINT, UINT> aNodeK; // 0:Single 1:Banded 일때 
    int nDrawNodeType;
    double dOffset;
    int nMethodLine1;          // 2:Distributed 일때 
    CArray<UINT, UINT> aLine1; // 2:Distributed 일때 
    int nMethodLine2;          // 2:Distributed 일때 
	CArray<UINT, UINT> aLine2; // 2:Distributed 일때  
    int nLineType;

    void Initialize()
    {
        TdntK = 0;
        TdpfK = 0;
        TdstK = 0;
        nLayoutType = 0;
        nDirection = 0;
        dBandWidth = 0.0;
        nTendonLineNo = 0;
        bAuto = false;
        nArrangement = 0;
        dArrSpacing = 0.0;
        nArrTendonLineNo = 0;
        nEndType = 0;
        nDrawType = 0;
		aNodeK.RemoveAll();
        nDrawNodeType = 0;
        dOffset = 0.0;
        nMethodLine1 = 0;
        aLine1.RemoveAll();
        nMethodLine2 = 0;
        aLine2.RemoveAll();
        nLineType = 0;
    }
    T_TDSL_D() { Initialize(); }
    T_TDSL_D(T_TDSL_D& src) { *this = src; }
    BOOL operator==(const T_TDSL_D& src) const
    {
        if (TdntK != src.TdntK) return FALSE;
        if (TdpfK != src.TdpfK) return FALSE;
        if (TdstK != src.TdstK) return FALSE;
        if (nLayoutType != src.nLayoutType) return FALSE;
        if (nDirection != src.nDirection) return FALSE;
        if (dgn::EQ(dBandWidth, src.dBandWidth)) return FALSE;
        if (nTendonLineNo != src.nTendonLineNo) return FALSE;
        if (bAuto != src.bAuto) return FALSE;
        if (nArrangement != src.nArrangement) return FALSE;
        if (dgn::EQ(dArrSpacing, src.dArrSpacing)) return FALSE;
        if (nArrTendonLineNo != src.nArrTendonLineNo) return FALSE;
        if (nEndType != src.nEndType) return FALSE;
        if (nDrawType != src.nDrawType) return FALSE;
		if (aNodeK.GetSize() != src.aNodeK.GetSize()) return FALSE;
        for (auto i = 0; i < src.aNodeK.GetSize(); i++)
        {
            if (aNodeK[i] != src.aNodeK[i]) return FALSE;
		}
        if (nDrawNodeType != src.nDrawNodeType) return FALSE;
        if (dgn::EQ(dOffset, src.dOffset)) return FALSE;
        if (nMethodLine1 != src.nMethodLine1) return FALSE;
        if (aLine1.GetSize() != src.aLine1.GetSize()) return FALSE;
        for (auto i = 0; i < src.aLine1.GetSize(); i++)
        {
            if (aLine1[i] != src.aLine1[i]) return FALSE;
        }
        if (nMethodLine2 != src.nMethodLine2) return FALSE;
        if (aLine2.GetSize() != src.aLine2.GetSize()) return FALSE;
        for (auto i = 0; i < src.aLine2.GetSize(); i++)
        {
            if (aLine2[i] != src.aLine2[i]) return FALSE;
        }
        if (nLineType != src.nLineType) return FALSE;
        return TRUE;
    }
    T_TDSL_D& operator=(const T_TDSL_D& src)
    {
        TdntK = src.TdntK;
        TdpfK = src.TdpfK;
        TdstK = src.TdstK;
        nLayoutType = src.nLayoutType;
        nDirection = src.nDirection;
        dBandWidth = src.dBandWidth;
        nTendonLineNo = src.nTendonLineNo;
        bAuto = src.bAuto;
        nArrangement = src.nArrangement;
        dArrSpacing = src.dArrSpacing;
        nArrTendonLineNo = src.nArrTendonLineNo;
        nEndType = src.nEndType;
        nDrawType = src.nDrawType;
		aNodeK.Copy(src.aNodeK);
        nDrawNodeType = src.nDrawNodeType;
        dOffset = src.dOffset;
        nMethodLine1 = src.nMethodLine1;
        aLine1.Copy(src.aLine1);
        nMethodLine2 = src.nMethodLine2;
        aLine2.Copy(src.aLine2);
        nLineType = src.nLineType;
        return *this;
    }
};

struct T_TDSL_D_CH
{
    UINT TdntK;
    UINT TdpfK;
    UINT TdstK;
    int nLayoutType;
    int nDirection;
    double dBandWidth;
    int nTendonLineNo;
    BOOL bAuto;
    int nArrangement;
    double dArrSpacing;
    int nArrTendonLineNo;
    int nEndType;
    int nDrawType;
	CArray<UINT, UINT> aNodeK;
    int nDrawNodeType;
    double dOffset;
    int nMethodLine1;
    CArray<UINT, UINT> aLine1;
    int nMethodLine2;
    CArray<UINT, UINT> aLine2;
    int nLineType;

    void ConvertToString(T_TDSL_D& rData)
    {
        rData.TdntK = TdntK;
        rData.TdpfK = TdpfK;
        rData.TdstK = TdstK;
        rData.nLayoutType = nLayoutType;
        rData.nDirection = nDirection;
        rData.dBandWidth = dBandWidth;
        rData.nTendonLineNo = nTendonLineNo;
        rData.bAuto = bAuto;
        rData.nArrangement = nArrangement;
        rData.dArrSpacing = dArrSpacing;
        rData.nArrTendonLineNo = nArrTendonLineNo;
        rData.nEndType = nEndType;
        rData.nDrawType = nDrawType;
		rData.aNodeK.Copy(aNodeK);
        rData.nDrawNodeType = nDrawNodeType;
        rData.dOffset = dOffset;
        rData.nMethodLine1 = nMethodLine1;
        rData.aLine1.Copy(aLine1);
        rData.nMethodLine2 = nMethodLine2;
        rData.aLine2.Copy(aLine2);
        rData.nLineType = nLineType;
    }
    void ConvertToChar(T_TDSL_D& rData)
    {
        TdntK = rData.TdntK;
        TdpfK = rData.TdpfK;
        TdstK = rData.TdstK;
        nLayoutType = rData.nLayoutType;
        nDirection = rData.nDirection;
        dBandWidth = rData.dBandWidth;
        nTendonLineNo = rData.nTendonLineNo;
        bAuto = rData.bAuto;
        nArrangement = rData.nArrangement;
        dArrSpacing = rData.dArrSpacing;
        nArrTendonLineNo = rData.nArrTendonLineNo;
        nEndType = rData.nEndType;
        nDrawType = rData.nDrawType;
        aNodeK.Copy(rData.aNodeK);
        nDrawNodeType = rData.nDrawNodeType;
        dOffset = rData.dOffset;
        nMethodLine1 = rData.nMethodLine1;
        aLine1.Copy(rData.aLine1);
        nMethodLine2 = rData.nMethodLine2;
        aLine2.Copy(rData.aLine2);
        nLineType = rData.nLineType;
    }
};

struct T_TDSL_UDRD_D
{
    T_TDSL_K key;
    T_TDSL_D data;

    void GetTdsl(T_TDSL_K& rKey, T_TDSL_D& rData)
    {
        rKey = key;
        rData = data;
    }
    void SetTdsl(const T_TDSL_K Key, const T_TDSL_D& rData)
    {
        key = Key;
        data = rData;
    }
};

struct T_TDSL_UDRD_D_CH
{
    T_TDSL_K key;
    T_TDSL_D_CH data;
};

struct T_TDSL_UDRD_D_CH_RW
{
    T_TDSL_K key;
    UINT TdntK;
    UINT TdpfK;
    UINT TdstK;
    int nLayoutType;
    int nDirection;
    int dBandWidth;
    int nTendonLineNo;
    int bAuto;
    int nArrangement;
    int dArrSpacing;
    int nArrTendonLineNo;
    int nEndType;
    int nDrawType;
    int nCountNodeK;
    // CArray aNodeK는 별도 처리
    int nDrawNodeType;
    int dOffset;
    int nMethodLine1;
    int nCountLine1;
    // CArray aLine1은 별도 처리
    int nMethodLine2;
    int nCountLine2;
    // CArray aLine2는 별도 처리
    int nLineType;

    void GetTdsl(T_TDSL_K& rKey, T_TDSL_D& rData)
    {
        rKey = key;
        rData.TdntK = TdntK;
        rData.TdpfK = TdpfK;
        rData.TdstK = TdstK;
        rData.nLayoutType = nLayoutType;
        rData.nDirection = nDirection;
        rData.dBandWidth = dBandWidth;
        rData.nTendonLineNo = nTendonLineNo;
        rData.bAuto = bAuto;
        rData.nArrangement = nArrangement;
        rData.dArrSpacing = dArrSpacing;
        rData.nArrTendonLineNo = nArrTendonLineNo;
        rData.nEndType = nEndType;
        rData.nDrawType = nDrawType;
        // aNodeK는 별도 처리
        rData.nDrawNodeType = nDrawNodeType;
        rData.dOffset = dOffset;
        rData.nMethodLine1 = nMethodLine1;
        // aLine1은 별도 처리
        rData.nMethodLine2 = nMethodLine2;
        // aLine2는 별도 처리
        rData.nLineType = nLineType;
    }
    void SetTdsl(const T_TDSL_K Key, const T_TDSL_D& rData)
    {
        key = Key;
        TdntK = rData.TdntK;
        TdpfK = rData.TdpfK;
        TdstK = rData.TdstK;
        nLayoutType = rData.nLayoutType;
        nDirection = rData.nDirection;
        dBandWidth = (int)rData.dBandWidth;
        nTendonLineNo = rData.nTendonLineNo;
        bAuto = rData.bAuto;
        nArrangement = rData.nArrangement;
        dArrSpacing = (int)rData.dArrSpacing;
        nArrTendonLineNo = rData.nArrTendonLineNo;
        nEndType = rData.nEndType;
        nDrawType = rData.nDrawType;
        // aNodeK는 별도 처리
        nCountNodeK = (int)rData.aNodeK.GetSize();
        nDrawNodeType = rData.nDrawNodeType;
        dOffset = (int)rData.dOffset;
        nMethodLine1 = rData.nMethodLine1;
        // aLine1은 별도 처리
        nCountLine1 = (int)rData.aLine1.GetSize();
        nMethodLine2 = rData.nMethodLine2;
        // aLine2는 별도 처리
        nCountLine2 = (int)rData.aLine2.GetSize();
        nLineType = rData.nLineType;
	}
};

struct T_TDSL_UNIT
{
    T_TDSL_UNIT()
    {
        TdntK = D_UNITSYS_NONE;
        TdpfK = D_UNITSYS_NONE;
        TdstK = D_UNITSYS_NONE;
        nLayoutType = D_UNITSYS_NONE;
        nDirection = D_UNITSYS_NONE;
        dBandWidth = D_UNITSYS_BASE_LENGTH;
        nTendonLineNo = D_UNITSYS_NONE;
        bAuto = D_UNITSYS_NONE;
        nArrangement = D_UNITSYS_NONE;
        dArrSpacing = D_UNITSYS_BASE_LENGTH;
        nArrTendonLineNo = D_UNITSYS_NONE;
        nEndType = D_UNITSYS_NONE;
        nDrawType = D_UNITSYS_NONE;
		aNodeK = D_UNITSYS_NONE;
        nDrawNodeType = D_UNITSYS_NONE;
        dOffset = D_UNITSYS_BASE_LENGTH;
        nMethodLine1 = D_UNITSYS_NONE;
        aLine1 = D_UNITSYS_NONE;
        nMethodLine2 = D_UNITSYS_NONE;
        aLine2 = D_UNITSYS_NONE;
        nLineType = D_UNITSYS_NONE;
    }
    int TdntK;
    int TdpfK;
    int TdstK;
    int nLayoutType;
    int nDirection;
    int dBandWidth;
    int nTendonLineNo;
    int bAuto;
    int nArrangement;
    int dArrSpacing;
    int nArrTendonLineNo;
    int nEndType;
    int nDrawType;
	int aNodeK;
    int nDrawNodeType;
    int dOffset;
    int nMethodLine1;
    int aLine1;
    int nMethodLine2;
    int aLine2;
    int nLineType;
};

#define HASHSIZETDSL 1001

// Post Tension Tendon
#define T_PTTN_K unsigned int
typedef CArray<T_PTTN_K, T_PTTN_K> T_PTTN_K_LIST;

struct T_PTTN_Base  // Span 별 데이터
{
    T_TDPF_INFO Info;

	T_NODE_D Data1; // 시점 위치
	T_NODE_D Data2; // 종점 위치
    T_NODE_K Node1; // 시점 Node (span 지점부 정보를 갖는 절점, Data1과 일치하지 않을수 있음)
    T_NODE_K Node2; // 종점 Node (span 지점부 정보를 갖는 절점, Data2과 일치하지 않을수 있음)

	void Initialize()
	{
        Info.Initialize();
		Data1.Initialize();
		Data2.Initialize();
		Node1 = 0;
		Node2 = 0;
	}
    T_PTTN_Base() { Initialize(); }
    T_PTTN_Base(T_PTTN_Base& src) { *this = src; }
    T_PTTN_Base& operator=(const T_PTTN_Base& src)
	{
        Info  = src.Info;
		Data1 = src.Data1;
        Data2 = src.Data2;
        Node1 = src.Node1;
        Node2 = src.Node2;
		return *this;
	}
	BOOL operator==(const T_PTTN_Base& src) const
	{
		if (!(Info  == src.Info))   return FALSE;
		if (!(Data1 == src.Data1))  return FALSE;
		if (!(Data2 == src.Data2))  return FALSE;
		if (Node1   != src.Node1)   return FALSE;
		if (Node2   != src.Node2)   return FALSE;

		return TRUE;
	}
};

struct T_PTTN_Base_CH
{
    T_TDPF_INFO_CH  Info;
	double  dx1  ;
	double  dy1  ;
	double  dz1  ;
	double  dx2  ;
	double  dy2  ;
	double  dz2  ;
	int     Node1;
    int     Node2;

	void ConvertToString(T_PTTN_Base& rData)
	{ 
        Info.ConvertToString(rData.Info);

        rData.Data1.x = dx1;
        rData.Data1.y = dy1;
        rData.Data1.z = dz1;
        rData.Data2.x = dx2;
        rData.Data2.y = dy2;
        rData.Data2.z = dz2;
        rData.Node1   = Node1;
        rData.Node2   = Node2;
	}

	void ConvertToChar(T_PTTN_Base& rData)
	{
        Info.ConvertToChar(rData.Info);

        dx1 = rData.Data1.x;
        dy1 = rData.Data1.y;
        dz1 = rData.Data1.z;
        dx2 = rData.Data2.x;
        dy2 = rData.Data2.y;
        dz2 = rData.Data2.z;
        Node1 = rData.Node1;
        Node2 = rData.Node2;
	}
};

//typedef CArray<T_TDPF_INFO, T_TDPF_INFO&> T_TDPF_INFO_LIST;
typedef CArray<T_PTTN_Base, T_PTTN_Base&> T_PTTN_Base_LIST;
struct T_PTTN_D
{
    CString strName;
    T_TDNT_K TdntK;
    T_TDPF_K TdpfK;
    T_TDST_K TdstK;
    BOOL bUser;
	int nEndType;           // 0 Live-Fized 1:Live-Live 2:Fixed-Fixed
    int nTendonLineNo;
    T_PTTN_Base_LIST aInfo; // Span 별
    CArray<double, double> aX;
    CArray<double, double> aY;
    CArray<int, int> aIdx;  // Span Index =0,1..

    void Initialize()
    {
        strName.Empty();
        TdntK = 0;
        TdpfK = 0;
        TdstK = 0;
        nEndType = 0;
        bUser = FALSE;
        nTendonLineNo = 0;
        aInfo.RemoveAll();
        aX.RemoveAll();
        aY.RemoveAll();
        aIdx.RemoveAll();
    }
    T_PTTN_D() { Initialize(); }
    T_PTTN_D(T_PTTN_D& src) { *this = src; }
    T_PTTN_D& operator=(const T_PTTN_D& src)
    {
        strName = src.strName;
        TdntK = src.TdntK;
        TdpfK = src.TdpfK;
        TdstK = src.TdstK;
		nEndType = src.nEndType;
        bUser = src.bUser;
        nTendonLineNo = src.nTendonLineNo;
        aInfo.Copy(src.aInfo);
        aX.Copy(src.aX);
        aY.Copy(src.aY);
        aIdx.Copy(src.aIdx);
        return *this;
    }
};

struct T_PTTN_UNIT
{
    T_PTTN_UNIT()
    {
        dYa = D_UNITSYS_BASE_LENGTH;
        dYb = D_UNITSYS_BASE_LENGTH;
        dYc = D_UNITSYS_BASE_LENGTH;
        dYd = D_UNITSYS_BASE_LENGTH;
        bUser = D_UNITSYS_NONE;
    }
    int dYa;
    int dYb;
    int dYc;
    int dYd;
    int bUser;
};

struct T_PTTN_D_CH
{
    char strName[80];
    T_TDNT_K TdntK;
    T_TDPF_K TdpfK;
    T_TDST_K TdstK;
    BOOL bUser;
    int nEndType;
    int nTendonLineNo;
    CArray<T_PTTN_Base_CH, T_PTTN_Base_CH&> aInfo;
    CArray<double, double> aX;
    CArray<double, double> aY;
    CArray<int, int> aIdx;

    void ConvertToString(T_PTTN_D& rData)
    {
        ConvertCharStr(strName, rData.strName, sizeof(strName));
        rData.TdntK = TdntK;
        rData.TdpfK = TdpfK;
        rData.TdstK = TdstK;
        rData.bUser = bUser;
        rData.nEndType = nEndType;
        rData.nTendonLineNo = nTendonLineNo;

        rData.aInfo.SetSize(aInfo.GetSize());
        for (int i = 0; i < aInfo.GetSize(); i++)
        {
            aInfo[i].ConvertToString(rData.aInfo[i]);
        }
        rData.aX.Copy(aX);
        rData.aY.Copy(aY);
        rData.aIdx.Copy(aIdx);
    }

    void ConvertToChar(T_PTTN_D& rData)
    {
        ConvertStrChar(rData.strName, strName, sizeof(strName));
        TdntK = rData.TdntK;
        TdpfK = rData.TdpfK;
        TdstK = rData.TdstK;
        bUser = rData.bUser;
        nEndType = rData.nEndType;
        nTendonLineNo = rData.nTendonLineNo;

        aInfo.SetSize(rData.aInfo.GetSize());
        for (int i = 0; i < rData.aInfo.GetSize(); i++)
        {
            aInfo[i].ConvertToChar(rData.aInfo[i]);
        }
        aX.Copy(rData.aX);
        aY.Copy(rData.aY);
        aIdx.Copy(rData.aIdx);
    }
};

struct T_PTTN_UDRD_D
{
    T_PTTN_K key;
    T_PTTN_D data;

    void GetPttn(T_PTTN_K& rKey, T_PTTN_D& rData)
    {
        rKey = key;
        rData = data;
    }
    void SetPttn(const T_PTTN_K Key, const T_PTTN_D& rData)
    {
        key = Key;
        data = rData;
    }
};

struct T_PTTN_UDRD_D_CH
{
    T_PTTN_K key;
    T_PTTN_D_CH data;
};

struct T_PTTN_UDRD_D_CH_RW
{
    T_PTTN_K key;
    char strName[80];
    T_TDNT_K TdntK;
    T_TDPF_K TdpfK;
    T_TDST_K TdstK;
    BOOL bUser;
    int nEndType;
    int nTendonLineNo;
    int nCountInfo;
    int nCountX;
    int nCountY;
    int nCountIdx;

    void GetPttn(T_PTTN_K& rKey, T_PTTN_D_CH& rData)
    {
        rKey = key;
        memcpy(rData.strName, strName, sizeof(strName));
        rData.TdntK = TdntK;
        rData.TdpfK = TdpfK;
        rData.TdstK = TdstK;
        rData.bUser = bUser;
		rData.nEndType = nEndType;
        rData.nTendonLineNo = nTendonLineNo;
        // aInfo, aX, aY, aIdx는 별도 처리
    }
    void SetPttn(const T_PTTN_K Key, const T_PTTN_D_CH& rData)
    {
        key = Key;
        memcpy(strName, rData.strName, sizeof(strName));
        TdntK = rData.TdntK;
        TdpfK = rData.TdpfK;
        TdstK = rData.TdstK;
        bUser = rData.bUser;
        nEndType = rData.nEndType;
        nTendonLineNo = rData.nTendonLineNo;
        nCountInfo = (int)rData.aInfo.GetSize();
        nCountX = (int)rData.aX.GetSize();
        nCountY = (int)rData.aY.GetSize();
        nCountIdx = (int)rData.aIdx.GetSize();
    }
};

#define HASHSIZEPTTN 1001

// Properties of Effective Beam Assign(��ȿ�� ��������)
#define T_PEBA_K unsigned int // Elem key
typedef CArray<T_PEBA_K, T_PEBA_K> T_PEBA_K_LIST;

struct T_PEBA_D
{
    T_PREB_K PrebK;

    void Initialize()  {  PrebK = 0;  }
    T_PEBA_D() { Initialize(); }
};

struct T_PEBA_UDRD_D
{
    T_PEBA_K key;
    T_PEBA_D data;
};

#define HASHSIZEPEBA 1001

// Design Section Highlight Info (for View display)
struct T_DSPS_HIGHLIGHT_INFO
{
	T_DSTR_K DstrK;      // Selected Design Strip Key
	int nSpanIdx;        // Selected Span Index (0-based, -1 if not selected)
	T_DSPS_K DspsK;      // Selected Design Section Key (0 if not selected)

	void Initialize()
	{
		DstrK = 0;
		nSpanIdx = -1;
		DspsK = 0;
	}

	T_DSPS_HIGHLIGHT_INFO() { Initialize(); }
	T_DSPS_HIGHLIGHT_INFO(const T_DSPS_HIGHLIGHT_INFO& src) { *this = src; }
	T_DSPS_HIGHLIGHT_INFO& operator=(const T_DSPS_HIGHLIGHT_INFO& src)
	{
		DstrK = src.DstrK;
		nSpanIdx = src.nSpanIdx;
		DspsK = src.DspsK;
		return *this;
	}
};

// Design Analysis Option(�ٴ��� �÷���Ʈ ���迡�� ������� ������ �� ���)
#define T_DAOP_K unsigned int
typedef CArray<T_DAOP_K, T_DAOP_K> T_DAOP_K_LIST;

struct T_DAOP_D
{
    BOOL bConsiderStiff; // Consider the Stiffness of Beam or Plate
    int	 nStiffType    ; // 0:EBWM, 1:FEM

    void Initialize()
    {
        bConsiderStiff = FALSE;
        nStiffType     = 0;
    }
    T_DAOP_D() { Initialize(); }
};

struct T_DAOP_UDRD_D
{
    T_DAOP_K key;
    T_DAOP_D data;
};

#define HASHSIZEDAOP 101

// Slab Design Control(������ ���� ����� ��� ��� ���� �ɼ�)
#define T_SLDC_K unsigned int
typedef CArray<T_SLDC_K, T_SLDC_K> T_SLDC_K_LIST;

struct T_SLDC_D
{
    BOOL bConsiderMxy;
    T_SLDC_D():bConsiderMxy(TRUE) {}
};

struct T_SLDC_UDRD_D
{
    T_SLDC_K key;
    T_SLDC_D data;
};

#define HASHSIZESLDC 101

// Column Location
#define T_COLC_K unsigned int  // T_ELEM_K
typedef CArray<T_COLC_K, T_COLC_K> T_COLC_K_LIST;

struct T_COLC_D
{
    int      nColumnLocation; // 0:Interior, 1:Edge, 2:Corner
    int      nCriSectDir[2];  // Direction of Critical Section. 0:+y, 1:-y, 2:+z, 3:-z
    double   dCriSectDis[2];  // Distance of Critical Section

    T_COLC_D() { Initialize(); }

    void Initialize()
    {
        nColumnLocation = 0;
        nCriSectDir[0] = 0;  // +y
        nCriSectDir[1] = 2;  // +z
        dCriSectDis[0] = 0.;
        dCriSectDis[1] = 0.;
    }
    BOOL operator==(const T_COLC_D& src) const
    {
        if(nColumnLocation != src.nColumnLocation) return FALSE;
        for(int i=0; i<2; i++)
        {
            if(nCriSectDir[i] != src.nCriSectDir[i]) return FALSE;
            if(dCriSectDis[i] != src.dCriSectDis[i]) return FALSE;
        }

        return TRUE;
    }
};
struct T_COLC_UDRD_D
{
    T_COLC_K key;
    T_COLC_D data;

    void GetColc(T_COLC_K& rKey, T_COLC_D& rData)
    {
        rKey=key;
        rData.nColumnLocation = data.nColumnLocation;
        for(int i=0; i<2; i++)
        {
            rData.nCriSectDir[i] = data.nCriSectDir[i];
            rData.dCriSectDis[i] = data.dCriSectDis[i];
        }
    }
    void SetColc(const T_COLC_K Key, const T_COLC_D& rData)
    {
        key=Key;
        data.nColumnLocation = rData.nColumnLocation;
        for(int i=0; i<2; i++)
        {
            data.nCriSectDir[i] = rData.nCriSectDir[i];
            data.dCriSectDis[i] = rData.dCriSectDis[i];
        }
    }
};
struct T_COLC_UNIT
{
    T_COLC_UNIT()
    {
        dCriSectDis = D_UNITSYS_BASE_LENGTH;
    }
    int dCriSectDis;
};
#define HASHSIZECOLC 1001


// Slab Shear Reinforcement
#define T_SSRI_K unsigned int  // T_ELEM_K
typedef CArray<T_SSRI_K, T_SSRI_K> T_SSRI_K_LIST;

struct T_SSRI_D
{
    double dAvY;       // Av(y-dir)
    double dAvZ;       // Av(z-dir)
    double dSpace;  // Space
    double dYield;     // Yield Strength

    T_SSRI_D() { Initialize(); }

    void Initialize()
    {
        dAvY = 0.;
        dAvZ = 0.;      
        dSpace = 0.;  
        dYield = 0.;
    }
    BOOL operator==(const T_SSRI_D& src) const
    {
        if(dAvY != src.dAvY) return FALSE;
        if(dAvZ != src.dAvZ) return FALSE;
        if(dSpace != src.dSpace) return FALSE;
        if(dYield != src.dYield) return FALSE;

        return TRUE;
    }
};
struct T_SSRI_UDRD_D
{
    T_SSRI_K key;
    T_SSRI_D data;

    void GetSsri(T_SSRI_K& rKey, T_SSRI_D& rData)
    {
        rKey=key;
        rData.dAvY        = data.dAvY;
        rData.dAvZ        = data.dAvZ;
        rData.dSpace    = data.dSpace;
        rData.dYield      = data.dYield;
    }
    void SetSsri(const T_SSRI_K Key, const T_SSRI_D& rData)
    {
        key=Key;

        data.dAvY      = rData.dAvY;
        data.dAvZ      = rData.dAvZ;
        data.dSpace  = rData.dSpace;
        data.dYield     = rData.dYield;
    }
};
struct T_SSRI_UNIT
{
    T_SSRI_UNIT()
    {
        dAvY = D_UNITSYS_BASE_AREA;
        dAvZ = D_UNITSYS_BASE_AREA;
        dSpace = D_UNITSYS_BASE_LENGTH;
        dYield = D_UNITSYS_BASE_STRESS;
    }
    int dAvY;
    int dAvZ;
    int dSpace;
    int dYield;
};
#define HASHSIZESSRI 1001


#pragma pack()

#endif