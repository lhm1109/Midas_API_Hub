#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_TVSF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_TVSF::CDB_TVSF()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_tvsf.InitHashTable(HASHSIZETVSF);
}

CDB_TVSF::CDB_TVSF(CDB_TVSF& src)
{
    *this = src;
    m_pDoc = CDBDoc::GetDocPoint();
    m_tvsf.InitHashTable(HASHSIZETVSF);
}

CDB_TVSF::~CDB_TVSF()
{
    DelAllList();
}

CDB_TVSF& CDB_TVSF::operator= (CDB_TVSF& src)
{
    POSITION pos;
    
    T_TVSF_K Key;
    T_TVSF_D DataTvsf;
    m_tvsf.RemoveAll();
    pos=src.m_tvsf.GetStartPosition();
    while(pos != NULL)
    {
        src.m_tvsf.GetNextAssoc(pos,Key,DataTvsf);
        m_tvsf.SetAt(Key, DataTvsf);
    }
    
    DelAllList();
    TvsfList* pList;
    pos=src.m_tvsflist.GetStartPosition();
    while(pos != NULL)
    {
        src.m_tvsflist.GetNextAssoc(pos,Key,pList);
        TvsfList* pListNew = new TvsfList;
        POSITION pos2=pList->GetHeadPosition();
        while(pos2 != NULL)
        {
            T_LIST_DATA ListData=pList->GetNext(pos2);
            pListNew->AddTail(ListData);
        }
        m_tvsflist.SetAt(Key, pListNew);
    }
    
    return *this;
}

void CDB_TVSF::Add(T_TVSF_K Key,T_TVSF_D& rData,CDB_SECT* pSect)
{
    T_TVSF_D Data;
    BOOL bExist=m_tvsf.Lookup(Key, Data);
    m_tvsf.SetAt(Key,rData);
    if(!bExist)pSect->AddListItem(Key, LT_TVSF_CMD, Key);
}

BOOL CDB_TVSF::Del(T_TVSF_K Key,CDB_SECT* pSect)
{
    BOOL ret=m_tvsf.RemoveKey(Key);
    ASSERT(ret);
    if(ret)VERIFY(pSect->DelListItem(Key, LT_TVSF_CMD, Key));
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_TVSF::Get(T_TVSF_K Key,T_TVSF_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return FALSE;
    return m_tvsf.Lookup(Key,rData);
}

int CDB_TVSF::GetCount()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_tvsf.GetCount();
}

POSITION CDB_TVSF::GetStart()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_tvsf.GetStartPosition();
}

void CDB_TVSF::GetNext(POSITION& rNextPosition,T_TVSF_K& rKey,T_TVSF_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    {
        ASSERT(0);
        return;
    }
    m_tvsf.GetNextAssoc(rNextPosition,rKey,rData);
}

void CDB_TVSF::AddList(T_TVSF_K Key)
{
    TvsfList* pList = new TvsfList;
    m_tvsflist.SetAt(Key, pList);
}

BOOL CDB_TVSF::DelList(T_TVSF_K Key)
{
    TvsfList* pList;
    BOOL bExist=m_tvsflist.Lookup(Key,pList);
    ASSERT(bExist);
    if(!bExist)return FALSE;
    m_tvsflist.RemoveKey(Key);
    delete pList;
    return TRUE;
}

void CDB_TVSF::DelAllList()
{
    T_TVSF_K  Key;
    TvsfList* pList;
    POSITION pos=m_tvsflist.GetStartPosition();
    while(pos != NULL)
    {
        m_tvsflist.GetNextAssoc(pos,Key,pList);
        delete pList;
    }
    m_tvsflist.RemoveAll();
}

BOOL CDB_TVSF::GetList(T_TVSF_K Key,TvsfList*& rpList)
{
    return m_tvsflist.Lookup(Key,rpList);
}

int CDB_TVSF::GetCountList()
{
    return m_tvsflist.GetCount();
}

void CDB_TVSF::AddListItem(T_TVSF_K Key, int nCmd, int nKey)
{
    TvsfList* pList;
    if(!GetList(Key, pList))return;
    
    T_LIST_DATA ListData;
    ListData.nCmd=nCmd;
    ListData.nKey=nKey;
    pList->AddTail(ListData);
}

BOOL CDB_TVSF::DelListItem(T_TVSF_K Key, int nCmd, int nKey)
{
    TvsfList* pList;
    if(!GetList(Key, pList))return TRUE;
    
    T_LIST_DATA ListData;
    POSITION pos=pList->GetHeadPosition(), posPrv;
    while(pos != NULL)
    {
        posPrv=pos;
        ListData=pList->GetNext(pos);
        if(ListData.nCmd == nCmd && ListData.nKey == nKey)
        {
            pList->RemoveAt(posPrv);
            return TRUE;
        }
    }
    return FALSE;
}