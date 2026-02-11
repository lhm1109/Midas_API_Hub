#ifndef __DB_REXC_DB_H__
#define __DB_REXC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_REXC
{
public:
	CDB_REXC();
	virtual ~CDB_REXC();
	CDBDoc* m_pDoc;

public:
	void Add(T_REXC_K Key,T_REXC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_REXC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_REXC_K Key,T_REXC_D& rData);
		//{return m_rexc.Lookup(Key,rData);}
	int GetCount();
		//{return m_rexc.GetCount();}
	POSITION GetStart();
		//{return m_rexc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_REXC_K& rKey,T_REXC_D& rData);
		//{m_rexc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_REXC_K,T_REXC_K,T_REXC_D,T_REXC_D&>m_rexc;
};

#endif

