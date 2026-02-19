#if !defined(__DgnCRCAnnexDgnParameter_BS_Dlg_H__)
#define __DgnCRCAnnexDgnParameter_BS_Dlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCAnnexDgnParameter_BS_Dlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

class CDgnCRCAnnexDgnParameter_BS_Dlg : public CDialogMove
{
public:
	CDgnCRCAnnexDgnParameter_BS_Dlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnCRCAnnexDgnParameter_BS_Dlg();

	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_BS_ANNEX };

	CEditUnit m_edtCsTndn;
	CEditUnit m_edtCsConc;
	CEditUnit m_edtSfTriCmp;
	CEditUnit m_edtSfUniCmp;
	CEditUnit m_edtSfPreTens;
	CEditUnit m_edtSfPostTens;
	BOOL      m_bIncrevtu;
	CEditUnit m_edtPreTensMemb;
	CTextUnit m_untPreTensMemb;
	CEditUnit m_edtPostTensMemb;
	CTextUnit m_untPostTensMemb;

public:
	T_PSCD_D* m_pParamData;

	CDBDoc* m_pDoc;

	void InitUnit();
    void EnableDisableControls();

	void Data2Dlg();
	BOOL Dlg2Data();

protected: 
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCRCAnnexDgnParameter_BS_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnCRCAnnexDgnParameter_BS_Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();	
	afx_msg void OnResetDgnParam_byAnnex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(__DgnCRCAnnexDgnParameter_BS_Dlg_H__)


