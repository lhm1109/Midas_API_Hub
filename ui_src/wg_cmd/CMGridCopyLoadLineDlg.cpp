// CMGridCopyLoadLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridCopyLoadLineDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_MathFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGridCopyLoadLineDlg dialog


CCMGridCopyLoadLineDlg::CCMGridCopyLoadLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGridCopyLoadLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGridCopyLoadLineDlg)
	m_nRefType = 1;
	//}}AFX_DATA_INIT
	m_arGlobal.Add(IDC_CMD_GLOBAL_STATIC1);
	m_arGlobal.Add(IDC_CMD_GLOBAL_STATIC2);
	m_arGlobal.Add(IDC_GRID_COPY_CMB2);
	m_arGlobal.Add(IDC_GRID_DISTANCE_EDT2);
	m_arGlobal.Add(IDC_CMD_DISTANCE_UNIT2);

	m_arNormal.Add(IDC_CMD_NORMAL_STATIC1);
	m_arNormal.Add(IDC_CMD_NORMAL_STATIC2);
	m_arNormal.Add(IDC_GRID_COPY_CMB);
	m_arNormal.Add(IDC_GRID_DISTANCE_EDT);
	m_arNormal.Add(IDC_CMD_DISTANCE_UNIT);

	m_arReference.Add(IDC_CMD_REF_STATIC1);
	m_arReference.Add(IDC_CMD_REF_STATIC2);
	m_arReference.Add(IDC_GRID_COPY_CMB3);
	m_arReference.Add(IDC_GRID_COPY_CMB4);
	m_arReference.Add(IDC_CMD_REF_OPT1);
	m_arReference.Add(IDC_CMD_REF_OPT2);
	m_arReference.Add(IDC_GRID_DISTANCE_EDT3);
	m_arReference.Add(IDC_GRID_RATIO_EDT);
	m_arReference.Add(IDC_CMD_DISTANCE_UNIT4);
}


void CCMGridCopyLoadLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridCopyLoadLineDlg)
	DDX_Control(pDX, IDC_CMD_DISTANCE_UNIT, m_wndDistanceUnit);
	DDX_Control(pDX, IDC_CMD_DISTANCE_UNIT2, m_wndDistanceGlobalUnit);
	DDX_Control(pDX, IDC_CMD_DISTANCE_UNIT4, m_wndDistanceRefUnit);
	DDX_Control(pDX, IDC_GRID_DISTANCE_EDT, m_wndDistance);
	DDX_Control(pDX, IDC_GRID_DISTANCE_EDT3, m_wndDistanceRef);
	DDX_Control(pDX, IDC_GRID_RATIO_EDT, m_wndRatio);
	DDX_Control(pDX, IDC_GRID_NAME_EDT, m_wndName);
	DDX_Control(pDX, IDC_GRID_COPY_CMB, m_cmbCopyFrom);
	DDX_Control(pDX, IDC_GRID_COPY_CMB2, m_cmbCopyFrom2);
	DDX_Control(pDX, IDC_GRID_COPY_CMB3, m_cmbCopyFromRef1);
	DDX_Control(pDX, IDC_GRID_COPY_CMB4, m_cmbCopyFromRef2);
	DDX_Control(pDX, IDC_GRID_COPY_TYPE, m_cmbCopyType);
	DDX_Radio(pDX, IDC_CMD_REF_OPT1, m_nRefType);
	DDX_Control(pDX, IDC_GRID_DISTANCE_EDT2, m_strDXDY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridCopyLoadLineDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGridCopyLoadLineDlg)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOk)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_CBN_SELCHANGE(IDC_GRID_COPY_TYPE, OnSelchangeGridCopyType)
	ON_BN_CLICKED(IDC_CMD_REF_OPT1, OnCmdRefOpt)
	ON_BN_CLICKED(IDC_CMD_REF_OPT2, OnCmdRefOpt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridCopyLoadLineDlg message handlers

void CCMGridCopyLoadLineDlg::OnCmdOk() 
{
	if (!Execute())
		return;
	CDialogMove::OnOK();
}

void CCMGridCopyLoadLineDlg::OnCmdApply() 
{
	if (!Execute())
		return;
	InitDlg();
}

BOOL CCMGridCopyLoadLineDlg::Execute()
{
	UpdateData();

	CString strName;
	m_wndName.GetWindowText(strName);
	if (strName == _T(""))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_NAME_MISS));
		return FALSE;
	}
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc->m_pAttrCtrl->ExistLdln(strName))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_NAME_EXIST));
		return FALSE;
	}

	int nSel = m_cmbCopyType.GetCurSel();
	if (nSel == 0)
		return ExecuteGlobal();
	else if (nSel == 1)
		return ExecuteNormal();
	else if (nSel == 2)
		return ExecuteReference();
	return FALSE;
}

