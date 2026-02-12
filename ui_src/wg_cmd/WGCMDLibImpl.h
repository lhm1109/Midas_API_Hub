//! WGCMDLIBIMPL kh1012 21.04.25
/*  
 *  wg_tb Interface Implementation
 *  using for get information calculated at wg_tb
 */

#pragma once

#include "../wg_base/IWGCMDLib.h"

class CWGCMDLibImpl final :
    public IWGCMDLib
{
private:
    CWGCMDLibImpl(const CWGCMDLibImpl& rhs);

public:
    CWGCMDLibImpl();
    virtual ~CWGCMDLibImpl();

public:
    virtual bool Get_H_FromCscs(int nSizePart, const T_CSCS_D& Cscs, CArray<double, double>& aH) const override;
    int CalcTimeFromHSTGAndTime(unsigned int hstgKey, int nTime) const override;
    void CalcStageTimeFromTotalTime(int nTotalTime, unsigned int& hstgKey, int& nTime) const override;
};