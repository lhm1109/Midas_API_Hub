#ifndef __DB_POLD_DB_H__
#define __DB_POLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_NODE;

class CDB_POLD
{
public:
	CDB_POLD();
	virtual ~CDB_POLD();
	CDBDoc* m_pDoc;

public:
	T_POLD_K m_nStartNum;
	T_POLD_K m_nLastNum;

public:
	void Add(T_POLD_K Key,T_POLD_D& rData, CDB_STLD* pStld, CDB_NODE* pNode);
	BOOL Del(T_POLD_K Key, CDB_STLD* pStld, CDB_NODE* pNode);

public:
	BOOL Get(T_POLD_K Key,T_POLD_D& rData);
		//{return m_pold.Lookup(Key,rData);}
	int GetCount();
		//{return m_pold.GetCount();}
	POSITION GetStart();
		//{return m_pold.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_POLD_K& rKey,T_POLD_D& rData);
		//{m_pold.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_POLD_K Key);
	BOOL DelList(T_POLD_K Key);
	void DelAllList();
	BOOL GetList(T_POLD_K Key,PoldList*& rpList);//{return m_poldlist.Lookup(Key,rpList);}
	int GetCountList();//{return m_poldlist.GetCount();}
	void AddListItem(T_POLD_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_POLD_K Key, int nCmd, int nKey);

protected:
	CMap<T_POLD_K,T_POLD_K,T_POLD_D,T_POLD_D&>m_pold;
	CMap<T_POLD_K,T_POLD_K,PoldList*,PoldList*>m_poldlist;
};

#endif
