// DlgExport.h: interface for the CDlgExport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGEXPORT_H__D9E840D9_ABF2_488C_88D9_ED886891760A__INCLUDED_)
#define AFX_DLGEXPORT_H__D9E840D9_ABF2_488C_88D9_ED886891760A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "headerpre.h"

class __MY_EXT_CLASS__ CDlgExport  
{
public:
	CDlgExport();
	virtual ~CDlgExport();

	static UINT OpenDgnConSectRebarDlg(UINT ElemKey);
	static BOOL OpenDgnUMDConSectRebarDlg(CString strElemName, T_SECT_D SectData, T_REBT_D RebtData, T_REBT_D& ReturnRebtData);// Used UMD_Design!!

};
#include "headerpost.h"
#endif // !defined(AFX_DLGEXPORT_H__D9E840D9_ABF2_488C_88D9_ED886891760A__INCLUDED_)
