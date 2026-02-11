#ifndef __DB_CONS_DB_H__
#define __DB_CONS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_BNGR;

class CDB_CONS
{
public:
	CDB_CONS();
	virtual ~CDB_CONS();
	CDBDoc* m_pDoc;

public:
	void Add(T_CONS_K Key,T_CONS_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr);
	BOOL Del(T_CONS_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_CONS_K Key,T_CONS_D& rData)
		{return m_cons.Lookup(Key.keymap,rData);}

	BOOL Get(T_CONS_K Key,T_CONS_D& rData);
		//{return m_cons.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_cons.GetCount();}
	POSITION GetStart();
		//{return m_cons.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CONS_K& rKey,T_CONS_D& rData);
		//{m_cons.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_CONS_KEY,T_CONS_KEY,T_CONS_D,T_CONS_D&>m_cons;
};

#endif