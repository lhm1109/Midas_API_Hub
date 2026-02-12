#pragma once

#include "..\wg_base\DialogMove.h"
#include "DgnTendonTemplateCtrl.h"

#include "..\wg_db\CobxTdgr.h"
#include "..\wg_db\CobxTdnt.h"
#include "..\MIT_frx\MButton.h"
using namespace mit::frx;

class CDBDoc;
class CCMDTendonTemplateSetPropDlg : public CDialogMove
{
	
public:
	CCMDTendonTemplateSetPropDlg(CDgnTendonTemplateCtrl* pCtrl, CWnd* pParent = NULL);
	virtual ~CCMDTendonTemplateSetPropDlg();
	
	enum { IDD = IDD_CMD_TENDON_TEMPLATE_SETPROP };
	
	void GetSelectedKeyList(vecTndnKey& keyList);
	void SetSelectedKeyList(const vecTndnKey& keyList, BOOL bSelect);
	
	
protected:
	void GetSelKeyByListBox(const CListBox& listBox, vecTndnKey& keyList);
	void InitAllList();
	void InitSelList();
	void SetControl();
	BOOL IsSelectedProfile(int nKey);
	void ComboBoxDynamicChange(CComboBox* pCombo);
	
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	
	afx_msg void OnCmdInclude();
	afx_msg void OnCmdExclude();
	afx_msg void OnCmdAll();
	afx_msg void OnCmdNone();
	afx_msg void OnCmdProp();
	afx_msg void OnCmdGrp();
	
public:
	
protected:	
	CDgnTendonTemplateCtrl*		m_pTemplateCtrl;
	CDBDoc*						m_pDoc;
	std::map<int, BOOL>			m_mapSelectedKey;
	
	CListBox	m_listSelProfiles;
	CListBox	m_listAllProfiles;
	CCobxTdgr	m_wndTdgrCobx;
	CCobxTdnt	m_wndTdntCobx;
	MButton   m_rdoOutTypeCurve;
	MButton   m_rdoOutTypeElem;
	
	DECLARE_MESSAGE_MAP()
};
