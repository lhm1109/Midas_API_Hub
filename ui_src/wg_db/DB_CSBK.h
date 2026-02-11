//#ifndef __DB_CSBK_DB_H__
//#define __DB_CSBK_DB_H__
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#include <afxtempl.h>
//#include "DB_ST_DT.h"
//
//class CDBDoc;
//
//class CDB_CSBK
//{
//public:
//	CDB_CSBK();
//	virtual ~CDB_CSBK();
//	CDBDoc* m_pDoc;
//
//public:
//	T_CSBK_K m_nStartNum;
//	T_CSBK_K m_nLastNum;
//
//public:
//	void Add(T_CSBK_K Key, T_CSBK_D& rData);
//	BOOL Del(T_CSBK_K Key);
//
//public:
//	BOOL Get(T_CSBK_K Key, T_CSBK_D& rData);
//	//{return m_csbk.Lookup(Key,rData);}
//	int GetCount();
//	//{return m_csbk.GetCount();}
//	POSITION GetStart();
//	//{ return m_csbk.GetStartPosition(); }
//	void GetNext(POSITION& rNextPosition, T_CSBK_K& rKey, T_CSBK_D& rData);
//	//{ m_csbk.GetNextAssoc(rNextPosition, rKey, rData); }
//
//protected:
//	CMap<T_CSBK_K, T_CSBK_K, T_CSBK_D, T_CSBK_D&>m_csbk;
//};
//
//#endif