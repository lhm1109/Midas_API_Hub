#ifndef __DB_VBEM_DB_H__
#define __DB_VBEM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_VSEC;
class CDB_VBEM;

class CDB_VBEM
{
public:
	CDB_VBEM();
	virtual ~CDB_VBEM();
	CDBDoc* m_pDoc;

public:
	T_VBEM_K m_nStartNum;
	T_VBEM_K m_nLastNum;

public:
	void Add(T_VBEM_K Key,T_VBEM_D& rData,CDB_VSEC* pVsec);
	BOOL Del(T_VBEM_K Key,CDB_VSEC* pVsec);

public:
	BOOL Get(T_VBEM_K Key,T_VBEM_D& rData);
		//{return m_vbem.Lookup(Key,rData);}
	int GetCount();
		//{return m_vbem.GetCount();}
	POSITION GetStart();
		//{return m_vbem.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_VBEM_K& rKey,T_VBEM_D& rData);
		//{m_vbem.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_VBEM_K Key);
	BOOL DelList(T_VBEM_K Key);
	void DelAllList();
	BOOL GetList(T_VBEM_K Key, VbemList*& rpList);//{return m_vbemlist.Lookup(Key,rpList);}
	int GetCountList();//{return m_vbemlist.GetCount();}
	void AddListItem(T_VBEM_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_VBEM_K Key, int nCmd, int nKey);

protected:
	CMap<T_VBEM_K,T_VBEM_K,T_VBEM_D,T_VBEM_D&>m_vbem;
	CMap<T_VBEM_K,T_VBEM_K, VbemList*,VbemList*>m_vbemlist;
};

#endif