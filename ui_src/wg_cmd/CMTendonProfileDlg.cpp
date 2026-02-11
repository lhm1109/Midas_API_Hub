// CMTendonProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonProfileDlg.h"


#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#include "CMTendonDlg.h"
#include "CMTendonTypeDlg.h"
#include "CMTendonDefDlg.h"
#include "ExportFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTendonProfileDlg dialog
#define CDialog CCMDlgBase

CCMTendonProfileDlg::CCMTendonProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMTendonProfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTendonProfileDlg)
//  m_bBottomZ = FALSE; //dlg에서 삭제 2005.10.25 jkpark
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();  // Add By Pig

	m_TdnaKey = 0;
	m_pParentDlg = 0;
	m_bIgnoreNotify = TRUE;
	m_bExternalSetting = 0;

	m_nAutoOrUser = 0;

	m_pModalDlg = 0;
	m_bCloseDlg = FALSE;
	m_bUserDef = FALSE;
	m_bDebonded = FALSE;

	// to show and hide Tendon count menu 
	m_aCtrlTendonCount.Add(IDC_CMD_STATIC5);
	m_aCtrlTendonCount.Add(IDC_CMD_TENDON_COUNT_EDIT);
	//m_aCtrlTendonCount.Add(IDC_CMD_TENDON_COUNT_SPIN);

	m_aCtrlType2D.Add(IDC_CMD_TENDON_TABLE_XY2);
	m_aCtrlType2D.Add(IDC_CMD_TENDON_TABLE_XZ2);
	m_aCtrlType3D.Add(IDC_CMD_TENDON_TABLE);

	m_aCtrlType2DTemp.Add(IDC_CMD_TENDON_GRAPH_XY);
	m_aCtrlType2DTemp.Add(IDC_CMD_TENDON_TABLE_XY2);
	m_aCtrlType2DTemp.Add(IDC_CMD_TENDON_GRAPH_XZ);
	m_aCtrlType2DTemp.Add(IDC_CMD_TENDON_TABLE_XZ2);

	m_aCtrlType3DTemp.Add(IDC_CMD_TENDON_GRAPH_XY);
	m_aCtrlType3DTemp.Add(IDC_CMD_TENDON_GRAPH_XZ);
	m_aCtrlType3DTemp.Add(IDC_CMD_TENDON_TABLE);

	m_aCtrlTypeRadio.Add(IDC_CMD_RADIO_2D);
	m_aCtrlTypeRadio.Add(IDC_CMD_RADIO_3D);

	m_aCtrlSymPtRadio.Add(IDC_CMD_SYM_POINT_FIRST);
	m_aCtrlSymPtRadio.Add(IDC_CMD_SYM_POINT_LAST);

	m_aCtrlTendonShape.Add(IDC_CMD_TENDON_SHAPE_RADIO_STRAIGHT);
	m_aCtrlTendonShape.Add(IDC_CMD_TENDON_SHAPE_RADIO_CURVE);
	m_aCtrlTendonShape.Add(IDC_CMD_TENDON_SHAPE_RADIO_ELEM);

	m_aCtrlXAxisDir.Add(IDC_CMD_TENDON_DIR_RADIO_X);
	m_aCtrlXAxisDir.Add(IDC_CMD_TENDON_DIR_RADIO_Y);
	m_aCtrlXAxisDir.Add(IDC_CMD_TENDON_DIR_RADIO_VECTOR);

	m_aCtrlCommon.Add(IDC_CMD_TENDON_INSERT_POS_TITLE);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_INSERT_POS_EDIT);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_INSERT_POS_UNIT);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_ROT_ANGLE_TITLE);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_AXIS_ANGLE_EDIT);
	//m_aCtrlCommon.Add(IDC_CMD_TENDON_AXIS_ANGLE_SPIN);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_AXIS_ANGLE_UNIT);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_PROJECTION_CHK);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_GRAD_ANGLE_TITLE);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_GRAD_ANGLE_COBX);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_GRAD_ANGLE_EDIT);
	//m_aCtrlCommon.Add(IDC_CMD_TENDON_GRAD_ANGLE_SPIN);
	m_aCtrlCommon.Add(IDC_CMD_TENDON_GRAD_ANGLE_UNIT);

	m_aCtrlStraight.Add(IDC_CMD_TENDON_DIR_TITLE);
	//m_aCtrlStraight.Add(IDC_CMD_TENDON_DIR_COLON);
	m_aCtrlStraight.Append(m_aCtrlXAxisDir);
	m_aCtrlStraight.Add(IDC_CMD_TENDON_VECTOR_EDIT);
	m_aCtrlStraight.Add(IDC_CMD_TENDON_VECTOR_UNIT);

	m_aCtrlCurve.Add(IDC_CMD_TENDON_CENTER_TITLE);
	//m_aCtrlCurve.Add(IDC_CMD_TENDON_CENTER_COLON);
	m_aCtrlCurve.Add(IDC_CMD_TENDON_CENTER_EDIT);
	m_aCtrlCurve.Add(IDC_CMD_TENDON_CENTER_UNIT);
	m_aCtrlCurve.Add(IDC_CMD_TENDON_OFFSET_TITLE);
	//m_aCtrlCurve.Add(IDC_CMD_TENDON_OFFSET_COLON);
	m_aCtrlCurve.Add(IDC_CMD_TENDON_OFFSET_EDIT);
	m_aCtrlCurve.Add(IDC_CMD_TENDON_OFFSET_UNIT);
	m_aCtrlCurve.Add(IDC_CMD_TENDON_CDIR_TITLE);
	m_aCtrlCurve.Add(IDC_CMD_TENDON_CDIR_COBX);

	m_aCtrlElement.Add(IDC_CMD_ELEM_STATIC1);
	m_aCtrlElement.Add(IDC_CMD_INSERTPOS_I_RD);
	m_aCtrlElement.Add(IDC_CMD_INSERTPOS_J_RD);
	m_aCtrlElement.Add(IDC_CMD_INSERTPOS_EDIT);
	m_aCtrlElement.Add(IDC_CMD_ELEM_STATIC2);
	m_aCtrlElement.Add(IDC_CMD_X_AXIS_I_RD);
	m_aCtrlElement.Add(IDC_CMD_X_AXIS_J_RD);
	m_aCtrlElement.Add(IDC_CMD_X_AXIS_EDIT);
	m_aCtrlElement.Add(IDC_CMD_ELEM_STATIC3);
	m_aCtrlElement.Add(IDC_CMD_ROT_ANGLE_EDIT);
	//m_aCtrlElement.Add(IDC_CMD_ROT_ANGLE_SPIN);
	m_aCtrlElement.Add(IDC_CMD_ANGLE_UNIT);
	m_aCtrlElement.Add(IDC_CMD_PROJECTION_CHECK);
	m_aCtrlElement.Add(IDC_CMD_ELEM_STATIC5);
	m_aCtrlElement.Add(IDC_CMD_OFFSET_Y_EDIT);
	m_aCtrlElement.Add(IDC_CMD_OFFSET_Y_UNIT);
	m_aCtrlElement.Add(IDC_CMD_ELEM_STATIC6);
	m_aCtrlElement.Add(IDC_CMD_OFFSET_Z_EDIT);
	m_aCtrlElement.Add(IDC_CMD_OFFSET_Z_UNIT);

	m_aCtrlVector.Add(IDC_CMD_TENDON_VECTOR_EDIT);
	m_aCtrlVector.Add(IDC_CMD_TENDON_VECTOR_UNIT);

	m_aCtrlCurveType.Add(IDC_CMD_RADIO_SPLINE);
	m_aCtrlCurveType.Add(IDC_CMD_RADIO_ROUND);
	m_aCtrlCurveType.Add(IDC_CMD_RADIO_PARABOLA);

	m_aCtrlXAxisInsertPos.Add(IDC_CMD_INSERTPOS_I_RD);
	m_aCtrlXAxisInsertPos.Add(IDC_CMD_INSERTPOS_J_RD);

	m_aCtrlXAxisDirection.Add(IDC_CMD_X_AXIS_I_RD);
	m_aCtrlXAxisDirection.Add(IDC_CMD_X_AXIS_J_RD);

	m_aCtrlBeginEndLength.Add(IDC_CMD_BEGIN_LENGTH_EDIT);
	m_aCtrlBeginEndLength.Add(IDC_CMD_END_LENGTH_EDIT); 

	m_TdnaData.Initialize();
}

void CCMTendonProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTendonProfileDlg)
	DDX_Control(pDX, IDC_CMD_TENDON_PROP_POST_TENSION, m_cobxPostTension);
	DDX_Control(pDX, IDC_CMD_TENDON_GROUP_COMBO, m_wndTdgrCobx);
	DDX_Control(pDX, IDC_CMD_END_USER_EDIT, m_wndEndUser);
	DDX_Control(pDX, IDC_CMD_BEGIN_USER_EDIT, m_wndBeginUser);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT10, m_unitEndUser);

	DDX_Control(pDX, IDC_CMD_TDNA_DEBONDED_LENGTH_BEGIN_EDIT, m_wndDebondBegin);
	DDX_Control(pDX, IDC_CMD_TDNA_DEBONDED_LENGTH_END_EDIT,   m_wndDebondEnd);
	DDX_Control(pDX, IDC_CMD_TDNA_DEBONDED_LENGTH_END_UNIT,   m_uniDebondEnd);

	DDX_Control(pDX, IDC_CMD_INSERTPOS_EDIT, m_editInsertPos);
	DDX_Control(pDX, IDC_CMD_X_AXIS_EDIT, m_editXDir);
	DDX_Control(pDX, IDC_CMD_ANGLE_UNIT, m_unitAngle);
	DDX_Control(pDX, IDC_CMD_ROT_ANGLE_EDIT, m_editAngle);
