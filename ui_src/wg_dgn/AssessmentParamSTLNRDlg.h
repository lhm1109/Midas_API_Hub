#if !defined(__CASSESSMENTPARAMSTLNRDLG_H__)
#define __CASSESSMENTPARAMSTLNRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssessmentParamSTLNRDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CAssessmentParamSTLNRDlg : public CDialogMove
{
public:
	CAssessmentParamSTLNRDlg(CWnd* pParent = NULL);   // standard constructor
	~CAssessmentParamSTLNRDlg();

	enum { IDD = IDD_DGN_ASSESSMENT_PARAM_STL_NR_DLG };

protected:
	afx_msg void OnUser();
	afx_msg void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnChkUser();
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void SetControl();

	void EnableCtrl();
	BOOL Dlg2Data();
	void Data2Dlg();

	int       m_nFomula;
	MComboBox m_cmbReqRouteAvail;
	CEditUnit m_edtCondFactor;
	int       m_nGamma_m;
	int       m_nSpanType;
	CEditUnit m_edtk2;
	CEditUnit m_edtk3;
	CEditUnit m_edtUframeL;
	CTextUnit m_untUframeL;
	BOOL      m_bEqA4;

	BOOL      m_bUser;

	BOOL      m_bUlsFlexure;
	BOOL      m_bUlsShear;
	BOOL      m_bUlsLgShear;
	BOOL      m_bUlsintBearStiff;

	CDBDoc* m_pDoc;
	T_RPSS_D m_Data;
};
#include "HeaderPost.h"    

#endif // !defined(__CASSESSMENTPARAMSTLNRDLG_H__)





