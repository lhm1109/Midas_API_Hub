#pragma once

class CDBDoc;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnDefaultDataManager final
{
public:
    CDgnDefaultDataManager(CDBDoc* pDoc);
    ~CDgnDefaultDataManager();

public:
    bool DoSetDefaultDcon_Civil(const CString& crDesignCode) const;
    bool DoSetDefaultDcon_Gen(const CString& crDesignCode) const;

private:
    CDBDoc* GetDoc() const;
    CDBDoc* m_pDoc;
};

#include "HeaderPost.h"