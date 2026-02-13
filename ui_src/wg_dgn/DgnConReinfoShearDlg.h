#if !defined(AFX_DGNCONREINFOSHEARDLG_H__INCLUDED_)
#define AFX_DGNCONREINFOSHEARDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Coded by Seungjun MNet:No.2393 ('20060829)
// DgnConReinfoShearDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

// Coded by Seungjun MNet:No.2393 ('20060915)

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConReinfoShearDlg dialog

class CDgnConReinfoShearDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:


// Construction
public:
	CDgnConReinfoShearDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConReinfoShearDlg)
	enum {	IDD = IDD_DGN_CON_REIN_DLG	};
	int       m_nOption;
	
	BOOL	m_bLongRein;
	BOOL	m_bShorRein;

	CEditUnit    m_edtLongRein;
	CEditUnit    m_edtShorRein;
	CTextUnit    m_untLongRein;
	CTextUnit    m_untShorRein;

	CEditUnit    m_edtShearRatio;

	BOOL	m_bLongAlpha;
	BOOL	m_bShorAlpha;
	int     m_nLongAlpha;
	int     m_nShorAlpha;
	CEditUnit    m_edtLongAlpha;
	CEditUnit    m_edtShorAlpha;


	int		    m_iSeletType;

	int       m_iShortReduction;
	CDBDoc* m_pDoc;

	//}}AFX_DATA

private:
	CArray<UINT, UINT> m_aDataCtrl;
	CArray<UINT, UINT> m_aLongAlpha;
	CArray<UINT, UINT> m_aShorAlpha;

public:
	void Initial_SelectItem();
	void SetIsCpgDesign(BOOL bIsCpgDesign);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConReinfoShearDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Initial_Unit();
	void Initial_Data();
	void UpdateBuffer();
	void Initial_Control();

	// Generated message map functions
	//{{AFX_MSG(CDgnConReinfoShearDlg)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnConOption();

	afx_msg void OnDgnReinLongChk();
	afx_msg void OnDgnReinShorChk();
	afx_msg void OnDgnAlphaLongChk();
	afx_msg void OnDgnAlphaShorChk();
	afx_msg void OnDgnAlphaLongAutoRdo();
	afx_msg void OnDgnAlphaShorAutoRdo();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONREINFOSHEARDLG_H__INCLUDED_)
