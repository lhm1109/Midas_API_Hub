#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_TMAT.h"

CDB_TMAT::CDB_TMAT()
{
	m_tmat.InitHashTable(HASHSIZETMAT);
}

CDB_TMAT::~CDB_TMAT()
{

}

CDB_TMAT::CDB_TMAT(CDB_TMAT& src)
{
	*this = src;
}

CDB_TMAT& CDB_TMAT::operator= (CDB_TMAT& src)
{
	POSITION pos;

	T_TMAT_K KeyTmat;
	T_TMAT_D DataTmat;
	m_tmat.RemoveAll();
	pos=src.m_tmat.GetStartPosition();
	while(pos != NULL)
	{
		src.m_tmat.GetNextAssoc(pos,KeyTmat,DataTmat);
		m_tmat.SetAt(KeyTmat, DataTmat);
	}

	return *this;
}

void CDB_TMAT::Add(T_TMAT_K Key,T_TMAT_D& rData,CDB_MATL* pMatl)
{
	T_TMAT_D Data;
	BOOL bExist=m_tmat.Lookup(Key, Data);
	m_tmat.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_TMAT_CMD, Key);
}

BOOL CDB_TMAT::Del(T_TMAT_K Key,CDB_MATL* pMatl)
{
	T_TMAT_D Data;
	BOOL bExist=m_tmat.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_tmat.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_TMAT_CMD, Key));
		}
		return ret;
	}
	return bExist;
}


