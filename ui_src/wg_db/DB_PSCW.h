#ifndef __DB_PSCW_DB_H__
#define __DB_PSCW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PSCW
{
public:
	CDB_PSCW();
	virtual ~CDB_PSCW();
	CDBDoc* m_pDoc;

public:
	void Add(T_PSCW_K Key,T_PSCW_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PSCW_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PSCW_K Key,T_PSCW_D& rData);
		//{return m_pscw.Lookup(Key,rData);}
	int GetCount();
		//{return m_pscw.GetCount();}
	POSITION GetStart();
		//{return m_pscw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PSCW_K& rKey,T_PSCW_D& rData);
		//{m_pscw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PSCW_K,T_PSCW_K,T_PSCW_D,T_PSCW_D&>m_pscw;
};

#endif

