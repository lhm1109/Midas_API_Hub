#if !defined(__CMNLACDLG_H__)
#define __CMNLACDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMNlacDlg.h : header file
//

#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_EditUnit.h"
#include "../wg_db/wg_db_TextUnit.h"

#include "../wg_db/FormulaEditUnitSpin.h"
#include "../MIT_frx/MComboBox.h"
#include "../MIT_frx/MButton.h"
#include "../wg_base/SpinBtnExCtrl.h"
#include "../wg_base/MouseEdit.h"

#include "CMDlgBase.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMNlacDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMNlacDlg : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CCMNlacDlg(CWnd* pParent = NULL);   // standard constructor
	void Data2Dlg();
	void AlignControls();
	BOOL Dlg2Data(T_NLCT_D& data);  
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual BOOL DestroyWindow();
	virtual void PostNcDestroy();

// Dialog Data
	//{{AFX_DATA(CCMNlacDlg)
	enum { IDD = IDD_ETC_NLAC };
	CTextUnit	m_maxdispunit;
	CFormulaEditSpin	m_maxiter;
	CEditUnit	m_maxdisp;
	CFormulaEditSpin	m_maxincr;
	CFormulaEditUnitSpin	m_initforce;
	CFormulaEditSpin	m_maximum;
	CFormulaEditSpin	m_number;
	CFormulaEditSpin m_DispCtrlMaxIter;
	CEditUnit m_DispCtrlMaxDisp;
	CTextUnit m_DispCtrlMaxDispUnit;
	CFormulaEditSpin m_DispCtrlDispStep;
	CMouseEdit m_DispCtrlMasterNode;
	MComboBox m_DispCtrlDirectionCmb;

	CListCtrl m_ListNewton;
	CListCtrl m_ListArclen;
	CListCtrl m_ListDispct;
	MButton		m_engergyChk;
	MButton		m_forceChk;
	MButton		m_dispChk;
	CEditUnit	m_forceEdit;
	CEditUnit	m_engergyEdit;
	CEditUnit	m_dispEdit;
	int		m_nMethod;
	MButton   m_chkGeom;
	MButton   m_chkMatl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMNlacDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetItemNewton(T_NLCT_NEWTON& DataNewton);
	void SetItemArclen(T_NLCT_ARCLEN& DataArclen);
	void SetItemDispct(T_NLCT_DISPCT& DataDispct);
	void SetDataNewton(T_NLCT_NEWTON& DataNewton);
	void SetDataArclen(T_NLCT_ARCLEN& DataArclen);
	void SetDataDispct(T_NLCT_DISPCT& DataDispct);

	void UpdateNewtonList(const T_NLCT_NEWTON& DataNewton);
	void UpdateArclenList(T_NLCT_ARCLEN& DataArclen);
	void UpdateDispctList(T_NLCT_DISPCT& DataDispct);

protected:
	CDBDoc* m_pDoc;
	T_NLCT_D m_Data;
	CArray<UINT, UINT> m_aCtrlNewton, m_aCtrlDispCtrl, m_aCtrlArcLeng;
	CArray<T_NLCT_NEWTON, T_NLCT_NEWTON&> m_arNewton;
	CArray<T_NLCT_ARCLEN, T_NLCT_ARCLEN&> m_arArclen;
	CArray<T_NLCT_DISPCT, T_NLCT_DISPCT&> m_arDispct;
	
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStrNewton(int i, T_NLCT_NEWTON &data);
	CString DataToStrArclen(int i, T_NLCT_ARCLEN &data);
	CString DataToStrDispct(int i, T_NLCT_DISPCT &data);    
	int GetInteractionMethodByName(CString strMethod);

	// Generated message map functions
	//{{AFX_MSG(CCMNlacDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeMethod();
	afx_msg void OnCmdNlacEngergyChk();
	afx_msg void OnCmdNlacDispChk();
	afx_msg void OnCmdNlacForceChk();
 	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdNlacRemove();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBtnGeom();
	afx_msg void OnCmdBtnMatl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMNLACDLG_H__)