// 	DDX_Control(pDX, IDC_CMD_ROT_ANGLE_SPIN, m_spinAngle);
	DDX_Control(pDX, IDC_CMD_PROJECTION_CHECK, m_chkProjection2);
	DDX_Control(pDX, IDC_CMD_OFFSET_Z_UNIT, m_unitOffsetZ);
	DDX_Control(pDX, IDC_CMD_OFFSET_Z_EDIT, m_editOffsetZ);
	DDX_Control(pDX, IDC_CMD_OFFSET_Y_UNIT, m_unitOffsetY);
	DDX_Control(pDX, IDC_CMD_OFFSET_Y_EDIT, m_editOffsetY);
	DDX_Control(pDX, IDC_CMD_TENDON_CDIR_COBX, m_cobxCurveDir);
	DDX_Control(pDX, IDC_CMD_TENDON_GRAD_ANGLE_UNIT, m_txtGradUnit);
// 	DDX_Control(pDX, IDC_CMD_TENDON_GRAD_ANGLE_SPIN, m_wndGradSpin);
	DDX_Control(pDX, IDC_CMD_TENDON_GRAD_ANGLE_EDIT, m_wndGradAngle);
	DDX_Control(pDX, IDC_CMD_TENDON_GRAD_ANGLE_COBX, m_cobxGradAxis);
	DDX_Control(pDX, IDC_CMD_TENDON_PROJECTION_CHK, m_chkProjection);
	DDX_Control(pDX, IDC_CMD_TENDON_OFFSET_UNIT, m_txtOffsetUnit);
	DDX_Control(pDX, IDC_CMD_TENDON_OFFSET_EDIT, m_wndOffsetEdit);
	DDX_Control(pDX, IDC_CMD_TENDON_CENTER_UNIT, m_txtCenterUnit);
	DDX_Control(pDX, IDC_CMD_TENDON_CENTER_EDIT, m_wndCenterEdit);
	DDX_Control(pDX, IDC_CMD_TENDON_VECTOR_UNIT, m_txtVectorUnit);
	DDX_Control(pDX, IDC_CMD_TENDON_VECTOR_EDIT, m_wndVectorEdit);
	DDX_Control(pDX, IDC_CMD_TENDON_AXIS_ANGLE_UNIT, m_txtAxisAngleUnit);
// 	DDX_Control(pDX, IDC_CMD_TENDON_AXIS_ANGLE_SPIN, m_wndAxisAngleSpin);
	DDX_Control(pDX, IDC_CMD_TENDON_AXIS_ANGLE_EDIT, m_wndAxisAngleEdit);
	DDX_Control(pDX, IDC_CMD_TENDON_INSERT_POS_UNIT, m_txtInsertPointUnit);
	DDX_Control(pDX, IDC_CMD_TENDON_INSERT_POS_EDIT, m_wndInsertPoint);
	DDX_Control(pDX, IDC_CMD_TENDON_PROP_COMBO, m_wndTdntCobx);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT4, m_txtEndLengthUnit);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT3, m_txtBeginLengthUnit);
	DDX_Control(pDX, IDC_CMD_END_LENGTH_EDIT, m_wndEndLength);
	DDX_Control(pDX, IDC_CMD_BEGIN_LENGTH_EDIT, m_wndBeginLength);
	DDX_Control(pDX, IDC_CMD_ASSIGNED_ELEM_EDIT, m_wndAssignElem);
	DDX_Control(pDX, IDC_CMD_TENDON_NAME_EDIT, m_wndTendonName);

	DDX_Control(pDX, IDC_CMD_TENDON_TYPICALTENDON_CHK, m_chkTypicalTendon);   // No. of Tendon. 
	DDX_Control(pDX, IDC_CMD_TENDON_COUNT_EDIT, m_wndTendonCountEdit);   // No. of Tendon. 
// 	DDX_Control(pDX, IDC_CMD_TENDON_COUNT_SPIN, m_wndTendonCountSpin);   // No. of Tendon. 
//  DDX_Check  (pDX, IDC_CMD_BOTTOM_CHK, m_bBottomZ); //dlg에서 삭제 2005.10.25 jkpark

	DDX_Control(pDX, IDC_CMD_TENDON_DETAIL_GRP,           m_chkDetail);

	DDX_Control(pDX, IDC_CMD_TENDON_TABLE, m_wndGrid3D);
	DDX_Control(pDX, IDC_CMD_TENDON_TABLE_XY2, m_wndGrid2DXY);
	DDX_Control(pDX, IDC_CMD_TENDON_TABLE_XZ2, m_wndGrid2DXZ);
	//}}AFX_DATA_MAP
}

void CCMTendonProfileDlg::SetTDNA(T_TDNA_K TdnaKey)
{
	m_TdnaKey = TdnaKey;  // if 0 : add else modify
	if (m_TdnaKey != 0) 
	{
		m_pDoc->m_pAttrCtrl->GetTdna(m_TdnaKey, m_TdnaData);
		m_csOldTdnaName = m_TdnaData.TendonName;
	}
	else 
	{
		m_TdnaData.Initialize();
		// 중국 버전은 직선으로.
#if defined(_CH)
		m_TdnaData.nTendonShape = 0;
#endif
	}
}

void CCMTendonProfileDlg::SetParentDlg(CWnd *pParentDlg)
{
	m_pParentDlg = pParentDlg;
	((CCMTendonDlg*)m_pParentDlg)->SetProfDlgFlag(TRUE);
}

void CCMTendonProfileDlg::ExternalSetting(T_TDNA_K key)
{
	m_TdnaKey = key;  
	if (!m_pDoc->m_pAttrCtrl->GetTdna(m_TdnaKey, m_TdnaData)) 
	{ ASSERT(0); return; }
	m_bExternalSetting = TRUE;

	// selected tendon
	if (I_GENModelBase::GetCurMySelfST())
	{
		I_GENModelBase::UTIL_DeleteAllDisplayObjST();
		I_GENModelBase::GetCurMySelfST()->UTIL_SetDisplayObj(UR_TDNA_ADD, &m_TdnaKey);
		I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()->Invalidate(FALSE);
	}
	m_csOldTdnaName = m_TdnaData.TendonName;
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	Data2Dlg();
}

void CCMTendonProfileDlg::AlignControls2(int type)
{
	CArray<UINT, UINT>* pControls;
	if (type == 0) pControls = &m_aCtrlType2DTemp;
	else if (type == 1) pControls = &m_aCtrlType3DTemp;
	else return;
	
	double dHeight;
	CRect rc, prerc;

	// 처음것은 그대로 놔두고 다음 것부터 
	for (int i=1; i<pControls->GetSize(); i++)
	{
		GetDlgItem((pControls->GetAt(i-1)))->GetWindowRect(&prerc);
		this->ScreenToClient(&prerc);
		GetDlgItem((pControls->GetAt(i)))->GetWindowRect(&rc);
		this->ScreenToClient(&rc);
		dHeight = rc.Height();
		rc.top = prerc.bottom + globalUtils.ScaleByDPI(3);    
		rc.bottom = rc.top + dHeight;
		if (pControls->GetAt(i) == IDC_CMD_TENDON_GRAPH_XY)
			m_GraphView3DXY.SetWindowPos(NULL,rc.left+2,rc.top+2,rc.Width()-4,rc.Height()-4,SWP_NOZORDER|SWP_NOACTIVATE);  
		if (pControls->GetAt(i) == IDC_CMD_TENDON_GRAPH_XZ)
			m_GraphView3DXZ.SetWindowPos(NULL,rc.left+2,rc.top+2,rc.Width()-4,rc.Height()-4,SWP_NOZORDER|SWP_NOACTIVATE);  
	  GetDlgItem((pControls->GetAt(i)))->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
}

void CCMTendonProfileDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(m_aCtrlStraight[0])->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlCurve[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlCurve, nDistY);

	GetDlgItem(IDC_CMD_TENDON_INSERT_POS_TITLE)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlElement[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlElement, nDistY);

	// resize self size
	CRect rectLast;
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);  
}

