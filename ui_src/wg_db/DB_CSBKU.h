//#ifndef __DB_CSBKU_DB_H__
//#define __DB_CSBKU_DB_H__
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#include <afxtempl.h>
//#include "DB_ST_DT.h"
//
//class CDB_CSBKU
//{
//public:
//	CDB_CSBKU()
//	{
//		m_csbk.InitHashTable(HASHSIZECSBK);
//	}
//	virtual ~CDB_CSBKU() {};
//
//public:
//	void Add(T_UDRD_KEY Key, T_CSBK_UDRD_D& rData)
//	{
//		m_csbk.SetAt(Key, rData);
//	}
//	BOOL Del(T_UDRD_KEY Key)
//	{
//		return m_csbk.RemoveKey(Key);
//	}
//	void DelAll()
//	{
//		m_csbk.RemoveAll();
//	}
//	BOOL Get(T_UDRD_KEY Key, T_CSBK_UDRD_D& rData)
//	{
//		return m_csbk.Lookup(Key, rData);
//	}
//	int GetCount()
//	{
//		return m_csbk.GetCount();
//	}
//	POSITION GetStart()
//	{
//		return m_csbk.GetStartPosition();
//	}
//	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_CSBK_UDRD_D& rData)
//	{
//		m_csbk.GetNextAssoc(rNextPosition, rKey, rData);
//	}
//
//protected:
//	CMap<T_UDRD_KEY, T_UDRD_KEY, T_CSBK_UDRD_D, T_CSBK_UDRD_D&>m_csbk;
//};
//
//#endif