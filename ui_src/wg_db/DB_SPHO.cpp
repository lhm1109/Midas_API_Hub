#include "stdafx.h"
#include "DB_SPHO.h"

CDB_SPHO::CDB_SPHO()
{
	m_spho.InitHashTable(HASHSIZESPHO);
}

CDB_SPHO::~CDB_SPHO()
{
}

CDB_SPHO::CDB_SPHO(CDB_SPHO& src)
{
	*this = src;
}

CDB_SPHO& CDB_SPHO::operator= (CDB_SPHO& src)
{
	POSITION pos;

	T_SPHO_K Key;
	T_SPHO_D Data;
	m_spho.RemoveAll();
	pos=src.m_spho.GetStartPosition();
	while(pos != NULL)
	{
		src.m_spho.GetNextAssoc(pos,Key.keymap,Data);
		m_spho.SetAt(Key.keymap, Data);
	}

	return *this;
}

void CDB_SPHO::Add(T_SPHO_K Key,T_SPHO_D& rData)
{
	m_spho.SetAt(Key.keymap,rData);
}

BOOL CDB_SPHO::Del(T_SPHO_K Key)
{
	BOOL ret=m_spho.RemoveKey(Key.keymap);
	ASSERT(ret);
	return ret;
}