BOOL CCMTendonProfileDlg::DoApply()
{
	if (!Dlg2Data()) return FALSE;

	// 여기서 Type으로 저장할 그리드만 저장해주고, 나머지는 RemoveAll 시켜준다.
	// 만약 편집중이면 편집중인 데이타 저장
	CArray<double, double> aKey;
	if (m_TdnaData.nInputType == 0) // 2D
	{
		if (!m_wndGrid2DXY.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;
		if (!m_wndGrid2DXZ.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;
		//aKey.RemoveAll();
		//for (int i=0; i<m_TdnaData.aProfile.GetSize(); i++)
		//  aKey.Add(m_TdnaData.aProfile[i].x);
		//m_wndGrid3D.DelData(aKey);
	}
	else // 3D
	{
		if (!m_wndGrid3D.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;
		//aKey.RemoveAll();
		//for (int i=0; i<m_TdnaData.aProfileY.GetSize(); i++)
		//  aKey.Add(m_TdnaData.aProfileY[i].x);
		//m_wndGrid2DXY.DelData(aKey);
		//aKey.RemoveAll();
		//for (i=0; i<m_TdnaData.aProfileZ.GetSize(); i++)
		//  aKey.Add(m_TdnaData.aProfileZ[i].x);
		//m_wndGrid2DXZ.DelData(aKey);
	}

	BOOL bAdd;
	if (m_csOldTdnaName.IsEmpty()) bAdd = TRUE;
	else bAdd = FALSE;

	// I_GENModel::__LableTendon 에서 적용된 함수와 동일.
	// 뷰가 안그려지는 경우는 저장이 안되도록.
	CArray<T_NODE_D, T_NODE_D&> aCPoints;
	CArray<BOOL, BOOL>          aInputPointFlag;
	if (!m_pDoc->m_pAttrCtrl->calcTendonProfile(m_TdnaData, aCPoints, aInputPointFlag, 36, FALSE, TRUE))
		return FALSE;

	// Add 또는 Modify였는데 데이타가 삭제된 경우
	if (bAdd || m_TdnaKey == 0 || !m_pDoc->m_pAttrCtrl->ExistTdna(m_TdnaKey))
	{
		if (!m_pDoc->m_pDataCtrl->AddTdna(m_TdnaData)) return FALSE;
		m_TdnaKey = m_pDoc->m_pAttrCtrl->GetTdnaKey(m_TdnaData.TendonName);
		if (!bAdd) m_csOldTdnaName = m_TdnaData.TendonName;
	}
	else 
	{
		if (!m_pDoc->m_pDataCtrl->ModifyTdna(m_csOldTdnaName, m_TdnaData)) return FALSE;
		m_csOldTdnaName = m_TdnaData.TendonName;

		if(m_pParentDlg) 
			((CCMTendonDlg*)m_pParentDlg)->DisplayCurrentTendon(m_TdnaKey);
	}
	return TRUE;
}

BOOL CCMTendonProfileDlg::Dlg2Data()
{
	UpdateData(TRUE);

//  m_TdnaData.Initialize();
	m_wndTendonName.GetWindowText(m_TdnaData.TendonName);
	m_wndTdntCobx.GetSelectedTdnt(m_TdnaData.TendonTypeKey);
	
	/* 640 추가된 값*/
	m_wndTdgrCobx.GetSelectedTdgr(m_TdnaData.GroupKey); 
	//m_TdnaData.bNoTensionAutoCalc = m_bUserDef; delete      
	//if (m_nAutoOrUser == 0)
	//{
		CString StrText1;
		m_wndEndUser.GetWindowText(StrText1); 
		m_TdnaData.dNoTensionEndLen = _tstof(StrText1); //skn
	    m_wndBeginUser.GetWindowText(StrText1); 
		m_TdnaData.dNoTensionBeginLen = _tstof(StrText1); 
	//}
    if (m_bDebonded) 
	{
		m_wndDebondBegin.GetWindowText(StrText1); 
		m_TdnaData.dDebondBeginLen = _tstof(StrText1);
		m_wndDebondEnd.GetWindowText(StrText1); 
		m_TdnaData.dDebondEndLen = _tstof(StrText1);
	}
	m_nAutoOrUser = m_cobxPostTension.GetItemData(m_cobxPostTension.GetCurSel());//;

	m_TdnaData.nNoTensionCalc = m_nAutoOrUser;
	/*****************/

	// get No. of Tendon 
	 m_wndTendonCountEdit.GetEditValue(m_TdnaData.dTendonNum);
	//m_TdnaData.bTypicalTendon = m_chkTypicalTendon.GetCheck();

	DWORD nItemData;
	CString StrText;
	m_wndAssignElem.GetWindowText(StrText);
	CArray<int,int> aElem;
	if(!CStrParser::ParsingListByTo(StrText,aElem)) return FALSE;
	int nElem = aElem.GetSize();
	m_TdnaData.aElemList.RemoveAll();
	for(int i = 0; i< nElem; i++)
		m_TdnaData.aElemList.Add(aElem[i]);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlTypeRadio, m_TdnaData.nInputType);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCurveType, m_TdnaData.nCurveType);
	
	m_wndBeginLength.GetWindowText(StrText); m_TdnaData.dBeginLength = _tstof(StrText);
	m_wndEndLength.GetWindowText(StrText); m_TdnaData.dEndLength = _tstof(StrText);
// if(!m_wndBeginLength.GetCoordData(&(m_TdnaData.dBeginLength),NULL,NULL)) return FALSE;
//  if(!m_wndEndLength.GetCoordData  (&(m_TdnaData.dEndLength  ),NULL,NULL)) return FALSE;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlTendonShape, m_TdnaData.nTendonShape);
	if (m_TdnaData.nTendonShape == 0)
	{
		if(!m_wndInsertPoint.GetCoordData(
			&m_TdnaData.dInsertPointX, &m_TdnaData.dInsertPointY, &m_TdnaData.dInsertPointZ))
			return FALSE;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlXAxisDir, m_TdnaData.nXAxisDirection);
		if (m_TdnaData.nXAxisDirection == 2)  // vector
			if (!m_wndVectorEdit.GetCoordData(&m_TdnaData.dVectorX, &m_TdnaData.dVectorY, NULL)) return FALSE;

		m_TdnaData.dCenterX = m_TdnaData.dCenterY = 0.0;
		m_TdnaData.dOffset = 0.0;
		m_TdnaData.nCurveDir = 0;
		if (!m_wndAxisAngleEdit.GetEditValue( m_TdnaData.dXAxisRotAngle)) return FALSE;
		m_TdnaData.bProjection = m_chkProjection.GetCheck()?TRUE:FALSE;
		if (!CDlgUtil::CobxGetItemDataByCurSel(m_cobxGradAxis, nItemData)) return FALSE;
		m_TdnaData.nGradRotAxis = (int)nItemData;
		if (!m_wndGradAngle.GetEditValue(m_TdnaData.dGradRotAngle)) return FALSE;
	}
	else if (m_TdnaData.nTendonShape == 1)
	{
		if(!m_wndInsertPoint.GetCoordData(
			&m_TdnaData.dInsertPointX, &m_TdnaData.dInsertPointY, &m_TdnaData.dInsertPointZ))
			return FALSE;
		if (!m_wndCenterEdit.GetCoordData(&m_TdnaData.dCenterX, &m_TdnaData.dCenterY, NULL)) return FALSE;
		if (!m_wndOffsetEdit.GetCoordData(&m_TdnaData.dOffset, NULL, NULL)) return FALSE;
		CDlgUtil::CobxGetItemDataByCurSel(m_cobxCurveDir, nItemData);
		m_TdnaData.nCurveDir = (int)nItemData;

		m_TdnaData.nXAxisDirection = 0;
		m_TdnaData.dVectorX = m_TdnaData.dVectorY = 0.0;
		if (!m_wndAxisAngleEdit.GetEditValue(m_TdnaData.dXAxisRotAngle)) return FALSE;
		m_TdnaData.bProjection = m_chkProjection.GetCheck()?TRUE:FALSE;
		if (!CDlgUtil::CobxGetItemDataByCurSel(m_cobxGradAxis, nItemData)) return FALSE;
		m_TdnaData.nGradRotAxis = (int)nItemData;
		if (!m_wndGradAngle.GetEditValue(m_TdnaData.dGradRotAngle)) return FALSE;
	}
	else if (m_TdnaData.nTendonShape == 2)
	{
		CString value;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlXAxisInsertPos, m_TdnaData.nInsertPoint);
		m_editInsertPos.GetWindowText(StrText);
		m_TdnaData.InsertElemKey = _ttoi(StrText);
		// 여기서 입력된 Elem Key는 Assigned Elements 중 하나여야 한다.
		/*
		int nNum = m_TdnaData.aElemList.GetSize();
		for (int i=0; i<nNum; i++)
			if (m_TdnaData.aElemList[i] == m_TdnaData.InsertElemKey) break;
		if (i == nNum)
		{
			AfxMessageBox(_LS(IDS_CMD_ERR_Profile_Insert_Element));
			return FALSE;
		}
		*/
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlXAxisDirection, m_TdnaData.nXAxisDirectionIJ);
		if (!m_editAngle.GetEditValue(m_TdnaData.dXAxisRotAngle)) return FALSE;
		m_TdnaData.bProjection = m_chkProjection2.GetCheck()? TRUE : FALSE;
		if (!m_editOffsetY.GetCoordData(&m_TdnaData.dOffsetY, NULL, NULL)) return FALSE;
		if (!m_editOffsetZ.GetCoordData(&m_TdnaData.dOffsetZ, NULL, NULL)) return FALSE;
	}
	else ASSERT(0);
/* 임시로 막음(KJH20051011)
	if(GetDlgItem(IDC_CMD_BOTTOM_CHK)->IsWindowEnabled()) //dlg에서 삭제 2005.10.25 jkpark
		m_TdnaData.bBottomZ = m_bBottomZ;
	else
		m_TdnaData.bBottomZ = FALSE;
*/
	return TRUE;
}

void CCMTendonProfileDlg::Data2Dlg()
{
	CString csText;
	m_wndTendonName.SetWindowText(m_TdnaData.TendonName);
	m_wndTdntCobx.ChangeSelect(m_TdnaData.TendonTypeKey);
	m_wndTdgrCobx.ChangeSelect(m_TdnaData.GroupKey);

	ComboBoxDynamicChange(&m_wndTdgrCobx);
 
	/* 640 추가 사항 skn */
	m_nAutoOrUser = m_TdnaData.nNoTensionCalc;

	// 0, 2 or 0,1
	if(m_nAutoOrUser == 2 || m_nAutoOrUser == 1)
		m_cobxPostTension.SetCurSel(1);
	else
		m_cobxPostTension.SetCurSel(0);
	//if (m_nAutoOrUser)
 // {
		csText.Format(_T("%.8g"),m_TdnaData.dNoTensionBeginLen);
		m_wndBeginUser.SetWindowText(csText);
		csText.Format(_T("%.8g"),m_TdnaData.dNoTensionEndLen);
		m_wndEndUser.SetWindowText(csText);
	//}
	//m_nTendonType 

	csText.Format(_T("%.8g"),m_TdnaData.dDebondBeginLen);
	m_wndDebondBegin.SetWindowText(csText);
	csText.Format(_T("%.8g"),m_TdnaData.dDebondEndLen);
	m_wndDebondEnd.SetWindowText(csText);

	CArray<T_NODE_K,T_NODE_K> rNodes;
	m_pDoc->m_pViewCtrl->Select(NULL,rNodes,m_TdnaData.aElemList,TRUE);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlTypeRadio, m_TdnaData.nInputType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCurveType, m_TdnaData.nCurveType);

/* 2D Element 형태를 구분하려고 m_TdnaData.nTendonShape 추가로 넘기기 위해 수정 // 2005.12. 23 jkpark
	m_wndGrid3D.SetCurveType(m_TdnaData.nCurveType);
	m_wndGrid2DXY.SetCurveType(m_TdnaData.nCurveType);
	m_wndGrid2DXZ.SetCurveType(m_TdnaData.nCurveType);
*/
	m_wndGrid3D.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
	m_wndGrid2DXY.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
	m_wndGrid2DXZ.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
