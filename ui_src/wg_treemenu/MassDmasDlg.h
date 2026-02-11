#if !defined(AFX_MASSDMASDLG_H__6E0CDB21_2A7A_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_MASSDMASDLG_H__6E0CDB21_2A7A_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MassDmasDlg.h : header file
//

#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CMassDmasDlg dialog

class CMassDmasDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CMassDmasDlg(CWnd* pParent = NULL);   // standard constructor
	~CMassDmasDlg();
	virtual void Execute() {}
	virtual BOOL ExternalInit(UINT key);

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();

//----------------------------------------------------------------------
// Implementation
protected:
	void AlignControls();
	void ChangePicture(int nMassType);
	void ShowHideCtrl(int nMassType);
	void FillStoryCombo(MComboBox& wndCombo, int nStart);

	BOOL ItemToData(CArray<T_STOR_K, T_STOR_K> &aStorKey, T_DMAS_D &Data);
	void DataToItem(T_DMAS_K Key, T_DMAS_D &Data);

	void SetHeaderTitle();
	void MakeItemEx();

	int  FindInsertionPos(T_DMAS_K Key);
	BOOL InsertItem(T_DMAS_K Key, T_DMAS_D &Data);
	BOOL DeleteItem(T_DMAS_K Key, T_DMAS_D &Data);
	BOOL ModifyItem(T_DMAS_K KeyOld, T_DMAS_K Key, T_DMAS_D &Data);
	void DataToStr(CStringArray &aStrVal, T_DMAS_K Key, T_DMAS_D &Data);
	void UpdateStorNameInList();

protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aPmCtrl;
	CArray<UINT, UINT> m_aLmCtrl;
	CArray<UINT, UINT> m_aAmCtrl;
	CArray<UINT, UINT> m_aCamCtrl;
	CArray<UINT, UINT> m_aTamCtrl;
	CArray<UINT, UINT> m_aRamCtrl;
	int       m_nOldSel;

public:
// Dialog Data
	//{{AFX_DATA(CMassDmasDlg)
	enum { IDD = IDD_TM_MASS_DMAS };
	CListCtrl	m_List;
	CTextUnit	m_wndRamD2Unit;
	CEditUnit	m_wndRamD2Edit;
	CTextUnit	m_wndRamD1Unit;
	CEditUnit	m_wndRamD1Edit;
	CTextUnit	m_wndRamXyUnit;
	CMouseEdit	m_wndRamXyEdit;
	CTextUnit	m_wndRamMaUnit;
	CEditUnit	m_wndRamMaEdit;
	CTextUnit	m_wndTamPt3Unit;
	CMouseEdit	m_wndTamPt3Edit;
	CTextUnit	m_wndTamPt2Unit;
	CMouseEdit	m_wndTamPt2Edit;
	CTextUnit	m_wndTamPt1Unit;
	CMouseEdit	m_wndTamPt1Edit;
	CTextUnit	m_wndTamMaUnit;
	CEditUnit	m_wndTamMaEdit;
	CTextUnit	m_wndCamD4Unit;
	CEditUnit	m_wndCamD4Edit;
	CTextUnit	m_wndCamD3Unit;
	CEditUnit	m_wndCamD3Edit;
	CTextUnit	m_wndCamD2Unit;
	CEditUnit	m_wndCamD2Edit;
	CTextUnit	m_wndCamD1Unit;
	CEditUnit	m_wndCamD1Edit;
	CTextUnit	m_wndCamXyUnit;
	CMouseEdit	m_wndCamXyEdit;
	CTextUnit	m_wndCamMaUnit;
	CEditUnit	m_wndCamMaEdit;
	CTextUnit	m_wndAmD3Unit;
	CEditUnit	m_wndAmD3Edit;
	CTextUnit	m_wndAmD2Unit;
	CEditUnit	m_wndAmD2Edit;
	CTextUnit	m_wndAmD1Unit;
	CEditUnit	m_wndAmD1Edit;
	CTextUnit	m_wndAmXyUnit;
	CMouseEdit	m_wndAmXyEdit;
	CTextUnit	m_wndAmMlUnit;
	CEditUnit	m_wndAmMlEdit;
	CTextUnit	m_wndLmXyUnit;
	CMouseEdit	m_wndLmXyEdit;
	CTextUnit	m_wndLmD1Unit;
	CEditUnit	m_wndLmD1Edit;
	CTextUnit	m_wndLmMlUnit;
	CEditUnit	m_wndLmMlEdit;
	CTextUnit	m_wndPmXyUnit;
	CMouseEdit	m_wndPmXyEdit;
	CTextUnit	m_wndPmMpUnit;
	CEditUnit	m_wndPmMpEdit;
	CTextUnit	m_wndPmMaUnit;
	CEditUnit	m_wndPmMaEdit;
	MComboBox	m_wndEndStor;
	MComboBox	m_wndStartStor;
	MComboBox	m_wndMassType;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMassDmasDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMassDmasDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTmMassType();
	afx_msg void OnSelchangeTmStartStor();
	afx_msg void OnTmStorBtn();
	afx_msg void OnTmAdd();
	afx_msg void OnTmModify();
	afx_msg void OnTmDelete();
	afx_msg void OnTmClose();
	afx_msg void OnItemchangedTmList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASSDMASDLG_H__6E0CDB21_2A7A_11D4_92DE_0000C0B0E6B3__INCLUDED_)
