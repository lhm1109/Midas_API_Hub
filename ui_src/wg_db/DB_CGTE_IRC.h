#ifndef __DB_CGTE_IRC_DB_H__
#define __DB_CGTE_IRC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_CGTE_IRC
{
public:
	CDB_CGTE_IRC();
	virtual ~CDB_CGTE_IRC();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGTE_IRC_K Key,T_CGTE_IRC_D& rData,CDB_NODE* pNode);
	BOOL Del(T_CGTE_IRC_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_CGTE_IRC_K Key,T_CGTE_IRC_D& rData);
		//{return m_cgte.Lookup(Key,rData);}
	int GetCount();
		//{return m_cgte.GetCount();}
	POSITION GetStart();
		//{return m_cgte.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGTE_IRC_K& rKey,T_CGTE_IRC_D& rData);
		//{m_cgte.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGTE_IRC_K,T_CGTE_IRC_K,T_CGTE_IRC_D,T_CGTE_IRC_D&>m_cgte_IRC;
};

#endif

