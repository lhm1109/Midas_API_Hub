// CMDTendonTemplateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDTendonTemplateAddModDlg.h"

#include "CMDTendonTemplateViewDlg.h"
#include "DgnTendonElemLengthCtrl.h"
#include "CMDTendonTemplateDlg.h"
#include "CMTendonTypeDlg.h"
#include "CMTendonDefDlg.h"
#include "DrawEditBtn_TndnTemplate.h"
#include "ExportFunc.h"

#include "..\wg_base\wg_base_MsgDll.h"


#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBDoc.h"

#define MAX_KEY_NUM (unsigned int)9999999999

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMDTendonTemplateAddModDlg::CCMDTendonTemplateAddModDlg(CDgnTendonTemplateCtrl* pCtrl, CWnd* pParent /* = NULL*/)
	:CCMDlgBase(CCMDTendonTemplateAddModDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pTemplateCtrl = pCtrl;
	m_nKey = -1;

	m_pDrawXZ = new CDrawEditBtn_TndnTemplate(m_pTemplateCtrl);
	m_pDrawXY = new CDrawEditBtn_TndnTemplate(m_pTemplateCtrl);

	m_vecRoundType.resize(3);	
	m_vecRoundType[0] = TNDN_TEMPLATE_TYPE_STRAIGHT;
	m_vecRoundType[1] = TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1;
	m_vecRoundType[2] = TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2;
	
	m_vecSplineType.resize(4);
	m_vecSplineType[0] = TNDN_TEMPLATE_TYPE_CURVE_LINE;
	m_vecSplineType[1] = TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1;
	m_vecSplineType[2] = TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2;
	m_vecSplineType[3] = TNDN_TEMPLATE_TYPE_CURVE_BENT_1;

	m_vecEnType.resize(8);
	m_vecEnType[0] = TNDN_TEMPLATE_TYPE_STRAIGHT;
	m_vecEnType[1] = TNDN_TEMPLATE_TYPE_CURVE_ST;
	m_vecEnType[2] = TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1;
	m_vecEnType[3] = TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2;
	m_vecEnType[4] = TNDN_TEMPLATE_TYPE_CURVE_LINE;
	m_vecEnType[5] = TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1;
	m_vecEnType[6] = TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2;
	m_vecEnType[7] = TNDN_TEMPLATE_TYPE_CURVE_BENT_1;

	m_bIgnoreMessage = FALSE;
	m_bOK = FALSE;
	m_pParent = (CCMDTendonTemplateDlg*)pParent;

	m_svStrComboImage.push_back(_T("SVG\\Illustration\\ComboBox\\TndnTemplate_type_list.svg"));
	m_svStrComboImage.push_back(_T("SVG\\Illustration\\ComboBox\\TndnTemplate_type_list_1.svg"));
	m_svStrComboImage.push_back(_T("SVG\\Illustration\\ComboBox\\TndnTemplate_type_list_2.svg"));
	m_svStrComboImage.push_back(_T("SVG\\Illustration\\ComboBox\\TndnTemplate_type_list_3.svg"));
	m_svStrComboImage.push_back(_T("SVG\\Illustration\\ComboBox\\TndnTemplate_type_list_4.svg"));
	m_svStrComboImage.push_back(_T("SVG\\Illustration\\ComboBox\\TndnTemplate_type_list_5.svg"));
	m_svStrComboImage.push_back(_T("SVG\\Illustration\\ComboBox\\TndnTemplate_type_list_6.svg"));
	m_svStrComboImage.push_back(_T("SVG\\Illustration\\ComboBox\\TndnTemplate_type_list.svg"));
}

CCMDTendonTemplateAddModDlg::~CCMDTendonTemplateAddModDlg()
{
	if(m_pDrawXZ)
	{
		delete m_pDrawXZ;
		m_pDrawXZ = NULL;
	}
	if(m_pDrawXY)
	{
		delete m_pDrawXY;
		m_pDrawXY = NULL;
	}

	if(m_pParent)
	{
		if(m_pParent->GetSafeHwnd())
		{
			Sleep(100);
			m_pParent->SetShowWnd(TRUE);
			m_pParent->ShowWindow(SW_SHOW);
		}
	}

	DestroyViewWindow();

	if(!m_bOK)
	{
		CString strPrefix;
		m_pParent->GetPrefixName(strPrefix);
		m_pTemplateCtrl->CancelNewNameNum(strPrefix);

	}
}


BEGIN_MESSAGE_MAP(CCMDTendonTemplateAddModDlg, CCMDlgBase)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN,	  OnDeltaposCmdSpanXY)     //Span Num
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN,	OnDeltaposCmdSpanXZ)     //Span Num
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADDMOD_PROPERTY_BTN,				OnCmdTendonPropButton)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADDMOD_GROUP_BTN,						OnCmdTendonGroupButton)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ,					OnCmdChangeSpanNumXZ)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY,					OnCmdChangeSpanNumXY)
	ON_CBN_SELCHANGE(IDC_CMD_TENDON_TEMPLATE_ADDMOD_TYPE_XY,					OnTypeChangeXY)
	ON_CBN_SELCHANGE(IDC_CMD_TENDON_TEMPLATE_ADDMOD_TYPE_XZ,					OnTypeChangeXZ)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_ADDMOD_SPANLEN_XY,						OnCmdChangeSpanLenXY)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_ADDMOD_SPANLEN_XZ,						OnCmdChangeSpanLenXZ)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADDMOD_USE_SPAN_SPAN_XY,		OnCmdCheckUseSameSpanInfoXY)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADDMOD_USE_SPAN_SPAN_XZ,		OnCmdCheckUseSameSpanInfoXZ)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADDMOD_USE_SPAN_SAME_LENGTH,OnCmdCheckUseSameInterval)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_ADDMOD_OFFSET_FRONT,					OnChangeOffset)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_ADDMOD_OFFSET_END,						OnChangeOffset)	
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADDMOD_SYM_XAXIS_XY,				OnCmdCheckSymmXY)	
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADDMOD_SYM_XAXIS_XZ,				OnCmdCheckSymmXZ)
END_MESSAGE_MAP()

void CCMDTendonTemplateAddModDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_GROUP_COMBO,			m_wndTdgrCobx);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_PROPERTY_COMBO,	  m_wndTdntCobx);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_TENDON_NAME,			m_wndTndnName);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_OFFSET_FRONT,			m_wndOffsetFront);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_OFFSET_END,				m_wndOffsetEnd);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ,			m_wndNumOfSpanXZ);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY,			m_wndNumOfSpanXY);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_TYPE_XY,				  m_cbxTypeXY);		
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_TYPE_XZ,				  m_cbxTypeXZ);		
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_SPANLEN_XY,				m_wndSpanLengthXY);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_SPANLEN_XZ,				m_wndSpanLengthXZ);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_SYM_XAXIS_XY,			m_chkSymXAxisXY);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_SYM_XAXIS_XZ,			m_chkSymXAxisXZ);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_USE_SPAN_SPAN_XY,		  m_chkSameSpanInfoXY);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_USE_SPAN_SPAN_XZ,		  m_chkSameSpanInfoXZ);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_USE_SPAN_SAME_LENGTH,	m_chkSameSpanInterval);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_RDO_REFAXIS_ELEM,	    m_rdoRefAxisElem);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_RDO_REFAXIS_CURVE,	  m_rdoRefAxisCurve);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_OFFSET_UNIT_FRONT,		m_wndUnitOffsetFront);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_OFFSET_UNIT_END,		  m_wndUnitOffsetEnd);	

	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_GRID_XZ, m_gridXZ);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_DRAW_XZ, *m_pDrawXZ);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_GRID_XY, m_gridXY);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ADDMOD_DRAW_XY, *m_pDrawXY);
}