//수정 끝 2005.12. 23 jkpark

	csText.Format(_T("%.8g"),m_TdnaData.dBeginLength);
	m_wndBeginLength.SetWindowText(csText);
	csText.Format(_T("%.8g"),m_TdnaData.dEndLength);
	m_wndEndLength.SetWindowText(csText);

	// 고정된 Defaut 값 1 사용
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlSymPtRadio, 1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlTendonShape, m_TdnaData.nTendonShape);
	csText.Format(_T("%.8g, %.8g, %.8g"), 
		m_TdnaData.dInsertPointX, m_TdnaData.dInsertPointY, m_TdnaData.dInsertPointZ);
	m_wndInsertPoint.SetWindowText(csText);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlXAxisDir, m_TdnaData.nXAxisDirection);
	csText.Format(_T("%.8g, %.8g"), m_TdnaData.dVectorX, m_TdnaData.dVectorY);
	m_wndVectorEdit.SetWindowText(csText);
	csText.Format(_T("%.8g, %.8g"), m_TdnaData.dCenterX, m_TdnaData.dCenterY);
	m_wndCenterEdit.SetWindowText(csText);
	csText.Format(_T("%.8g"), m_TdnaData.dOffset);
	m_wndOffsetEdit.SetWindowText(csText);
	
	csText.Format(_T("%.8g"), m_TdnaData.dXAxisRotAngle);
	m_editAngle.SetWindowText(csText);
	m_wndAxisAngleEdit.SetWindowText(csText);
	CDlgUtil::CobxSetCurSelItemData(m_cobxCurveDir, (DWORD)m_TdnaData.nCurveDir);
	m_chkProjection.SetCheck(m_TdnaData.bProjection);
	m_chkProjection2.SetCheck(m_TdnaData.bProjection);
	CDlgUtil::CobxSetCurSelItemData(m_cobxGradAxis, m_TdnaData.nGradRotAxis);
	csText.Format(_T("%.8g"), m_TdnaData.dGradRotAngle);
	m_wndGradAngle.SetWindowText(csText);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlXAxisInsertPos, m_TdnaData.nInsertPoint);
	csText.Format(_T("%d"), m_TdnaData.InsertElemKey);
	m_editInsertPos.SetWindowText(csText);
	m_editXDir.SetWindowText(csText);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlXAxisDirection, m_TdnaData.nXAxisDirectionIJ);
	csText.Format(_T("%g"), m_TdnaData.dOffsetY);
	m_editOffsetY.SetWindowText(csText);
	csText.Format(_T("%g"), m_TdnaData.dOffsetZ);
	m_editOffsetZ.SetWindowText(csText);

	// 700 추가
	// 대표텐던의 반복 갯수 입력. 
	CString strTendonNum;
	m_chkTypicalTendon.SetCheck(m_TdnaData.bTypicalTendon);
	if (m_TdnaData.bTypicalTendon) 
		m_wndTendonCountEdit.SetValue(m_TdnaData.dTendonNum);  // init No. of Tendon. 
	else 
		m_wndTendonCountEdit.SetValue(1.0);
	OnCmdTypicalTendonCheck();

	// 임시로 막음(KJH20051011)
	//m_bBottomZ = m_TdnaData.bBottomZ; //dlg에서 삭제 2005.10.25 jkpark

	// Reset ignore notify flag
	m_bIgnoreNotify = FALSE;

	UpdateData(FALSE);

	ChangeInputType();  // Update Graph 포함
	EnableDisableCtrl();

	// Update Graph
	//OnGridDataChanged();

	// Update Grid
	m_wndGrid3D.MakeItemEx();
	m_wndGrid2DXY.MakeItemEx();
	m_wndGrid2DXZ.MakeItemEx();
}

void CCMTendonProfileDlg::InitGrid()
{
	m_wndGrid3D.Initialize(1, &m_TdnaData);
	m_wndGrid2DXY.Initialize(2, &m_TdnaData);
	m_wndGrid2DXZ.Initialize(3, &m_TdnaData);
}

void CCMTendonProfileDlg::InitChart()
{
	InitGraph(IDC_CMD_TENDON_GRAPH_XY, &m_GraphView3DXY, &m_Graph3DXY);
	AddGraphLabel(&m_Graph3DXY, 0, SRGraphLabel::BottomRight, 98.0, 100.0, _T("x"));
	AddGraphLabel(&m_Graph3DXY, 1, SRGraphLabel::TopLeft, 2.0, 0.0, _T("y"));
	m_GraphView3DXY.Invalidate();

	InitGraph(IDC_CMD_TENDON_GRAPH_XZ, &m_GraphView3DXZ, &m_Graph3DXZ);
	AddGraphLabel(&m_Graph3DXZ, 0, SRGraphLabel::BottomRight, 98.0, 100.0, _T("x"));
	AddGraphLabel(&m_Graph3DXZ, 1, SRGraphLabel::TopLeft, 2.0, 0.0, _T("z"));
	m_GraphView3DXZ.Invalidate();
}

void CCMTendonProfileDlg::InitGraph(UINT nPlaceHolderID, SREGraphView* pGraphView, CMySRGraph* pGraph)
{
	////////////////////////////////////////////////////////////////
	// Initialize pGraph  
	pGraphView->SetDragable(FALSE); // set data-drag unable
	
	// Add Background component
	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	pGraph->AddComponent((SRGraphComponent *) pB);
	
	// Add Display Component
	SRGraphDisplay *pDisplay=new SRGraphDisplay;	
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetRect(0.2,0,97.0,97.0);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	//pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(FALSE);
	  
	// Logarithmic Axis Scaling
	pDisplay->GetStyle()->SetLogX(FALSE);
	pDisplay->GetStyle()->SetLog(FALSE);

	pGraph->AddComponent(pDisplay);

	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview
	CWnd* wnd = GetDlgItem(nPlaceHolderID);
	CRect rc;

	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left+=2;
	rc.top+=2;
	rc.right -= 2;
	rc.bottom -= 2;

	pGraphView->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_CMD_PLACEHOLDER);
	pGraphView->m_pGraph = pGraph; 
}

void CCMTendonProfileDlg::AddGraphLabel(CMySRGraph *pGraph, int ix, int lp, double x, double y, LPCTSTR lpszLabel)
{
	SRGraphLabel	*pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(lp);
	pLabel->SetTextPosition(x, y);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(pGraph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pGraph->AddComponent(pLabel);
	SetAxisLabelText(pGraph, ix, lpszLabel);
}

void CCMTendonProfileDlg::SetValue(CMySRGraph* pGraph, int nIndex, int nGroup, double x,double y)
{
	pGraph->SetValue(nIndex, nGroup*2, x);
	pGraph->GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	pGraph->GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(1);
	pGraph->SetValue(nIndex, nGroup*2+1, y);
}

void CCMTendonProfileDlg::SetAxisLabelText(CMySRGraph* pGraph, int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) pGraph->GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CCMTendonProfileDlg::EnableDisableCtrl()
{
//  CDlgUtil::CtrlEnableDisable(this, m_aCtrlRadius, m_TdnaData.nCurveType == 1);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBeginEndLength, m_TdnaData.nCurveType != 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlStraight, m_TdnaData.nTendonShape == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCurve, m_TdnaData.nTendonShape == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCommon, m_TdnaData.nTendonShape != 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlElement, m_TdnaData.nTendonShape == 2);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlVector, m_TdnaData.nTendonShape == 0 && m_TdnaData.nXAxisDirection == 2);

//  if(m_TdnaData.nTendonShape!=2 && m_bBottomZ) //dlg에서 삭제 2005.10.25 jkpark
	if(m_TdnaData.nTendonShape!=2)
	{
//    m_bBottomZ = FALSE;//    m_bBottomZ = FALSE;

		UpdateData(FALSE);
	}

//  GetDlgItem(IDC_CMD_BOTTOM_CHK)->EnableWindow(m_TdnaData.nTendonShape==2); //dlg에서 삭제 2005.10.25 jkpark
}


BEGIN_MESSAGE_MAP(CCMTendonProfileDlg, CDialog)
	//{{AFX_MSG_MAP(CCMTendonProfileDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CMD_TENDON_PROP_BUTTON, OnCmdTendonPropButton)
	ON_BN_CLICKED(IDC_CMD_TENDON_GROUP_BUTTON, OnCmdTendonGroupButton) //
	ON_BN_CLICKED(IDC_CMD_TENDON_SHAPE_RADIO_STRAIGHT, OnChangeTendonShape)
	ON_BN_CLICKED(IDC_CMD_TENDON_DIR_RADIO_X, OnChangeDir)
	ON_CBN_SELCHANGE(IDC_CMD_TENDON_PROP_COMBO, OnChangeProp)
	ON_BN_CLICKED(IDC_CMD_TENDON_SYMMETRIC_BUTTON, OnCmdTendonSymmetricButton)
	ON_EN_CHANGE(IDC_CMD_INSERTPOS_EDIT, OnChangeCmdInsertposEdit)
	ON_BN_CLICKED(IDC_CMD_RADIO_2D, OnCmdInputTypeRadio)
	ON_BN_CLICKED(IDC_CMD_TENDON_SHAPE_RADIO_CURVE, OnChangeTendonShape)
	ON_BN_CLICKED(IDC_CMD_TENDON_SHAPE_RADIO_ELEM, OnChangeTendonShape)
	ON_BN_CLICKED(IDC_CMD_TENDON_DIR_RADIO_Y, OnChangeDir)
	ON_BN_CLICKED(IDC_CMD_TENDON_DIR_RADIO_VECTOR, OnChangeDir)
	ON_BN_CLICKED(IDC_CMD_RADIO_SPLINE, OnChangeCurveType)
	ON_BN_CLICKED(IDC_CMD_RADIO_ROUND, OnChangeCurveType)
	ON_BN_CLICKED(IDC_CMD_RADIO_PARABOLA, OnChangeCurveType)
	ON_BN_CLICKED(IDC_CMD_RADIO_3D, OnCmdInputTypeRadio)
	ON_CBN_SELCHANGE(IDC_CMD_TENDON_PROP_POST_TENSION, OnSelchangeCmdTendonPropPostTension)

// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TENDON_COUNT_SPIN, OnDeltaposCmdTendonCountSpin)     // No. of Tendon spin button 
	ON_BN_CLICKED(IDC_CMD_TENDON_TYPICALTENDON_CHK, OnCmdTypicalTendonCheck)

	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDIT_TEXT_CHANGED, OnMouseEditTextChanged)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()
//	ON_EN_UPDATE(IDC_CMD_BEGIN_LENGTH_EDIT, OnChangeBeginEndLength)
//	ON_EN_UPDATE(IDC_CMD_END_LENGTH_EDIT, OnChangeBeginEndLength)
//	ON_EN_KILLFOCUS(IDC_CMD_BEGIN_LENGTH_EDIT, OnKillfocusCmdBeginLengthEdit)
//	ON_EN_KILLFOCUS(IDC_CMD_END_LENGTH_EDIT, OnKillfocusCmdEndLengthEdit)

