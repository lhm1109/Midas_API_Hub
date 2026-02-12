#ifndef __DB_DSAS_DB_H__
#define __DB_DSAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DSAS
{
public:
	CDB_DSAS();
	virtual ~CDB_DSAS();
	CDBDoc* m_pDoc;

public:
	void Add(T_DSAS_K Key,T_DSAS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DSAS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DSAS_K Key,T_DSAS_D& rData);
		//{return m_DSAS.Lookup(Key,rData);}
	int GetCount();
		//{return m_DSAS.GetCount();}
	POSITION GetStart();
		//{return m_DSAS.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DSAS_K& rKey,T_DSAS_D& rData);
		//{m_DSAS.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DSAS_K,T_DSAS_K,T_DSAS_D,T_DSAS_D&>m_dsas;
};

#endif

