#ifndef __DB_JDAC_DB_H__
#define __DB_JDAC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_NODE;
class CDB_JDAC
{
public:
	CDB_JDAC();
	virtual ~CDB_JDAC();

public:
	void Add(T_JDAC_D& rData, CDB_NODE* pNode);
	BOOL Del(CDB_NODE* pNode); 

public:
	BOOL Get(T_JDAC_D& rData)
		{return m_JDAC.Lookup(1,rData);}
	int GetCount()
		{return m_JDAC.GetCount();}

protected:
	CMap<T_JDAC_K,T_JDAC_K,T_JDAC_D,T_JDAC_D&> m_JDAC;
};

#endif