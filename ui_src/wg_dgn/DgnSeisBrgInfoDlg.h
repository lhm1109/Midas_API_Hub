#if !defined(__DgnSeisBrgInfoDlg_H__)
#define __DgnSeisBrgInfoDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisBrgInfoDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"


using namespace mit::frx;

class CDBDoc;
#include "HeaderPre.h" 
class __MY_EXT_CLASS__ CDgnSeisBrgInfoDlg: public CDialogMove
{
public:
	CDgnSeisBrgInfoDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnSeisBrgInfoDlg();

	enum { IDD = IDD_DGN_SEIS_BRG_INFO_DLG};

protected: 
	// 함수(컨트럴)
	afx_msg void OnOpen();
	afx_msg void OnSaveAs();
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnSelchangeCmdMvCode();
	afx_msg void OnSelchangeCmdMvhlStd();
	afx_msg void OnSelchangeForEnable();
	afx_msg void OnSelchangeCmdMvhlType();
	DECLARE_MESSAGE_MAP()

	// 함수(상속)    
	
	virtual BOOL OnInitDialog();  
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// 함수(일반)
	void SetControl();
	BOOL Dlg2Data();
	void Data2Dlg();
	void EnableCtrl();
	void SetMvCodeCbx();
	void SetMvhlStdCbx();
	void SetMvhlStdCbxEuroBS();
	void SetMvhlTypeCbx();
	void SetLoadTypeListEuroBS(int nIndex);
	void SetLoadTypeList(int nIndex);
	void SetMvhlNameCbx();
	void AdjustComboListBox(CComboBox& Cbx) /* 콤보박스의 길이를 현재 사용되고 있는 리스트의 길이에 맞추어 변경한다. */;
	void SetDefVal();

	// 변수(컨트럴)
	MEdit m_edtBrgName;
	MEdit m_edtLocation;
	MEdit m_edtManager;
	MEdit m_edtstrLineNum;
	MEdit m_edtOrgLoc;
	CEditUnit m_edtBuildYear;
	CEditUnit m_edtTotalLen;
	MEdit m_edtSpan;
	CEditUnit m_edtBrgWidth;
	MEdit m_edtUpper;
	MEdit m_edtAbutmentType;
	MEdit m_edtAbutmentBase;
	MEdit m_edtPierType;
	MEdit m_edtPierBase;
	MEdit m_edtSupportMatl;
	CEditUnit m_edtSupportCapaAbut;
	CEditUnit m_edtSupportCapaPier;
	MEdit m_edtPierSect;
	
	CTextUnit m_untTotalLen;
	CTextUnit m_untSpan;
	CTextUnit m_untBrgWidth;
	CTextUnit m_untSupportCapaAbut;
	CTextUnit m_untSupportCapaPier;

	MComboBox m_cbxSeismicClass;
	MComboBox m_cbxEarthquake;
	MComboBox m_cbxMvCode;
	MComboBox m_cbxMvhlStd;
	MComboBox m_cbxMvhlType;
	MComboBox m_cbxMvhlName;
	MComboBox m_cbxUpper;
	MComboBox m_cbxAbutmentType;
	MComboBox m_cbxAbutmentBase;
	MComboBox m_cbxPierType;
	MComboBox m_cbxPierBase;
	MComboBox m_cbxSupportMatl;
	MComboBox m_cbxPierSect;
	MComboBox m_cbxSpfc;
	
	// 변수(일반)
	CDBDoc* m_pDoc;
	T_SBRI_D m_Data;
};
#include "HeaderPost.h"

#endif // !defined(__DgnSeisBrgInfoDlg_H__)


