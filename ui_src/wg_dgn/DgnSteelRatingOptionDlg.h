
#if !defined(AFX_DgnSteelRatingOptionDlg_H__)
#define AFX_DgnSteelRatingOptionDlg_H__

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

/////////////////////////////////////////////////////////////////////////////
// CDgnSteelRatingOptionDlg dialog
class CDgnSteelRatingOptionDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnSteelRatingOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	
	enum { IDD = IDD_DGN_STL_RATING_OPTION_DLG };

	int	m_nOption;
	int	m_nElemType;
	int m_nPosition;
	int m_nCalcType;

	CEditUnit	m_edtDisplI;
	CTextUnit	m_untDisplI;
	CEditUnit	m_edtDisplJ;
	CTextUnit	m_untDisplJ;	
	CEditUnit	m_edtStrainI;
	CEditUnit	m_edtStrainJ;
	CEditUnit	m_edtImpact_I;
	CEditUnit	m_edtImpact_J;
	CEditUnit	m_edtAddResponceF_I;
	CEditUnit	m_edtAddResponceF_J;
	CEditUnit m_edtK;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	CDBDoc* m_pDoc;
	int  m_nRatingCode;
	BOOL m_bShowDispl;

	CArray<UINT, UINT> m_aAutoUser;
	CArray<UINT, UINT> m_aStrain_I;
	CArray<UINT, UINT> m_aStrain_J;
	CArray<UINT, UINT> m_aDispl_I;
	CArray<UINT, UINT> m_aDispl_J;
	CArray<UINT, UINT> m_aImpact_I;
	CArray<UINT, UINT> m_aImpact_J;
	CArray<UINT, UINT> m_aAddReponce_I;
	CArray<UINT, UINT> m_aAddReponce_J;
	CArray<UINT, UINT> m_aK;
	CArray<UINT, UINT> m_aEtc;
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

	void InitUnit();
	void InitSetiing(); // Member 변수, Controls ID 
	void InitText();    // Controls Text 수정
	void InitDefaultValue();
	void Initial_SelectItem();
	void AlignControl();
	void EnableDisableControls();
	void ControlsShowHide();

	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnArDelRdo();
	afx_msg void OnDgnPositionRdo();
	afx_msg void OnDgnCalcTypeRdo();
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_DgnSteelRatingOptionDlg_H__)
