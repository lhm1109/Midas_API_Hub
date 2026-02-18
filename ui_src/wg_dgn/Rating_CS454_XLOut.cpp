#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "Rating_CS454_XLOut.h"

CRating_CS454_XLOut::CRating_CS454_XLOut(void) : CRating_BD21_XLOut()
{
}


CRating_CS454_XLOut::~CRating_CS454_XLOut(void)
{
}


CString CRating_CS454_XLOut::GetFlexTableHeadName()
{
    return _T("Rating_Flex_Table_Head_CS454");
}

CString CRating_CS454_XLOut::GetShearTableHeadName()
{
    return _T("Rating_Shear_Table_Head_CS454");
}

CString CRating_CS454_XLOut::GetAssessFactorContentsName()
{
    return _T("Rating_Assessment_Factor_contents_CS454");
}
