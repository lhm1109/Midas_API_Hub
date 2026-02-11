#if !defined(AFX_MESHELEMSUBTYPEWALLDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_)
#define AFX_MESHELEMSUBTYPEWALLDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshElemSubTypeWallDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypeWallDlg dialog

#include "..\wg_db\EditWid.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CMeshElemSubTypeWallDlg : public CDialogMove
{
// Construction
public:
	CMeshElemSubTypeWallDlg(CWnd* pParent = NULL);   // standard constructor
	void SetData(int nMembPlate, int nWallIDType, int nWallID);
	void GetData(int& nMembPlate, int& nWallIDType, int& nWallID);
	

// Dialog Data
	//{{AFX_DATA(CMeshElemSubTypeWallDlg)
	enum { IDD = IDD_TM_MESH_ELEM_SUBTYPE_WALL };
	MButton	m_radioMembrane;
	MButton	m_radioPlate;
	CEditWid	m_editWall;
	MComboBox	m_cboWall;
	//}}AFX_DATA
	int m_nMembPlate;//0:Membrane, 1:Plate
	int m_nWallIDType;//0:Fixed No., 1:Auto Inc, 2:Last N0.+1
	int m_nWallID;
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshElemSubTypeWallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int GetWallMode();

	// Generated message map functions
	//{{AFX_MSG(CMeshElemSubTypeWallDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();	
	afx_msg void OnSelchangeTmWIdCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHELEMSUBTYPEWALLDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_)
