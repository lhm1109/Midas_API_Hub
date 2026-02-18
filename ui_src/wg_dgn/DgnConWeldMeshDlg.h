#if !defined(AFX_DGNCONWELDMESHDLG_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONWELDMESHDLG_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConWeldMeshDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnConBarDlg dialog

class CDgnConWeldMeshDlg : public CDialogMove
{
// Construction
public:
	CDgnConWeldMeshDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConWeldMeshDlg)
	enum { IDD = IDD_DGN_CON_WELDMESH_DLG };
	CComboBox	m_Fabric;
	int	m_nType;
	//}}AFX_DATA

public:
	CGXGridWnd m_wndGrid;

	CString m_strMeshName;
	CString m_strVBarName, m_strHBarName;
	double m_dVBarSpace, m_dVBarArea;
	double m_dHBarSpace, m_dHBarArea;

public:
	void InitGrid();
	void Write_GridTitle();

	void Init_WeldMeshComboBox(int iFabricType);
	void Get_WeldMeshData(CString strMeshName, CString& strVBarName, double& dVBarSpace, double& dVBarArea, CString& strHBarName, double& dHBarSpace, double& dHBarArea);
	CString FindMeshName(CString strVBarName, double dVBarSpace, CString strHBarName, double dHBarSpace);
	void Get_LengthAreaFactor(double& dLenFactor, double& dAreaFactor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConWeldMeshDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConWeldMeshDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDgnWeldMeshName();
	afx_msg void OnDgnChangeMeshType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONWELDMESHDLG_H__5D7AB3A9_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