/////////////////////////////////////////////////////////////////////////////
// CCMTendonProfileDlg message handlers

BOOL CCMTendonProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);

	// TODO: Add extra initialization here
	AlignControls();

	m_wndAssignElem.Connect(SC_ID_ELEM,&m_wndAssignElem);

//  m_wndTPRadiusEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
//  m_wndTPRadiusUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
 
	m_wndBeginLength.SetAttWcsDistance();
	m_wndBeginLength.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndBeginLength.SetDistValueMode(TRUE);
	m_wndBeginLength.SetLButtonDownNotifyWindow(this);
	m_wndBeginLength.SetEnterNotifyWindow(this);
	m_txtBeginLengthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_wndEndLength.SetAttWcsDistance();
	m_wndEndLength.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndEndLength.SetDistValueMode(TRUE);
	m_wndEndLength.SetLButtonDownNotifyWindow(this);
	m_wndEndLength.SetEnterNotifyWindow(this);
	m_txtEndLengthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndInsertPoint.SetAttWcsPos();
	m_wndInsertPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_txtInsertPointUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndVectorEdit.SetAttWcsDistance();
	m_wndVectorEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndVectorEdit.SetDistComponent(4);  // X,Y
	m_txtVectorUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndCenterEdit.SetAttWcsPos();
	m_wndCenterEdit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	m_txtCenterUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndOffsetEdit.SetAttWcsDistance();
	m_wndOffsetEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndOffsetEdit.SetDistValueMode(TRUE);
	m_txtOffsetUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_editOffsetY.SetAttWcsDistance();
	m_editOffsetY.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_editOffsetY.SetDistValueMode(TRUE);
	m_unitOffsetY.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_editOffsetZ.SetAttWcsDistance();
	m_editOffsetZ.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_editOffsetZ.SetDistValueMode(TRUE);
	m_unitOffsetZ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_editInsertPos.SetAttElemList();
	m_editInsertPos.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_editInsertPos.SetMaxElemKeyNum(1);

	// 640 추가 작업//////////////////////////////////////////////
 
	//m_unitBeginUser.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_unitEndUser.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_uniDebondEnd.SetUnitType(D_UNITSYS_BASE_LENGTH);

	//m_editXDir.SetAttElemList();
	//m_editXDir.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	//m_editXDir.SetMaxElemKeyNum(1);
	m_editXDir.SetReadOnly(TRUE);

	CDlgUtil::CobxAddItem(m_cobxCurveDir, _LS(IDS_CMD_TDNA_ITEM_CW), 0);
	CDlgUtil::CobxAddItem(m_cobxCurveDir, _LS(IDS_CMD_TDNA_ITEM_CCW), 1);
	
	// 640 추가 작업 /////////////////////////////
	//CDlgUtil::CobxAddItem(m_cobxPostTension, _T("Auto Calc( 65 * Strand Diameter )"),2); //_LS(IDS_CMD_Auto_Calc_Strand)
	//CDlgUtil::CobxAddItem(m_cobxPostTension, _LS(IDS_CMD_Auto_Calc),1); //(Auto Calc( 0.5 * (H + Bf/n) )
	//CDlgUtil::CobxAddItem(m_cobxPostTension, _LS(IDS_CMD_User_defined_Length),0); //User defined Length
	///////////////////////////////////////////

	m_wndAxisAngleEdit.SetRange(-85, 85);
	m_txtAxisAngleUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);

	m_editAngle.SetRange(-85, 85);
	m_unitAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);

	CDlgUtil::CobxAddItem(m_cobxGradAxis, _T("X"), 0);
	CDlgUtil::CobxAddItem(m_cobxGradAxis, _T("Y"), 1);

	m_wndGradAngle.SetRange(-85, 85);
	m_txtGradUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);

	m_wndTendonCountEdit.SetRange(1, 1000);
// 	CDlgUtil::SetSpinValue(this, IDC_CMD_TENDON_COUNT_EDIT, 1, 1000, pNMHDR, pResult);


	InitGrid();
	InitChart();
// 	m_wndTendonCountEdit.SetUnitType(D_UNITSYS_NONE);

	//OnCmdUserDefineCheck(); // skn

	OnChangeProp();
	Data2Dlg();

	OnSelchangeCmdTendonPropPostTension();

	// init Typical tendon check 
	m_chkTypicalTendon.SetCheck(m_TdnaData.bTypicalTendon);

	OnCmdTypicalTendonCheck();

	m_chkDetail.InitControl(this, CCMTendonProfileDlg::IDD, IDC_CMD_TENDON_DETAIL_GRP,TRUE,TRUE);

	m_chkDetail.SetFoldState(FALSE);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTendonProfileDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (m_bCloseDlg) return;
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
		// 방식 변경:후처리 모드에서 Modify하는 경우 전처리로 변경되면서 에러 야기
		//DestroyWindow(); 
		if (m_pModalDlg != 0) m_bCloseDlg = TRUE;
		else PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
		break;
	case D_UPDATE_UNIT:
		OnUnitChange();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}
}

void CCMTendonProfileDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bTdntChanged = FALSE;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_TDNT_ADD): 
		case(UR_TDNT_DEL): 
		case(UR_TDNT_MFS): 
		case(UR_TDNT_MFD):
				bTdntChanged = TRUE;
				break;
		}
	}
	if (bTdntChanged) OnGridDataChanged();
}

void CCMTendonProfileDlg::OnUnitChange()
{
	m_pDoc->m_pUnitCtrl->ConvertUnitTdnaPrevious(m_TdnaData);
	m_wndGrid3D.OnUnitChange();
	m_wndGrid2DXY.OnUnitChange();
	m_wndGrid2DXZ.OnUnitChange();
	OnGridDataChanged();
}

void CCMTendonProfileDlg::OnGridDataChanged(BOOL bSave/*=TRUE*/)
{
	int ix;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlTypeRadio, ix);

	if (ix == 0) // 2D
	{
		OnGridDataChanged2DXY(bSave);
		OnGridDataChanged2DXZ(bSave);
	}
	else if (ix == 1) OnGridDataChanged3D(bSave);  // 3D
	else return;

	// 디버그 용
	//CString msg;
	//msg.Format(_T("Begin : %g , End : %g"), m_TdnaData.dBeginLength, m_TdnaData.dEndLength);
	//AfxMessageBox(msg);

}

void CCMTendonProfileDlg::OnGridDataChanged2DXY(BOOL bSave)
{
	if (m_bIgnoreNotify) return;
	if (m_GraphView3DXY.GetSafeHwnd() == 0) return;
	m_Graph3DXY.KillData();	

	int nSize = m_TdnaData.aProfileY.GetSize();

	if (nSize < 2 || (nSize != 3 && m_TdnaData.nCurveType == 2))
	{
		SetValue(&m_Graph3DXY, 0, 0, 0.0, 0.0);
	}
	else
	{
		double dMinRangeX1, dMinRangeY, dMaxRangeX1, dMaxRangeY;

		if (bSave) Dlg2Data();

		if (!CFormulaEdit::GetEditValue(&m_wndBeginLength, m_TdnaData.dBeginLength))
			m_TdnaData.dBeginLength = 0.0;
		if (!CFormulaEdit::GetEditValue(&m_wndEndLength, m_TdnaData.dEndLength))
			m_TdnaData.dEndLength = 0.0;

		CArray<T_NODE_D, T_NODE_D&> aTendonProfile, aTendonProfile_xy, aTendonProfile_xz;
		CArray<BOOL, BOOL> aProfileInputPoint, aProfileInputPoint_xy, aProfileInputPoint_xz;
		m_pDoc->m_pAttrCtrl->calcTendonProfile(m_TdnaData, aTendonProfile, aProfileInputPoint, 20, TRUE, FALSE, FALSE,
												 &aTendonProfile_xy, &aProfileInputPoint_xy, &aTendonProfile_xz, &aProfileInputPoint_xz);
		
		CalcMinMaxRange_xy(aTendonProfile_xy, dMinRangeX1, dMinRangeY, dMaxRangeX1, dMaxRangeY);

		SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph3DXY.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	  pD->SetMinRangeX(dMinRangeX1);   
	  pD->SetMinRangeY(dMinRangeY);
	  pD->SetMaxRangeX(dMaxRangeX1);
	  pD->SetMaxRangeY(dMaxRangeY);   
	  pD->GetStyle()->SetUseMaxScale(TRUE);    
		pD->GetStyle()->SetScaleLimit(TRUE);
	}

	m_GraphView3DXY.Invalidate();
}

void CCMTendonProfileDlg::OnGridDataChanged2DXZ(BOOL bSave)
{
	if (m_bIgnoreNotify) return;
	if (m_GraphView3DXZ.GetSafeHwnd() == 0) return;

	m_Graph3DXZ.KillData();	

	int nSize = m_TdnaData.aProfileZ.GetSize();
	if (nSize < 2)
	{
		SetValue(&m_Graph3DXZ, 0, 0, 0.0, 0.0);
	}
	else
	{
		double dMinRangeX2, dMinRangeZ, dMaxRangeX2, dMaxRangeZ;

		if (bSave) Dlg2Data();

		if (!CFormulaEdit::GetEditValue(&m_wndBeginLength, m_TdnaData.dBeginLength))
			m_TdnaData.dBeginLength = 0.0;
		if (!CFormulaEdit::GetEditValue(&m_wndEndLength, m_TdnaData.dEndLength))
			m_TdnaData.dEndLength = 0.0;

		CArray<T_NODE_D, T_NODE_D&> aTendonProfile, aTendonProfile_xy, aTendonProfile_xz;
		CArray<BOOL, BOOL> aProfileInputPoint, aProfileInputPoint_xy, aProfileInputPoint_xz;
		m_pDoc->m_pAttrCtrl->calcTendonProfile(m_TdnaData, aTendonProfile, aProfileInputPoint, 20, TRUE, FALSE, FALSE,
												 &aTendonProfile_xy, &aProfileInputPoint_xy, &aTendonProfile_xz, &aProfileInputPoint_xz);
		
		CalcMinMaxRange_xz(aTendonProfile_xz, dMinRangeX2, dMinRangeZ, dMaxRangeX2, dMaxRangeZ);

	  SRGraphDisplay* pD = (SRGraphDisplay *) m_Graph3DXZ.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	  pD->SetMinRangeX(dMinRangeX2);   
	  pD->SetMinRangeY(dMinRangeZ);
	  pD->SetMaxRangeX(dMaxRangeX2);
	  pD->SetMaxRangeY(dMaxRangeZ);   
	  pD->GetStyle()->SetUseMaxScale(TRUE);
		pD->GetStyle()->SetScaleLimit(TRUE);
	}

	m_GraphView3DXZ.Invalidate();
}

