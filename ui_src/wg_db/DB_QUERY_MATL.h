// DB_QUERY_MATL.h: interface for the query class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DB_QUERY_MATL_H__
#define __DB_QUERY_MATL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeaderPre.h"

class CDBDoc;
class CUnitCtrl;
class CDataMemb;

class CDB_MATD_REIN;

class __MY_EXT_CLASS__ QMatdRein
{
	CUnitCtrl* m_pUnitCtrl;
	CDB_MATD_REIN* m_pMatdRein;

public:
	QMatdRein(CDBDoc* pDoc, CDataMemb* pDataMemb);
	virtual ~QMatdRein();

	// Operations
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_MATD_REIN_K& rKey, T_MATD_REIN_D& rData);
	BOOL Exist(T_MATD_REIN_K Key);
	void GetKeyList(CArray<T_MATD_REIN_K, T_MATD_REIN_K>& rKeyList);  //Key 번호로 정렬된 리스트
	BOOL Get(T_MATD_REIN_K Key, T_MATD_REIN_D& rData);
	T_MATD_REIN_K GetStartNum();
	void GetReinCfDoubleData(const CString& strMainType, const CString& strSubType, std::tuple<double, double, double>& rData);
};

#include "HeaderPost.h"
#endif  // __DB_QUERY_MATL_H__