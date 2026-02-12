#pragma once


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MCoolButton.h"
#include "..\mit_frx\MComboBox.h"
using namespace frx;

// 프로그램 상에서 이름이 Insert Story by Height 로 바뀌었습니다.
class CCMStorAddDlg : public CDialogMove
{
	DECLARE_MESSAGE_MAP()
public:
	CCMStorAddDlg(CWnd* pParent = NULL);
	virtual ~CCMStorAddDlg(void);

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();

private:
	void InitCtrl();

	BOOL AddStory();

public:
	enum { IDD = IDD_ETC_STOR_ADD_DLG };

private:
	CEditUnit	m_edtNumAdded;
	CEditUnit	m_edtNumHeight;
	CTextUnit	m_stHeight;

	CDBDoc* m_pDoc;
};