BOOL CCMGridCopyLoadLineDlg::ExecuteReference()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CString strName;
	m_wndName.GetWindowText(strName);

	unsigned long key1, key2;
	CString strRef1, strRef2;
	BOOL bMgdr1=TRUE, bMgdr2 = TRUE;
	int nSel;
	nSel = m_cmbCopyFromRef1.GetCurSel();
	m_cmbCopyFromRef1.GetLBText(nSel, strRef1);
	nSel = m_cmbCopyFromRef2.GetCurSel();
	m_cmbCopyFromRef2.GetLBText(nSel, strRef2);

	if (strRef1.CompareNoCase(strRef2) == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_COMBO_SAMEERROR));
		return FALSE;
	}
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_cmbCopyFromRef1, key1))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_COMBO_ERROR));
		return FALSE;
	}
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_cmbCopyFromRef2, key2))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_COMBO_ERROR));
		return FALSE;
	}

	if (strRef1.GetAt(0) == 'L')
		bMgdr1 = FALSE;
	if (strRef2.GetAt(0) == 'L')
		bMgdr2 = FALSE;

	double dDistOrRatio;
	if (m_nRefType == 0)
		dDistOrRatio = m_wndDistanceRef.GetEditValue();
	else 
		dDistOrRatio = m_wndRatio.GetEditValue();

	T_LDLN_D data;
	data.Initialize();
	data.Name = strName;
	data.nSeq = pDoc->m_pAttrCtrl->GetCountLdln() + 1;
	//DB함수 사용
	CGridInfo* pGridInfo;
	pGridInfo = pDoc->m_pPostCtrl->GetGridInfo();
	pGridInfo->GetCopiedLoadLine(key1, !bMgdr1, key2, !bMgdr2, m_nRefType, dDistOrRatio, data);
	if (!pDoc->m_pDataCtrl->AddLdln(data))
		return FALSE;
	return TRUE;
}

BOOL CCMGridCopyLoadLineDlg::ExecuteGlobal()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	CString strName;
	m_wndName.GetWindowText(strName);

	unsigned long key;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_cmbCopyFrom2, key))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_COMBO_ERROR));
		return FALSE;
	}
	int nSel = m_cmbCopyFrom2.GetCurSel();
	CString comboName;
	m_cmbCopyFrom2.GetLBText(nSel, comboName);

	BOOL bMgdr = TRUE;
	if (comboName.GetAt(0) == 'L')
		bMgdr = FALSE;

	//Global distance를 받아옴 //////////////////////////////////
	CString str;
	m_strDXDY.GetWindowText(str);
	int nIndex = str.Find(',');
	if (nIndex == -1)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_COORD_ERROR));
		return FALSE;
	}

	double dX, dY;
	dX = _tstof(str.Left(nIndex));
	dY = _tstof(str.Right(str.GetLength()-nIndex-1));
	/////////////////////////////////////////////////////////////
	CArray<T_NODE_D, T_NODE_D> src;
	GetNodeCoord(bMgdr, key, src);

	T_LDLN_D data;
	data.Initialize();
	data.Name = strName;
	data.nSeq = pDoc->m_pAttrCtrl->GetCountLdln() + 1;

	T_LDLN_BASE base;
	int nSize = src.GetSize();
	for (int i=0; i<nSize; i++)
	{
		base.x = src[i].x + dX;
		base.y = src[i].y + dY;
		base.z = src[i].z;
		data.aData.Add(base);
	}
	if (!pDoc->m_pDataCtrl->AddLdln(data))
		return FALSE;
	return TRUE;
}

