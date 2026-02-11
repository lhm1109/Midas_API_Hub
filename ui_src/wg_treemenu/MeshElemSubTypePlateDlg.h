#if !defined(AFX_MESHELEMSUBTYPEPLATEDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_)
#define AFX_MESHELEMSUBTYPEPLATEDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshElemSubTypePlateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypePlateDlg dialog

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CMeshElemSubTypePlateDlg : public CDialogMove
{
// Construction
public:
	CMeshElemSubTypePlateDlg(CWnd* pParent = NULL);   // standard constructor
	void SetData(int nThickThin, BOOL bDOF);
	void GetData(int& nThickThin, BOOL& bDOF);
	

// Dialog Data
	//{{AFX_DATA(CMeshElemSubTypePlateDlg)
	enum { IDD = IDD_TM_MESH_ELEM_SUBTYPE_PLATE };
	MButton	m_radioThick;
	MButton	m_radioThin;
	MButton	m_chkDOF;
	//}}AFX_DATA
	int m_nThickThin;// 0:Thick, 1:Thin
	BOOL m_bDOF;

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshElemSubTypePlateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeshElemSubTypePlateDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHELEMSUBTYPEPLATEDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_)
