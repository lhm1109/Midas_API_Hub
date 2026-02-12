#pragma once

#include "CMDlgBase.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "DgnTendonTemplateCtrl.h"

class CDBDoc;
class CCMDTendonTemplateDlg;
class CDrawEditBtn_TndnTemplate;
class CCMDTendonTemplateCopyDlg : public CCMDlgBase
{

public:
	CCMDTendonTemplateCopyDlg(CDgnTendonTemplateCtrl* pCtrl, CDrawEditBtn_TndnTemplate* pParentDraw, CWnd* pParent = NULL);
	virtual ~CCMDTendonTemplateCopyDlg();
	
	enum { IDD = IDD_CMD_TENDON_TEMPLATE_COPYDLG};

	BOOL SetPreviewDataDraw();

protected:
	void SetControl();
	void InitData(BOOL bType = TRUE);

	BOOL Dlg2Data();
	BOOL Data2Dlg(); //초기화 하는 용도로 쓴다....

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnChangeRdoButton();
	afx_msg void OnChangeInfo();
	afx_msg void OnDeltaposCmdCopyNum(NMHDR* pNMHDR, LRESULT* pResult);
	
protected:	
	CDBDoc*							m_pDoc;
	CDgnTendonTemplateCtrl*			m_pTemplateCtrl;
	CCMDTendonTemplateDlg*			m_pParentDlg;
	CDrawEditBtn_TndnTemplate*		m_pParentDraw;

	MButton			m_rdoMove;
	MButton			m_rdoCopy;
	CEditUnit		m_edtOffY;
	CEditUnit		m_edtOffZ;
	CTextUnit   m_stcOffY;
	CTextUnit   m_stcOffZ;
	CFormulaEditSpin m_edtCopyNum;

	BOOL		m_bCopy;
	int			m_nCopyNum;
	double		m_dOffsetY;
	double		m_dOffsetZ;
	vecTndnKey	m_vecKeyList;

	DECLARE_MESSAGE_MAP()
};
