#ifndef __DB_SPHO_DB_H__
#define __DB_SPHO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPHO
{
public:
	CDB_SPHO();
	CDB_SPHO(CDB_SPHO& src);
	CDB_SPHO& operator=(CDB_SPHO& src);
	virtual ~CDB_SPHO();

public:
	void Add(T_SPHO_K Key,T_SPHO_D& rData);
	BOOL Del(T_SPHO_K Key);

public:
	BOOL Get(T_SPHO_K Key,T_SPHO_D& rData)
	{return m_spho.Lookup(Key.keymap,rData);}
	int GetCount()
	{return m_spho.GetCount();}
	POSITION GetStart()
	{return m_spho.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPHO_K& rKey,T_SPHO_D& rData)
	{m_spho.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_SPHO_KEY,T_SPHO_KEY,T_SPHO_D,T_SPHO_D&>m_spho;
};

#endif
