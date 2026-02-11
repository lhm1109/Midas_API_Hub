#include "stdafx.h"
#include "DB_PFDN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PFDN::CDB_PFDN()
{
  m_pDoc = CDBDoc::GetDocPoint();
  m_dctl.InitHashTable(HASHSIZEPFDN);
}

CDB_PFDN::~CDB_PFDN()
{

}

void CDB_PFDN::Add(T_PFDN_D& rData)
{
  m_dctl.SetAt(1,rData);
}

BOOL CDB_PFDN::Del()
{
  T_PFDN_D Data;
  BOOL bExist=m_dctl.Lookup(1, Data);
  if(bExist)
  {
    BOOL ret=m_dctl.RemoveKey(1);
    ASSERT(ret);
    return ret;
  }
  return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PFDN::Get(T_PFDN_D& rData)
{
  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    return FALSE;
  return m_dctl.Lookup(1,rData);
}

int CDB_PFDN::GetCount()
{
  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    return 0;
  return m_dctl.GetCount();
}
