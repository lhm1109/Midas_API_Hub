#ifndef __DB_RKPO_DB_H__
#define __DB_RKPO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RKPO
{
public:
	CDB_RKPO();
	virtual ~CDB_RKPO();
	CDBDoc* m_pDoc;

public:
	void Add(T_RKPO_K Key,T_RKPO_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RKPO_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RKPO_K Key,T_RKPO_D& rData);
		//{return m_rkpo.Lookup(Key,rData);}
	int GetCount();
		//{return m_rkpo.GetCount();}
	POSITION GetStart();
		//{return m_rkpo.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RKPO_K& rKey,T_RKPO_D& rData);
		//{m_rkpo.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RKPO_K,T_RKPO_K,T_RKPO_D,T_RKPO_D&>m_rkpo;
};

#endif

