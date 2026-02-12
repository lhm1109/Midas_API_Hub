#ifndef __DB_CGTS_DB_H__
#define __DB_CGTS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CGTS
{
public:
	CDB_CGTS();
	virtual ~CDB_CGTS();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGTS_K Key,T_CGTS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CGTS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CGTS_K Key,T_CGTS_D& rData);
		//{return m_cgts.Lookup(Key,rData);}
	int GetCount();
		//{return m_cgts.GetCount();}
	POSITION GetStart();
		//{return m_cgts.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGTS_K& rKey,T_CGTS_D& rData);
		//{m_cgts.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGTS_K,T_CGTS_K,T_CGTS_D,T_CGTS_D&>m_cgts;
};

#endif

