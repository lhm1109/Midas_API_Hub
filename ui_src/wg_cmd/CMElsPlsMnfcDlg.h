#if !defined(__CMELSPLSMNFCDLG_H__)
#define __CMELSPLSMNFCDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGroupDampingStrainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMnfcDlg 
#include "..\wg_db\wg_db_EditUnit.h"
class CDBDoc;

#include "HeaderPre.h"
struct T_MNFC_D;
class __MY_EXT_CLASS__ CCMElsPlsMnfcDlg : public CDialogMove
{
public:
	CCMElsPlsMnfcDlg(CWnd* pParent = NULL);   // standard constructor
	enum { IDD = IDD_CMD_MNFC_DLG };

protected:
	T_MNFC_D m_Data;

	CDBDoc* m_pDoc;

	int    m_nFibrAreaSize;	// Fiber Areas size control, 0:Auto Size 1: Equal

	CEditUnit m_edtnSectNum;
	CEditUnit m_edtBeamDivNumNy;
	CEditUnit m_edtBeamDivNumNz;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	void Data2Dlg();
	BOOL Dlg2Data();

	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

#endif // !defined(__CMELSPLSMNFCDLG_H__)