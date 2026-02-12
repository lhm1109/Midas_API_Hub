// DB_QUERY_SEIS_CVL_JP.h: interface for the query class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DB_QUERY_SEIS_CVL_JP_H__
#define __DB_QUERY_SEIS_CVL_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeaderPre.h"

struct T_PREFERENCE;

class CDBDoc;
class CUnitCtrl;
class CDB_SIDC;
class CDB_SIFA_H29;
class CDB_SIGR;
class CDB_SIGR_USER;
class CDB_SIGR_UPPER;
class CDB_SIGR_LOWER;
class CDB_SIMD;
class CDB_SIRD;
class CDB_SIBD;
class CDB_SIDP;
class CDB_SISR;
class CDB_SISR_USER;
class CDB_SIET;
class CDB_SIPA;
class CDB_GRUP;
class CDB_ELEM;
class CDB_NODE;
class CDataMemb;

class __MY_EXT_CLASS__ QSidc
{
	T_PREFERENCE* m_pPreference;
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIDC* m_pSidc;

public:
	QSidc(CDBDoc* pDoc, CDataMemb* pDataMemb, T_PREFERENCE* pPref);
	virtual ~QSidc();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIDC_K& rKey, T_SIDC_D& rData);
	BOOL Exist(T_SIDC_K Key);
	BOOL Get(T_SIDC_D& rData);

	BOOL GetDgnSidc(T_SIDC_K Key, T_SIDC_D& rData);
};

class __MY_EXT_CLASS__ QSifaH29
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIFA_H29* m_pSifaH29;

public:
	QSifaH29(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSifaH29();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIFA_H29_K& rKey, T_SIFA_H29_D& rData);
	BOOL Exist(T_SIFA_H29_K Key);
	BOOL Get(T_SIFA_H29_D& rData);

	BOOL GetSifaH29(T_SIFA_H29_K Key, T_SIFA_H29_D& rData);

	double GetXi1(const int& nLimitType, const int& nMaterialType);
	double GetXi2(const int& nLimitType, const int& nMaterialType);
	double GetPhiS(const int& nLimitType, const int& nMaterialType);
	double GetFactorK(const int& nMaterialType);

	/**
	* nLimitType : T_SIPA_D::eLimitStateType
	* nMaterialType :T_SIGR_LOWER_D::eMaterialType
	* nRcDisp : T_SIFA_RC_Disp::eRcDisp
	*/
	void GetHoriDistFactor(const int& nLimitType, const int& nMaterialType, const int& nRcDisp, OUT double& rVal);
};

class __MY_EXT_CLASS__ QSipa
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIPA* m_pSipa;

public:
	QSipa(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSipa();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIPA_K& rKey, T_SIPA_D& rData);
	BOOL Exist(T_SIPA_K Key);
	void GetKeyList(CArray<T_SIPA_K, T_SIPA_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIPA_K Key, T_SIPA_D& rData);
	T_SIPA_K GetStartNum();
	
	T_SIPA_K GetKey(CString& rstrName);
};

class __MY_EXT_CLASS__ QSigr
{
private:
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIGR* m_pSigr;

public:
	QSigr(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSigr();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIGR_K& rKey, T_SIGR_D& rData);
	BOOL Exist(T_SIGR_K Key);
	void GetKeyList(CArray<T_SIGR_K, T_SIGR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIGR_K Key, T_SIGR_D& rData);
	T_SIGR_K GetStartNum();
	T_SIGR_K GetKey(const CString& rstrName);
};

class __MY_EXT_CLASS__ QSigrUser
{
private:
	CUnitCtrl* m_pUnitCtrl;

	CDB_SIGR*	   m_pSigr;
	CDB_SIGR_USER* m_pSigrUser;

public:
	QSigrUser(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSigrUser();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIGR_USER_K& rKey, T_SIGR_USER_D& rData);
	BOOL Exist(T_SIGR_USER_K Key);
	void GetKeyList(CArray<T_SIGR_USER_K, T_SIGR_USER_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIGR_USER_K Key, T_SIGR_USER_D& rData);