void CCMDTendonTemplateAddModDlg::OnCancel()
{
	DestroyViewWindow();
	DestroyWindow();
}

void CCMDTendonTemplateAddModDlg::OnOK()
{
	if(Dlg2Data() && m_templateData.keyProperty > 0)
	{
		if(m_templateData.keyProperty == MAX_KEY_NUM)
		{
			AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_TDNT));
		}
		else
		{
			m_bOK = TRUE;
			m_pParent->OnOkSubDlgADDMOD(m_templateData, m_nKey);

			DestroyViewWindow();
			DestroyWindow();
		}
	}
}

BOOL CCMDTendonTemplateAddModDlg::OnInitDialog()
{
	CCMDlgBase::OnInitDialog();

	SetControl();
	
	Data2Dlg();

	return TRUE;
}

void CCMDTendonTemplateAddModDlg::SetControl()
{
	m_wndUnitOffsetFront.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndUnitOffsetEnd.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndOffsetFront.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndOffsetEnd.SetUnitType(D_UNITSYS_BASE_LENGTH);

	//Draw와 Grid를 초기화한다.
	m_gridXZ.Initialize(TNDN_TEMPALTE_TYPE_XYXZ_XZ, &m_templateData);

	m_pDrawXZ->Init();
	m_pDrawXZ->SetMode(EN_DRAW_WINMODE_SELECT);
	m_pDrawXZ->SetDrawType(DRAW_EDIT_BTN_TYPE_XZ);
	m_pDrawXZ->SetTdnaData(&m_templateData);
	m_pDrawXZ->SetHasList(FALSE);
	
	m_gridXY.Initialize(TNDN_TEMPALTE_TYPE_XYXZ_XY, &m_templateData);
	
	m_pDrawXY->Init();
	m_pDrawXY->SetMode(EN_DRAW_WINMODE_SELECT);
	m_pDrawXY->SetDrawType(DRAW_EDIT_BTN_TYPE_XY);
	m_pDrawXY->SetTdnaData(&m_templateData);
	m_pDrawXY->SetHasList(FALSE);

	//텐던그룹, 텐던프로퍼티의 콤보박스 드롭부분의 너비 조정.
	ComboBoxDynamicChange(&m_wndTdgrCobx);
	ComboBoxDynamicChange(&m_wndTdntCobx);
	
	//오른쪽에뜨는 대화상자를 만든다.
	CRect rectNewPos, rectParent, rectChild;
	m_pViewer = new CCMDTendonTemplateViewDlg();
	m_pViewer->SetInitPos(D_INIT_POS_RT);
	m_pViewer->Create(IDD_CMD_TENDON_TEMPLATE_VIEW, this);
	m_pViewer->ShowWindow(SW_SHOW);
	m_pViewer->GetWindowRect(rectChild);
	GetWindowRect(rectParent); 

	int   nSystemWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int   nRightPos = rectParent.right + rectChild.Width();
	
	//모니터를 벗어나면.
	if(rectParent.right > nSystemWidth) nRightPos -= nSystemWidth;
	
	rectNewPos.left = rectParent.right;

	if(nRightPos > nSystemWidth) //밖에 나간다면 그만큼 들인다.
		rectNewPos.left += (nSystemWidth - nRightPos);
	
	rectNewPos.top    = rectParent.top;
	rectNewPos.bottom = rectNewPos.top + rectChild.Height();
	rectNewPos.right  = rectNewPos.left + rectChild.Width();
	
	m_pViewer->MoveWindow(rectNewPos);

	m_wndNumOfSpanXY.SetRange(1, 1000);
	m_wndNumOfSpanXY.SetInteger(TRUE);
	m_wndNumOfSpanXZ.SetRange(1, 1000);
	m_wndNumOfSpanXZ.SetInteger(TRUE);
}

void CCMDTendonTemplateAddModDlg::ComboBoxDynamicChange(CComboBox* pCombo)
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


void CCMDTendonTemplateAddModDlg::OnCmdTendonPropButton() 
{
	if(m_bIgnoreMessage)	return;

	CCMTendonTypeDlg Dlg;
	Dlg.DoModal();
	ComboBoxDynamicChange(&m_wndTdntCobx);
}

void CCMDTendonTemplateAddModDlg::OnCmdTendonGroupButton()
{
	if(m_bIgnoreMessage)	return;	

	CreateOrActivateDlg(m_pDoc, CCMTendonDefDlg::IDD, this);
	ComboBoxDynamicChange(&m_wndTdgrCobx);
}

