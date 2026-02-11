
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTlgtDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void TLGT_INFO::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	CUnitCtrl* m_pUnitCtrl = pDoc->m_pUnitCtrl;

	const double dVelocityFactor = 1.0 / m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_VELOCITY);
	
	//dVelocity *= dVelocityFactor;
}

void TLGT_INFO::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	CUnitCtrl* m_pUnitCtrl = pDoc->m_pUnitCtrl;

	const double dVelocityFactor = m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_VELOCITY);

	//dVelocity *= dVelocityFactor;
}
