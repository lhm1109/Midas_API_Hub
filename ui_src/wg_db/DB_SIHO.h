#ifndef __DB_SIHO_DB_H__
#define __DB_SIHO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIHO
{
public:
	CDB_SIHO();
	CDB_SIHO(CDB_SIHO& src);
	CDB_SIHO& operator=(CDB_SIHO& src);
	virtual ~CDB_SIHO();

public:
	T_SIHO_K m_nLastNum;
	T_SIHO_K m_nStartNum;

public:
	void Add(T_SIHO_K Key,T_SIHO_D& rData);
	BOOL Del(T_SIHO_K Key);

public:
	BOOL Get(T_SIHO_K Key,T_SIHO_D& rData)
	{return m_siho.Lookup(Key,rData);}
	int GetCount()
	{return m_siho.GetCount();}
	POSITION GetStart()
	{return m_siho.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SIHO_K& rKey,T_SIHO_D& rData)
	{m_siho.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SIHO_K,T_SIHO_K,T_SIHO_D,T_SIHO_D&>m_siho;
};

#endif
