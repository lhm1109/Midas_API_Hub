#if !defined(AFX_DGNSTLRATINGASSESSMENTREPORTDLG_H__)
#define AFX_DGNSTLRATINGASSESSMENTREPORTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRationPrintDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingAssessmentReportDlg dialog

class CDgnStlRatingAssessmentReportDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlRatingAssessmentReportDlg(CWnd* pParent = NULL);   // standard constructor

	void Initial_SelectItem();
	void Initial_Data();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	enum { IDD = IDD_DGN_STL_RATING_ASSESSMENT_RPT_DLG };

	int		m_nOption;
	int 	m_nElemType;
	mit::frx::MButton	m_ratioI;
	mit::frx::MButton	m_ratioJ;
	mit::frx::MButton	m_ratioIJ;

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnAr();
	afx_msg void OnDgnDel();
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc* m_pDoc;
	BOOL SetEnableByDgnCode();
	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();
};

#endif // !defined(AFX_DGNSTLRATINGASSESSMENTREPORTDLG_H__)
