#ifndef __DB_BCIF_DB_H__
#define __DB_BCIF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_BCIF
{
public:
	CDB_BCIF();
	virtual ~CDB_BCIF();
	CDBDoc* m_pDoc;

public:
	void Add(T_BCIF_K Key,T_BCIF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_BCIF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_BCIF_K Key,T_BCIF_D& rData);
		//{return m_bcif.Lookup(Key,rData);}
	int GetCount();
		//{return m_bcif.GetCount();}
	POSITION GetStart();
		//{return m_bcif.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BCIF_K& rKey,T_BCIF_D& rData);
		//{m_bcif.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_BCIF_K,T_BCIF_K,T_BCIF_D,T_BCIF_D&>m_bcif;
};

#endif

