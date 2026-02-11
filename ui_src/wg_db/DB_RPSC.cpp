#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_RPSC.h"

CDB_RPSC::CDB_RPSC()
{
	m_rpsc.InitHashTable(HASHSIZERPSC);
	m_rpscDesign.InitHashTable(HASHSIZERPSC);
}

CDB_RPSC::~CDB_RPSC()
{

}

void CDB_RPSC::Add(T_RPSC_K Key,T_RPSC_D& rData,CDB_SECT* pSect)
{
	T_RPSC_D Data;
	BOOL bExist=m_rpsc.Lookup(Key, Data);
	m_rpsc.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_RPSC_CMD, Key);
}

BOOL CDB_RPSC::Del(T_RPSC_K Key,CDB_SECT* pSect)
{
	T_RPSC_D Data;
	BOOL bExist=m_rpsc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_rpsc.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_RPSC_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

void CDB_RPSC::AddDesign(T_RPSC_K Key,T_RPSC_D& rData,CDB_SECT* pSect)
{
	T_RPSC_D Data;
	BOOL bExist=m_rpscDesign.Lookup(Key, Data);
	m_rpscDesign.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_RPSD_CMD, Key);
}

BOOL CDB_RPSC::DelDesign(T_RPSC_K Key,CDB_SECT* pSect)
{
	T_RPSC_D Data;
	BOOL bExist=m_rpscDesign.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_rpscDesign.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_RPSD_CMD, Key));
		}
		return ret;
	}
	return bExist;
}
