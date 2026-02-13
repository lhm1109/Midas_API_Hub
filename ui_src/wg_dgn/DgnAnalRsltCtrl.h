#pragma once

#include "../../dgnengine/idesign/DGN_link/TAnalRsltCtrl.h"
#include "../../dgnengine/idesign/DGN_def/TAnalRsltKey.h"

#include "TDgnAnalRsltKey.h"

class CDBDoc;

class CDgnAnalRsltConverter;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnAnalRsltCtrl final :
    public dgn::link::TAnalRsltCtrl<TDgnAnalRsltKey<enMembKeyType, T_KEY>>
{
public:
    CDgnAnalRsltCtrl(CDgnAnalRsltConverter* pConverter, CDBDoc* pDBDoc);
    virtual ~CDgnAnalRsltCtrl();

private:
    CDBDoc* GetDBDoc() const;
    CDBDoc* m_pDBDoc;
};

#include "HeaderPost.h"
