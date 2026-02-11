#include "stdafx.h"
#include "DB_PLCB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PLCB::CDB_PLCB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_plcb.InitHashTable(HASHSIZEPLCB);
}

CDB_PLCB::~CDB_PLCB()
{

}

void CDB_PLCB::Add(T_PLCB_D& rData)
{
	T_PLCB_D Data;
	BOOL bExist=m_plcb.Lookup(1, Data);
	if(bExist)
	{
//    for(int i=0; i<Data.arLoad.GetSize(); i++)
//    {
//      VERIFY(pStld->DelListItem(Data.arLoad[i], LT_PLCB_CMD, 1));
//    }
	}
	m_plcb.SetAt(1,rData);
//  for(int i=0; i<rData.arEseq.GetSize(); i++)
//  {
//    VERIFY(pStld->AddListItem(rData.arLoad[i], LT_PLCB_CMD, 1));
//  }
}

BOOL CDB_PLCB::Del()
{
	T_PLCB_D Data;
	BOOL bExist=m_plcb.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_plcb.RemoveKey(1);
		ASSERT(ret);
		if(ret)
		{
//      for(int i=0; i<Data.arLoad.GetSize(); i++)
//      {
//        VERIFY(pStld->DelListItem(Data.arLoad[i], LT_ESEQ_CMD, 1));
//      }
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PLCB::Get(T_PLCB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_plcb.Lookup(1,rData);
}

int CDB_PLCB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_plcb.GetCount();
}
