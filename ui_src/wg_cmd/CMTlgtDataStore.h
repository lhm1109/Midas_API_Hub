#ifndef __CMTLGTDATASTORE_H__
#define __CMTLGTDATASTORE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_FileRW.h"

#include "..\wg_db\UserDefType.h"
#include "CMTlgtDataDef.h"
#include <vector>
#include <map>


/* TLFuncData 구조체 : 1개 축하중에 대한 삼각형 그래프 */
//  ArriveTime : T
//  AppliedLoad : L
//  Point(dArriveTime[i], dAppliedLoad[i]) : P
//
//   Load                                 AppliedLoad
//    ↑                      P1  . . . . . . . L[0]
//    │                    / . \                .
//    │                   /  .  \               .
//    │                  /   .   \              .
//    │                 /    .    \             .
//    │          leftEQ/     .     \rightEQ     .
//    │               /      .      \           .
//    │              /       .       \          .
//    │             P0       .        P2        .
//    │            T[0]    T[1]      T[2] . . . L[1], L[2]
//    └─────────────────────────────────────────────────────> Time


struct LinearEQ {
	double dSlope; // 기울기
	double dIntercept; // y절편
	double dRange[2]; // x값 범위
};

struct TLFuncData {
	double dArriveTime[3]; // 포인트3개 x값
	double dAppliedLoad[3]; // 포인트3개 y값
	LinearEQ leftEQ; // 왼쪽 직선방정식
	LinearEQ rightEQ; // 오른쪽 직선방정식
};

class CCMTlgtDataStore
{
protected:
	TLGT_INFO	m_TlGenerator; // Train Load Generator
	CDBDoc* m_pDoc;

protected:
	BOOL CompareTimeHistoryFunctionBase(T_THFC_DT& arThfcBase1, T_THFC_DT& arThfcBase2);
public:
	CCMTlgtDataStore(CDBDoc* pDoc);
	virtual ~CCMTlgtDataStore();
	void GetDefaultData(int nCode, int nType, int nVehcleNum, ARR_TLGT_ITEM& arUserItem, ARR_TLGT_ITEM& arItem);
    bool GetDataEUTrainTypeB(UINT unForceNumber, double dSpace, ARR_TLGT_ITEM& arItem) const;
	double GetNodeDist(const T_NODE_D& Node1, const T_NODE_D& Node2);
	void CalcDynLoad_GTS(TLGT_INFO& trainInfo, double dElemSize, double dArrivalTime, CArray <double, double>& arTime, CArray <double, double>& arValue, BOOL bPreview);
	void CalcDynLoad(TLGT_INFO& trainInfo, double dLeftElemSize, double dRightElemSize, double dArrivalTime, CArray <double, double>& arResTime, CArray <double, double>& arResValue, double dVelocity, BOOL bPreview);
	BOOL MakeTimeHistoryFunc(T_THFC_D& ThfcD, T_THFC_K& ThfcKey);
	BOOL MakeTimeHistoryFunc(CArray<T_THFC_D, T_THFC_D&>& aThfcD, CArray<T_THFC_K, T_THFC_K>& aThfcKey);
	void GetNewName(const CString& strPrefix, CString& newName);
	BOOL IsExistTimeHistoryFunc(CString& strName);
	void AddThfcMapItem(T_THFC_K& ThfcKey, T_THFC_D& ThfcD);

	BOOL SaveToFile(TLGT_INFO& rData);
	BOOL ReadFromFile(TLGT_INFO& rData);

private:
	std::map<CString, UINT>       m_mapNameSuffix;
	std::map<UINT, T_THFC_D>       m_mapThfc;
};

// start 900 -------
class TDataRW_TLGT_INFO_900 : public DB_TDataRW
{
public:
	TDataRW_TLGT_INFO_900() {}
	virtual ~TDataRW_TLGT_INFO_900() {}

public:
	virtual void Initialize() { m_data.Initialize(); }
	virtual BOOL IsFixedSize() { return FALSE; }
	virtual UINT GetSize() { ASSERT(0); return 0; }
	virtual BOOL Stream2Data(const void* pStream, int nMaxSize);
	virtual BOOL Data2Stream(void* pStream, int nMaxSize) { ASSERT(0); return FALSE; }

public:
	BOOL ConvertToLatest(TLGT_INFO& rData);

public:
	TLGT_INFO_900 m_data;
};



