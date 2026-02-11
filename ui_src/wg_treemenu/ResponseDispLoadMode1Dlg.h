
#pragma once

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode1Dlg dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_base\DlgChild.h"

struct Sspring;


class ResponseDispLoadMode1Dlg : public CMenuBarChildDlg
{
// Construction
public:
	ResponseDispLoadMode1Dlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(ResponseDispLoadMode1Dlg)
	enum { IDD = IDD_TM_RESPONS_DISP_LOAD_MODE1 };
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResponseDispLoadMode1Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	// Generated message map functions
	//{{AFX_MSG(ResponseDispLoadMode1Dlg)
	virtual BOOL OnInitDialog();
	virtual void Execute() override { OnBnClickedTmExecute(); }
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnBnClickedTmExecute();
		afx_msg void OnBnClickedTmClose();
		afx_msg void OnBnClickedCheckRsmm1Cs();
		afx_msg void OnBnClickedCheckRsmm1Ss();
private:
		BOOL GetParameter_Compress(Sspring& Sspr);
		BOOL GetParameter_Shear   (Sspring& Sspr);
		int  GetDirection();
		int  GetElemType();
		void InitialData();
		T_KEY MakeGroup(CDBDoc *pDoc, LPCTSTR strName);
		CTextUnit m_UnitRat;
		CTextUnit m_UnitLen;
		CTextUnit m_UnitKx ;
		CTextUnit m_UnitKy ;
		CTextUnit m_UnitKz ;
		CTextUnit m_UnitWidth;

		CEditUnit m_EditRat;
		CEditUnit m_EditLen;
		CEditUnit m_EditKx ;
		CEditUnit m_EditKy ;
		CEditUnit m_EditKz ;
		CEditUnit m_EditWidth;
public:
		afx_msg void OnCbnSelchangeComboRsmm1Elemtype();
};