BOOL CCMDTendonTemplateAddModDlg::Data2Dlg()
{
	m_bIgnoreMessage = TRUE;

	int nSpanSize;

	m_wndTdgrCobx.ChangeSelect(m_templateData.keyGroup);

	if(m_templateData.keyProperty != 0)
	{
		// 할단된값이 존재한다면. 선택
		m_wndTdntCobx.ChangeSelect(m_templateData.keyProperty);
	}
	else
	{
		// 할단된값이 존재하지 않는다면. 첫번째로 세팅
		T_TDNT_K tdntK;
		if(m_wndTdntCobx.GetCount() > 0)
		{
			m_wndTdntCobx.SetCurSel(0);
			if(m_wndTdntCobx.GetSelectedTdnt(tdntK))
				m_templateData.keyProperty = tdntK;
		}
	}
	
	EnableDisableAllCtrl(TRUE, m_templateData);

	/*  EnableDisableAllCtrl가 수행한다.
	if(m_pParent)
	{
		//프리픽스를 사용한다면 이름이 자동으로 정해지기 때문에 Disable
		if(m_pParent->IsUsePrefix())	m_wndTndnName.EnableWindow(FALSE);	
		else                          m_wndTndnName.EnableWindow(TRUE);	

		//요소가 휘어있다면 Ref. Axis 가 Curve Type으로 출력될 수도 있다.
		//라디오를 추가하고 세팅하는 부분을 만들자. #NS_RF
		BOOL bIsCurveElem = m_pParent->IsCurveElem();
		m_rdoRefAxisCurve.EnableWindow(bIsCurveElem);
		if(bIsCurveElem)
		{
				m_rdoRefAxisElem.SetCheck(!m_templateData.bCurveTypeOut);
				m_rdoRefAxisCurve.SetCheck(m_templateData.bCurveTypeOut);
		}
		else
		{
			m_templateData.bCurveTypeOut = FALSE;
			m_rdoRefAxisElem.SetCheck(TRUE);
			m_rdoRefAxisCurve.SetCheck(FALSE);
		}
	}
	m_wndTndnName.SetWindowText(m_templateData.strName);
	*/

	CString strTemp;

	//front, end offset의 크기에 따라 소수점 이하자리수 조정.
	if(m_templateData.dOffsetFront < 1)
		m_wndOffsetFront.SetEditUnit(m_templateData.dOffsetFront, 4);
	else
		m_wndOffsetFront.SetEditUnit(m_templateData.dOffsetFront, 1);

	if(m_templateData.dOffsetEnd< 1)
		m_wndOffsetEnd.SetEditUnit(m_templateData.dOffsetEnd, 4);
	else
		m_wndOffsetEnd.SetEditUnit(m_templateData.dOffsetEnd, 1);

	ResetCombobox(TRUE, m_templateData.nType);

	SetCurSel(); //선택하고  

	m_gridXY.SetTemplateType(m_templateData.tndnSubXY.nType);
	m_gridXZ.SetTemplateType(m_templateData.tndnSubXZ.nType);

	m_chkSameSpanInterval.SetCheck(m_templateData.bSameSpanInterval); //Same span Length
	//EnableDisableCtrlSameSpanLength(m_templateData.bSameSpanInterval, TRUE, TRUE);
	
	BOOL bEnableCtrlXY = (m_templateData.tndnSubXY.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 ||
		m_templateData.tndnSubXY.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2);
	BOOL bEnableCtrlXZ = (m_templateData.tndnSubXZ.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 ||
		m_templateData.tndnSubXZ.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2);
	//EnableDisableCtrlSpin(bEnableCtrlXY, bEnableCtrlXZ);

	m_chkSymXAxisXY.SetCheck(m_templateData.tndnSubXY.bXSymmetry);
	m_chkSymXAxisXZ.SetCheck(m_templateData.tndnSubXZ.bXSymmetry);

	nSpanSize = m_templateData.tndnSubXY.nNumOfSpan;
	m_wndNumOfSpanXY.SetValue(nSpanSize);

	nSpanSize = m_templateData.tndnSubXZ.nNumOfSpan;
	m_wndNumOfSpanXZ.SetValue(nSpanSize);

	m_chkSameSpanInfoXY.SetCheck(m_templateData.tndnSubXY.bSameSpanInfo);
	m_chkSameSpanInfoXZ.SetCheck(m_templateData.tndnSubXZ.bSameSpanInfo);
	
	if(m_templateData.bSameSpanInterval)
	{  
		m_templateData_Draw = m_pTemplateCtrl->ConvertUnitIn(m_templateData);
		
		CString strTemp;
		
		m_pTemplateCtrl->GetSameSpanStr(m_templateData_Draw, TRUE, strTemp);
		m_pTemplateCtrl->ConvertUnitOut(strTemp);
		m_templateData.tndnSubXY.strSpanLength = strTemp;
		m_wndSpanLengthXY.SetWindowText(strTemp);
		
		m_pTemplateCtrl->GetSameSpanStr(m_templateData_Draw, FALSE, strTemp);
		m_pTemplateCtrl->ConvertUnitOut(strTemp);
		m_templateData.tndnSubXZ.strSpanLength = strTemp;
		m_wndSpanLengthXZ.SetWindowText(strTemp);
	}

	m_wndSpanLengthXY.SetWindowText(m_templateData.tndnSubXY.strSpanLength);
	m_wndSpanLengthXZ.SetWindowText(m_templateData.tndnSubXZ.strSpanLength);
	
	RemakeGridDraw(TRUE, TRUE);
	
	//Bitmap을 바꾼다.
	m_pViewer->ChangeBitmap(m_templateData.tndnSubXY.nType);
	
	EnableDisableAllCtrl(FALSE, m_templateData);

	m_bIgnoreMessage = FALSE;

	return TRUE;
}

BOOL CCMDTendonTemplateAddModDlg::Dlg2Data()
{
	T_TDGR_K tdgrK;
	if(!m_wndTdgrCobx.GetSelectedTdgr(tdgrK))		return FALSE;
	m_templateData.keyGroup = tdgrK;

	T_TDNT_K tdntK;
	if(!m_wndTdntCobx.GetSelectedTdnt(tdntK))
	{
		//프로퍼티가 없더라도 일단 그림을 그릴수 있도록.
		tdntK = MAX_KEY_NUM;
	}
	m_templateData.keyProperty = tdntK;
	
	m_wndTndnName.GetWindowText(m_templateData.strName);
	
	//#NS_RF 이부분 수정해라.
	m_templateData.bCurveTypeOut = m_rdoRefAxisCurve.GetCheck() ? TRUE : FALSE;

	m_templateData.dOffsetFront = m_wndOffsetFront.GetEditValue();
	m_templateData.dOffsetEnd	= m_wndOffsetEnd.GetEditValue();
	
	m_templateData.tndnSubXY.nType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXY.GetItemData(m_cbxTypeXY.GetCurSel());
	m_templateData.tndnSubXZ.nType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXZ.GetItemData(m_cbxTypeXZ.GetCurSel());

	//set type
	TNDN_TEMPLATE_TYPE enXYType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXY.GetItemData(m_cbxTypeXY.GetCurSel());
	if(IS_TYPE_STRAIGHT(enXYType))
		m_templateData.nType = 1;
	else if(IS_TYPE_CURVE(enXYType))
		m_templateData.nType = 0;
	else
		ASSERT(0);

	m_templateData.tndnSubXY.nType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXY.GetItemData(m_cbxTypeXY.GetCurSel());
	m_templateData.tndnSubXZ.nType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXZ.GetItemData(m_cbxTypeXZ.GetCurSel());
	
	//num of span
	m_templateData.tndnSubXY.nNumOfSpan = m_wndNumOfSpanXY.GetEditValueInt();
	m_templateData.tndnSubXZ.nNumOfSpan = m_wndNumOfSpanXZ.GetEditValueInt();

	//use same span info
	m_templateData.tndnSubXY.bSameSpanInfo = m_chkSameSpanInfoXY.GetCheck() ? TRUE : FALSE;
	m_templateData.tndnSubXZ.bSameSpanInfo = m_chkSameSpanInfoXZ.GetCheck() ? TRUE : FALSE;

	//symmetry
	m_templateData.tndnSubXY.bXSymmetry = m_chkSymXAxisXY.GetCheck() ? TRUE : FALSE;
	m_templateData.tndnSubXZ.bXSymmetry = m_chkSymXAxisXZ.GetCheck() ? TRUE : FALSE;

	//span length
	m_wndSpanLengthXY.GetWindowText(m_templateData.tndnSubXY.strSpanLength);
	m_wndSpanLengthXZ.GetWindowText(m_templateData.tndnSubXZ.strSpanLength);

	//span info grid
		if (!m_gridXZ.OnTerminate(D_TB_SAVE_ALWAYS))		return FALSE;
		if (!m_gridXY.OnTerminate(D_TB_SAVE_ALWAYS))		return FALSE;

	if(!CheckValidData())
		return FALSE;
	
	return TRUE;
}

