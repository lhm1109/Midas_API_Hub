#pragma once

#include "..\wg_base\DialogMove.h"
#include "DgnTendonTemplateCtrl.h"
#include "..\MIT_frx\MButton.h"

class CCMDTendonTemplateExportDlg : public CDialogMove
{

public:
	CCMDTendonTemplateExportDlg(CDgnTendonTemplateCtrl* pCtrl, CWnd* pParent = NULL);
	virtual ~CCMDTendonTemplateExportDlg();
	
	enum { IDD = IDD_CMD_TENDON_TEMPLATE_EXPORT };

	void GetSelectedKeyList(vecTndnKey& keyList);
	void SetSelectedKeyList(const vecTndnKey& keyList, BOOL bSelect);


protected:
	void GetSelKeyByListBox(const CListBox& listBox, vecTndnKey& keyList);
	void InitAllList();
	void InitSelList();
	BOOL IsSelectedProfile(int nKey);

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnCmdInclude();
	afx_msg void OnCmdExclude();
	afx_msg void OnCmdAll();
	afx_msg void OnCmdNone();

public:
	
protected:	
	CDgnTendonTemplateCtrl*		m_pTemplateCtrl;
	std::map<int, BOOL>			m_mapSelectedKey;

	CListBox	m_listSelProfiles;
	CListBox	m_listAllProfiles;
	mit::frx::MButton		m_chkUseRatio;

	DECLARE_MESSAGE_MAP()
};
