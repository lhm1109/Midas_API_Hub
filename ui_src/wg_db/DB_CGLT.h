#ifndef __DB_CGLT_DB_H__
#define __DB_CGLT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CGLT
{
public:
	CDB_CGLT();
	virtual ~CDB_CGLT();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGLT_K Key,T_CGLT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CGLT_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CGLT_K Key,T_CGLT_D& rData);
		//{return m_cglt.Lookup(Key,rData);}
	int GetCount();
		//{return m_cglt.GetCount();}
	POSITION GetStart();
		//{return m_cglt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGLT_K& rKey,T_CGLT_D& rData);
		//{m_cglt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGLT_K,T_CGLT_K,T_CGLT_D,T_CGLT_D&>m_cglt;
};

#endif