BOOL CCMGridCopyLoadLineDlg::ExecuteNormal()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CString strName;
	m_wndName.GetWindowText(strName);
	
	double dDistance;
	dDistance = m_wndDistance.GetEditValue();
	if (dDistance == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_DISTANCE_ZERO));
		return FALSE;
	}
	
	unsigned long key;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_cmbCopyFrom, key))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_COPY_LOADLINE_COMBO_ERROR));
		return FALSE;
	}

	int nSel = m_cmbCopyFrom.GetCurSel();
	CString comboName;
	m_cmbCopyFrom.GetLBText(nSel, comboName);

	BOOL bMgdr = TRUE;
	if (comboName.GetAt(0) == 'L')
		bMgdr = FALSE;

	T_LDLN_D data;
	data.Initialize();
	data.Name = strName;
	data.nSeq = pDoc->m_pAttrCtrl->GetCountLdln() + 1;
	CArray<T_NODE_D, T_NODE_D> src;
	CArray<T_NODE_D, T_NODE_D> target;
	
	GetNodeCoord(bMgdr, key, src);
	target.SetSize(src.GetSize());

	GetTransData(src, target, dDistance);
	
	T_LDLN_BASE base;
	for (int i=0; i<target.GetSize(); i++)
	{
		base.x = target[i].x;
		base.y = target[i].y;
		base.z = target[i].z;
		data.aData.Add(base);
	}
	if (!pDoc->m_pDataCtrl->AddLdln(data))
		return FALSE;
	return TRUE;
}

BOOL CCMGridCopyLoadLineDlg::GetTransData(CArray<T_NODE_D, T_NODE_D>& src, CArray<T_NODE_D, T_NODE_D>& target, double dDistance)
{
	CArray<double, double> aZCoord;
	int nSize = src.GetSize();
	for (int i=0; i<nSize; i++)
	{
		aZCoord.Add(src[i].z);
		src[i].z = 0.;
	}

	//Newells
	double normal[3] = {0., 0., 0.};
	NewellsMethod(src, normal);

	double dLength = normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2];
	if (dLength < 1e-3)
	{
		normal[0] = 0.;
		normal[1] = 0.;
		normal[2] = 1.;
	}
	else
	{
		normal[0] = -normal[0];
		normal[1] = -normal[1];
		normal[2] = -normal[2];
	}

	T_NODE_D point;

	if (nSize < 2)
		return FALSE;

	double xDir[3] = {0., 0., 0.};
	double yDir[3] = {0., 0., 0.};
	GetDirVector(src[0], src[1], xDir);
	CMathFunc::mathCross(xDir, normal, yDir);
	CMathFunc::mathNormalize(yDir, yDir);
	GetTransCoord(src[0], yDir, dDistance, point);
	target.SetAt(0, point);
	GetTransCoord(src[1], yDir, dDistance, point);
	target.SetAt(1, point);
	
	double pl1_i[3];
	double pl1_j[3];
	double pl2_i[3];
	double pl2_j[3];
	double intersect[3];
	double dblDistance;

	for (int i=1; i<nSize-1; i++)
	{
		GetDirVector(src[i], src[i+1], xDir);
		CMathFunc::mathCross(xDir, normal, yDir);
		if (!CMathFunc::mathNormalize(yDir, yDir))
			return FALSE;
		GetTransCoord(src[i+1], yDir, dDistance, point);
		target.SetAt(i+1, point);

		GetTransCoord(src[i], yDir, dDistance, point);
		
		if (!IsSameNode(point, target[i]))
		{
			pl1_i[0] = target[i-1].x;
			pl1_i[1] = target[i-1].y;
			pl1_i[2] = target[i-1].z;
			pl1_j[0] = target[i].x;
			pl1_j[1] = target[i].y;
			pl1_j[2] = target[i].z;

			pl2_i[0] = target[i+1].x;
			pl2_i[1] = target[i+1].y;
			pl2_i[2] = target[i+1].z;
			pl2_j[0] = point.x;
			pl2_j[1] = point.y;
			pl2_j[2] = point.z;

			if (!CMathFunc::mathIntersectLine(pl1_i,pl1_j, pl2_i,pl2_j, 10e5, dblDistance, intersect))
				return FALSE;
			point.x = intersect[0];
			point.y = intersect[1];
			point.z = intersect[2];
			target.SetAt(i, point);
		}
	}
	for (int i=0; i<nSize; i++)
	{
		target[i].z = aZCoord[i];
	}
	return TRUE;
}

