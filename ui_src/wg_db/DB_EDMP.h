#ifndef __DB_EDMP_DB_H__
#define __DB_EDMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_EDMP
{
public:
	CDB_EDMP();
	virtual ~CDB_EDMP();
	CDBDoc* m_pDoc;

public:
	void Add(T_EDMP_K Key,T_EDMP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_EDMP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_EDMP_K Key,T_EDMP_D& rData);
		//{return m_edmp.Lookup(Key,rData);}
	int GetCount();
		//{return m_edmp.GetCount();}
	POSITION GetStart();
		//{return m_edmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EDMP_K& rKey,T_EDMP_D& rData);
		//{m_edmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_EDMP_K,T_EDMP_K,T_EDMP_D,T_EDMP_D&>m_edmp;
};

#endif