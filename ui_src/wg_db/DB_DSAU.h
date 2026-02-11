#ifndef __DB_DSAU_DB_H__
#define __DB_DSAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DSAU
{
public:
	CDB_DSAU();
	virtual ~CDB_DSAU();
	CDBDoc* m_pDoc;

public:
	void Add(T_DSAU_K Key,T_DSAU_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DSAU_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DSAU_K Key,T_DSAU_D& rData);
		//{return m_DSAU.Lookup(Key,rData);}
	int GetCount();
		//{return m_DSAU.GetCount();}
	POSITION GetStart();
		//{return m_DSAU.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DSAU_K& rKey,T_DSAU_D& rData);
		//{m_DSAU.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DSAU_K,T_DSAU_K,T_DSAU_D,T_DSAU_D&>m_dsau;
};

#endif