BOOL CCMDTendonTemplateAddModDlg::CheckValidData()
{
	return m_pTemplateCtrl->IsValidTndnTemplate(m_templateData, TRUE);
}

void CCMDTendonTemplateAddModDlg::OnCmdChangeSpanNumXZ()
{
	if(m_bIgnoreMessage)	return;
	
	BOOL bOldLock = m_gridXZ.LockUpdate(TRUE);

	int nNewSpanNum = m_wndNumOfSpanXZ.GetEditValueInt();
	int nPreSpanNum = m_templateData.tndnSubXZ.nNumOfSpan;
	BOOL bReset = FALSE;
	
	if(nNewSpanNum <= 0)
	{
		nNewSpanNum = 1; //최소 1 이 되도록.
		bReset = TRUE;
	}
	else if (nNewSpanNum > 100)
	{
		nNewSpanNum	= 99;	//최대 100이 되도록.
		bReset = TRUE;
	}

	if(bReset)
	{
		m_wndNumOfSpanXZ.SetValue(nNewSpanNum);
		return;
	}

	if(nNewSpanNum != m_templateData.tndnSubXZ.nNumOfSpan)
	{
		m_templateData.tndnSubXZ.nNumOfSpan = nNewSpanNum;
		m_templateData.tndnSubXZ.aSpanInfo.resize(nNewSpanNum);
	}
	OnChangeOffset();

	m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, FALSE, 0, TRUE, FALSE);
	m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, FALSE, nPreSpanNum, FALSE, TRUE);

	m_gridXY.LockUpdate(bOldLock);
	RemakeGridDraw(FALSE, TRUE);
}

void CCMDTendonTemplateAddModDlg::OnCmdChangeSpanNumXY()
{
	if(m_bIgnoreMessage)	return;
	
	BOOL bOldLock = m_gridXY.LockUpdate(TRUE);

	int nNewSpanNum = m_wndNumOfSpanXY.GetEditValueInt();
	int nPreSpanNum = m_templateData.tndnSubXY.nNumOfSpan;
	BOOL bReset = FALSE;

	if(nNewSpanNum <= 0)
	{
		nNewSpanNum = 1; //최소 1 이 되도록.
		bReset = TRUE;
	}
	else if (nNewSpanNum > 100)
	{
		nNewSpanNum	= 100;	//최대 100이 되도록.
		bReset = TRUE;
	}
	if(bReset)
	{
		m_wndNumOfSpanXY.SetValue(nNewSpanNum);
		return;
	}

	if(nNewSpanNum != m_templateData.tndnSubXY.nNumOfSpan)
	{
		m_templateData.tndnSubXY.aSpanInfo.resize(nNewSpanNum);
		if(nNewSpanNum > m_templateData.tndnSubXY.nNumOfSpan)
		{	//이럴땐 새로 추가되는 span info에 기본 값을 체워준다.

		}
		m_templateData.tndnSubXY.nNumOfSpan = nNewSpanNum;
	}
	OnChangeOffset();

	m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, TRUE, 0, TRUE, FALSE);
	m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, TRUE, nPreSpanNum, FALSE, TRUE);
	
	m_gridXY.LockUpdate(bOldLock);
	RemakeGridDraw(TRUE, FALSE);  
}

void CCMDTendonTemplateAddModDlg::OnDeltaposCmdSpanXY( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(m_bIgnoreMessage)	return;	
	
		m_gridXY.OnTerminate(D_TB_SAVE_ALWAYS);
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY,1,1000,pNMHDR,pResult);
	
	OnChangeOffset();
	
	*pResult = 0;
}

void CCMDTendonTemplateAddModDlg::OnDeltaposCmdSpanXZ( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(m_bIgnoreMessage)	return;
	
		m_gridXZ.OnTerminate(D_TB_SAVE_ALWAYS);

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ,1,1000,pNMHDR,pResult);

	OnChangeOffset();
	
	*pResult = 0;
}

void CCMDTendonTemplateAddModDlg::OnTypeChangeXY()
{
	if(m_bIgnoreMessage)	return;

	TNDN_TEMPLATE_TYPE nTypePre = m_templateData.tndnSubXY.nType;
	
	m_templateData.tndnSubXY.nType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXY.GetItemData(m_cbxTypeXY.GetCurSel());

	if(m_templateData.tndnSubXY.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 ||
		m_templateData.tndnSubXY.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2)
	{
//     m_templateData.bSameSpanInterval = TRUE;
//     m_chkSameSpanInterval.SetCheck(TRUE);
//     m_wndSpanLengthXY.EnableWindow(FALSE);
//     m_wndSpanLengthXZ.EnableWindow(FALSE);
//     m_wndNumOfSpanXY.EnableWindow(FALSE);
//     GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(FALSE);
//     m_chkSameSpanInfoXY.EnableWindow(FALSE);
		m_wndNumOfSpanXY.SetValue(1);
		m_templateData.tndnSubXY.aSpanInfo.resize(1);
		m_templateData.tndnSubXY.nNumOfSpan = 1;
		m_templateData.tndnSubXY.strSpanLength.Format(_T("%f"), m_pTemplateCtrl->GetTotalLength());
		m_bIgnoreMessage = TRUE;
		m_wndSpanLengthXY.SetWindowText(m_templateData.tndnSubXY.strSpanLength);
		m_bIgnoreMessage = FALSE;
	}
	else
	{
		//m_chkSameSpanInfoXY.EnableWindow(TRUE);
		
		BOOL bCheck = m_chkSameSpanInterval.GetCheck() ? TRUE : FALSE;
		
		if(bCheck)
		{
//       m_wndSpanLengthXY.EnableWindow(FALSE); 
//       m_wndNumOfSpanXY.EnableWindow(TRUE);
//       GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(TRUE);
		}
		else
		{
//       m_wndSpanLengthXY.EnableWindow(TRUE);  
//       m_wndNumOfSpanXY.EnableWindow(FALSE);
//       GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(FALSE);
		}
	}
	
	EnableDisableAllCtrl(FALSE, m_templateData);
	m_gridXY.SetTemplateType(m_templateData.tndnSubXY.nType);
	m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, TRUE, 0, TRUE, TRUE);

	BOOL bDiffType = IS_TYPE_CURVE(nTypePre) != IS_TYPE_CURVE(m_templateData.tndnSubXY.nType);

	if(bDiffType)
	{
		if(IS_TYPE_CURVE(m_templateData.tndnSubXY.nType))
		{
			m_templateData.nType = 0;
			ResetCombobox(FALSE, 0);
			OnTypeChangeXZ();
		}
		else if(IS_TYPE_STRAIGHT(m_templateData.tndnSubXY.nType))
		{
			m_templateData.nType = 1;
			ResetCombobox(FALSE, 1);
			OnTypeChangeXZ();
		}
		else
			ASSERT(0);
	}

	RemakeGridDraw(TRUE, FALSE);
	
	//Bitmap을 바꾼다.
	m_pViewer->ChangeBitmap(m_templateData.tndnSubXY.nType);
}

