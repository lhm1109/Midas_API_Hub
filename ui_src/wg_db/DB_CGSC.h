#ifndef __DB_CGSC_DB_H__
#define __DB_CGSC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CGSC
{
public:
	CDB_CGSC();
	virtual ~CDB_CGSC();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGSC_K Key,T_CGSC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CGSC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CGSC_K Key,T_CGSC_D& rData);
		//{return m_cgsc.Lookup(Key,rData);}
	int GetCount();
		//{return m_cgsc.GetCount();}
	POSITION GetStart();
		//{return m_cgsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGSC_K& rKey,T_CGSC_D& rData);
		//{m_cgsc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGSC_K,T_CGSC_K,T_CGSC_D,T_CGSC_D&>m_cgsc;
};

#endif

