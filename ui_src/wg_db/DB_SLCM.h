#ifndef __DB_SLCM_DB_H__
#define __DB_SLCM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SLCM
{
public:
	CDB_SLCM();
	virtual ~CDB_SLCM();
	CDBDoc* m_pDoc;

public:
	void Add(T_SLCM_K Key,T_SLCM_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SLCM_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SLCM_K Key,T_SLCM_D& rData);
		//{return m_slcm.Lookup(Key,rData);}
	int GetCount();
		//{return m_slcm.GetCount();}
	POSITION GetStart();
		//{return m_slcm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SLCM_K& rKey,T_SLCM_D& rData);
		//{m_slcm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SLCM_K,T_SLCM_K,T_SLCM_D,T_SLCM_D&>m_slcm;
};

#endif

