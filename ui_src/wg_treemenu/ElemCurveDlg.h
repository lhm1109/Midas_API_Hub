#if !defined(__ELEMCURVEDLG_H__)
#define __ELEMCURVEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemCurveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElemCurveDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\EditMatl.h"
#include "..\wg_db\EditSect.h"
#include "..\wg_db\CobxMatl.h"
#include "..\wg_db\CobxSect.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

class CElemCurveDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemCurveDlg(CWnd* pParent = NULL);   // standard constructor
	~CElemCurveDlg();
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CElemCurveDlg)
	enum { IDD = IDD_TM_ELEM_CURVE };
	MComboBox	m_cboAngle;
	MButton	m_chkSemiEllipse;
	CTextUnit	m_AngleUnit;
	CFormulaEditSpin	m_edtSeqNum;
	CEditSect	m_SecNoEdit;
	CCobxSect	m_SecNameCbo;
	CCobxMatl	m_MatNameCbo;
	CEditMatl	m_MatNoEdit;
	CTextUnit	m_PointUnit4;
	CTextUnit	m_PointUnit3;
	CTextUnit	m_PointUnit2;
	CTextUnit	m_PointUnit1;
	CMouseEdit	m_PointEdit4;
	CMouseEdit	m_PointEdit3;
	CMouseEdit	m_PointEdit2;
	CMouseEdit	m_PointEdit1;
	MComboBox	m_BAngle;
	CTextUnit	m_BAngleUnit;
	MComboBox	m_CurveType;
	BOOL	m_bIntersectNode;
	BOOL	m_bIntersectElem;
	int		m_nDivOpt;
	int		m_nArcEndPoint;
	int		m_nElementType;
	MillustViewer m_wndPicture;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemCurveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreateElemCurve();
	BOOL GetParameter(T_CURVEDELEM &CurvedElem);
	void CurveTypeCtrlMan();
private:
	CBitmap* m_pBitmap;
	void ChangeBitmap(int i);
	void InitPropertyCtrls();
	void AlignControls();
	void InitEditValue();
	BOOL GetEditPosition(CMouseEdit& edtCtrl, double& Px, double& Py, double& Pz);

	/*********************************************************************
	 *
	 *
 *****  FUNCTION FOR MOUSE EDIT PLUGIN 
	***
	 */
	BOOL SetPlugInParam();
	void EndPlugInParam(); //종료할때... 호출할것...
	
private:
	CArray<UINT,UINT> aCtrlIDAll;
	
	CArray<UINT,UINT> aCtrlID12;
	CArray<UINT,UINT> aCtrlID34;
	CArray<UINT,UINT> aCtrlID5;
	CArray<UINT,UINT> aCtrlID6;

	CArray<UINT,UINT> aCtrlIDBase16;
	CArray<UINT,UINT> aCtrlIDBase7;

	CArray<UINT,UINT> aCtrlAngle;

	T_CURVEDELEM          m_CurvedElem;
	T_CURVEDELEM_ARC3P    m_Arc3p;
	T_CURVEDELEM_ARC2P1C  m_Arc2p1c;
	T_CURVEDELEM_CIR3P    m_Cir3p;
	T_CURVEDELEM_CIR2P1C  m_Cir2p1c;
	T_CURVEDELEM_ELP2P1C  m_Elp2p1c;
	T_CURVEDELEM_PAR3P    m_Par3p;
	T_CURVEDELEM_CUB4P    m_Cub4p;

	BOOL m_bPropInit;
	// Generated message map functions
	//{{AFX_MSG(CElemCurveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTmCurvetypeCbo();
	afx_msg void OnTmMatButton();
	afx_msg void OnTmSecButton();
	afx_msg void OnTmIntstTol();
	afx_msg void OnDeltaposTmCurvesegSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTmAngleSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmEndoptP3Radio();
	afx_msg void OnTmEndoptAngleRadio();
	afx_msg void OnUpdateTmCurvesegEdit();
	afx_msg void OnTmElemcurveSemiellipseChk();
	afx_msg void OnEditchangeTmEndoptAngleCbo();
	afx_msg void OnSelchangeTmEndoptAngleCbo();
	afx_msg void OnEditupdateTmEndoptAngleCbo();
	afx_msg void OnSelendokTmEndoptAngleCbo();
	//}}AFX_MSG

	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)  ;
	afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam)    ;
	afx_msg LRESULT OnPlugInOperationEnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMCURVEDLG_H__EE66B441_4CCA_11D4_9AAF_0000C0B9C58C__INCLUDED_)
