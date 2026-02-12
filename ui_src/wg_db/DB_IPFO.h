#ifndef __DB_IPFO_DB_H__
#define __DB_IPFO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IPFO
{
public:
	CDB_IPFO();
	virtual ~CDB_IPFO();
	
public:
	void Add(T_IPFO_D& rData);
	BOOL Del();

public:
	BOOL Get(T_IPFO_D& rData)
		{return m_ipfo.Lookup(1,rData);}
	int GetCount()
		{return m_ipfo.GetCount();}

protected:
	CMap<T_IPFO_K,T_IPFO_K,T_IPFO_D,T_IPFO_D&>m_ipfo;
};

#endif