#if !defined(__ELEMPLATEREDUFACTORDLG_H__)
#define      __ELEMPLATEREDUFACTORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemPlateReduFactorDlg.h : header file
//

#include "..\wg_db\wg_db_CobxBngr.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CElemPlateReduFactorDlg dialog
class CElemPlateReduFactorDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemPlateReduFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CElemPlateReduFactorDlg)
	enum { IDD = IDD_TM_ELEM_PLATE_REDU_FACTOR };
	CCobxBngr	m_wndGroupCombo;
	int		m_nAddOrDel;

	mit::frx::MEdit m_wndAxialX;
	mit::frx::MEdit m_wndAxialY;
	mit::frx::MEdit m_wndShear;

	mit::frx::MEdit m_wndOutBendingX;
	mit::frx::MEdit m_wndOutBendingY;
	mit::frx::MEdit m_wndOutTorsion;
	mit::frx::MEdit m_wndOutShearX;
	mit::frx::MEdit m_wndOutShearY;

	BOOL	m_bUseUserDir;

	CArray<UINT,UINT> m_aLocalAxisCtrl;
	CArray<UINT,UINT> m_aLocalAxisTypeCtrl;
	CArray<UINT,UINT> m_aPointCtrl;
	CArray<UINT,UINT> m_aVectorCtrl;
	
	mit::frx::MComboBox	m_cmbCoordDir;
	CMouseEdit	m_edtPoint;
	CTextUnit	m_unitPoint;
	CMouseEdit	m_edtVector;
	CTextUnit	m_unitVector;
	int		m_nLocalAxis;
	int		m_nDirType;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemPlateReduFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void UpdateUserDirCtrls(BOOL bShow);

	void Execute_Direction(CArray<T_ELEM_K, T_ELEM_K>& aKeyList, CArray<T_PSSF_D, T_PSSF_D>& aDataList);
	void Execute_Vector(CArray<T_ELEM_K, T_ELEM_K>& aKeyList, CArray<T_PSSF_D, T_PSSF_D>& aDataList);

	void CtrlEnableDisable(CArray<UINT, UINT> &aCtrl, BOOL bEnable);
	void CtrlEnableDisable();

	// Generated message map functions
	//{{AFX_MSG(CElemPlateReduFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmOption();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnTmUserDirButton();
	afx_msg void OnClickLocalAxisType1Rdo();
	afx_msg void OnClickLocalAxisType2Rdo();
	afx_msg void OnClickLocalAxisXRdo();
	afx_msg void OnClickLocalAxisYRdo();
	afx_msg void OnSelchangeCoordDirCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ELEMPLATEREDUFACTORDLG_H__)
