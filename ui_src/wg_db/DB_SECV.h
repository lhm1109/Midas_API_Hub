#ifndef __DB_SECV_DB_H__
#define __DB_SECV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_VBEM;

class CDB_SECV
{
public:
	CDB_SECV();
	virtual ~CDB_SECV();

public:
	T_SECV_K m_nStartNum;
	T_SECV_K m_nLastNum;

public:
	void Add(T_SECV_K Key,T_SECV_D& rData, CDB_VBEM* pVbem);
	BOOL Del(T_SECV_K Key, CDB_VBEM* pVbem);

public:
	BOOL Get(T_SECV_K Key,T_SECV_D& rData)
		{return m_secv.Lookup(Key,rData);}
	int GetCount()
		{return m_secv.GetCount();}
	POSITION GetStart()
		{return m_secv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SECV_K& rKey,T_SECV_D& rData)
		{m_secv.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_SECV_K Key);
	BOOL DelList(T_SECV_K Key);
	void DelAllList();
	BOOL GetList(T_SECV_K Key,SecvList*& rpList){return m_secvlist.Lookup(Key,rpList);}
	int GetCountList(){return m_secvlist.GetCount();}
	void AddListItem(T_SECV_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SECV_K Key, int nCmd, int nKey);

protected:
	CMap<T_SECV_K,T_SECV_K,T_SECV_D,T_SECV_D&>m_secv;
	CMap<T_SECV_K,T_SECV_K,SecvList*,SecvList*>m_secvlist;
};

#endif