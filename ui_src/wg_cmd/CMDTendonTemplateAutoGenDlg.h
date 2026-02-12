#pragma once

#include "CMDlgBase.h"
#include "DgnTendonTemplateCtrl.h"

#include "..\wg_db\CobxTdgr.h"
#include "..\wg_db\CobxTdnt.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\EditUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

struct T_AUTOGEN_DATA
{
	BOOL     bReset;
	T_TDNT_K tdntKey;
	T_TDGR_K tdgrKey;
	CString  namePrefix;
	CString  strCode;
	CString  strT;
	CString  strN;
	double   dOrigin[3];

	T_AUTOGEN_DATA()
	{
		bReset = TRUE;
		tdntKey = 0;
		tdgrKey = 0;
		namePrefix.Empty();
		strCode.Empty();
		strT.Empty();
		strN.Empty();
		dOrigin[0] = 0.0;
		dOrigin[1] = 0.0;
		dOrigin[2] = 0.0;
	}
};

class CDBDoc;
class CCMDTendonTemplateDlg;
class CCMDTendonTemplateAutoGenDlg : public CCMDlgBase
{

public:
	CCMDTendonTemplateAutoGenDlg(CDgnTendonTemplateCtrl* pCtrl, CWnd* pParent = NULL);
	virtual ~CCMDTendonTemplateAutoGenDlg();
	
	enum { IDD = IDD_CMD_TENDON_TEMPLATE_AUTOGEN};
	CString		m_strPrefixName;

	//현재 단위계로만 처리됨.
	BOOL GetXYPos(double* dPos);
	void SetXYPos(double* dPos);

protected:
	void ComboBoxDynamicChange(CComboBox* pCombo);
	void SetControl();
	void SetSectType(const CString& strType);
	void SetSectName(const CString& strName);

	void AutoAdjustComboWidth(CComboBox& Combo);

	BOOL Dlg2Data();

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnChangeOrigin();
	afx_msg void OnChangeCode();
	afx_msg void OnChangeType();

	afx_msg void OnCmdProp();
	afx_msg void OnCmdGrp();

protected:	
	CDgnTendonTemplateCtrl*	m_pTemplateCtrl;
	CCMDTendonTemplateDlg*  m_pParentDlg;
	CDBDoc*					m_pDoc;
	T_AUTOGEN_DATA          m_Data;
	MEdit		  m_edtNamePrefix;
	MButton		m_chkInit;
	CCobxTdgr	m_wndTdgrCobx;
	CCobxTdnt	m_wndTdntCobx;
	MComboBox m_wndCobxCode;
	MComboBox m_wndCobxType;
	MComboBox m_wndCobxName;
	CTextUnit m_stcOrigin;
	MEdit     m_edtOrigin;

	DECLARE_MESSAGE_MAP()
};