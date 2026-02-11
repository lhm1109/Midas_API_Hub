#include "stdafx.h"
#include "DB_PSCV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PSCV::CDB_PSCV()
{
  m_pDoc = CDBDoc::GetDocPoint();
  m_pscv.InitHashTable(HASHSIZEPSCV);
}

CDB_PSCV::~CDB_PSCV()
{

}

void CDB_PSCV::Add(T_PSCV_K Key, T_PSCV_D& rData)
{
  T_PSCV_D Data;
  BOOL bExist = m_pscv.Lookup(Key, Data);
  m_pscv.SetAt(Key, rData);
}

BOOL CDB_PSCV::Del(T_PSCV_K Key)
{
  BOOL ret = m_pscv.RemoveKey(Key);
  ASSERT(ret);
  return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PSCV::Get(T_PSCV_K Key, T_PSCV_D& rData)
{
  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    return FALSE;
  return m_pscv.Lookup(Key, rData);
}

int CDB_PSCV::GetCount()
{
  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    return 0;
  return m_pscv.GetCount();
}

POSITION CDB_PSCV::GetStart()
{
  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    return 0;
  return m_pscv.GetStartPosition();
}

void CDB_PSCV::GetNext(POSITION& rNextPosition, T_PSCV_K& rKey, T_PSCV_D& rData)
{
  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
  {
    ASSERT(0);
    return;
  }
  m_pscv.GetNextAssoc(rNextPosition, rKey, rData);
}

