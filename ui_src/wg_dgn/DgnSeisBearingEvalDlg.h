#if !defined(__DGNSEISBEARINGEVALDLG_H__)
#define __DGNSEISBEARINGEVALDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisBearingEvalDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "DgnMPhiCurveWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingEvalDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisBearingEvalDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisBearingEvalDlg(CWnd* pParent = NULL);   // standard constructor
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Dialog Data
	//{{AFX_DATA(CDgnMPhiCurveConditionDlg)
	enum { IDD = IDD_DGN_SEIS_BEARING_EVAL_DLG };

	MEdit     m_edtName;
	MComboBox m_cmbEvalGrup;

// 	int       m_nBearing;
// 	MComboBox m_cmbStlBearing;
// 	CEditUnit m_edtds;
// 	CTextUnit m_untds;
// 	CEditUnit m_edtCapaL;
// 	CTextUnit m_untCapaL;
// 	CEditUnit m_edtCapaT;
// 	CTextUnit m_untCapaT;
// 	CEditUnit m_edtDispL;
// 	CTextUnit m_untDispL;
// 	CEditUnit m_edtDispT;
// 	CTextUnit m_untDispT;
// 
	CEditUnit m_edt2wFix;
	CEditUnit m_edt2wMov;
	CEditUnit m_edt1wLMov;
	CEditUnit m_edt1wTMov;
	CListCtrl m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisBearingEvalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitUnit();
	void InitCombo();

	void EnableDisableControls(int nItem);

	void Data2Dlg(T_BREV_D &Data);
	BOOL Dlg2Data(T_BREV_D &Data);

	void UpdateName();

	void SetHeaderTitle();
	void MakeItemEx();

	int  FindInsertionPos(int nID);	
	BOOL InsertItem(T_BREV_K Key, T_BREV_D &Data);
	BOOL DeleteItem(T_BREV_K Key, T_BREV_D &Data);
	BOOL ModifyItem(T_BREV_K KeyOld, T_BREV_K Key, T_BREV_D &Data);
	CString DataToStr(int i, T_BREV_K Key, T_BREV_D &Data);
	BOOL CheckData(T_BREV_K Key);
	
	CDBDoc* m_pDoc;
	
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisBearingEvalDlg)
	virtual BOOL OnInitDialog();
 	afx_msg void OnSeisBearingEvalEvgpCmb();
// 	afx_msg void OnSeisBearingEvalBearingRdo();
// 	afx_msg void OnSeisBearingEvalStlBearingCmb();
 	afx_msg void OnSeisBearingEvalAddBtn();
 	afx_msg void OnSeisBearingEvalModBtn();
 	afx_msg void OnSeisBearingEvalDelBtn();
 	afx_msg void OnSeisBearingEvalCloseBtn();
 	afx_msg void OnSeisBearingEvalDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISBEARINGEVALDLG_H__)
