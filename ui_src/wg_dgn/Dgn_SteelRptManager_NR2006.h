//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DGN_SteelRptManager_NR2006_H__INCLUDED_)
#define AFX_DGN_SteelRptManager_NR2006_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"

#include "..\dgnengine\idesign\DGN_lib\IExcel.h"

#include "DgnEngineProgressDlg.h"

struct CSG_RPT_CS457_D;

class CDgn_SteelRptManager_NR2006 : public CDgn_DllManager_Base
{
public:
	CDgn_SteelRptManager_NR2006();
	virtual ~CDgn_SteelRptManager_NR2006();

protected:
	PDGNENGINE_RPT_PROG* m_pNR2006Detail;

public:
	BOOL InitialData(int iNationType);

	BOOL Print_DetailReport(dgn::lib::IExcel* pExcel, D_XL_PROGRESS_INTERFACE* pProgDlg, int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName, CSG_RPT_CS457_D& rData);

};
#endif // !defined(AFX_DGN_SteelRptManager_NR2006_H__INCLUDED_)