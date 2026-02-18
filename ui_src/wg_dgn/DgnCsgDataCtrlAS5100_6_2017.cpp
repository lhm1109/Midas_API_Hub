#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "..\wg_db\DBLib.h"

#include "DgnCsgDataCtrlAS5100_6_2017.h"
#include "Dgn_CSGManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnCsgDataCtrlAS5100_6_2017::CDgnCsgDataCtrlAS5100_6_2017() : CDgnCsgDataCtrl()
{
}

CDgnCsgDataCtrlAS5100_6_2017::~CDgnCsgDataCtrlAS5100_6_2017()
{
}

void CDgnCsgDataCtrlAS5100_6_2017::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	T_UNIT_INDEX CodeUnit;

	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;

	m_pDataCtrl->Set_UnitIndex(CodeUnit);

	InitialData(m_nDgnCode);
}
