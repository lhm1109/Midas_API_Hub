#ifndef __DB_NSPR_DB_H__
#define __DB_NSPR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_BNGR;

class CDB_NSPR
{
public:
	CDB_NSPR();
	virtual ~CDB_NSPR();
	CDBDoc* m_pDoc;

public:
	void Add(T_NSPR_K Key,T_NSPR_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr);
	BOOL Del(T_NSPR_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_NSPR_K Key,T_NSPR_D& rData)
		{return m_nspr.Lookup(Key.keymap,rData);}

	BOOL Get(T_NSPR_K Key,T_NSPR_D& rData);
		//{return m_nspr.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_nspr.GetCount();}
	POSITION GetStart();
		//{return m_nspr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NSPR_K& rKey,T_NSPR_D& rData);
		//{m_nspr.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_NSPR_KEY,T_NSPR_KEY,T_NSPR_D,T_NSPR_D&>m_nspr;
};

#endif