void CCMDTendonTemplateAddModDlg::OnTypeChangeXZ()
{
	if(m_bIgnoreMessage)	return;	

	m_templateData.tndnSubXZ.nType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXZ.GetItemData(m_cbxTypeXZ.GetCurSel());

	if(m_templateData.tndnSubXZ.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 ||
		m_templateData.tndnSubXZ.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2)
	{
//     m_templateData.bSameSpanInterval = TRUE;
//     m_chkSameSpanInterval.SetCheck(TRUE);
//     m_wndSpanLengthXY.EnableWindow(FALSE);
//     m_wndSpanLengthXZ.EnableWindow(FALSE);
//     m_wndNumOfSpanXZ.EnableWindow(FALSE);
//     GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(FALSE);
//     m_chkSameSpanInfoXZ.EnableWindow(FALSE);
		m_wndNumOfSpanXZ.SetValue(1);
		m_templateData.tndnSubXZ.aSpanInfo.resize(1);
		m_templateData.tndnSubXZ.nNumOfSpan = 1;
		m_templateData.tndnSubXZ.strSpanLength.Format(_T("%f"), m_pTemplateCtrl->GetTotalLength());
		m_bIgnoreMessage = TRUE;
		m_wndSpanLengthXZ.SetWindowText(m_templateData.tndnSubXZ.strSpanLength);
		m_bIgnoreMessage = FALSE;
	}
	else
	{
		//m_chkSameSpanInfoXZ.EnableWindow(TRUE);

		BOOL bCheck = m_chkSameSpanInterval.GetCheck() ? TRUE : FALSE;    

		if(bCheck)
		{
//       m_wndSpanLengthXZ.EnableWindow(FALSE);     
//       m_wndNumOfSpanXZ.EnableWindow(TRUE);
//       GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(TRUE);
		}
		else
		{
//       m_wndSpanLengthXZ.EnableWindow(TRUE);    
//       m_wndNumOfSpanXZ.EnableWindow(FALSE);
//       GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(FALSE);
		}
	}
	
	EnableDisableAllCtrl(FALSE, m_templateData);
	m_gridXZ.SetTemplateType(m_templateData.tndnSubXZ.nType);	  
	m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, FALSE, 0, TRUE, TRUE);

	RemakeGridDraw(FALSE, TRUE);
	
	//Bitmap을 바꾼다.
	m_pViewer->ChangeBitmap(m_templateData.tndnSubXZ.nType);
}

void CCMDTendonTemplateAddModDlg::OnCmdChangeSpanLenXY()
{
	if(m_bIgnoreMessage)	return;
	
	CString strLeng;
	CArray<double, double> aSpan;

	m_wndSpanLengthXY.GetWindowText(strLeng);	
	CStrParser::GetUEDistance(strLeng, aSpan);
	
	int nSize = aSpan.GetSize();

	if( m_templateData.tndnSubXY.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 ||
		m_templateData.tndnSubXY.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2 )
	{
		if(nSize != 1)
		{
			aSpan.SetSize(1);
			aSpan[0] = m_pTemplateCtrl->GetTotalLength();
			strLeng.Format(_T("%f"), aSpan[0]);
			m_bIgnoreMessage = TRUE;
			m_wndSpanLengthXY.SetWindowText(strLeng);
			m_bIgnoreMessage = FALSE;
			nSize = 1;
		}
	}
	
	m_templateData.tndnSubXY.strSpanLength = strLeng;

	BOOL bCheck = m_chkSameSpanInterval.GetCheck() ? TRUE : FALSE;
	BOOL bOnleyDraw = TRUE;
	if(!bCheck)
	{
		m_bIgnoreMessage = TRUE;
		if(nSize > 0 && m_templateData.tndnSubXY.nNumOfSpan != nSize)
		{ // 다르면 일단 span info의 사이즈를 수정.
			m_templateData.tndnSubXY.aSpanInfo.resize(nSize);
			if(nSize > m_templateData.tndnSubXY.nNumOfSpan)
			{
				m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, TRUE, 0, TRUE, FALSE);
				m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, TRUE, m_templateData.tndnSubXY.nNumOfSpan, FALSE, TRUE);
			}
			m_wndNumOfSpanXY.SetValue(nSize);
			m_templateData.tndnSubXY.nNumOfSpan = nSize;
			bOnleyDraw = FALSE;
		}
		m_bIgnoreMessage = FALSE;
	}

 	if(nSize == m_templateData.tndnSubXY.nNumOfSpan)
	{
		RemakeGridDraw(TRUE, FALSE, bOnleyDraw);
	}
}

void CCMDTendonTemplateAddModDlg::OnCmdChangeSpanLenXZ()
{
	if(m_bIgnoreMessage)	return;
	
	CString strLeng;
	CArray<double, double> aSpan;
	
	m_wndSpanLengthXZ.GetWindowText(strLeng);	
	CStrParser::GetUEDistance(strLeng, aSpan);
	
	int nSize = aSpan.GetSize();  
	
	if( m_templateData.tndnSubXZ.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 ||
		m_templateData.tndnSubXZ.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2 )
	{
		if(nSize != 1)
		{
			aSpan.SetSize(1);
			aSpan[0] = m_pTemplateCtrl->GetTotalLength();
			strLeng.Format(_T("%f"), aSpan[0]);
			m_bIgnoreMessage = TRUE;
			m_wndSpanLengthXZ.SetWindowText(strLeng);
			m_bIgnoreMessage = FALSE;
			nSize = 1;
		}
	}

	m_templateData.tndnSubXZ.strSpanLength = strLeng;

	BOOL bCheck = m_chkSameSpanInterval.GetCheck() ? TRUE : FALSE;
	BOOL bOnleyDraw = TRUE;
	if(!bCheck)
	{
		m_bIgnoreMessage = TRUE;
		if(nSize > 0 && m_templateData.tndnSubXZ.nNumOfSpan != nSize)
		{ // 다르면 일단 span info의 사이즈를 수정.
			m_templateData.tndnSubXZ.aSpanInfo.resize(nSize);
			if(nSize > m_templateData.tndnSubXZ.nNumOfSpan)
			{
				m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, FALSE, 0, TRUE, FALSE);
				m_pTemplateCtrl->GetDefaultSpanValue(m_templateData, FALSE, m_templateData.tndnSubXZ.nNumOfSpan, FALSE, TRUE);
			}
			m_wndNumOfSpanXZ.SetValue(nSize);
			m_templateData.tndnSubXZ.nNumOfSpan = nSize;
			bOnleyDraw = FALSE;
		}
		m_bIgnoreMessage = FALSE;
	}

	if(nSize == m_templateData.tndnSubXZ.nNumOfSpan)
	{
		RemakeGridDraw(FALSE, TRUE, bOnleyDraw);
	}	
}