//wizard
class TDataRW_TLGT_INFO : public DB_TDataRW
{
public:
	TDataRW_TLGT_INFO() {}
	virtual ~TDataRW_TLGT_INFO() {}

public:
	virtual void Initialize() { m_data.Initialize(); }
	virtual BOOL IsFixedSize() { return FALSE; }
	virtual UINT GetSize();
	virtual BOOL Stream2Data(const void* pStream, int nMaxSize);
	virtual BOOL Data2Stream(void* pStream, int nMaxSize);

public:
	TLGT_INFO m_data;
};


class TLGT_InfoRW : public DB_CDataRW
{
public:
	TLGT_InfoRW() { m_bAdded = FALSE; }
	virtual void Init() {}
	virtual CString GetHdrName() { return "TRAIN_G"; }  // 8자 이내
	virtual CString GetDataName() { return _LS(IDS_SW_TLGT_HDR); }
	virtual UINT GetCount() { return 1; } // 변경 없음
	virtual POSITION GetStartData() { return POSITION(-1); } // 변경 없음
	virtual void GetNextData(POSITION& rPos, DB_TDataRW* pDataRW);
	virtual BOOL AddToDB(UINT nVer, DB_TDataRW* pDataRW);
	virtual DB_TDataRW* CreateData(UINT nVer);
	virtual UINT GetLatestVersion() { return D_PGM_VER(9, 5, 5, 0); }
public:
	TLGT_INFO m_data;
	BOOL m_bAdded;
};

///////////////////////////////////////////////////////////////////////
// Read or Save Wizard data
///////////////////////////////////////////////////////////////////////
#pragma pack(1)
struct TLGT_INFO_RW
{
#define ATTR(DRW) \
	D_VT_FARR_##DRW(arKeyNode, sizeof(T_NODE_K));\
	D_VT_UINT_##DRW(keyThis);\
	D_VT_STR__##DRW(strName);\
	D_VT_INT__##DRW(nVehicleCode);\
	D_VT_INT__##DRW(nVehicleType);\
	D_VT_INT__##DRW(nNumWheel);\
	D_VT_DBL__##DRW(dVelocity);\
	D_VT_INT__##DRW(nScaling);\
	D_VT_DBL__##DRW(dSacleFactor);\
	D_VT_DBL__##DRW(dMaximumValue);\
	D_VT_DBL__##DRW(dTime);\
	D_VT_INT__##DRW(nDir);\
	D_VT_INT__##DRW(nNumVehicle);\
	D_VT_FARR_##DRW(arTlgtItem, sizeof(TLGT_ITEM));
	ATTR(D)
	BOOL S2D(TLGT_INFO& rData, D_S2D_ARG_LIST) { ATTR(S2D); return TRUE; }
	BOOL D2S(TLGT_INFO& rData, D_D2S_ARG_LIST) { ATTR(D2S); return TRUE; }
#undef ATTR
};

struct TLGT_INFO_900_RW
{
#define ATTR(DRW) \
	D_VT_FARR_##DRW(arKeyNode, sizeof(T_NODE_K));\
	D_VT_UINT_##DRW(keyThis);\
	D_VT_STR__##DRW(strName);\
	D_VT_INT__##DRW(nVehicleCode);\
	D_VT_INT__##DRW(nVehicleType);\
	D_VT_INT__##DRW(nNumWheel);\
	D_VT_DBL__##DRW(dVelocity);\
	D_VT_INT__##DRW(nScaling);\
	D_VT_DBL__##DRW(dSacleFactor);\
	D_VT_DBL__##DRW(dMaximumValue);\
	D_VT_DBL__##DRW(dTime);\
	D_VT_INT__##DRW(nDir);\
	D_VT_FARR_##DRW(arTlgtItem, sizeof(TLGT_ITEM));
	ATTR(D)
	BOOL S2D(TLGT_INFO_900& rData, D_S2D_ARG_LIST) { ATTR(S2D); return TRUE; }
	BOOL D2S(TLGT_INFO_900& rData, D_D2S_ARG_LIST) { ATTR(D2S); return TRUE; }
#undef ATTR
};

struct TLGT_ITEM_RW
{
#define ATTR(DRW) \
	D_VT_DBL__##DRW(dLength);\
	D_VT_DBL__##DRW(dForce);
	ATTR(D)
	BOOL S2D(TLGT_ITEM& rData, D_S2D_ARG_LIST) { ATTR(S2D); return TRUE; }
	BOOL D2S(TLGT_ITEM& rData, D_D2S_ARG_LIST) { ATTR(D2S); return TRUE; }
#undef ATTR
};
#pragma pack()

#endif