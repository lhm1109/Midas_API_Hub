#if !defined(AFX_MESHELEMSUBTYPEPLSTDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_)
#define AFX_MESHELEMSUBTYPEPLSTDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshElemSubTypePlstDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypePlstDlg dialog

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CMeshElemSubTypePlstDlg : public CDialogMove
{
// Construction
public:
	CMeshElemSubTypePlstDlg(CWnd* pParent = NULL);   // standard constructor
	void SetData(BOOL bDOF);
	void GetData(BOOL& bDOF);
	

// Dialog Data
	//{{AFX_DATA(CMeshElemSubTypePlstDlg)
	enum { IDD = IDD_TM_MESH_ELEM_SUBTYPE_PLST };
	MButton	m_chkDOF;
	//}}AFX_DATA
	BOOL m_bDOF;

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshElemSubTypePlstDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeshElemSubTypePlstDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHELEMSUBTYPEPLSTDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_)
