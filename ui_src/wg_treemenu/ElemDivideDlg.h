#if !defined(__ELEMDIVIDEDLG_H__)
#define      __ELEMDIVIDEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemDivideDlg.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CElemDivideDlg dialog
typedef CArray<double, double> CDblArray;

class  _ElemDivide
{
public:
		int  m_nElemType;
		int  m_nOption;

		int  m_nDivision[3];
		CDblArray m_List[3];

		CArray<UINT,UINT> m_aEdgeNode;

		UINT m_nElemNo;
		UINT m_nNodeNo;

		BOOL m_bMergeDupNode;
		BOOL m_bSubDivideFrame;

		int nPatternType;   // For by Pattern
		int nBaseEdge; 
		int nBasePoint;
};


class CElemDivideDlg : public CMenuBarChildDlg
{
public:
	CArray<UINT,UINT> m_ElemTypeCtrl;
	CArray<UINT,UINT> m_EqCtrl;
	CArray<UINT,UINT> m_EqCtrlY;
	CArray<UINT,UINT> m_EqCtrlZ;
	CArray<UINT,UINT> m_UeqCtrl;
	CArray<UINT,UINT> m_UeqCtrlY;
	CArray<UINT,UINT> m_UeqCtrlZ;
	CArray<UINT,UINT> m_ParamCtrl;
	CArray<UINT,UINT> m_ParamCtrlY;
	CArray<UINT,UINT> m_ParamCtrlZ;
	CArray<UINT,UINT> m_PBraceCtrl;
	CArray<UINT,UINT> m_ByNodeCtrl;
	CArray<UINT,UINT> m_PlaneSolidCtrl;

	CArray<UINT,UINT> m_PatternCtrl; //added by water tree.
	CArray<UINT,UINT> m_PatternEdgeCtrl;
	CArray<UINT,UINT> m_PatternPointCtrl;
	CImageList* m_pImageList;

// Construction
public:
	CElemDivideDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	void DivideElem(_ElemDivide& ED);
	BOOL GetParameter(_ElemDivide& ED);

	BOOL GetNumOfDiv(CEdit* pEdit, int &nDiv);
	BOOL GetUeqDistance(CEdit* pEdit, CDblArray &aDistList);
	BOOL GetParamRatio(CEdit* pEdit, CDblArray &aRatioList);

	int DivPatternModeCheck;

	void CheckMode(BOOL bCheck);
	void InitValue();

	void AlignControls();

// Dialog Data
	//{{AFX_DATA(CElemDivideDlg)
	enum { IDD = IDD_TM_ELEM_DIVIDE };
	MComboBox	m_cboPatternPoint;
	MComboBox	m_cboPatternEdge;
	CComboBoxEx	m_cboPicture;
	CMouseEdit	m_wndByNodeNode;
	CMouseEdit	m_wndByNodeElem;
	CMouseEdit m_wndEdgeEdit;
	CFormulaEditSpin m_wndNumOfDivPB;
	CFormulaEditSpin m_wndNumOfDivX;
	CFormulaEditSpin m_wndNumOfDivY;
	CFormulaEditSpin m_wndNumOfDivZ;
	CTextUnit	m_wndUeqDistUnitX;
	CTextUnit m_wndUeqDistUnitY;
	CTextUnit m_wndUeqDistUnitZ;
	MEdit	m_wndUeqDistX;
	MEdit	m_wndUeqDistY;
	MEdit	m_wndUeqDistZ;
	MEdit	m_wndRatioX;
	MEdit	m_wndRatioY;
	MEdit	m_wndRatioZ;
	BOOL	m_bMergeDupNode;
	BOOL	m_bSubDivideFrame;
	int		m_nOption;
	int   m_nElemType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemDivideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void CtrlEnableDisableEQ(BOOL bEnable);
	void CtrlEnableDisableUEQ(BOOL bEnable);
	void CtrlEnableDisablePUEQ(BOOL bEnable);
	void CtrlEnableDisablePBrace(BOOL bEnable);
	void CtrlEnableDisableByNode(BOOL bEnable);
	void CtrlEnableDisableOption(BOOL bEnable);
	void CtrlEnableDisableByPattern(BOOL bEnable);
	void EnableDisablePatternCtrl(BOOL flag1, BOOL flag2); 
	void SetupImageList();
	int GetEdgeorPoint(); 

	void MakeWallDataByLocalAxis(_ElemDivide& ED);

	// Generated message map functions
	//{{AFX_MSG(CElemDivideDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmEqualDist();
	afx_msg void OnTmUnequalDist();
	afx_msg void OnTmParamDist();
	afx_msg void OnDeltaposTmNumofdivSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmElemType();
	afx_msg void OnTmTolerance();
	afx_msg void OnTmParallelBracing();
	afx_msg void OnTmByNode();
	afx_msg void OnTmByPattern();
	afx_msg void OnSelchangeTmPatternPicture();
	afx_msg void OnSelchangeTmPatternEdgeCbo();
	afx_msg void OnSelchangeTmPatternPointCbo();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMDIVIDEDLG_H__E3A50C05_6049_11D3_8899_0000C0A2329D__INCLUDED_)
