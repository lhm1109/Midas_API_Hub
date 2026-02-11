#ifndef __DB_GUNP_DB_H__
#define __DB_GUNP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NPLN;
class CDB_GUNP
{
public:
	CDB_GUNP();
	virtual ~CDB_GUNP();
	CDBDoc* m_pDoc;

public:
	T_GUNP_K m_nStartNum;
	T_GUNP_K m_nLastNum;

public:
	void Add(T_GUNP_K Key, T_GUNP_D& rData, CDB_NPLN* pNpln);
	BOOL Del(T_GUNP_K Key, CDB_NPLN* pNpln);

public:
	BOOL Get(T_GUNP_K Key, T_GUNP_D& rData);
	//{return m_gunp.Lookup(Key,rData);}
	int GetCount();
	//{return m_gunp.GetCount();}
	POSITION GetStart();
	//{return m_gunp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUNP_K& rKey, T_GUNP_D& rData);
	//{m_gunp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUNP_K,T_GUNP_K,T_GUNP_D,T_GUNP_D&>m_gunp;
};

#endif