#ifndef __DB_KFAC_DB_H__
#define __DB_KFAC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_KFAC
{
public:
	CDB_KFAC();
	virtual ~CDB_KFAC();
	CDBDoc* m_pDoc;

public:
	void Add(T_KFAC_K Key,T_KFAC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_KFAC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_KFAC_K Key,T_KFAC_D& rData);
		//{return m_kfac.Lookup(Key,rData);}
	int GetCount();
		//{return m_kfac.GetCount();}
	POSITION GetStart();
		//{return m_kfac.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_KFAC_K& rKey,T_KFAC_D& rData);
		//{m_kfac.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_KFAC_K,T_KFAC_K,T_KFAC_D,T_KFAC_D&>m_kfac;
};

#endif

