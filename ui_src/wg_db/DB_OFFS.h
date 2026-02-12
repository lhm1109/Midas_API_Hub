#ifndef __DB_OFFS_DB_H__
#define __DB_OFFS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_OFFS
{
public:
	CDB_OFFS();
	virtual ~CDB_OFFS();
	CDBDoc* m_pDoc;

public:
	void Add(T_OFFS_K Key,T_OFFS_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_OFFS_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_OFFS_K Key,T_OFFS_D& rData)
		{return m_offs.Lookup(Key.keymap,rData);}

	BOOL Get(T_OFFS_K Key,T_OFFS_D& rData);
		//{return m_offs.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_offs.GetCount();}
	POSITION GetStart();
		//{return m_offs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_OFFS_K& rKey,T_OFFS_D& rData);
		//{m_offs.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_OFFS_KEY,T_OFFS_KEY,T_OFFS_D,T_OFFS_D&>m_offs;
};

#endif