void CCMTendonProfileDlg::OnGridDataChanged3D(BOOL bSave)
{
	if (m_bIgnoreNotify) return;
	if (m_GraphView3DXY.GetSafeHwnd() == 0) return;
	if (m_GraphView3DXZ.GetSafeHwnd() == 0) return;

	m_Graph3DXY.KillData();	
	m_Graph3DXZ.KillData();

	int nSize = m_TdnaData.aProfile.GetSize();
	if (nSize < 2)
	{
		SetValue(&m_Graph3DXY, 0, 0, 0.0, 0.0);
		SetValue(&m_Graph3DXZ, 0, 0, 0.0, 0.0);
	}
	else
	{
		double dMinX, dMaxX, dMinY, dMaxY; 
		double dMinRangeX1, dMinRangeY, dMaxRangeX1, dMaxRangeY;
		double dMinRangeX2, dMinRangeZ, dMaxRangeX2, dMaxRangeZ;

		if (bSave) Dlg2Data();

		if (!CFormulaEdit::GetEditValue(&m_wndBeginLength, m_TdnaData.dBeginLength))
			m_TdnaData.dBeginLength = 0.0;
		if (!CFormulaEdit::GetEditValue(&m_wndEndLength, m_TdnaData.dEndLength))
			m_TdnaData.dEndLength = 0.0;

		CArray<T_NODE_D, T_NODE_D&> aTendonProfile, aTendonProfile_xy, aTendonProfile_xz;
		CArray<BOOL, BOOL> aProfileInputPoint, aProfileInputPoint_xy, aProfileInputPoint_xz;
		m_pDoc->m_pAttrCtrl->calcTendonProfile(m_TdnaData, aTendonProfile, aProfileInputPoint, 20, TRUE, FALSE, FALSE,
												 &aTendonProfile_xy, &aProfileInputPoint_xy, &aTendonProfile_xz, &aProfileInputPoint_xz);
		
		CalcMinMaxRange_xy(aTendonProfile_xy, dMinRangeX1, dMinRangeY, dMaxRangeX1, dMaxRangeY);
		CalcMinMaxRange_xz(aTendonProfile_xz, dMinRangeX2, dMinRangeZ, dMaxRangeX2, dMaxRangeZ);

		dMinX = __min(dMinRangeX1, dMinRangeX2);
		dMaxX = __max(dMaxRangeX1, dMaxRangeX2);
		dMinY = __min(dMinRangeY, dMinRangeZ);
		dMaxY = __max(dMaxRangeY, dMaxRangeZ); 

		SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph3DXY.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	  pD->SetMinRangeX(dMinX);   
	  pD->SetMinRangeY(dMinY);
	  pD->SetMaxRangeX(dMaxX);
	  pD->SetMaxRangeY(dMaxY);   
	  pD->GetStyle()->SetUseMaxScale(TRUE);    
		pD->GetStyle()->SetScaleLimit(TRUE);

	  pD = (SRGraphDisplay *) m_Graph3DXZ.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	  pD->SetMinRangeX(dMinX);   
	  pD->SetMinRangeY(dMinY);
	  pD->SetMaxRangeX(dMaxX);
	  pD->SetMaxRangeY(dMaxY);   
	  pD->GetStyle()->SetUseMaxScale(TRUE);
		pD->GetStyle()->SetScaleLimit(TRUE);
		/*
#ifdef _DEBUG
		{
				// FEANX 텐던 좌표 검증용 코드임.. ( 파일로 출력해서 값 비교용 )
				// Local 에서만 잠시 주석을 풀어놓고 svn에 올려진 코드는 항상 주석 처리 되어 있어야 함
				std::vector<T_NODE_D> aPos3DForWrite;
				Conv2DPlanesTo3DPlane(aTendonProfile_xy, aTendonProfile_xz, aPos3DForWrite);

				CFile file;
				if(!file.Open(_T("c:\\FEANXTndnTest_CVL.txt"), CFile::modeCreate|CFile::modeWrite, NULL))
				{
						return ;
				}

				CString strFullValue = _T("");
				CString strLine = _T("");

				for(long i=0;i<aPos3DForWrite.size();i++)
				{
						strLine.Format(_T("%g\t%g\t%g"), aPos3DForWrite[i].x, aPos3DForWrite[i].y, aPos3DForWrite[i].z);

						strLine += _T("\r\n");
						strFullValue += strLine;
				}

				file.Write(strFullValue, strFullValue.GetLength());
				file.Close();
		}
#endif // _DEBUG
		*/
	}
	
	m_GraphView3DXY.Invalidate();
	m_GraphView3DXZ.Invalidate();
}

BOOL CCMTendonProfileDlg::Conv2DPlanesTo3DPlane(CArray<T_NODE_D, T_NODE_D&>& aPos2DXY, CArray<T_NODE_D, T_NODE_D&>& aPos2DXZ, std::vector<T_NODE_D>& aPos3D)
{
		// FEANX 텐던 좌표 검증용 코드임.. ( 파일로 출력해서 값 비교용 )
		aPos3D.clear();

		int nCntXY = 0;
		int nCntXZ = 0;

		if(aPos2DXZ.GetSize() == 0 || aPos2DXZ.GetSize() == 0)
		{
				ASSERT(0);
				return FALSE;
		}

		T_NODE_D EachPos;
		while (true)
		{
				if(nCntXY >= aPos2DXY.GetSize() && nCntXZ >= aPos2DXZ.GetSize())
						break;

				if(nCntXY >= aPos2DXY.GetSize())
				{
						EachPos.x = aPos2DXZ[nCntXZ].x;
						EachPos.y = aPos2DXY[aPos2DXY.GetSize() - 1].y;
						EachPos.z = aPos2DXZ[nCntXZ].z;
						aPos3D.push_back(EachPos);

						nCntXZ++;
				}
				else if(nCntXZ >= aPos2DXZ.GetSize())
				{
						EachPos.x = aPos2DXY[nCntXY].x;
						EachPos.y = aPos2DXY[nCntXY].y;
						EachPos.z = aPos2DXZ[aPos2DXZ.GetSize() - 1].z;
						aPos3D.push_back(EachPos);

						nCntXY++;
				}
				else
				{
						if(aPos2DXY[nCntXY].x == aPos2DXZ[nCntXZ].x)
						{
								EachPos.x = aPos2DXY[nCntXY].x;
								EachPos.y = aPos2DXY[nCntXY].y;
								EachPos.z = aPos2DXZ[nCntXZ].z;
								aPos3D.push_back(EachPos);

								nCntXY++;
								nCntXZ++;
						}
						else if(aPos2DXY[nCntXY].x < aPos2DXZ[nCntXZ].x)
						{
								EachPos.x = aPos2DXY[nCntXY].x;
								EachPos.y = aPos2DXY[nCntXY].y;
								EachPos.z = aPos2DXZ[(nCntXZ == 0) ? 0 : (nCntXZ - 1)].z;
								aPos3D.push_back(EachPos);

								nCntXY++;
						}
						else if(aPos2DXZ[nCntXZ].x < aPos2DXY[nCntXY].x)
						{
								EachPos.x = aPos2DXZ[nCntXZ].x;
								EachPos.y = aPos2DXY[(nCntXY == 0) ? 0 : (nCntXY - 1)].y;
								EachPos.z = aPos2DXZ[nCntXZ].z;
								aPos3D.push_back(EachPos);

								nCntXZ++;
						}
						else
						{
								ASSERT(0);
						}
				}
		}

		return TRUE;
}

void CCMTendonProfileDlg::CalcMinMaxRange_xy(CArray<T_NODE_D, T_NODE_D&> &aTendonProfile, 
																						 double &dMinRangeX, double &dMinRangeY, double &dMaxRangeX, double &dMaxRangeY)
{
	double dLenX=0.0, dLenY=0.0;
	double dRatio = 6.5;  // x축길이:y축길이 비 (6.5:1)
	double xmin=0.0, ymin=0.0, xmax=0.0, ymax=0.0; 
	
	int nSize = aTendonProfile.GetSize();
	if (nSize > 0)
	{
		xmin = xmax = aTendonProfile[0].x;
		ymin = ymax = aTendonProfile[0].y;
	}
	for (int i = 0; i < nSize; i++)
	{
		SetValue(&m_Graph3DXY, i, 0, aTendonProfile[i].x, aTendonProfile[i].y);
		if (aTendonProfile[i].x < xmin) xmin = aTendonProfile[i].x;
		else if (aTendonProfile[i].x > xmax) xmax = aTendonProfile[i].x;
		if (aTendonProfile[i].y < ymin) ymin = aTendonProfile[i].y;
		else if (aTendonProfile[i].y > ymax) ymax = aTendonProfile[i].y;
	}
	if (xmax-xmin > dRatio*(ymax-ymin)) { dLenX = xmax-xmin; dLenY = (xmax-xmin)/dRatio; }
	else { dLenX = (ymax-ymin)*dRatio; dLenY = ymax-ymin; }
	
	dMinRangeX = (xmin+xmax)/2.0 - (dLenX)/2.0;
	dMaxRangeX = (xmin+xmax)/2.0 + (dLenX)/2.0;
	dMinRangeY = (ymin+ymax)/2.0 - (dLenY)/2.0;
	dMaxRangeY = (ymin+ymax)/2.0 + (dLenY)/2.0;
}