	CString GetSigrName(const T_SIGR_USER_K& key);
};

class __MY_EXT_CLASS__ QSigrUpper
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIGR* m_pSigr;
	CDB_SIGR_UPPER* m_pSigrUpper;

public:
	QSigrUpper(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSigrUpper();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIGR_UPPER_K& rKey, T_SIGR_UPPER_D& rData);
	BOOL Exist(T_SIGR_UPPER_K Key);
	void GetKeyList(CArray<T_SIGR_UPPER_K, T_SIGR_UPPER_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIGR_UPPER_K Key, T_SIGR_UPPER_D& rData);

	CString GetSigrName(const T_SIGR_UPPER_K& key);
};

struct __MY_EXT_CLASS__ ColumnData
{
	T_ELEM_K kElem;
	T_NODE_K kNodeLow;
	T_SIGR_LOWER_D::eMaterialType eMatlType;
	T_SIGR_LOWER_COLM_D::ePosType ePosNodeLow;
	T_SIGR_LOWER_COLM_D Column;

	ColumnData()
	{
		kElem = 0;
		kNodeLow = 0;
		eMatlType = T_SIGR_LOWER_D::kUnKnown;
		ePosNodeLow = T_SIGR_LOWER_COLM_D::kUnKnown;
		Column.Initialize();
	}
};

struct __MY_EXT_CLASS__ FootingData
{
	T_ELEM_K kElem;
	T_NODE_K kNodeLow;
};

class __MY_EXT_CLASS__ QSigrLower
{
	CDBDoc* m_pDoc;
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIGR* m_pSigr;
	CDB_SIGR_LOWER* m_pSigrLower;
	CDB_ELEM* m_pElem;
	CDB_NODE* m_pNode;

public:
	QSigrLower(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSigrLower();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIGR_LOWER_K& rKey, T_SIGR_LOWER_D& rData);
	BOOL Exist(T_SIGR_LOWER_K Key);
	void GetKeyList(CArray<T_SIGR_LOWER_K, T_SIGR_LOWER_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIGR_LOWER_K Key, T_SIGR_LOWER_D& rData);

	CString GetSigrName(const T_SIGR_LOWER_K& key);
 	void GetInvestigatedElem(const T_SIGR_LOWER_K& key, CArray<T_ELEM_K, T_ELEM_K>& rElements);
	void GetNodeKeysOfColumn(const T_SIGR_LOWER_K& key, CArray<T_NODE_K, T_NODE_K>& rNodeKeys);
	BOOL FindColumn(const T_SIGR_LOWER_K& key, const CString& strNameCOLM, OUT T_SIGR_LOWER_COLM_D& rColumn);
	BOOL FindColumnLowNode(const T_SIGR_LOWER_K& key, const CString& strNameCOLM, OUT ColumnData& rColumn);
	void GetColumnElemKeysFromPos(	const BOOL& fromBottom,
									const T_SIGR_LOWER_K& key,
									const T_SIGR_LOWER_COLM_D* pColumn,
									OUT T_ELEM_K_LIST& sortedElemKeys);
	T_SIGR_LOWER_BEAR_D::eBearingType GetBearingType(const T_SIGR_LOWER_K& key);
	T_SIGR_LOWER_D::eMaterialType GetMaterialType(const CString& strGroupName);

	BOOL FindFooting(const T_SIGR_LOWER_K& key, const T_NODE_K& kNodeBotOfColoum, OUT FootingData& rFooting);
};

class __MY_EXT_CLASS__ QSimd
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIMD* m_pSimd;
	CDB_SIPA* m_pSipa;
	CDB_SIGR* m_pSigr;
	CDB_SIGR_LOWER* m_pSigrLower;

