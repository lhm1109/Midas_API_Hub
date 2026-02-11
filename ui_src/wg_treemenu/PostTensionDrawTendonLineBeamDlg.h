#if !defined(__POSTTENSIONDRAWTENDONLINEBEAM__)
#define __POSTTENSIONDRAWTENDONLINEBEAM__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_selectctrl.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\NotifyCtrl.h"

class CPostTensionDrawTendonLineBeamDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CPostTensionDrawTendonLineBeamDlg(CWnd* pParent = NULL);
	virtual void Execute();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	enum { IDD = IDD_TM_TENDON_LINE_BEAM };

	mit::frx::MEdit m_edtTendonNum;
	// Tendon Property
	CStatic m_stcTendonProp;
	mit::frx::MComboBox m_cmbTendonProp;
	mit::frx::MButton m_btnTendonProp;
	// Tendon Property
	CStatic m_stcTendonProf;
	mit::frx::MComboBox m_cmbTendonProf;
	mit::frx::MButton m_btnTendonProf;
	// Tendon Stressing
	CStatic m_stcTendonStrs;
	mit::frx::MComboBox m_cmbTendonStrs;
	mit::frx::MButton m_btnTendonStrs;
	int m_nEndType;
	int m_nEndType2;
	CSelectEdit m_edtElem;

	T_TDBM_D m_Data;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void Dlg2Data();

	CDBDoc* m_pDoc;

	virtual BOOL OnInitDialog();
	afx_msg void OnClickTendonPropBtn();
	afx_msg void OnClickTendonProfBtn();
	afx_msg void OnClickTendonStrsBtn();
	afx_msg void OnEndTypeRadioClicked(); // 라디오 버튼 클릭 핸들러 선언 추가
	DECLARE_MESSAGE_MAP()

private:
	void UpdateTendonPropertyList();
	void UpdateTendonProfileList();
	void UpdateTendonStressingList();
};

#endif