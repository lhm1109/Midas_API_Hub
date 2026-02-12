#ifndef __DB_ATYP_DB_H__
#define __DB_ATYP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ATYP
{
public:
	CDB_ATYP();
	CDB_ATYP(CDB_ATYP& src);
	CDB_ATYP& operator=(CDB_ATYP& src);
	virtual ~CDB_ATYP();

public:
	T_ATYP_K m_nStartNum;
	T_ATYP_K m_nLastNum;

public:
	void Add(T_ATYP_K Key,T_ATYP_D& rData);
	BOOL Del(T_ATYP_K Key);

public:
	BOOL Get(T_ATYP_K Key,T_ATYP_D& rData)
		{return m_atyp.Lookup(Key,rData);}
	int GetCount()
		{return m_atyp.GetCount();}
	POSITION GetStart()
		{return m_atyp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ATYP_K& rKey,T_ATYP_D& rData)
		{m_atyp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ATYP_K,T_ATYP_K,T_ATYP_D,T_ATYP_D&>m_atyp;
};

#endif
