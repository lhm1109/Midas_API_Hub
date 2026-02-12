#include "StdAfx.h"

#include "WGCMDLibImpl.h"

#include "wg_cmd.h"
#include "CMStageCompSectItemDlg.h"
#include "CMHydrCoolElemItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CWGCMDLibImpl _g_Instance;

CWGCMDLibImpl::CWGCMDLibImpl(const CWGCMDLibImpl& rhs) {
    ASSERT(0);
}

CWGCMDLibImpl::CWGCMDLibImpl() {
    m_pInstance = this;
}

CWGCMDLibImpl::~CWGCMDLibImpl() {}

bool CWGCMDLibImpl::Get_H_FromCscs(int nSizePart, const T_CSCS_D& Cscs, CArray<double, double>& aH) const {
    CCMStageCompSectItemDlg Dlg;
    return Dlg.Get_H_FromCscs(nSizePart, Cscs, aH);
}

int CWGCMDLibImpl::CalcTimeFromHSTGAndTime(unsigned int hstgKey, int nTime) const
{
    CCMHydrCoolElemItem item;
	return item.CalcTime(hstgKey, nTime);
}

void CWGCMDLibImpl::CalcStageTimeFromTotalTime(int nTotalTime, unsigned int& hstgKey, int& nTime) const
{
    CCMHydrCoolElemItem item;
    return item.CalcStageTime(nTotalTime, hstgKey, nTime);
}