void CCMTendonProfileDlg::CalcMinMaxRange_xz(CArray<T_NODE_D, T_NODE_D&> &aTendonProfile, 
																						 double &dMinRangeX, double &dMinRangeZ, double &dMaxRangeX, double &dMaxRangeZ)
{
	double dLenX=0.0, dLenZ=0.0;
	double dRatio = 6.5;  // x축길이:y축길이 비 (6.5:1)
	double xmin=0.0, zmin=0.0, xmax=0.0, zmax=0.0; 

	int nSize = aTendonProfile.GetSize();
	if (nSize > 0)
	{
		xmin = xmax = aTendonProfile[0].x;
		zmin = zmax = aTendonProfile[0].z;
	}
	for (int i = 0; i < nSize; i++)
	{
		SetValue(&m_Graph3DXZ, i, 0, aTendonProfile[i].x, aTendonProfile[i].z);
		if (aTendonProfile[i].x < xmin) xmin = aTendonProfile[i].x;
		else if (aTendonProfile[i].x > xmax) xmax = aTendonProfile[i].x;
		if (aTendonProfile[i].z < zmin) zmin = aTendonProfile[i].z;
		else if (aTendonProfile[i].z > zmax) zmax = aTendonProfile[i].z;
	}
	if (xmax-xmin > dRatio*(zmax-zmin)) { dLenX = xmax-xmin; dLenZ = (xmax-xmin)/dRatio; }
	else { dLenX = (zmax-zmin)*dRatio; dLenZ = zmax-zmin; }

	dMinRangeX = (xmin+xmax)/2.0 - (dLenX)/2.0;
	dMaxRangeX = (xmin+xmax)/2.0 + (dLenX)/2.0;
	dMinRangeZ = (zmin+zmax)/2.0 - (dLenZ)/2.0;
	dMaxRangeZ = (zmin+zmax)/2.0 + (dLenZ)/2.0;
}

void CCMTendonProfileDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pParentDlg && m_pParentDlg->GetSafeHwnd())
	{
		m_pParentDlg->ShowWindow(SW_SHOW);
		((CCMTendonDlg*)m_pParentDlg)->SetProfDlgFlag(FALSE);
	}
	
	CDialog::PostNcDestroy();
}

void CCMTendonProfileDlg::UnselectAll()
{
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	if (!m_bExternalSetting) return;
	if (!I_GENModelBase::GetCurMySelfST()) return;
	I_GENModelBase::UTIL_DeleteAllDisplayObjST();
	I_GENModelBase::GetCurMySelfST()->ViewInvalidate(FALSE);
}

void CCMTendonProfileDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!DoApply()) return;
	UnselectAll();
	DestroyWindow();
}

void CCMTendonProfileDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	UnselectAll();
	DestroyWindow();
}

void CCMTendonProfileDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	DoApply();
}

void CCMTendonProfileDlg::OnCmdTendonPropButton() 
{
	// TODO: Add your control notification handler code here
	CCMTendonTypeDlg Dlg;
	m_pModalDlg = &Dlg;
	Dlg.DoModal();
	if (m_bCloseDlg) PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	m_pModalDlg = 0;

	OnChangeProp();
	//AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_TENDON_PROP, 0));
	//AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_TENDON_PROP, 0));
}

void CCMTendonProfileDlg::OnChangeTendonShape() 
{
	UpdateData(TRUE);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlTendonShape, m_TdnaData.nTendonShape);
	EnableDisableCtrl();
/* 2D Element 형태를 구분하려고 m_TdnaData.nTendonShape 추가로 넘기기 위해 수정 // 2005.12. 23 jkpark
	m_wndGrid3D.SetCurveType(m_TdnaData.nTendonShape);
	m_wndGrid2DXY.SetCurveType(m_TdnaData.nTendonShape);
	m_wndGrid2DXZ.SetCurveType(m_TdnaData.nTendonShape);
*/
	m_wndGrid3D.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
	m_wndGrid2DXY.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
	m_wndGrid2DXZ.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
//수정 끝 2005.12. 23 jkpark
	OnGridDataChanged();  // Redraw Graph
}

void CCMTendonProfileDlg::OnChangeDir() 
{
	// TODO: Add your control notification handler code here
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlXAxisDir, m_TdnaData.nXAxisDirection);
	EnableDisableCtrl();
}

/*
void CCMTendonProfileDlg::OnChangeBeginEndLength() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

////////////////////////////////////////////////////////////////////////////
//  MouseEdit 에서 정의된 Notify를 사용해야 함..
//  차후 개선...^^=
	
	// TODO: Add your control notification handler code here
	//m_wndBeginLength.GetCoordData(&(m_TdnaData.dBeginLength),NULL,NULL);
	//m_wndEndLength.GetCoordData  (&(m_TdnaData.dEndLength  ),NULL,NULL);

	// 호출 안됨 : MouseEdit에서 NOTIFY 되도록 해야 함(by jbseon)
	OnGridDataChanged();  // Redraw Graph
}

void CCMTendonProfileDlg::OnKillfocusCmdBeginLengthEdit() 
{
	// TODO: Add your control notification handler code here
	OnGridDataChanged();  // Redraw Graph
}

void CCMTendonProfileDlg::OnKillfocusCmdEndLengthEdit() 
{
	// TODO: Add your control notification handler code here
	//m_wndBeginLength.GetCoordData(&(m_TdnaData.dBeginLength),NULL,NULL);
	//m_wndEndLength.GetCoordData  (&(m_TdnaData.dEndLength  ),NULL,NULL);
	OnGridDataChanged();  // Redraw Graph
}
*/

void CCMTendonProfileDlg::OnChangeCurveType() 
{
	// TODO: Add your control notification handler code here
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCurveType, m_TdnaData.nCurveType);
	EnableDisableCtrl();
/* 2D Element 형태를 구분하려고 m_TdnaData.nTendonShape 추가로 넘기기 위해 수정 // 2005.12. 23 jkpark
	m_wndGrid3D.SetCurveType(m_TdnaData.nCurveType);
	m_wndGrid2DXY.SetCurveType(m_TdnaData.nCurveType);
	m_wndGrid2DXZ.SetCurveType(m_TdnaData.nCurveType);
*/
	m_wndGrid3D.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
	m_wndGrid2DXY.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
	m_wndGrid2DXZ.SetCurveType(m_TdnaData.nCurveType, m_TdnaData.nTendonShape);
//수정 끝 2005.12. 23 jkpark
	OnGridDataChanged();  // Redraw Graph
}


/*
void CCMTendonProfileDlg::OnUpdateCmdEditTpRadius() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	OnGridDataChanged();  // Redraw Graph
}
*/

void CCMTendonProfileDlg::OnChangeProp()
{
	OnGridDataChanged();  // Redraw Graph
	
	//int idx = m_wndTdntCobx.GetCurSel();
	//BOOL ChangeSelect(T_TDNT_K Key);
	
	T_TDNT_K rKey;

	m_wndTdntCobx.GetSelectedTdnt(rKey);
	
	//CString tempString;
	//m_wndTdntCobx.GetLBText(idx,tempString);

	T_TDNT_D tempData;
	tempData.Initialize();

	m_pDoc = CDBDoc::GetDocPoint();
	m_pDoc->m_pAttrCtrl->GetTdnt(rKey,tempData);
	/*
	if (tempData.nTendonType == 1 && tempData.nLoadType == 1)
	{
		GetDlgItem(IDC_CMD_TENDON_PROP_POST_TENSION)->EnableWindow(TRUE);
		OnSelchangeCmdTendonPropPostTension();
	}
	else
	{
		GetDlgItem(IDC_CMD_TENDON_PROP_POST_TENSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BEGIN_USER_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_END_USER_EDIT)->EnableWindow(FALSE);
	}
	*/

	if( tempData.nTendonType == 1 && tempData.nLoadType == 0 ) // Internal-Pre
	{
		GetDlgItem(IDC_CMD_STATIC12)->SetWindowText(_LS(IDS_CMD_Transfer_Length)); //_LS(IDS_CMD_Lead_Length) _T("Lead Length")
		m_cobxPostTension.ResetContent(); 
		CDlgUtil::CobxAddItem(m_cobxPostTension, _LS(IDS_CMD_User_defined_Length),0); //User defined Length
		CDlgUtil::CobxAddItem(m_cobxPostTension, _LS(IDS_CMD_Auto_Calc_Strand),2); // _T("Auto Calc( 65 * Strand Diameter )")
		//m_cobxPostTension.SetCurSel(2-m_nAutoOrUser);
		m_bDebonded = TRUE;
	}
	else
	{
		GetDlgItem(IDC_CMD_STATIC12)->SetWindowText(_LS(IDS_CMD_Transfer_Length)); //_LS(IDS_CMD_Transfer_Length) _T("Transfer_Length")
		m_cobxPostTension.ResetContent();
		CDlgUtil::CobxAddItem(m_cobxPostTension, _LS(IDS_CMD_User_defined_Length),0); //User defined Length
		CDlgUtil::CobxAddItem(m_cobxPostTension, _LS(IDS_CMD_Auto_Calc),1); //(Auto Calc( 0.5 * (H + Bf/n) )
		m_bDebonded = FALSE;
	}

	// Debonded Length
	GetDlgItem(IDC_CMD_TDNA_DEBONDED_GRP              )->EnableWindow(m_bDebonded);
	GetDlgItem(IDC_CMD_TDNA_DEBONDED_LENGTH_STC       )->EnableWindow(m_bDebonded);
	GetDlgItem(IDC_CMD_TDNA_DEBONDED_LENGTH_BEGIN_STC )->EnableWindow(m_bDebonded);
	GetDlgItem(IDC_CMD_TDNA_DEBONDED_LENGTH_BEGIN_EDIT)->EnableWindow(m_bDebonded);
	GetDlgItem(IDC_CMD_TDNA_DEBONDED_LENGTH_END_STC   )->EnableWindow(m_bDebonded);
	GetDlgItem(IDC_CMD_TDNA_DEBONDED_LENGTH_END_EDIT  )->EnableWindow(m_bDebonded);
	GetDlgItem(IDC_CMD_TDNA_DEBONDED_LENGTH_END_UNIT  )->EnableWindow(m_bDebonded);

	if(m_nAutoOrUser == 2 || m_nAutoOrUser == 1)
		m_cobxPostTension.SetCurSel(1);
	else
		m_cobxPostTension.SetCurSel(0);

	ComboBoxDynamicChange( &m_cobxPostTension );

	//////ComboBox Dynamic Change///////////////
	//CString str1; CSize sz; int dx = 0;
	//CDC*  pDC = GetDC();
	//for(int h=0; h<m_cobxPostTension.GetCount(); h++)
	//{
	//  m_cobxPostTension.GetLBText(h,str1);
	//  sz = pDC->GetTextExtent(str1);
	//  if(sz.cx > dx) dx = sz.cx;
	//}
	//m_cobxPostTension.ReleaseDC(pDC);
	//if (dx > m_cobxPostTension.GetDroppedWidth()) m_cobxPostTension.SetDroppedWidth(dx);
	////////////////////////////////////////////

	OnSelchangeCmdTendonPropPostTension();
}

//////ComboBox Dynamic Change///////////////
void CCMTendonProfileDlg::ComboBoxDynamicChange(CComboBox* pCombo)
{
	CString str1; CSize sz; int dx = 0;
	CDC*  pDC = GetDC();

	for(int h=0; h<pCombo->GetCount(); h++)
	{
		pCombo->GetLBText(h,str1);
		sz = pDC->GetTextExtent(str1);

		if(sz.cx > dx) dx = sz.cx;
	}
	m_cobxPostTension.ReleaseDC(pDC);
	
	if ( dx > pCombo->GetDroppedWidth() ) 
		pCombo->SetDroppedWidth(dx);
}

BOOL CCMTendonProfileDlg::MakeSymmetricTondon()
{ 
	int nSymType, nInputType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlTypeRadio, nInputType);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSymPtRadio, nSymType);

	int nNum;
	double dx;
	int nStart, nEnd;
	T_TDNA_BASE Item;
	CArray<T_TDNA_BASE, T_TDNA_BASE&> aItem;
	if (nInputType == 1)
	{
		T_TDNA_BASE Axis;
		nNum = m_TdnaData.aProfile.GetSize();
		if (nNum >= 1)
		{
			if (nSymType == 0) 
			{
				Axis = m_TdnaData.aProfile.GetAt(0);
				nStart = 1; nEnd = nNum;
			}
			else 
			{
				Axis = m_TdnaData.aProfile.GetAt(nNum-1);
				nStart = 0; nEnd = nNum-1;
			}
			aItem.RemoveAll();
			for (int i=nStart; i<nEnd; i++)
			{
				dx = Axis.x - m_TdnaData.aProfile[i].x;
				Item.x = Axis.x + dx;
				//if (Item.x < 0.) continue;
				Item.y = m_TdnaData.aProfile[i].y;
				Item.z = m_TdnaData.aProfile[i].z;
				Item.bFix = m_TdnaData.aProfile[i].bFix;
				Item.dRadius = m_TdnaData.aProfile[i].dRadius;
				Item.dRy = m_TdnaData.aProfile[i].dRy;
				Item.dRz = m_TdnaData.aProfile[i].dRz;
				aItem.Add(Item);
			}
			for (int i=0; i<aItem.GetSize(); i++)
			{
				if (!m_wndGrid3D.AddData(aItem[i].x, aItem[i])) return FALSE;
			}
		}
	}
	else if (nInputType == 0) 
	{
		T_TDNA_BASE_2D Axis;
		nNum = m_TdnaData.aProfileY.GetSize();
		if (nNum >= 1)
		{
			if (nSymType == 0) 
			{
				Axis = m_TdnaData.aProfileY.GetAt(0);
				nStart = 1; nEnd = nNum;
			}
			else
			{
				Axis = m_TdnaData.aProfileY.GetAt(nNum-1);
				nStart = 0; nEnd = nNum-1;
			}
			aItem.RemoveAll();
			for (int i=nStart; i<nEnd; i++)
			{
				dx = Axis.x - m_TdnaData.aProfileY[i].x;
				Item.x = Axis.x + dx;
				//if (Item.x < 0.) continue;
				Item.y = m_TdnaData.aProfileY[i].yz;
				Item.z = 0.0;
				Item.bFix = m_TdnaData.aProfileY[i].bFix;
				Item.dRadius = m_TdnaData.aProfileY[i].dRadius;
				Item.dRy = m_TdnaData.aProfileY[i].dR;
				Item.dRz = 0.0;
				aItem.Add(Item);
			}
			for (int i=0; i<aItem.GetSize(); i++)
			{
				if (!m_wndGrid2DXY.AddData(aItem[i].x, aItem[i])) return FALSE;
			}
		}

		nNum = m_TdnaData.aProfileZ.GetSize();
		if (nNum >= 1)
		{
			if (nSymType == 0) 
			{
				Axis = m_TdnaData.aProfileZ.GetAt(0);
				nStart = 1; nEnd = nNum;
			}
			else 
			{
				Axis = m_TdnaData.aProfileZ.GetAt(nNum-1);
				nStart = 0; nEnd = nNum-1;
			}
			aItem.RemoveAll();
			for (int i=nStart; i<nEnd; i++)
			{
				dx = Axis.x - m_TdnaData.aProfileZ[i].x;
				Item.x = Axis.x + dx;
				//if (Item.x < 0.) continue;
				Item.y = 0.0;
				Item.z = m_TdnaData.aProfileZ[i].yz;
				Item.bFix = m_TdnaData.aProfileZ[i].bFix;
				Item.dRadius = m_TdnaData.aProfileZ[i].dRadius;
				Item.dRy = 0.0;
				Item.dRz = m_TdnaData.aProfileZ[i].dR;

				aItem.Add(Item);
			}
			for (int i=0; i<aItem.GetSize(); i++)
			{
//        if (!m_wndGrid2DXZ.AddData(aItem[i].x, aItem[i])) return FALSE;
				if (!m_wndGrid2DXZ.AddData(aItem[i].x, aItem[i], Axis.bBottomZ)) return FALSE;  //2DXZ에는 bottom이 들어간다
			}
		}
	}
	else ASSERT(0);

	return TRUE;
}

void CCMTendonProfileDlg::OnCmdTendonSymmetricButton() 
{
	// TODO: Add your control notification handler code here
	MakeSymmetricTondon();
}

void CCMTendonProfileDlg::OnChangeCmdInsertposEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString csElemNum;
	m_editInsertPos.GetWindowText(csElemNum);
	m_editXDir.SetWindowText(csElemNum);
}

void CCMTendonProfileDlg::ChangeInputType()
{
	int ix;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlTypeRadio, ix);

	AlignControls2(ix);

	CDlgUtil::CtrlShowHide(this, m_aCtrlType2D, ix==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType3D, ix==1);
	OnGridDataChanged();
} 

void CCMTendonProfileDlg::OnCmdInputTypeRadio() 
{
	// TODO: Add your control notification handler code here
	ChangeInputType();
}

LRESULT CCMTendonProfileDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if (m_wndEndLength.GetSafeHwnd() == (HWND)lParam || 
			m_wndBeginLength.GetSafeHwnd() == (HWND)lParam)
	{
		if (pIGM->GetModelingModeST() == IG_STATE_SET_DIST_SECOND)
		{
			OnGridDataChanged(FALSE);
		}
	}
	return 0L;
}

LRESULT CCMTendonProfileDlg::OnMouseEditTextChanged(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if (m_wndEndLength.GetSafeHwnd() == (HWND)lParam || 
			m_wndBeginLength.GetSafeHwnd() == (HWND)lParam)
	{
		if (m_wndEndLength.IsKeyPressed() || m_wndBeginLength.IsKeyPressed())
		{
			OnGridDataChanged(FALSE);
		}
	}
	return 0L;
}
/*
void CCMTendonProfileDlg::OnCmdUserDefineCheck() 
{   
	UpdateData(TRUE);
	m_wndEndUser.EnableWindow(m_bUserDef);
	m_wndBeginUser.EnableWindow(m_bUserDef);
}
*/
void CCMTendonProfileDlg::OnCmdTendonGroupButton()
{
	/**
	CCMTendonDefDlg Dlg;
	m_pModalDlg = &Dlg;
	Dlg.DoModal();
	if (m_bCloseDlg) PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	m_pModalDlg = 0;*/

	CreateOrActivateDlg(m_pDoc, CCMTendonDefDlg::IDD, this);
}

void CCMTendonProfileDlg::OnSelchangeCmdTendonPropPostTension() 
{
	m_nAutoOrUser = m_cobxPostTension.GetItemData(m_cobxPostTension.GetCurSel());
	//m_nAutoOrUser = m_cobxPostTension.GetCurSel();
	m_wndEndUser.EnableWindow(m_nAutoOrUser == 0);
	m_wndBeginUser.EnableWindow(m_nAutoOrUser == 0);
}


// void CCMTendonProfileDlg::OnDeltaposCmdTendonCountSpin(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	CDlgUtil::SetSpinValue(this,IDC_CMD_TENDON_COUNT_EDIT,1,1000,pNMHDR,pResult);
// 	
// 	*pResult = 0;
// }

void CCMTendonProfileDlg::OnCmdTypicalTendonCheck() 
{
	UpdateData(TRUE);

	// show hide Tendon count
	m_TdnaData.bTypicalTendon = m_chkTypicalTendon.GetCheck()? TRUE : FALSE;
	// CDlgUtil::CtrlShowHide(this, m_aCtrlTendonCount, m_TdnaData.bTypicalTendon);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlTendonCount, m_TdnaData.bTypicalTendon);

	UpdateData(FALSE);
}

LRESULT CCMTendonProfileDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	BOOL bCheck = m_chkDetail.GetFoldState();

	GetDlgItem(IDC_CMD_RADIO_2D)->EnableWindow(!bCheck);
	GetDlgItem(IDC_CMD_RADIO_3D)->EnableWindow(!bCheck);

	if(!bCheck)
	{		
		OnCmdInputTypeRadio();
		OnChangeCurveType();
		RedrawWindow();
	}

	// Apply 가 안따라 오네...왜그런걸까.. 일단 아래 코드로 해놓자.. ///////////
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_APPLY);
	
	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	/////////////////////////////////////////////////////////////////////////////

	return 0L;
}