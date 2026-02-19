// DlgExport.cpp: implementation of the CDlgExport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "wg_dgnres2.h"
#include "DlgExport.h"
#include "oncSectionRebarViewDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgExport::CDlgExport()
{

}

CDlgExport::~CDlgExport()
{

}

UINT CDlgExport::OpenDgnConSectRebarDlg(UINT ElemKey)
{
	ConcSectionRebarViewDlg dlg;
	dlg.SetSectionInputType(0);
	dlg.SetElemKey(ElemKey);
	return dlg.DoModal();
} 

////////////////////////////////////////////////////////////////////////////////////////////
// Code By Unsang ('07.11.22) :: General_Section's Rebar_Data Input_Tool!!
BOOL CDlgExport::OpenDgnUMDConSectRebarDlg(CString strElemName, T_SECT_D SectData, T_REBT_D RebtData, T_REBT_D& ReturnRebtData)
{
	ConcSectionRebarViewDlg dlg;
	dlg.SetSectionInputType(2, strElemName, SectData, RebtData);// Unsang: Modify!!

	/////////////////////////////////////////

	ReturnRebtData.Initialize();
	if(dlg.DoModal() == IDOK)
	{
		ReturnRebtData = dlg.m_RebtData;
		return TRUE;
	}
	else
		return FALSE; 
} 
////////////////////////////////////////////////////////////////////////////////////////////