public:
	QSimd(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSimd();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIMD_K& rKey, T_SIMD_D& rData);
	BOOL Exist(T_SIMD_K Key);
	void GetKeyList(CArray<T_SIMD_K, T_SIMD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIMD_K Key, T_SIMD_D& rData);
	BOOL Get(T_SIGR_K KSigr, T_SIPA_K kSpia, T_SIMD_D& rData);
	T_SIMD_K GetStartNum();

	BOOL GetInertialForceNode(const T_SIMD_K& Key, const seis_jp::eDirAnal& eDir, OUT T_NODE_K& kNode);
	void GetTimeRange(const T_SIMD_K& Key, const T_THIS_K& kThis, OUT double& rTimeStt, OUT double& rTiemEnd);
};

class __MY_EXT_CLASS__ QSird
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIRD* m_pSird;
	CDB_SIPA* m_pSipa;
	CDB_SIGR_LOWER* m_pSigrLower;

public:
	QSird(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSird();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIRD_K& rKey, T_SIRD_D& rData);
	BOOL Exist(T_SIRD_K Key);
	void GetKeyList(CArray<T_SIRD_K, T_SIRD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIRD_K Key, T_SIRD_D& rData);
	T_SIRD_K GetStartNum();

	BOOL Get(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, T_SIRD_D& rData);
	BOOL GetInertialForceNode(T_SIRD_K Key, const seis_jp::eDirAnal& eDir, OUT T_NODE_K& kNode);
	void GetTimeRange(const T_SIRD_K& Key, const T_THIS_K& kThis, OUT double& rTimeStt, OUT double& rTiemEnd);
};

class __MY_EXT_CLASS__ QSibd
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIBD* m_pSibd;
	CDB_SIGR* m_pSigr;
	CDB_SIGR_USER* m_pSigrUser;
	CDB_SIGR_LOWER* m_pSigrLower;
	CDB_SIGR_UPPER* m_pSigrUpper;

public:
	QSibd(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSibd();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIBD_K& rKey, T_SIBD_D& rData);
	BOOL Exist(T_SIBD_K Key);
	void GetKeyList(CArray<T_SIBD_K, T_SIBD_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIBD_K Key, T_SIBD_D& rData);
	T_SIBD_K GetStartNum();

	void GetElementKeys(const T_SIBD_K& Key, OUT T_ELEM_K_LIST& rElemList);
};

class __MY_EXT_CLASS__ QSidp
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIDP* m_pSidp;

public:
	QSidp(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSidp();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIDP_K& rKey, T_SIDP_D& rData);
	BOOL Exist(T_SIDP_K Key);
	void GetKeyList(CArray<T_SIDP_K, T_SIDP_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIDP_K Key, T_SIDP_D& rData);
	T_SIDP_K GetStartNum();
};

class __MY_EXT_CLASS__ QSisr
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SISR* m_pSisr;
	CDB_SIGR* m_pSigr;
	CDB_SIGR_USER* m_pSigrUser;
	CDB_SIGR_LOWER* m_pSigrLower;
	CDB_SIGR_UPPER* m_pSigrUpper;

public:
	QSisr(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSisr();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SISR_K& rKey, T_SISR_D& rData);
	BOOL Exist(T_SISR_K Key);
	void GetKeyList(CArray<T_SISR_K, T_SISR_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SISR_K Key, T_SISR_D& rData);
	T_SISR_K GetStartNum();
	T_SISR_K GetKeyEqualSipaAndSigrKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr);

	void GetElementKeys(const T_SISR_K& Key, OUT T_ELEM_K_LIST& rElemList);
};

class __MY_EXT_CLASS__ QSisrUser
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SISR_USER* m_pSisrUser;

