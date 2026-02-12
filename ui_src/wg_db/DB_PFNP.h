#ifndef __DB_PFNP_DB_H__
#define __DB_PFNP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PFNP
{
public:
	CDB_PFNP();
	virtual ~CDB_PFNP();
	CDBDoc* m_pDoc;

public:
	void Add(T_PFNP_K Key,T_PFNP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PFNP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PFNP_K Key,T_PFNP_D& rData);
		//{return m_PFNP.Lookup(Key,rData);}
	int GetCount();
		//{return m_PFNP.GetCount();}
	POSITION GetStart();
		//{return m_PFNP.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PFNP_K& rKey,T_PFNP_D& rData);
		//{m_PFNP.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PFNP_K,T_PFNP_K,T_PFNP_D,T_PFNP_D&>m_PFNP;
};

#endif

