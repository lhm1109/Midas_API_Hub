#if !defined(__CMMvhlItemStdPatchLoadInfoDlg_H__)
#define __CMMvhlItemStdPatchLoadInfoDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdPatchLoadInfoDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

#include "CMMvhlItemStdPatchLoadInfoGrid.h"

using namespace mit::frx;


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdPatchLoadInfoDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdPatchLoadInfoDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdPatchLoadInfoDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdPatchLoadInfoDlg();

	T_MVHL_D  m_Data;
	void SetData(T_MVHL_D& Data);

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();  

protected:
	CDBDoc*   m_pDoc;
	
	CCMMvhlItemStdPatchLoadInfoGrid m_wndPatchGrid;
	
public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdPatchLoadInfoDlg)
	enum { IDD = IDD_CMD_ML_MVHL_PATCHLOAD_DLG };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdPatchLoadInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nTrainType;
	int m_nLaneType;
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdPatchLoadInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMvhlItemStdPatchLoadInfoDlg_H__)
