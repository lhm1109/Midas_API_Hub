#if !defined(__CMADDIMPFDLG_CPP__)
#define __CMADDIMPFDLG_CPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMlsrDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxThfc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\DB_ST_DT_MOVE.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MCheckListBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CMAddImpfDlg dialog

#include "CMChildBarBase.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMAddImpfDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CMAddImpfDlg();           // protected constructor used by dynamic creation
//	DECLARE_DYNCREATE(CMAddImpfDlg)

//----------------------------------------------------------------
// Overridables
public:
	void AlignControls();
//----------------------------------------------------------------
// Implementations
protected:
	T_IMPF_D m_Data;
	BOOL m_chkGrpBeamPart[5];
	BOOL m_chkGrpBeamComp[6];
	BOOL m_chkGrpPlatePart[5];
	BOOL m_chkGrpPlateComp[8];

	CArray<UINT , UINT> m_aCtrlOption;
	CArray<UINT , UINT> m_aCtrlEslImpf;
	CArray<UINT , UINT> m_aCtrlLaneCheck;
	CArray<UINT , UINT> m_aCtrlElemBeamPart;
	CArray<UINT , UINT> m_aCtrlElemPlatePart;
	CArray<UINT , UINT> m_aCtrlElemTrussPart;
	CArray<UINT , UINT> m_aCtrlUserInput;
	CArray<UINT , UINT> m_aCtrlBeamID;
	CArray<UINT , UINT> m_aCtrlPlateID;
	CArray<UINT , UINT> m_aCtrlTrussID;
	

protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetListItem(const int nType);                // List 의 Item를 Name과 Key를 Setting
	BOOL InsertItem(T_IMPF_K Key, T_IMPF_D &Data);
	int  GetTypeCombIndex(UINT nItem);                // ComBoBox UNIT
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);      
	void UpdateBuffer();
	void MakeItemEx();

public:
	CDBDoc* m_pDoc;

protected:
// Dialog Data
public:
	//{{AFX_DATA(CMAddImpfDlg)
	enum { IDD = IDD_DBAR_ADD_IMPF_DLG };
 	CEditUnit       m_edtEsl;                      // ESL EDIT
	CTextUnit       m_untEsl;                      // ESL EDIT
	CEditUnit       m_edtImpf;                     // IMPF EDIT
	MCheckListBox   m_List;                        // LIST EDIT
	MComboBox	      m_cmbElemType;                 // ComboBox 
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMAddImpfDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);   
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMAddImpfDlg();

	virtual void Execute() { OnApply(); }
	// Generated message map functions
	//{{AFX_MSG(CMAddImpfDlg)	
	virtual BOOL OnInitDialog();                        // 초기화 함수
	afx_msg void OnLineSurfaceRdo();                  // Lane Select (Line , Surface) 함수  
	afx_msg void OnAddReplaceRdo();                   // Add/Replace Option 함수
	afx_msg void OnEslImpfRdo();                      // Select Esl or Impf 함수
	afx_msg void OnUserInputRdo();                    // User Input or Auto Calculation
	afx_msg void OnSelchangeElemTypeCmb();            // Element Type ComboBox
	virtual void OnApply();                           //Apply 함수 
	virtual void OnClose();                           //Close 함수
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMADDIMPFDLG_CPP__)
