#ifndef __DB_RROP_DB_H__
#define __DB_RROP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RROP
{
public:
	CDB_RROP();
	virtual ~CDB_RROP();
	CDBDoc* m_pDoc;

public:
	void Add(T_RROP_K Key,T_RROP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RROP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RROP_K Key,T_RROP_D& rData);
	//{return m_rrop.Lookup(Key,rData);}
	int GetCount();
	//{return m_rrop.GetCount();}
	POSITION GetStart();
	//{return m_rrop.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RROP_K& rKey,T_RROP_D& rData);
	//{m_rrop.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RROP_K,T_RROP_K,T_RROP_D,T_RROP_D&>m_rrop;
};

#endif

