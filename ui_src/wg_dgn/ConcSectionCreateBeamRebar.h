#if !defined(__CONCSECTIONCREATEBEAMREBAR_H__)
#define __CONCSECTIONCREATEBEAMREBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConcSectionCreateBeamRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateBeamRebar dialog
#include "..\wg_base\ChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\DB_ST_DN.h"

#include "CDgnCreateBeamRebarGrid.h"
#include "ConcSectionBeamRebarViewDlg.h"

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MComboBox.h"

#include "DgnStruct.h"

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
class I_DlgViewPanel_MouseEdit;
class I_DlgViewPanel;
class CConcSectionMainBeamRebar;
using namespace mit::frx;
//class ConcSectionRebarViewDlg;
class __MY_EXT_CLASS__ CConcSectionCreateBeamRebar : public CChildDialog
{
protected:
 I_DlgViewPanel * m_pIDVP;
// Construction
public:
	CConcSectionCreateBeamRebar(CWnd* pParent = NULL, int nCurTab = 0);   // standard constructor
	virtual ~CConcSectionCreateBeamRebar();
	void ControlMan(BOOL bInit);
	virtual void UpdateBuffer();

	void SetDlgViewPanel(I_DlgViewPanel* pIDVP);
	void SetCreationType();
// Dialog Data
	//{{AFX_DATA(CConcSectionCreateBeamRebar)
	enum { IDD = IDD_DGN_CON_CREATE_BEAM_REBAR };

	MComboBox	m_CobxAreaDBP;
	MComboBox	m_CobxAreaDBL;
	MComboBox	m_CobxAreaDBC;
	CTextUnit	m_wndLengthUnit5;
	CTextUnit	m_wndLengthUnit4;
	CTextUnit	m_wndLengthUnit3;
	CTextUnit	m_wndLengthUnit2;
	CTextUnit	m_wndLengthUnit1;
	int		m_nTypeofCreation;
	CString	m_strPointTypeCent;
	CString	m_strLineTypeStartP;
	CFormulaEditSpin m_edtLineTypeNumOfRebar;
	CString	m_strLineTypeEndP;
	CString	m_strCirTypeStartP;	
	CFormulaEditSpin m_editCirTypeNum;
	CString	m_strCirTypeCent;
	CString	m_strCirTypeAngle;
	CString	m_strDiaP;
	CString	m_strDiaL;
	CString	m_strDiaC;
	CTextUnit	m_AsUT;
	CEditUnit	m_As;
	CEditUnit m_edtPointBeta;
	CEditUnit m_edtLineBeta;
	CEditUnit m_edtCirBeta;

	//CMouseEdit	m_wndPointTypeCentP;
	//CMouseEdit	m_wndLineTypeEndP;
	//CMouseEdit	m_wndLineTypeStartP;
	//CMouseEdit	m_wndCirTypeCent;
	//CMouseEdit	m_wndCirTypeStart;
	//}}AFX_DATA
	 
	
	I_DlgViewPanel_MouseEdit* m_wndPointTypeCentP ;
	I_DlgViewPanel_MouseEdit*	m_wndLineTypeEndP   ;
	I_DlgViewPanel_MouseEdit* m_wndLineTypeStartP ;
	I_DlgViewPanel_MouseEdit*	m_wndCirTypeCent    ;
	I_DlgViewPanel_MouseEdit* m_wndCirTypeStart   ;
	

	CDgnCreateBeamRebarGrid* m_pRebarGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionCreateBeamRebar)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


// Implementation

public:
	//DB에 저장할데이터 리스트 (주철근의 모든 정보가 들어있다)
	CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR> m_MabrDataListDB;
	T_REBT_SHBR m_ShbrData;
	int m_nCurTab;

	void SetInitData(CArray<T_RBGC_MABR,T_RBGC_MABR&>& MainRebarDataList);

	void SaveDlgData();

	void RemoveMainRebarDataList(REBAR_KEY Key, T_RBGC_MABR Data);

	void SetRebarListData(CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR>& m_MDataList);

	void SetAsRebarArea();

	void EnableDisableControls(BOOL bSameIMJ);

protected:
	CConcSectionMainBeamRebar* m_pParent;
	CDBDoc* m_pDoc;

	void SetInitUnit();
	BOOL Dlg2Data();
	BOOL Data2Dlg();

	REBAR_KEY RebarKey;
	T_RBGC_MABR m_MabrData;

	//현재 입력한 주철근 데이터를 그리드와 판넬뷰에 보내기전 잠시 저장하는 곳
	CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR> m_MabrDataList;

	CArray<T_RBGC_MABR,T_RBGC_MABR&> m_arData;

	int addDelMod;

	CArray<UINT, UINT> m_arCtrlPType;
	CArray<UINT, UINT> m_arCtrlLType;
	CArray<UINT, UINT> m_arCtrlCType;

	//판넬뷰에 데이터를 보내줌
	void AddRebarDrawData();
	//직선에서 Z값을 구함
	double GetZ(double Y, double a, double b, double c, double d);

	int GetComboRebarIndex(CString strRebarname);
	

protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionCreateBeamRebar)
	afx_msg void OnClickTypeRadioBtn();
	afx_msg void OnChangePointTypeParam();
	afx_msg void OnChangeLineTypeParam();
	afx_msg void OnChangeCirTypeParam();	
	afx_msg void OnDgnCreateButton();
	afx_msg void OnDgnDeleteButton();
	afx_msg void OnSelchangeDgnRebardbCombo();
	afx_msg void OnChangeLineTypeDivNum();
	afx_msg void OnOnChangeCirTypeDivNum();
	afx_msg void OnChangeCirTypeAngle();
	afx_msg void OnDgnRedrawButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONCREATEBEAMREBAR_H__)
