#include "stdafx.h"
#include "DB_TDMF.h"

CDB_TDMF::CDB_TDMF()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdmf.InitHashTable(HASHSIZETDMF);
}

CDB_TDMF::~CDB_TDMF()
{
}

CDB_TDMF::CDB_TDMF(CDB_TDMF& src)
{
	*this = src;
}

CDB_TDMF& CDB_TDMF::operator= (CDB_TDMF& src)
{
	POSITION pos;

	m_nStartNum = src.m_nStartNum;
	m_nLastNum = src.m_nLastNum;

	T_TDMF_K KeyTdmf;
	T_TDMF_D DataTdmf;
	m_tdmf.RemoveAll();
	pos = src.m_tdmf.GetStartPosition();
	while (pos != NULL)
	{
		src.m_tdmf.GetNextAssoc(pos, KeyTdmf, DataTdmf);
		m_tdmf.SetAt(KeyTdmf, DataTdmf);
	}

	return *this;
}

void CDB_TDMF::Add(T_TDMF_K Key,T_TDMF_D& rData)
{
	m_tdmf.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_TDMF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDMF::Del(T_TDMF_K Key)
{
	BOOL ret=m_tdmf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_TDMF_K key;
			T_TDMF_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
	return ret;
}

