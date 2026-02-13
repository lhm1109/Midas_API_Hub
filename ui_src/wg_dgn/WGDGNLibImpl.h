#pragma once

#include "../wg_base/IWGDGNLib.h"

class CWGDGNLibImpl final :
    public IWGDGNLib
{
public:
    CWGDGNLibImpl();

private:
    CWGDGNLibImpl(const CWGDGNLibImpl& rhs) = default;

public:
    virtual bool IsConCodeUseMeshedPM4DgnEngine(const CString& strCode) const override;
};