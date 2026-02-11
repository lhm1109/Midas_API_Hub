#ifndef __DB_BNGR_DB_H__
#define __DB_BNGR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_BNGR
{
public:
	CDB_BNGR();
	virtual ~CDB_BNGR();
	CDBDoc* m_pDoc;

public:
	T_BNGR_K m_nLastNum;
	T_BNGR_K m_nStartNum;

public:
	void Add(T_BNGR_K Key,T_BNGR_D& rData);
	BOOL Del(T_BNGR_K Key);

public:
	BOOL Get(T_BNGR_K Key,T_BNGR_D& rData)
		{return m_bngr.Lookup(Key,rData);}
	int GetCount()
		{return m_bngr.GetCount();}
	POSITION GetStart()
		{return m_bngr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BNGR_K& rKey,T_BNGR_D& rData)
		{m_bngr.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_BNGR_K Key);
	BOOL DelList(T_BNGR_K Key);
	void DelAllList();
	BOOL GetList(T_BNGR_K Key,BngrList*& rpList);
	BOOL GetListFromBaseDB(T_BNGR_K Key,BngrList*& rpList){return m_bngrlist.Lookup(Key,rpList);}
	int GetCountList(){return m_bngrlist.GetCount();}
	void AddListItem(T_BNGR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_BNGR_K Key, int nCmd, int nKey);

protected:
	CMap<T_BNGR_K,T_BNGR_K,T_BNGR_D,T_BNGR_D&>m_bngr;
	CMap<T_BNGR_K,T_BNGR_K,BngrList*,BngrList*>m_bngrlist;
};

#endif
