// (060123) sshan Create

#if !defined(AFX_CONCSECTIONCREATEREBAR_J_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
#define AFX_CONCSECTIONCREATEREBAR_J_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConcSectionCreateRebar_J.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateRebar_J dialog


#include "..\wg_base\ChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\DB_ST_DN.h"

//#include "..\wg_gr\GMEdit_Position.h"

#include "CDgnCreateRebarGrid_J.h"

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

#include "oncSectionRebarViewDlg.h"

#include "DgnStruct.h"

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
class I_DlgViewPanel_MouseEdit;
class I_DlgViewPanel;
class CConcSectionMainRebar;
using namespace mit::frx;
//class ConcSectionRebarViewDlg;
class __MY_EXT_CLASS__ CConcSectionCreateRebar_J : public CChildDialog
{
protected:
 I_DlgViewPanel * m_pIDVP;
// Construction
public:
	CConcSectionCreateRebar_J(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConcSectionCreateRebar_J();
	void ControlMan(BOOL bInit);
	virtual void UpdateBuffer();

	void SetDlgViewPanel(I_DlgViewPanel* pIDVP);
	void SetCreationType();
// Dialog Data
	//{{AFX_DATA(CConcSectionCreateRebar_J)
	enum { IDD = IDD_DGN_CON_CREATE_REBAR_J };
	MComboBox	m_CobxShearRebarDia;
	MComboBox	m_CobxAreaDBP;
	MComboBox	m_CobxAreaDBL;
	MComboBox	m_CobxAreaDBC;
	CTextUnit	m_wndLengthUnit6;
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
	CFormulaEditSpin m_edtCirTypeNum;
	CString	m_strCirTypeCent;
	CString	m_strCirTypeAngle;
	CString	m_strDiaP;
	CString	m_strDiaL;
	CString	m_strDiaC;
	CString	m_strShearRebarDia;
	CEditUnit	m_dShearRebarXStep;
	int	m_strShearRebarYNum;
	int	m_strShearRebarZNum;

	MButton	m_Ctrl_P_Radio;
	MButton	m_Ctrl_L_Radio;
	MButton	m_Ctrl_C_Radio;

	MEdit	m_Ctrl_P_CenterPoint_Edit;
	CEditUnit	m_Ctrl_P_CenterPoint_Unit;

	MButton	m_Ctrl_Create_Btn;
	MButton	m_Ctrl_Delete_Btn;
	MButton	m_Ctrl_ReDraw_Btn;

	int		m_iType;

	CTextUnit	m_AsUT;
	CEditUnit	m_As;

	CEditUnit m_edtPointBeta;
	CEditUnit m_edtLineBeta;
	CEditUnit m_edtCirBeta;
	//}}AFX_DATA
	 
	
	I_DlgViewPanel_MouseEdit* m_wndPointTypeCentP ;
	I_DlgViewPanel_MouseEdit*	m_wndLineTypeEndP   ;
	I_DlgViewPanel_MouseEdit* m_wndLineTypeStartP ;
	I_DlgViewPanel_MouseEdit*	m_wndCirTypeCent    ;
	I_DlgViewPanel_MouseEdit* m_wndCirTypeStart   ;
	

	CDgnCreateRebarGrid_J* m_pRebarGrid;

	CConcSectionMainRebar* m_pParent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionCreateRebar_J)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


// Implementation

public:
	//DB에 저장할데이터 리스트 (주철근의 모든 정보가 들어있다)
	CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR> m_MabrDataListDB;
	T_REBT_SHBR m_ShbrData;

	void SetInitData(CArray<T_REBT_MABR,T_REBT_MABR&>& MainRebarDataList);

	void SaveDlgData();

	void RemoveMainRebarDataList(REBAR_KEY Key, T_REBT_MABR Data);

	void SetRebarListData(CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR>& m_MDataList);

	void AllControlEnableWindow(BOOL bEnable);	

	void SetAsRebarArea();

protected:

	
	CDBDoc* m_pDoc;

	void SetInitUnit();
	BOOL Dlg2Data();
	BOOL Data2Dlg();

	REBAR_KEY RebarKey;
	T_REBT_MABR m_MabrData;

	//현재 입력한 주철근 데이터를 그리드와 판넬뷰에 보내기전 잠시 저자하는 곳
	CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR> m_MabrDataList;

	CArray<T_REBT_MABR,T_REBT_MABR&> m_arData;

	int addDelMod;

	//판넬뷰에 데이터를 보내줌
	void AddRebarDrawData();
	//직선에서 Z값을 구함
	double GetZ(double Y, double a, double b, double c, double d);

	int GetComboRebarIndex(CString strRebarname);



protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionCreateRebar_J)
	afx_msg void OnClickTypeRadioBtn();
	afx_msg void OnChangePointTypeParam();
	afx_msg void OnChangeLineTypeParam();
	afx_msg void OnChangeCirTypeParam();	
	afx_msg void OnDgnCreateButton();
	afx_msg void OnDgnDeleteButton();
	afx_msg void OnDgnRedoButton();
	afx_msg void OnDgnUndoButton();
	afx_msg void OnUpdateUI(CCmdUI*pCmdUI);
	afx_msg void OnSelchangeDgnRebardbCombo();
	afx_msg void OnOutofmemoryDgnShearYStepSpin(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnEditchangeDgnRebardbComboShear();
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

#endif // !defined(AFX_CONCSECTIONCREATEREBAR_J_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