void CCMDTendonTemplateAddModDlg::OnCmdCheckUseSameSpanInfoXY()
{
	if(m_bIgnoreMessage)	return;
	
	BOOL bCheck = m_chkSameSpanInfoXY.GetCheck() ? TRUE : FALSE;
	
	m_templateData.tndnSubXY.bSameSpanInfo = bCheck;
	
	RemakeGridDraw(TRUE, FALSE);
}

void CCMDTendonTemplateAddModDlg::OnCmdCheckUseSameSpanInfoXZ()
{
	if(m_bIgnoreMessage)	return;
	
	BOOL bCheck = m_chkSameSpanInfoXZ.GetCheck() ? TRUE : FALSE;
	
	m_templateData.tndnSubXZ.bSameSpanInfo = bCheck;

	RemakeGridDraw(FALSE, TRUE);
}

void CCMDTendonTemplateAddModDlg::OnCmdCheckUseSameInterval()
{
	if(m_bIgnoreMessage)	return;
	
	BOOL bCheck = m_chkSameSpanInterval.GetCheck() ? TRUE : FALSE;
	
	m_templateData.bSameSpanInterval = bCheck;
	//EnableDisableCtrlSameSpanLength(bCheck, TRUE, TRUE);

	if(m_templateData.tndnSubXY.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 ||
		m_templateData.tndnSubXY.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2)
	{
//     m_wndSpanLengthXY.EnableWindow(FALSE);
//     m_wndNumOfSpanXY.EnableWindow(FALSE);
	}

	if(m_templateData.tndnSubXZ.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 ||
		m_templateData.tndnSubXZ.nType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2)
	{
//     m_wndSpanLengthXZ.EnableWindow(FALSE);
//     m_wndNumOfSpanXZ.EnableWindow(FALSE);
	}

	EnableDisableAllCtrl(FALSE, m_templateData);

	OnChangeOffset();
	RemakeGridDraw(TRUE, TRUE);
}

void CCMDTendonTemplateAddModDlg::ResetCombobox( BOOL bBoth, UINT nType ) //0: Spline 1 : Round
{
	int i, j;
	int nSize;
	int nIdxInsert;
	std::vector<TNDN_TEMPLATE_TYPE>* typeVec;
	MComboBox* pCbx;
	
	for(i = 0 ; i < 2; i++)
	{
		if(i == 0 && bBoth == FALSE)
			continue;

		if(i == 0)      pCbx = &m_cbxTypeXY;
		else if(i == 1) pCbx = &m_cbxTypeXZ;

		pCbx->ResetContent();

		if(i == 0) //xy는 무조건 7개를 포함하고 , xz는 xy의 선택에 따라서 종류가 바뀐다.
		{
			typeVec = &m_vecEnType;
			nSize	= (int)typeVec->size();
		}    
		else if(nType == 0 ) // spline
		{
			typeVec	= &m_vecSplineType;
			nSize	= (int)typeVec->size();
		}
		else if(nType == 1) //Round
		{
			typeVec	= &m_vecRoundType;
			nSize	= (int)typeVec->size();
		}
		else
		{
			ASSERT(0);
			continue;
		}		

		std::vector<CString> svStrComboImage;
		for(j = 0; j < nSize; j++)
		{
			UINT nTndnType = (UINT)(*typeVec)[j];
			CString typeName;			
			m_pTemplateCtrl->GetTypeName((TNDN_TEMPLATE_TYPE)nTndnType, typeName);
			svStrComboImage.push_back(m_svStrComboImage[nTndnType]);

			nIdxInsert = pCbx->AddString(typeName);
			pCbx->SetItemData(nIdxInsert, nTndnType);
		}
		pCbx->SetImageList(svStrComboImage);

		pCbx->SetCurSel(0);
	}
}

void CCMDTendonTemplateAddModDlg::SetCurSel()
{
	TNDN_TEMPLATE_TYPE tndnType;
	int i;
	int nCnt;
	
	nCnt = m_cbxTypeXY.GetCount();
	for(i = 0 ; i < nCnt; i++)
	{
		tndnType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXY.GetItemData(i);
		if(tndnType == m_templateData.tndnSubXY.nType)
		{
			m_cbxTypeXY.SetCurSel(i);
			break;
		}
	}

	nCnt =  m_cbxTypeXZ.GetCount();
	for(i = 0 ; i < nCnt; i++)
	{
		tndnType = (TNDN_TEMPLATE_TYPE)m_cbxTypeXZ.GetItemData(i);
		if(tndnType == m_templateData.tndnSubXZ.nType)
		{
			m_cbxTypeXZ.SetCurSel(i);
			break;
		}
	}
}

void CCMDTendonTemplateAddModDlg::OnChangeOffset()
{
	if(m_bIgnoreMessage)	return;
	
	BOOL bUseSameSpan = m_templateData.bSameSpanInterval;
	
	m_templateData.dOffsetFront	= m_wndOffsetFront.GetEditValue();
	m_templateData.dOffsetEnd	= m_wndOffsetEnd.GetEditValue();

	if(bUseSameSpan)
	{
		m_bIgnoreMessage = TRUE;

		m_templateData_Draw = m_pTemplateCtrl->ConvertUnitIn(m_templateData);

		CString strTemp;

		m_pTemplateCtrl->GetSameSpanStr(m_templateData_Draw, TRUE, strTemp);
		m_pTemplateCtrl->ConvertUnitOut(strTemp);
		m_templateData.tndnSubXY.strSpanLength = strTemp;
		m_wndSpanLengthXY.SetWindowText(strTemp);

		m_pTemplateCtrl->GetSameSpanStr(m_templateData_Draw, FALSE, strTemp);
		m_pTemplateCtrl->ConvertUnitOut(strTemp);
		m_templateData.tndnSubXZ.strSpanLength = strTemp;
		m_wndSpanLengthXZ.SetWindowText(strTemp);

		m_bIgnoreMessage = FALSE;
	}

	RemakeGridDraw(TRUE, TRUE, TRUE);
}

