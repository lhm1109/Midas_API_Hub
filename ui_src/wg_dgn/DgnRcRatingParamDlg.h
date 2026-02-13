#if !defined(__DGNRCRATINGPARAMDLG_H__)
#define __DGNRCRATINGPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRatingCodeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnRcRatingParamDlg dialog


// CDgnRcRatingParamDlg 대화 상자입니다.

class __MY_EXT_CLASS__ CDgnRCRatingParamDlg : public CDialogMove
{

public:
	CDgnRCRatingParamDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDgnRCRatingParamDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DGN_RC_RATIGN_PARAM };

	CEditUnit m_edtLiveLoadStaType; // _T("DB") or _T("LS")
	CEditUnit m_edtLiveLoadStaNum; 

	int m_nMeasureType; // 0 : Displacement ,  1 : Strain

	double m_dLiveLoadStaType;
	double m_dLiveLoadStaNum;

	BOOL m_bSectForceRpt;
	BOOL m_bStrengthCheckRpt;

	CString m_strLiveLoadType;
	MButton m_chkUserInput;

	CArray<UINT,UINT> m_aLSDCtrl;
	CArray<UINT,UINT> m_aDefaultOutputCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnClickModDgnParam();
	afx_msg void OnCheckUserInputData();

	void InitUnit();
	void ControlsEnableDisable();

	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL GetLiveLoadTypeName();

    T_RPAC_D m_Data;
	
     BOOL InitCtrl();    
     virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};


#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNRCRATINGPARAMDLG_H__)
