#include "stdafx.h"
#include "SeisEvalTextOutMgr.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "SeisEvalTextOutMOE2019.h"
#include "SeisEvalTextOutKISTEC2019.h"

CSeisEvalTextOutMgr::CSeisEvalTextOutMgr(void)
{
    m_pTextOut = nullptr;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    int nSemeMethod = pDoc->m_pAttrCtrl2->GetSemeMethod();    
    switch ( nSemeMethod )
    {
    case D_SEME_METHOD_MOE2019:        
        m_pTextOut = new CSeisEvalTextOutMOE2019();
        break;
    case D_SEME_METHOD_KISTEC2019:
        m_pTextOut = new CSeisEvalTextOutKISTEC2019();
        break;
    default: ASSERT(0);
        break;
    }
}


CSeisEvalTextOutMgr::~CSeisEvalTextOutMgr(void)
{
    if ( m_pTextOut ) { delete m_pTextOut;  m_pTextOut = nullptr; }
}


bool CSeisEvalTextOutMgr::PrintSeisEval1stCon(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{
    m_pTextOut->InitPrintSetting(_T("1stCon"));
    m_pTextOut->PrintSeisEval1stCon(aElemK, LcomK);

    CDBLib::RunTextEditor(m_pTextOut->GetFileName(), _LSX(1stEvalCon), TRUE);

    return true;
}

bool CSeisEvalTextOutMgr::PrintSeisEval1stStl(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{
    return true;
}

bool CSeisEvalTextOutMgr::PrintSeisEval1stMas(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{
    return true;
}

bool CSeisEvalTextOutMgr::PrintSeisEvalUphgCon(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{
    m_pTextOut->InitPrintSetting(_T("UphgCon"));
    m_pTextOut->PrintSeisEvalUphgCon(aElemK);

    CDBLib::RunTextEditor(m_pTextOut->GetFileName(), _LSX(UpdateHingeCon), TRUE);

    return true;
}

bool CSeisEvalTextOutMgr::PrintSeisEvalUphgStl(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{
    return true;
}

bool CSeisEvalTextOutMgr::PrintSeisEvalUphgMas(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{
    return true;
}