void CCMDTendonTemplateAddModDlg::RemakeGridDraw( BOOL bXY, BOOL bXZ, BOOL bOnlyDraw /*= FALSE*/ )
{
	BOOL bSuc = Dlg2Data();

	if(m_bPreSuccess_Dlg2Data == FALSE && bSuc == TRUE)//전에는 실패했고, 이번에 성공했다면.
	{
		bXY = TRUE;
		bXZ = TRUE;
	}
	
	m_bPreSuccess_Dlg2Data = bSuc;

	if(!bSuc)
	{
		return;
	}

	if(bXY)
	{
		m_gridXY.LockUpdate(TRUE);
		if(!bOnlyDraw)
		{
			m_gridXY.MakeItemEx();
		}
		m_templateData_Draw = m_pTemplateCtrl->ConvertUnitIn(m_templateData);
		BOOL bSuccess = m_pDrawXY->SetTdnaData(&m_templateData_Draw);
		if(bSuccess == TRUE && m_bPreSuccess == FALSE)
		{
			m_pDrawXZ->SetTdnaData(&m_templateData_Draw);

		}
		m_bPreSuccess = bSuccess;
		
		m_gridXY.SetTemplateTypeSameSpan(TRUE, m_templateData_Draw.tndnSubXY.bSameSpanInfo);
		m_gridXY.LockUpdate(FALSE);
		m_gridXY.Redraw();
	}
	if(bXZ)
	{
		m_gridXZ.LockUpdate(TRUE);
		if(!bOnlyDraw)
		{
			m_gridXZ.MakeItemEx();
		}
		m_templateData_Draw = m_pTemplateCtrl->ConvertUnitIn(m_templateData);
		BOOL bSuccess = m_pDrawXZ->SetTdnaData(&m_templateData_Draw);
		if(bSuccess == TRUE && m_bPreSuccess == FALSE)
		{
			m_pDrawXY->SetTdnaData(&m_templateData_Draw);
		}
		m_bPreSuccess = bSuccess;
		
		m_gridXZ.SetTemplateTypeSameSpan(FALSE, m_templateData_Draw.tndnSubXZ.bSameSpanInfo);
		m_gridXZ.LockUpdate(FALSE);
		m_gridXZ.Redraw();
	}
}

void CCMDTendonTemplateAddModDlg::OnCmdCheckSymmXY()
{
	if(m_bIgnoreMessage)	return;

	BOOL bCheck = m_chkSymXAxisXY.GetCheck()?TRUE:FALSE;
	m_templateData.tndnSubXY.bXSymmetry = bCheck;

	RemakeGridDraw(TRUE, FALSE, TRUE);
}

void CCMDTendonTemplateAddModDlg::OnCmdCheckSymmXZ()
{
	if(m_bIgnoreMessage)	return;

	BOOL bCheck = m_chkSymXAxisXZ.GetCheck()?TRUE:FALSE;
	m_templateData.tndnSubXZ.bXSymmetry = bCheck;

	RemakeGridDraw(FALSE, TRUE, TRUE);
}

void CCMDTendonTemplateAddModDlg::PostNcDestroy()
{
	CCMDlgBase::PostNcDestroy(); 
}

/*
void CCMDTendonTemplateAddModDlg::EnableDisableCtrlSameSpanLength( BOOL bSet, BOOL bXY, BOOL bXZ )
{
	return;
	if(bSet)
	{
		m_wndOffsetEnd.EnableWindow(TRUE);
		if(bXY)
		{
			m_wndNumOfSpanXY.EnableWindow(TRUE);
			m_wndSpanLengthXY.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(TRUE);
		}
		if(bXZ)
		{
			m_wndNumOfSpanXZ.EnableWindow(TRUE);
			m_wndSpanLengthXZ.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_wndOffsetEnd.EnableWindow(FALSE);
		if(bXY)
		{
			m_wndNumOfSpanXY.EnableWindow(FALSE);
			m_wndSpanLengthXY.EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(FALSE);
		}
		if(bXZ)
		{
			m_wndNumOfSpanXZ.EnableWindow(FALSE);
			m_wndSpanLengthXZ.EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(FALSE);
		}
	}  
}
*/

void CCMDTendonTemplateAddModDlg::OnUpdate( CWnd* pSender, LPARAM lHint, CObject* pHint )
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_BUFFER_AFTER:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
		// 방식 변경:후처리 모드에서 Modify하는 경우 전처리로 변경되면서 에러 야기
		DestroyViewWindow();
		DestroyWindow();
		break;
	case D_UPDATE_UNIT:
		OnUnitChange();
		m_gridXY.SetTemplateTypeSameSpan(TRUE,  m_templateData_Draw.tndnSubXY.bSameSpanInfo);
		m_gridXZ.SetTemplateTypeSameSpan(FALSE, m_templateData_Draw.tndnSubXZ.bSameSpanInfo);
	default:
		break;
	}	  
}

void CCMDTendonTemplateAddModDlg::OnUnitChange()
{
	int i, nSize;
	double dFactorCur = m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	double dFactorPre = m_pDoc->m_pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
	double dFactor = dFactorPre / dFactorCur;

	caDouble caSpanLength;
	if(CStrParser::GetUEDistance(m_templateData.tndnSubXY.strSpanLength, caSpanLength))
	{
		nSize = caSpanLength.GetSize();
		for(i = 0 ; i < nSize; i++)
		{
			caSpanLength[i] = dFactor * caSpanLength[i];
		}
		m_templateData.tndnSubXY.strSpanLength = CStrParser::ConvtDistArr2DistStr(caSpanLength);
		m_wndSpanLengthXY.SetWindowText(m_templateData.tndnSubXY.strSpanLength);
	}

	if(CStrParser::GetUEDistance(m_templateData.tndnSubXZ.strSpanLength, caSpanLength))
	{
		nSize = caSpanLength.GetSize();
		for(i = 0 ; i < nSize; i++)
		{
			caSpanLength[i] = dFactor * caSpanLength[i];
		}
		m_templateData.tndnSubXZ.strSpanLength = CStrParser::ConvtDistArr2DistStr(caSpanLength);
		m_wndSpanLengthXZ.SetWindowText(m_templateData.tndnSubXZ.strSpanLength);
	}

	nSize = (int)m_templateData.tndnSubXY.aSpanInfo.size();
	for(i = 0 ; i < nSize; i++)
	{
		m_templateData.tndnSubXY.aSpanInfo[i].dDb *= dFactor;
		m_templateData.tndnSubXY.aSpanInfo[i].dDb1 *= dFactor;
		m_templateData.tndnSubXY.aSpanInfo[i].dDe *= dFactor;
		m_templateData.tndnSubXY.aSpanInfo[i].dDf *= dFactor;
		m_templateData.tndnSubXY.aSpanInfo[i].dDt *= dFactor;
		m_templateData.tndnSubXY.aSpanInfo[i].dDt1 *= dFactor;
		m_templateData.tndnSubXY.aSpanInfo[i].dR1 *= dFactor;
		m_templateData.tndnSubXY.aSpanInfo[i].dR2 *= dFactor;
	}
	nSize = (int)m_templateData.tndnSubXZ.aSpanInfo.size();
	for(i = 0 ; i < nSize; i++)
	{
		m_templateData.tndnSubXZ.aSpanInfo[i].dDb *= dFactor;
		m_templateData.tndnSubXZ.aSpanInfo[i].dDb1 *= dFactor;
		m_templateData.tndnSubXZ.aSpanInfo[i].dDe *= dFactor;
		m_templateData.tndnSubXZ.aSpanInfo[i].dDf *= dFactor;
		m_templateData.tndnSubXZ.aSpanInfo[i].dDt *= dFactor;
		m_templateData.tndnSubXZ.aSpanInfo[i].dDt1 *= dFactor;
		m_templateData.tndnSubXZ.aSpanInfo[i].dR1 *= dFactor;
		m_templateData.tndnSubXZ.aSpanInfo[i].dR2 *= dFactor;
	}
	RemakeGridDraw(TRUE, TRUE, TRUE);
	m_gridXZ.OnUnitChange();
	m_gridXY.OnUnitChange();
}

/*
void CCMDTendonTemplateAddModDlg::EnableDisableCtrlSpin( BOOL bEnableXY, BOOL bEnableXZ )
{  
	return ;
	if(bEnableXY)
	{
		m_chkSameSpanInterval.SetCheck(TRUE);
		m_wndSpanLengthXY.EnableWindow(FALSE);
		m_wndSpanLengthXZ.EnableWindow(FALSE);
		m_templateData.bSameSpanInterval = TRUE;
		m_wndNumOfSpanXY.SetWindowText(_T("1"));
		m_wndNumOfSpanXY.EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(FALSE);
		m_chkSameSpanInfoXY.EnableWindow(FALSE);
	}
	else
	{
		m_chkSameSpanInfoXY.EnableWindow(TRUE);
		
		BOOL bCheck = m_chkSameSpanInterval.GetCheck() ? TRUE : FALSE;
		
		if(bCheck)
		{
			m_wndSpanLengthXY.EnableWindow(FALSE); 
			m_wndNumOfSpanXY.EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(TRUE);
		}
		else
		{
			m_wndSpanLengthXY.EnableWindow(TRUE);  
			m_wndNumOfSpanXY.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(FALSE);
		}
	}
	
	if(bEnableXZ)
	{
		m_chkSameSpanInterval.SetCheck(TRUE);
		m_wndSpanLengthXY.EnableWindow(FALSE);
		m_wndSpanLengthXZ.EnableWindow(FALSE);
		m_templateData.bSameSpanInterval = TRUE;
		m_wndNumOfSpanXZ.SetWindowText(_T("1"));
		m_wndNumOfSpanXZ.EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(FALSE);
		m_chkSameSpanInfoXZ.EnableWindow(FALSE);
	}
	else
	{
		m_chkSameSpanInfoXZ.EnableWindow(TRUE);
		
		BOOL bCheck = m_chkSameSpanInterval.GetCheck() ? TRUE : FALSE;    
		
		if(bCheck)
		{
			m_wndSpanLengthXZ.EnableWindow(FALSE);     
			m_wndNumOfSpanXZ.EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(TRUE);
		}
		else
		{
			m_wndSpanLengthXZ.EnableWindow(TRUE);    
			m_wndNumOfSpanXZ.EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(FALSE);
		}
	}
}
*/

BOOL CCMDTendonTemplateAddModDlg::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message == WM_KEYDOWN)
	{
		//엔터 치면...
		if(pMsg->wParam == VK_RETURN)
		{
			m_gridXY.OnTerminate(D_TB_SAVE_ALWAYS);     
			m_gridXZ.OnTerminate(D_TB_SAVE_ALWAYS);
			RemakeGridDraw(TRUE, TRUE, TRUE);
		}
	}
	
	return CCMDlgBase::PreTranslateMessage(pMsg);
}

void CCMDTendonTemplateAddModDlg::DestroyViewWindow()
{
	if(m_pViewer)
	{
		if(::IsWindow(m_pViewer->GetSafeHwnd()))
		{
			m_pViewer->DestroyWindow();
			m_pViewer = NULL;  
		}
	}  
}

void CCMDTendonTemplateAddModDlg::EnableDisableAllCtrl( BOOL bData2Dlg, TndnTemplate& tndnTemp )
{  
	if(bData2Dlg)//최초의 세팅값... add/mod대화상자가 열리고 바귀지 않는 것들.
	{
		if(m_pParent)
		{
			//이름
			BOOL bUsePrefix = m_pParent->IsUsePrefix();
			BOOL bIsCurveElem = m_pParent->IsCurveElem();
		
			if(bUsePrefix) m_wndTndnName.EnableWindow(FALSE);	
			else           m_wndTndnName.EnableWindow(TRUE);	
			
			//요소가 휘어있다면 Ref. Axis 가 Curve Type으로 출력될 수도 있다.
			m_rdoRefAxisCurve.EnableWindow(bIsCurveElem);
			if(bIsCurveElem)
			{
				m_rdoRefAxisElem.SetCheck(!tndnTemp.bCurveTypeOut);
				m_rdoRefAxisCurve.SetCheck(tndnTemp.bCurveTypeOut);
			}
			else
			{
				tndnTemp.bCurveTypeOut = FALSE;
				m_rdoRefAxisElem.SetCheck(TRUE);
				m_rdoRefAxisCurve.SetCheck(FALSE);
			}
		}
		m_wndTndnName.SetWindowText(tndnTemp.strName);
	}
	else
	{
		BOOL bSameSpanLen = tndnTemp.bSameSpanInterval;
		TNDN_TEMPLATE_TYPE enTypeXY = tndnTemp.tndnSubXY.nType;
		TNDN_TEMPLATE_TYPE enTypeXZ = tndnTemp.tndnSubXZ.nType;
		
		BOOL bNotSimpleCurveXY = enTypeXY != TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 &&
			enTypeXY != TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2;
		BOOL bNotSimpleCurveXZ = enTypeXZ != TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1 &&
			enTypeXZ != TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2;
		
		//front, end offset.
		m_wndOffsetFront.EnableWindow(TRUE);
		m_wndOffsetEnd.EnableWindow(bSameSpanLen);
		m_wndSpanLengthXY.EnableWindow(!bSameSpanLen);
		m_wndSpanLengthXZ.EnableWindow(!bSameSpanLen);
		
		//Simple이 아닐때..
		if(bNotSimpleCurveXY)
		{
			m_chkSameSpanInfoXY.EnableWindow(TRUE);
			m_wndNumOfSpanXY.EnableWindow(bSameSpanLen);
			//GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(bSameSpanLen);
		}
		else //Simple Curve type
		{
			m_chkSameSpanInfoXY.EnableWindow(FALSE);
			m_wndNumOfSpanXY.EnableWindow(FALSE);
			//m_wndSpanLengthXY.EnableWindow(FALSE);
			//GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XY_SPIN)->EnableWindow(FALSE);
		}
		
		if(bNotSimpleCurveXZ)
		{
			m_chkSameSpanInfoXZ.EnableWindow(TRUE);
			m_wndNumOfSpanXZ.EnableWindow(bSameSpanLen);
			//GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(bSameSpanLen);    
		}
		else //Simple Curve type
		{
			m_chkSameSpanInfoXZ.EnableWindow(FALSE);
			m_wndNumOfSpanXZ.EnableWindow(FALSE);
			//m_wndSpanLengthXZ.EnableWindow(FALSE);
			//GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADDMOD_NUM_SPAN_XZ_SPIN)->EnableWindow(FALSE);
		}
	}
}
