#include "StdAfx.h"

#include <array>
#include <vector>
#include <map>

#include "DgnAnalRsltConverter.h"

#include "DgnAnalRsltCtrl.h"

using namespace dgn::link;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDgnAnalRsltCtrl::CDgnAnalRsltCtrl(CDgnAnalRsltConverter* pConverter, CDBDoc* pDBDoc) :
    TAnalRsltCtrl<TDgnAnalRsltKey<enMembKeyType, T_KEY>>(pConverter),
    m_pDBDoc(pDBDoc)
{
}

CDgnAnalRsltCtrl::~CDgnAnalRsltCtrl()
{
}

CDBDoc* CDgnAnalRsltCtrl::GetDBDoc() const
{
    if ( !m_pDBDoc )
    {
        ASSERT(0); return nullptr;
    }

    return m_pDBDoc;
}