#if !defined(__CMMvhlSItemUsrLaneFactor_H__)
#define __CMMvhlSItemUsrLaneFactor_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlSItemUsrLaneFactor.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"

class CDBDoc;
class CCMMvhlSItemUsrLaneFactor: public CDialogMove
{
public:
	CCMMvhlSItemUsrLaneFactor(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlSItemUsrLaneFactor();

	enum { IDD = IDD_CMD_ML_MVLD_SITEM_BS_USER_FACTOR};

	void SetData(T_MVHL_D& Data);
	T_MVHL_D& GetData();;

protected: 
	// 함수(컨트럴)
	afx_msg void OnOk();
	DECLARE_MESSAGE_MAP()

	// 함수(상속)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// 함수(일반)
	void SetControl();
	BOOL Dlg2Data();
	void Data2Dlg();

	// 변수(컨트럴)
	CEditUnit m_edtLaneFact1;
	CEditUnit m_edtLaneFact2;
	CEditUnit m_edtLaneFact3;
	CEditUnit m_edtLaneFact4;

	// 변수(일반)
	CDBDoc* m_pDoc;
	T_MVHL_D m_Data;
};

#endif // !defined(__CMMvhlSItemUsrLaneFactor_H__)


