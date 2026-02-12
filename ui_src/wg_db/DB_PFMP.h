#ifndef __DB_PFMP_DB_H__
#define __DB_PFMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PFMP
{
public:
	CDB_PFMP();
	virtual ~CDB_PFMP();
	CDBDoc* m_pDoc;

public:
	void Add(T_PFMP_K Key,T_PFMP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PFMP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PFMP_K Key,T_PFMP_D& rData);
		//{return m_PFMP.Lookup(Key,rData);}
	int GetCount();
		//{return m_PFMP.GetCount();}
	POSITION GetStart();
		//{return m_PFMP.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PFMP_K& rKey,T_PFMP_D& rData);
		//{m_PFMP.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PFMP_K,T_PFMP_K,T_PFMP_D,T_PFMP_D&>m_PFMP;
};

#endif

