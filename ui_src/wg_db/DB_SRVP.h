#ifndef __DB_SRVP_DB_H__
#define __DB_SRVP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SRVP
{
public:
	CDB_SRVP();
	virtual ~CDB_SRVP();
	CDBDoc* m_pDoc;

public:
	void Add(T_SRVP_K Key, T_SRVP_D& rData/*, CDB_ELEM* pElem*/);
	BOOL Del(T_SRVP_K Key/*, CDB_ELEM* pElem*/);

public:
	BOOL Get(T_SRVP_K Key, T_SRVP_D& rData);
	//{return m_srvp.Lookup(Key,rData);}
	int GetCount();
	//{return m_srvp.GetCount();}
	POSITION GetStart();
	//{return m_srvp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SRVP_K& rKey, T_SRVP_D& rData);
	//{m_srvp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SRVP_K, T_SRVP_K, T_SRVP_D, T_SRVP_D&>m_srvp;
};

#endif