BOOL CCMGridCopyLoadLineDlg::IsSameNode(T_NODE_D& node1, T_NODE_D& node2)
{
	double dTol = 1e-3;
	double dX1 = node1.x-node2.x;
	double dX2 = node1.y-node2.y;
	double dX3 = node1.z-node2.z;

	double dLength = dX1*dX1 + dX2*dX2 + dX3*dX3;
	if (dLength > dTol)
		return FALSE;
	return TRUE;
}

void CCMGridCopyLoadLineDlg::GetTransCoord(T_NODE_D node, double Direction[3], double dDistance, T_NODE_D& target)
{
	target.x = node.x + Direction[0]*dDistance;
	target.y = node.y + Direction[1]*dDistance;
	target.z = node.z + Direction[2]*dDistance;
}

void CCMGridCopyLoadLineDlg::GetDirVector(T_NODE_D v1, T_NODE_D v2, double dirVector[3])
{
	dirVector[0] = v2.x -v1.x;
	dirVector[1] = v2.y -v1.y;
	dirVector[2] = v2.z -v1.z;
}

void CCMGridCopyLoadLineDlg::NewellsMethod(CArray<T_NODE_D, T_NODE_D>&src, double normal[3])
{
	double nor_X=0,nor_Y=0,nor_Z=0;
	int nV;
	int i,j;
	
	nV = src.GetSize();
	
	T_NODE_D NodeI;
	T_NODE_D NodeJ;
	double norm[3] = {0., 0., 0.};
	for (i=0;i<nV;i++) 
	{
		if (i== nV-1 ) j=0;
		else j=i+1;

		NodeI = src[i];
		NodeJ = src[j];

		norm[0]=norm[0]+(NodeI.y-NodeJ.y) * (NodeI.z+NodeJ.z);
		norm[1]=norm[1]+(NodeI.z-NodeJ.z) * (NodeI.x+NodeJ.x);
		norm[2]=norm[2]+(NodeI.x-NodeJ.x) * (NodeI.y+NodeJ.y);
	}
	CMathFunc::mathNormalize(norm, normal);
}

void CCMGridCopyLoadLineDlg::InitDlg()
{
	InitCombo();
	//m_wndName.SetWindowText(_T(""));
	m_wndName.SetWindowText(CreateGirderName());
	m_wndDistance.SetEditUnit(0.0);
}

void CCMGridCopyLoadLineDlg::AlignCtrl()
{
	CRect ref, target;
	GetDlgItem(IDC_CMD_REF_STATIC1)->GetWindowRect(ref);
	GetDlgItem(IDC_CMD_NORMAL_STATIC1)->GetWindowRect(target);

	int dX, dY;
	dX = ref.left - target.left;
	dY = ref.top - target.top;

	CDlgUtil::CtrlMoveDistXY(this, m_arNormal, dX, dY);

	GetDlgItem(IDC_CMD_GLOBAL_STATIC1)->GetWindowRect(target);
	dX = ref.left - target.left;
	dY = ref.top - target.top;
	CDlgUtil::CtrlMoveDistXY(this, m_arGlobal, dX, dY);
}

BOOL CCMGridCopyLoadLineDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_wndDistanceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistanceGlobalUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistanceRefUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_cmbCopyType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbCopyType, _LS(IDS_CMD_GRID_COPYOPTION_GLOBAL), 0);
	CDlgUtil::CobxAddItem(m_cmbCopyType, _LS(IDS_CMD_GRID_COPYOPTION_NORMAL), 1);
	CDlgUtil::CobxAddItem(m_cmbCopyType, _LS(IDS_CMD_GRID_COPYOPTION_REFERENCE), 2);
	m_cmbCopyType.SetCurSel(1);

	InitCombo();
	AlignCtrl();
	
	m_strDXDY.SetWindowText(_T("0, 0"));
	m_wndName.SetWindowText(CreateGirderName());
	m_wndDistance.SetEditUnit(0.0);

	CDlgUtil::CtrlShowHide(this, m_arGlobal, m_nRefType==0);
	CDlgUtil::CtrlShowHide(this, m_arNormal, m_nRefType==1);
	CDlgUtil::CtrlShowHide(this, m_arReference, m_nRefType==2);

	OnCmdRefOpt();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridCopyLoadLineDlg::InitCombo()
{
	m_cmbCopyFrom.ResetContent();
	m_cmbCopyFrom2.ResetContent();
	m_cmbCopyFromRef1.ResetContent();
	m_cmbCopyFromRef2.ResetContent();

	CDBDoc *pDoc = CDBDoc::GetDocPoint();

	T_MGDR_K MgdrK;
	T_MGDR_D MgdrD;
	T_LDLN_K LdlnK;
	T_LDLN_D LdlnD;
	CString strName;

	POSITION Pos;
	Pos = pDoc->m_pAttrCtrl->GetStartMgdr();
	while(Pos)
	{
		pDoc->m_pAttrCtrl->GetNextMgdr(Pos, MgdrK, MgdrD);
		strName.Format(_T("G:%s"), MgdrD.Name);
		CDlgUtil::CobxAddItem(m_cmbCopyFrom, strName, MgdrK);
		CDlgUtil::CobxAddItem(m_cmbCopyFrom2, strName, MgdrK);
		CDlgUtil::CobxAddItem(m_cmbCopyFromRef1, strName, MgdrK);
		CDlgUtil::CobxAddItem(m_cmbCopyFromRef2, strName, MgdrK);
	}

	Pos = pDoc->m_pAttrCtrl->GetStartLdln();
	while(Pos)
	{
		pDoc->m_pAttrCtrl->GetNextLdln(Pos, LdlnK, LdlnD);
		strName.Format(_T("L:%s"), LdlnD.Name);
		CDlgUtil::CobxAddItem(m_cmbCopyFrom, strName, LdlnK);
		CDlgUtil::CobxAddItem(m_cmbCopyFrom2, strName, LdlnK);
		CDlgUtil::CobxAddItem(m_cmbCopyFromRef1, strName, LdlnK);
		CDlgUtil::CobxAddItem(m_cmbCopyFromRef2, strName, LdlnK);
	}
}

CString CCMGridCopyLoadLineDlg::CreateGirderName()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();

	CString strName;
	int nCount = pDoc->m_pAttrCtrl->GetCountLdln() + 1;
	strName.Format(_T("L%d"), nCount);

	while(pDoc->m_pAttrCtrl->ExistLdln(strName))
	{
		nCount++;
		strName.Format(_T("L%d"), nCount);
	}
	return strName;
}

void CCMGridCopyLoadLineDlg::OnSelchangeGridCopyType() 
{
	int nSel = m_cmbCopyType.GetCurSel();
	
	CDlgUtil::CtrlShowHide(this, m_arGlobal, nSel==0);
	CDlgUtil::CtrlShowHide(this, m_arNormal, nSel==1);
	CDlgUtil::CtrlShowHide(this, m_arReference, nSel==2);
}

void CCMGridCopyLoadLineDlg::OnCmdRefOpt() 
{
	UpdateData();
	GetDlgItem(IDC_GRID_DISTANCE_EDT3)->EnableWindow(m_nRefType==0);
	GetDlgItem(IDC_GRID_RATIO_EDT)->EnableWindow(m_nRefType==1);
}

void CCMGridCopyLoadLineDlg::GetNodeCoord(BOOL bMgdr, UINT key, CArray<T_NODE_D, T_NODE_D>& coord)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	T_NODE_D point;
	if (bMgdr)
	{
		T_MGDR_D MgdrD;
		pDoc->m_pAttrCtrl->GetMgdr(key, MgdrD);
		coord.SetSize(MgdrD.aData.GetSize());

		T_NODE_K NodeK;
		T_NODE_D NodeD;
		
		int nSize = MgdrD.aData.GetSize();
		for (int i=0; i<nSize; i++)
		{
			NodeK = MgdrD.aData[i].KeyNode;
			pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD);
			
			point.x = NodeD.x;
			point.y = NodeD.y;
			point.z = NodeD.z;

			coord.SetAt(i, point);
		}
	}
	else
	{
		T_LDLN_D LdlnD;
		pDoc->m_pAttrCtrl->GetLdln(key, LdlnD);
		coord.SetSize(LdlnD.aData.GetSize());

		int nSize = LdlnD.aData.GetSize();
		for (int i=0; i<nSize; i++)
		{
			point.x = LdlnD.aData[i].x;
			point.y = LdlnD.aData[i].y;
			point.z = LdlnD.aData[i].z;

			coord.SetAt(i, point);
		}
	}
}
