#ifndef __DB_POLC_DB_H__
#define __DB_POLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_NODE;
class CDB_POFC;

class CDB_POLC
{
public:
	CDB_POLC();
	virtual ~CDB_POLC();
	CDBDoc* m_pDoc;

public:
	T_POLC_K m_nStartNum;
	T_POLC_K m_nLastNum;

public:
	void Add(T_POLC_K Key, T_POLC_D& rData, CDB_STLD* pStld, CDB_NODE* pNode, CDB_POFC* pPofc);
	BOOL Del(T_POLC_K Key, CDB_STLD* pStld, CDB_NODE* pNode, CDB_POFC* pPofc);

public:
	BOOL Get(T_POLC_K Key,T_POLC_D& rData);
		//{return m_polc.Lookup(Key,rData);}
	int GetCount();
		//{return m_polc.GetCount();}
	POSITION GetStart();
		//{return m_polc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_POLC_K& rKey,T_POLC_D& rData);
		//{m_polc.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_POLC_K Key);
	BOOL DelList(T_POLC_K Key);
	void DelAllList();
	BOOL GetList(T_POLC_K Key, PolcList*& rpList);//{return m_polclist.Lookup(Key,rpList);}
	int GetCountList();//{return m_polclist.GetCount();}
	void AddListItem(T_POLC_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_POLC_K Key, int nCmd, int nKey);

protected:
	CMap<T_POLC_K,T_POLC_K,T_POLC_D,T_POLC_D&>m_polc;
	CMap<T_POLC_K,T_POLC_K, PolcList*,PolcList*>m_polclist;
};

#endif