public:
	QSisrUser(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSisrUser();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SISR_USER_K& rKey, T_SISR_USER_D& rData);
	BOOL Exist(T_SISR_USER_K Key);
	void GetKeyList(CArray<T_SISR_USER_K, T_SISR_USER_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SISR_USER_K Key, T_SISR_USER_D& rData);
	T_SISR_USER_K GetStartNum();
	
	void GetKeysEqualSisrKey(const T_SISR_K& kSisr, OUT CArray<T_SISR_USER_K, T_SISR_USER_K>& rKeyList);
	void GetElemKeysEqualSisrKey(const T_SISR_K& kSisr, OUT CArray<T_ELEM_K, T_ELEM_K>& rElemKeys);
	T_SISR_USER_K GetKeyEqualElemKey(const T_ELEM_K& kElem);
	T_SISR_USER_K GetKeyEqualSisrAndElem(const T_SISR_K& kSisr, const T_ELEM_K kElem);
	void GetAllElemKeyList(OUT CArray<T_ELEM_K, T_ELEM_K>& rElemKeys);
	BOOL ExistElem(const T_SISR_K& kSisr, const T_ELEM_K kElem);
};

class __MY_EXT_CLASS__ QSiet
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_SIET* m_pSiet;

public:
	QSiet(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSiet();

// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIET_K& rKey, T_SIET_D& rData);
	BOOL Exist(T_SIET_K Key);
	void GetKeyList(CArray<T_SIET_K, T_SIET_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SIET_K Key, T_SIET_D& rData);
	T_SIET_K GetStartNum();
};

struct MPhiProp
{
public:
	enum eDir
	{
		kPlus,
		kMinus,
		kDirMax,
	};

	double dStrength_C[kDirMax];
	double dStrength_Y[kDirMax];
	double dStrength_U[kDirMax];

	double dDisplace_C[kDirMax];
	double dDisplace_Y[kDirMax];
	double dDisplace_U[kDirMax];

	void Initialize()
	{
		for (int i = 0; i < kDirMax; ++i)
		{
			dStrength_C[i] = 0.0;
			dStrength_Y[i] = 0.0;
			dStrength_U[i] = 0.0;

			dDisplace_C[i] = 0.0;
			dDisplace_Y[i] = 0.0;
			dDisplace_U[i] = 0.0;
		}
	}
};

class __MY_EXT_CLASS__ QIehp
{
private:
	CDBDoc* m_pDoc;

public:

	enum eCom
	{
		kUnknown,
		kFx,
		kFy,
		kFz,
		kMx,
		kMy,
		kMz,
	};

	enum ePos
	{
		kUnKnownPos,
		ePosI,
		ePosJ
	};

	QIehp(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QIehp();

	/**
	* component : QIehp::eCom
	* nPos : QIehp::ePos
	*/
	BOOL GetHingeProp(const T_ELEM_K& kElem, IN int component, IN int nPos, OUT MPhiProp& rProp);

private:
	///////////////////////////////// GetHingeProp 관련함수 /////////////////////////////////
	BOOL GetHingeProp_IEHP_KINE(const T_IEHP_KINEMA& KINE, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_ORIG(const T_IEHP_ORIGIN& ORIG, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_PEAK(const T_IEHP_PEAK& PEAK, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_NBIL(const T_IEHP_NORBIL& NBIL, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_CLOU(const T_IEHP_CLOUGH& CLOU, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_DEGR(const T_IEHP_DEGRAD& DEGR, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_TAKE(const T_IEHP_TAKEDA& TAKE, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_TTET(const T_IEHP_TAKTET& TTET, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_EBIL(const T_IEHP_ELABIL& EBIL, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_ETRI(const T_IEHP_ELATRI& ETRI, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_ETET(const T_IEHP_ELATET& ETET, const int& nDOF, OUT MPhiProp& rProp);
	BOOL GetHingeProp_IEHP_SRCT(const T_IEHP_SRCTET& SRCT, const int& nDOF, OUT MPhiProp& rProp);

	void GetHingeParam(int nDOF, const double adFor[2], const double adMom[2], OUT double adVal[2]);
};

#include "HeaderPost.h"
#endif  // __DB_QUERY_SEIS_CVL_JP_H__