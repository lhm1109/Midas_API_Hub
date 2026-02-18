
#if !defined(AFX_DgnRCRatingOptionDlg_H__)
#define AFX_DgnRCRatingOptionDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSteelRatingOptionDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;

class CDgnRCRatingOptionDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnRCRatingOptionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDgnRCRatingOptionDlg();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DGN_RC_RATING_OPTION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// Controls ID
	CArray<UINT, UINT> m_aStrain_I;
	CArray<UINT, UINT> m_aStrain_J;
	CArray<UINT, UINT> m_aDispl_I;
	CArray<UINT, UINT> m_aDispl_J;
	CArray<UINT, UINT> m_aImpact_I;
	CArray<UINT, UINT> m_aImpact_J;
	CArray<UINT, UINT> m_aAddResponceK_I;
	CArray<UINT, UINT> m_aAddResponceK_J;
	CArray<UINT, UINT> m_aEtc;


	void InitUnit();
	void InitSetiing(); // Member 변수, Controls ID 
	void InitText();    // Controls Text 수정
	void InitDefaultValue();
	void AlignControl();
	void EnableDisableControls();
	void ControlsShowHide();	

	// Generated message map functions
	//{{AFX_MSG(CDgnSteelRatingOptionDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnExecute();
// 	afx_msg void OnDgnClose();
	afx_msg void OnDgnArDelRdo();
	afx_msg void OnDgnPositionRdo();

public:

protected:
	CDBDoc* m_pDoc;
	int  m_nRatingCode;
	BOOL m_bShowDispl;

	int		    m_nOption;
	int       m_nPosition;
	CEditUnit	m_edtDisplI;
	CTextUnit	m_untDisplI;
	CEditUnit	m_edtDisplJ;
	CTextUnit	m_untDisplJ;	
	CEditUnit	m_edtStrainI;
	CEditUnit	m_edtStrainJ;
	CEditUnit	m_edtImpactI;
	CEditUnit	m_edtImpactJ;
	CEditUnit m_edtK_I;
	CEditUnit m_edtK_J;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnRCRatingOptionDlg_H__)