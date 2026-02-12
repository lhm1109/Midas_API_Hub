#if !defined(__SEISCVLEVALGRUPDLG_H__)
#define __SEISCVLEVALGRUPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCvlEvalGrupDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_selectctrl.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlEvalGrupDlg dialog

class CSeisCvlEvalGrupDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSeisCvlEvalGrupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CSeisCvlEvalGrupDlg)
	enum { IDD = IDD_TM_SEIS_CVL_EVAL_GRUP };

	int              m_nOption;
	MEdit            m_edtName;
	int              m_nType;
	BOOL             m_bPierCap;
	CSelectEditColor m_edtPierCap;
	MEdit            m_edtColmName;
	CSelectEditColor m_edtColumn;
	CListCtrl	     m_lstColumn;
	BOOL             m_bLink;
	CSelectEdit      m_edtLink;
	int              m_nLink;
	int              m_nAbutPos;
	CSelectEdit      m_edtAbutNode;
	CListCtrl	     m_lstEvalGrup;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlEvalGrupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	void InitControl();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();

	void Dlg2Data();
	void Data2Dlg();

	void SetHeaderTitle_Column();
	void MakeItemEx_Column(T_EVGP_D& Data);
	BOOL InsertItem_Column(int nIndex, T_EVGP_COLM_D& EvgpColm);
	BOOL ModifyItem_Column(int nIndex, T_EVGP_COLM_D& EvgpColm);
	CString DataToStr_Column(int i, T_EVGP_COLM_D &EvgpColm);

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(T_EVGP_K Key, T_EVGP_D& Data);
	BOOL ModifyItem(T_EVGP_K KeyOld, T_EVGP_K Key, T_EVGP_D& Data);
	BOOL DeleteItem(T_EVGP_K Key, T_EVGP_D& Data);	
	CString DataToStr(int i, T_EVGP_K key, T_EVGP_D &Data);
	
	BOOL IsEvalGrupElem(CMouseEdit* pMEdit, T_ELEM_K ElemK);
	void RedrawSelectEditColor();

	void RemoveNodeSelect();
	void RemoveElemSelect();
	void ResetControls();
	void ResetColmColntrols();

	BOOL SeperateColumnData(T_EVGP_COLM_D &EvgpColm, CArray<T_EVGP_COLM_D, T_EVGP_COLM_D>& aEvgpColm);
	BOOL CheckColumnData(T_EVGP_COLM_D &EvgpColm);

	CDBDoc* m_pDoc;

	T_EVGP_D m_Data;

	CArray<UINT, UINT> m_aPier;
	CArray<UINT, UINT> m_aAbut;
	CArray<UINT, UINT> m_aAbut2;

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlEvalGrupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmEvalGrupOptionRdo();
	afx_msg void OnTmEvalGrupTypeRdo();
	afx_msg void OnTmEvalGrupPierCapChk();
	afx_msg void OnTmEvalGrupSetfocusPierCapEdt();
	afx_msg void OnTmEvalGrupColumnNameEdt();
	afx_msg void OnTmEvalGrupSetfocusColumnEdt();  
	afx_msg void OnTmEvalGrupColumnAddBtn();  
	afx_msg void OnTmEvalGrupColumnModBtn();  
	afx_msg void OnTmEvalGrupColumnDelBtn();
	afx_msg void OnTmEvalGrupLinkChk();	
	afx_msg void OnTmEvalGrupSetfocusLinkEdt();
	afx_msg void OnTmEvalGrupLinkTypeRdo();
	afx_msg void OnTmEvalGrupSetfocusAbutNodeEdt();
	//afx_msg void OnTmEvalGrupApplyBtn();
	//afx_msg void OnTmEvalGrupCloseBtn();	
	afx_msg void OnTmEvalGrupColumnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnTmEvalGrupItemChanged(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISCVLEVALGRUPDLG_H__)
