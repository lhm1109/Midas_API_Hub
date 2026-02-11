// CMDTendonTemplateAutoGen.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMDTendonTemplateAutoGenDlg.h"
#include "CMTendonTypeDlg.h"
#include "CMTendonDefDlg.h"
#include "ExportFunc.h"
#include "CMDTendonTemplateDlg.h"
#include "DgnTendonAutoGenCtrl.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\DBLib.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMDTendonTemplateAutoGenDlg::CCMDTendonTemplateAutoGenDlg(CDgnTendonTemplateCtrl* pCtrl, CWnd* pParent /* = NULL*/)
	:CCMDlgBase(CCMDTendonTemplateAutoGenDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pTemplateCtrl = pCtrl;
	m_pParentDlg = (CCMDTendonTemplateDlg*)pParent;
}

CCMDTendonTemplateAutoGenDlg::~CCMDTendonTemplateAutoGenDlg()
{
	if(m_pParentDlg)
	{
		m_pParentDlg->SetChidDlgNULL(EN_DLG_TYPE_AUTOGEN);
		m_pParentDlg->SetUseOriginDraw(FALSE);
	}
}


BEGIN_MESSAGE_MAP(CCMDTendonTemplateAutoGenDlg, CCMDlgBase)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_AUTOGEN_BTN_PROP,		 OnCmdProp)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_AUTOGEN_BTN_GRP,		 OnCmdGrp)
	ON_CBN_SELCHANGE(IDC_CMD_TENDON_TEMPLATE_AUTOGEN_CBX_TYPE, OnChangeType)
	ON_CBN_SELCHANGE(IDC_CMD_TENDON_TEMPLATE_AUTOGEN_CBX_CODE, OnChangeCode)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_AUTOGEN_ORIGIN_PT,		 OnChangeOrigin)
END_MESSAGE_MAP()

void CCMDTendonTemplateAutoGenDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_CBX_PROP,		m_wndTdntCobx);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_CBX_GRP,		  m_wndTdgrCobx);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_NAME_PREFIX,	m_edtNamePrefix);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_CHK_INIT,		m_chkInit);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_CBX_CODE,		m_wndCobxCode);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_CBX_TYPE,		m_wndCobxType);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_CBX_NAME,		m_wndCobxName);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_ORIGIN_PT_UNIT, m_stcOrigin);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_AUTOGEN_ORIGIN_PT,		   m_edtOrigin);	
}

void CCMDTendonTemplateAutoGenDlg::OnCancel()
{
	DestroyWindow();
	//CCMDlgBase::OnCancel();
}

void CCMDTendonTemplateAutoGenDlg::OnOK()
{
	if(!Dlg2Data())
	{
		return;
	}

	if(!m_pTemplateCtrl->AutoGeneration(m_Data.bReset, m_Data.tdntKey, m_Data.tdgrKey, m_Data.namePrefix, m_Data.strCode, m_Data.strT, m_Data.strN, m_Data.dOrigin))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_AUTOGEN_INVALID_SECTION));
		m_pParentDlg->UpdateListDraw();
		return;
	}

	m_pParentDlg->UpdateListDraw();

	DestroyWindow();
	//CCMDlgBase::OnOK();
}

BOOL CCMDTendonTemplateAutoGenDlg::OnInitDialog()
{
	CCMDlgBase::OnInitDialog();
	
	m_edtNamePrefix.SetWindowText(m_strPrefixName);
	m_chkInit.SetCheck(TRUE);

	SetControl();
	return TRUE;
}

void CCMDTendonTemplateAutoGenDlg::ComboBoxDynamicChange(CComboBox* pCombo)
{
	CString str1;
	CSize sz;
	int dx = 0;
	CDC*  pDC = GetDC();
	
	for(int h = 0; h < pCombo->GetCount(); h++)
	{
		pCombo->GetLBText(h, str1);
		sz = pDC->GetTextExtent(str1);
		
		if(sz.cx > dx)
		{
			dx = sz.cx;
		}
	}
	
	if ( dx > pCombo->GetDroppedWidth() ) 
	{
		pCombo->SetDroppedWidth(dx);
	}
}

void CCMDTendonTemplateAutoGenDlg::OnCmdProp()
{
	CCMTendonTypeDlg Dlg;
	Dlg.DoModal();
	ComboBoxDynamicChange(&m_wndTdntCobx);
}

void CCMDTendonTemplateAutoGenDlg::OnCmdGrp()
{
	CreateOrActivateDlg(m_pDoc, CCMTendonDefDlg::IDD, this);
	ComboBoxDynamicChange(&m_wndTdgrCobx);	
}

BOOL CCMDTendonTemplateAutoGenDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnOK();
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			OnCancel();
			return TRUE;
		}
	}
	return CCMDlgBase::PreTranslateMessage(pMsg);  
}

void CCMDTendonTemplateAutoGenDlg::SetControl()
{
	m_stcOrigin.SetUnitType(D_UNITSYS_BASE_LENGTH);
	//m_edtOrigin.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_pParentDlg->SetUseOriginDraw(TRUE);

	CString strCode;
	CString strType;
	CString strName;
	m_pTemplateCtrl->GetAutoGenSectName(strCode, strType, strName);

	int i, nSize, nTemp, nSelIdx;
	m_wndCobxCode.ResetContent();
	CStringArray* pStrAryCode;
	if(!m_pTemplateCtrl->GetAutoGenCtrl()->GetStringsCode(pStrAryCode)) { ASSERT(0); return; }

	nSize = pStrAryCode->GetSize();
	if(nSize == 0) { ASSERT(0); return; }

	nSelIdx = -1;
	for(i = 0 ; i < nSize; i++)
	{
		nTemp = m_wndCobxCode.AddString(pStrAryCode->GetAt(i));
		if(pStrAryCode->GetAt(i) == strCode) nSelIdx = nTemp;
	}
	if(nSelIdx > 0) m_wndCobxCode.SetCurSel(nSelIdx);
	else            m_wndCobxCode.SetCurSel(0);

	SetSectType(strType);

	SetSectName(strName);
}

void CCMDTendonTemplateAutoGenDlg::OnChangeType()
{
	SetSectName(_T(""));
}

void CCMDTendonTemplateAutoGenDlg::OnChangeCode()
{
	SetSectType(_T(""));
	SetSectName(_T(""));
}

void CCMDTendonTemplateAutoGenDlg::SetSectName(const CString& strName)
{
	m_wndCobxName.ResetContent();

	CStringArray strAryName;
	CString strCurCode;
	CString strCurType;
	int nCodeSel = m_wndCobxCode.GetCurSel();
	int nTypeSel = m_wndCobxType.GetCurSel();

	if(nCodeSel < 0 || nTypeSel < 0) //선택된게 없다면.. 설정할 수 없다.
		return;

	m_wndCobxCode.GetLBText(nCodeSel, strCurCode);
	m_wndCobxType.GetLBText(nTypeSel, strCurType);

	int nSize = m_pTemplateCtrl->GetAutoGenCtrl()->GetStringsName(strCurCode, strCurType, strAryName);
	if(nSize == 0)
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_AUTOGEN_NOSUPPORTSECT));
		return; 
	}

	int nTemp;
	int nSelIdx = -1;
	for(int i = 0 ; i < nSize; i++)
	{
		nTemp = m_wndCobxName.AddString(strAryName.GetAt(i));
		if(strAryName.GetAt(i) == strName) nSelIdx = nTemp;
	}
	if(nSelIdx > 0) m_wndCobxName.SetCurSel(nSelIdx);
	else            m_wndCobxName.SetCurSel(0);  

	AutoAdjustComboWidth(m_wndCobxName);
}

void CCMDTendonTemplateAutoGenDlg::SetSectType( const CString& strType )
{
	m_wndCobxType.ResetContent();

	CStringArray strAryType;
	CString strCurCode;
	int nTypeSel = m_wndCobxCode.GetCurSel();
	if(nTypeSel < 0)
		return;
	m_wndCobxCode.GetLBText(nTypeSel, strCurCode);

	int nSize = m_pTemplateCtrl->GetAutoGenCtrl()->GetStringsType(strCurCode, strAryType);
	if(nSize == 0)
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_AUTOGEN_NOSUPPORTSECT));
		return; 
	}

	int nTemp;
	int nSelIdx = -1;
	for(int i = 0 ; i < nSize; i++)
	{
		nTemp = m_wndCobxType.AddString(strAryType.GetAt(i));
		if(strAryType.GetAt(i) == strType) nSelIdx = nTemp;
	}
	if(nSelIdx > 0) m_wndCobxType.SetCurSel(nSelIdx);
	else            m_wndCobxType.SetCurSel(0);
}

BOOL CCMDTendonTemplateAutoGenDlg::Dlg2Data()
{
	m_Data.bReset = m_chkInit.GetCheck();

	if(!m_wndTdgrCobx.GetSelectedTdgr(m_Data.tdgrKey))
	{
		m_Data.tdgrKey = 0;//Default Tdgr
	}

	if(!m_wndTdntCobx.GetSelectedTdnt(m_Data.tdntKey))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_TDNT));
		return FALSE;
	}

	m_edtNamePrefix.GetWindowText(m_Data.namePrefix);
	if(m_Data.namePrefix.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_SW_GRIL_TENDON_NAME_ERROR));
		return FALSE;
	}

	int nIdx;

	nIdx        = m_wndCobxCode.GetCurSel();
	if(nIdx >= 0) m_wndCobxCode.GetLBText(nIdx, m_Data.strCode);
	else
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_AUTOGEN_INVALID_SECT));
		return FALSE;
	}

	nIdx        = m_wndCobxType.GetCurSel();
	if(nIdx >= 0) m_wndCobxType.GetLBText(nIdx, m_Data.strT);
	else
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_AUTOGEN_INVALID_SECT));
		return FALSE;
	}

	nIdx        = m_wndCobxName.GetCurSel();
	if(nIdx >= 0) m_wndCobxName.GetLBText(nIdx, m_Data.strN);
	else
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_AUTOGEN_INVALID_SECT));
		return FALSE;
	}

	if(!GetXYPos(m_Data.dOrigin))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_AUTOGEN_INVALID_ORIGIN));
		return FALSE;
	}
	//현재 단위계를 중립단위계로...
	m_Data.dOrigin[0] = m_pDoc->m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, m_Data.dOrigin[0]);
	m_Data.dOrigin[1] = m_pDoc->m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, m_Data.dOrigin[1]);

	return TRUE;
}

BOOL CCMDTendonTemplateAutoGenDlg::GetXYPos( double* dPos )
{
	CString strPos;
	m_edtOrigin.GetWindowText(strPos);

	CArray<double, double> caPos;
	if(!CStrParser::GetUEDistance(strPos, caPos))
		return FALSE;

	if(caPos.GetSize() != 2)
		return FALSE;

	dPos[0] = caPos[0];
	dPos[1] = caPos[1];
	return TRUE;
}

void CCMDTendonTemplateAutoGenDlg::SetXYPos( double* dPos )
{
	CString strPos;
	strPos.Format(_T("%.3f, %.3f"), dPos[0], dPos[1]);  
	m_edtOrigin.SetWindowText(strPos);
}

void CCMDTendonTemplateAutoGenDlg::OnChangeOrigin()
{
	double dPos[2];
	if(GetXYPos(dPos))
	{
		m_pParentDlg->SetOriginPos_Draw(dPos);
	}    
}

void CCMDTendonTemplateAutoGenDlg::AutoAdjustComboWidth(CComboBox& Combo)
{
	if(!Combo.GetSafeHwnd() || !::IsWindow(Combo.GetSafeHwnd()))
	{
		ASSERT(FALSE);
		return;
	}

	CDC& ComboDC = *Combo.GetDC();
	const int RestorePoint = ComboDC.SaveDC();
	CFont& ComboFont = *Combo.GetFont();

	ComboDC.SelectObject(&ComboFont);

	int MaxX = -1;

	const UINT ItemCount = Combo.GetCount();
	for(UINT Index = 0; Index < ItemCount; ++Index)
	{
		CString Text;
		Combo.GetLBText(Index, Text);

		// Get width of item string.
		const SIZE sz = ComboDC.GetTextExtent(Text);

		if(sz.cx > MaxX)
		{
			MaxX = sz.cx;
		}
	}// End for

	if(MaxX > 0)
	{
		const int ScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
		const int Border = ::GetSystemMetrics(SM_CXEDGE) * 2;
		Combo.SetDroppedWidth(MaxX + ScrollWidth * 2);
	}

	ComboDC.RestoreDC(RestorePoint);
}