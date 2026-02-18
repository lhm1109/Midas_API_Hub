#if !defined(AFX_DGNSTLRATINGASSESSMENTCHECKDLG_H__)
#define AFX_DGNSTLRATINGASSESSMENTCHECKDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnPSCAssessmentOptionDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingAssessmentCheckDlg dialog

class CDgnStlRatingAssessmentCheckDlg : public CMenuBarChildDlg
{
public:
	CDgnStlRatingAssessmentCheckDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DGN_STL_RATING_ASSESSMENT_CHK_DLG };

	int		m_nOption;
	int 	m_nElemType;
	int   m_nPart;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	CDBDoc* m_pDoc;

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;
	
	void InitCtrl();
	void AlignControl();
	void ShowHideControls();
	void EnableDisableControls();
	void Initial_SelectItem();

	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnArDel();
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DGNSTLRATINGPRINTDLG_H__)
