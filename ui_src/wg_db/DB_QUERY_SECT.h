// DB_QUERY_SECT.h: interface for the query class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DB_QUERY_SECT_H__
#define __DB_QUERY_SECT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeaderPre.h"

class CDBDoc;
class CUnitCtrl;
class CDataMemb;

class CDB_ELEM;
class CDB_RBAR_SSM;
class CDB_RGSC_SSM;
class CDB_RRSC_SSM;
class CDB_REIN_SSM;
class CDB_SECT_SSM;
class CDB_MPST_SSM;

class __MY_EXT_CLASS__ QReinSsm
{
	CDBDoc* m_pDoc;
	CDataMemb* m_pDataMemb;
	CDB_REIN_SSM* m_pReinSsm;

public:
	QReinSsm(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QReinSsm();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_REIN_SSM_K& rKey, T_REIN_SSM_D& rData);
	BOOL Exist(T_REIN_SSM_K Key);
	void GetKeyList(CArray<T_REIN_SSM_K, T_REIN_SSM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_REIN_SSM_K Key, T_REIN_SSM_D& rData);
	T_REIN_SSM_K GetStartNum();

	BOOL GetArea(const T_SECT_K& kSect, const T_REIN_SSM_K& kReinSsm, OUT double& dSectArea, OUT double& dReinArea, char chIJ = 'I');
	BOOL GetTotalArea(const T_SECT_K& kSect, const T_REIN_SSM_K& kReinSsm, OUT double& dTotalArea, char chIJ = 'I');
	void ModifySectSize(const CString& strShape, IN T_REIN_SSM_D* pReinSsm, OUT T_SECT_SECTBASE_D* pSectBase);
	void GetKeysEqualReinKey(T_REIN_SSM_K kReinSsm, CArray<T_REIN_SSM_K, T_REIN_SSM_K>& aKeyData);
};

class __MY_EXT_CLASS__ QSectSsm
{
	CDBDoc* m_pDoc;
	CDataMemb* m_pDataMemb;
	CDB_SECT_SSM* m_pSectSsm;

public:
	QSectSsm(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QSectSsm();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SECT_SSM_K& rKey, T_SECT_SSM_D& rData);
	BOOL Exist(T_SECT_SSM_K Key);
	void GetKeyList(CArray<T_SECT_SSM_K, T_SECT_SSM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_SECT_SSM_K Key, T_SECT_SSM_D& rData);
	T_SECT_SSM_K GetStartNum();

	void GetKeysEqualElemKey(T_ELEM_K kElem, CArray< T_SECT_SSM_K, T_SECT_SSM_K>& aKeyData);
	void GetKeysEqualReinSsmKey(T_REIN_SSM_K kReinSsm, CArray< T_SECT_SSM_K, T_SECT_SSM_K>& aKeyData);
};

class __MY_EXT_CLASS__ QRbarSsm
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_RBAR_SSM* m_pRbarSsm;

public:
	QRbarSsm(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QRbarSsm();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_RBAR_SSM_K& rKey, T_RBAR_SSM_D& rData);
	BOOL Exist(T_RBAR_SSM_K Key);
	void GetKeyList(CArray<T_RBAR_SSM_K, T_RBAR_SSM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_RBAR_SSM_K Key, T_RBAR_SSM_D& rData);
	T_RBAR_SSM_K GetStartNum();
	void GetKeysEqualElemKey(T_ELEM_K kElem, CArray< T_RBAR_SSM_K, T_RBAR_SSM_K>& aKeyData);
	void GetKeysEqualRrscSsmKey(T_RRSC_SSM_K kRrscSsm, CArray< T_RBAR_SSM_K, T_RBAR_SSM_K>& aKeyData);
	void GetKeysEqualRgscSsmKey(T_RGSC_SSM_K kRgscSsm, CArray< T_RBAR_SSM_K, T_RBAR_SSM_K>& aKeyData);
	void GetKeysEqualMpstSsmKey(T_MPST_SSM_K kMpstSsm, CArray< T_RBAR_SSM_K, T_RBAR_SSM_K>& aKeyData);
};

class __MY_EXT_CLASS__ QRrscSsm
{
	CDBDoc* m_pDoc;
	CDB_RRSC_SSM* m_pRrscSsm;
	CDB_RBAR_SSM* m_pRbarSsm;

public:
	QRrscSsm(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QRrscSsm();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_RRSC_SSM_K& rKey, T_RRSC_SSM_D& rData);
	BOOL Exist(T_RRSC_SSM_K Key);
	void GetKeyList(CArray<T_RRSC_SSM_K, T_RRSC_SSM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_RRSC_SSM_K Key, T_RRSC_SSM_D& rData);
	T_RRSC_SSM_K GetStartNum();

	void GetRebarFromElem(const T_ELEM_K& kElem, OUT T_RRSC_SSM_D& rSectRebar, OUT T_RRSC_SSM_D& rReinRebar);
};

class __MY_EXT_CLASS__ QRgscSsm
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_RGSC_SSM* m_pRgscSsm;

public:
	QRgscSsm(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QRgscSsm();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_RGSC_SSM_K& rKey, T_RGSC_SSM_D& rData);
	BOOL Exist(T_RGSC_SSM_K Key);
	void GetKeyList(CArray<T_RGSC_SSM_K, T_RGSC_SSM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_RGSC_SSM_K Key, T_RGSC_SSM_D& rData);
	T_RGSC_SSM_K GetStartNum();
};

class __MY_EXT_CLASS__ QMpstSsm
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_MPST_SSM* m_pMpstSsm;

public:
	QMpstSsm(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QMpstSsm();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_MPST_SSM_K& rKey, T_MPST_SSM_D& rData);
	BOOL Exist(T_MPST_SSM_K Key);
	void GetKeyList(CArray<T_MPST_SSM_K, T_MPST_SSM_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_MPST_SSM_K Key, T_MPST_SSM_D& rData);
	T_MPST_SSM_K GetStartNum();
};


#include "HeaderPost.h"
#endif  // __DB_QUERY_SECT_H__