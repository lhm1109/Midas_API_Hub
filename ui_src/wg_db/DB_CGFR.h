#ifndef __DB_CGFR_DB_H__
#define __DB_CGFR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CGFR
{
public:
	CDB_CGFR();
	virtual ~CDB_CGFR();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGFR_K Key,T_CGFR_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CGFR_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CGFR_K Key,T_CGFR_D& rData);
		//{return m_cgfr.Lookup(Key,rData);}
	int GetCount();
		//{return m_cgfr.GetCount();}
	POSITION GetStart();
		//{return m_cgfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGFR_K& rKey,T_CGFR_D& rData);
		//{m_cgfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGFR_K,T_CGFR_K,T_CGFR_D,T_CGFR_D&>m_cgfr;
};

#endif

