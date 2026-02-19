
#if !defined(__DGNCFPFDGDLG_H__)
#define __DGNCFPFDGDLG_H__
#pragma once

#include "..\wg_base\DlgChild.h"
#include "..\wg_base\MyCheckComboBox.h"
// CDgnCfPfdgDlg 对话框

#include "HeaderPre.h"
class CDBDoc;
class __MY_EXT_CLASS__ CDgnCfPfdgDlg : public CDlgChild
{
	DECLARE_DYNAMIC(CDgnCfPfdgDlg)

public:
	CDgnCfPfdgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDgnCfPfdgDlg();

// 对话框数据
	enum { IDD = IDD_DGN_STL_PFDN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual void OnOK();
	DECLARE_MESSAGE_MAP()
private:
    CComboBox m_nPerforLevel;
    CComboBox m_nDucLevel;
    double m_dNoPlaCoe;
    double m_dMinCoe;
    CComboBox m_nGravityCom;
    CMyCheckComboBox m_aHSeismicCom;
    CComboBox m_nVSeismicCom;
    CComboBox m_FrameType;
    CDBDoc* m_pDoc;
    BOOL ExistKey(const CArray<T_LCOM_BASE>& aSeis,T_LCOM_BASE nKey);
	BOOL GetPfdnData(T_PFDN_D& PfdnD);
	afx_msg void OnSelectPfdl();
public:
    virtual BOOL OnInitDialog();
    //
  
};

#include "HeaderPost.h"

#endif 
