#include "stdafx.h"
#include "DB_STCT.h"
#include "DBDoc.h"
#include "AttrCtrl.h"

CDB_STCT::CDB_STCT()
{
	m_stct.InitHashTable(HASHSIZESTCT);
}

CDB_STCT::~CDB_STCT()
{

}

void CDB_STCT::Add(T_STCT_D& rData)
{
	m_stct.SetAt(1,rData);
    auto* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc->m_pAttrCtrl )
    {
        pDoc->m_pAttrCtrl->ClearSgldMap();
    }
}

BOOL CDB_STCT::Del()
{
	T_STCT_D Data;
	BOOL bExist=m_stct.Lookup(1, Data);
    auto* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc->m_pAttrCtrl )
    {
        pDoc->m_pAttrCtrl->ClearSgldMap();
    }

	if(bExist)
	{
		BOOL ret=m_stct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
