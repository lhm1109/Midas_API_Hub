// Dgn_RCManager.h: interface for the CDgn_RCManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGN_RCMANAGER_H__255A98A4_6B14_41FA_B3A9_6E400C1F86CE__INCLUDED_)
#define AFX_DGN_RCMANAGER_H__255A98A4_6B14_41FA_B3A9_6E400C1F86CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\dgnengine\src\CVL_DgnCalc_CH\DgnCalc_CH_RCSC_ColumnStruct.h"

#include "Dgn_DllManager_Base.h"

class CDgn_RCManager : public CDgn_DllManager_Base   
{
public:
	CDgn_RCManager();
	virtual ~CDgn_RCManager();

public:
	BOOL Calc_RC_Column_Execute_Chk(_CVL_CH_RC_COLUMN_JTG_W& InData, _CVL_CH_RC_COL_RES_W& OutData);
};

#endif // !defined(AFX_DGN_RCMANAGER_H__255A98A4_6B14_41FA_B3A9_6E400C1F86CE__INCLUDED_)
