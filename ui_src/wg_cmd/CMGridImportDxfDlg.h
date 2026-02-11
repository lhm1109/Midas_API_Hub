#if !defined(AFX_CMGRIDIMPORTDXFDLG_H__0F685CA6_E734_4D0C_BE10_75FAD948E6C4__INCLUDED_)
#define AFX_CMGRIDIMPORTDXFDLG_H__0F685CA6_E734_4D0C_BE10_75FAD948E6C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridImportDxfDlg.h : header file
//

#include "..\wg_base\wg_base_MouseEdit.h"
#include "CMDlgBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMGridImportDxfDlg dialog
class CDBDoc;
class CCMGridImportDxfDlg : public CCMDlgBase
{
// Construction
public:
	CCMGridImportDxfDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMGridImportDxfDlg)
	enum { IDD = IDD_CMD_GRID_IMPORT_DXF_DLG };
	CListBox m_listAllLayers;
	CListBox m_listSelLayers;
	CEdit	m_ctrlFileName;
	CEdit	m_ctrlName;
	CMouseEdit	m_ctrlOriginPoint;
	CEdit	m_ctrlScaleFactor;
	CComboBox	m_ctrlRotationZ;
	CComboBox	m_ctrlRotationY;
	CComboBox	m_ctrlRotationX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridImportDxfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CStringArray m_aAllLayers;
	CStringArray m_aSelLayers;
	CArray<UINT, UINT> m_aCtrlBtn;
	CString DXF_file_name;
	CString strLoadLineName;
	double m_dOrgX, m_dOrgY, m_dOrgZ;
	double m_dScaleFactor;
	double m_aRotateAngle[3];

	void SetAllLayerList();
	void MoveAlltoSelList();
	void MoveSeltoAllList();
	BOOL Dlg2Data();
	BOOL ImportDXF();
	BOOL Execute();
	BOOL IsSelectedLayer(CString &csLayer);

	// Generated message map functions
	//{{AFX_MSG(CCMGridImportDxfDlg)
	afx_msg void OnDoubleclickedAllList();
	afx_msg void OnDoubleclickedSelList();
	afx_msg void OnButtonSelLayers();
	afx_msg void OnButtonUnSelLayers();
	afx_msg void OnButtonDxfBrowse();
	afx_msg void OnChangeEditFileName();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGRIDIMPORTDXFDLG_H__0F685CA6_E734_4D0C_BE10_75FAD948E6C4__INCLUDED_)
