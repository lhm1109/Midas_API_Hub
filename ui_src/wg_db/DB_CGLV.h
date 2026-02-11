#ifndef __DB_CGLV_DB_H__
#define __DB_CGLV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CGLV
{
public:
	CDB_CGLV();
	virtual ~CDB_CGLV();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGLV_K Key,T_CGLV_D& rData);
	BOOL Del(T_CGLV_K Key);

public:
	BOOL Get(T_CGLV_K Key,T_CGLV_D& rData);
		//{return m_cglv.Lookup(Key,rData);}
	int GetCount();
		//{return m_cglv.GetCount();}
	POSITION GetStart();
		//{return m_cglv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGLV_K& rKey,T_CGLV_D& rData);
		//{m_cglv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGLV_K,T_CGLV_K,T_CGLV_D,T_CGLV_D&>m_cglv;
};

#endif

