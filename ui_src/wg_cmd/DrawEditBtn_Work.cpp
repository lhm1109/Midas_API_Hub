#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"
// #include "../NS_lib/IProductInfo.h"
// #include "../NS_lib/IMSGInfo.h"
// 
// #include "../NS_opt/IOptionDB.h"
// #include "../NS_opt/OptionDBStruct.h"


#include "DrawEditBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 작업전 /////////////////////////////////
void CDrawEditBtn::BeforeCurrUnitSelect(CPoint CurrP)
{
	CurrRemoveDrawData();
	SelectRemoveDrawData();
	RedrawWindow();
}

void CDrawEditBtn::BeforeCurrUnitPoint(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);	
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	if(m_pPointEdit->GetSafeHwnd()) m_pPointEdit->SetWindowText(strCoord);	

	CurrRemoveDrawData();
	MakeCurrUnitPoint(dCoord);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::BeforeCurrUnitLine(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);	
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	if(m_pLineStartPEdit) m_pLineStartPEdit->SetWindowText(strCoord);
	if(m_pLineEndPEdit)   m_pLineEndPEdit->SetWindowText(strCoord);

	CurrRemoveDrawData();
	MakeCurrUnitLine(dCoord, dCoord);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::BeforeCurrUnitArc(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	CString strValue;  strValue.Format(_T("%g"), 0.0);
	if(m_pArcStartPEdit) m_pArcStartPEdit->SetWindowText(strCoord);
	if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);
	if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);
	if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strValue);
	if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strValue);
	//if(m_pArcInsideCheck) m_pArcRadiusEdit->SetCheck(TRUE);

	CurrRemoveDrawData();
	MakeCurrUnitArc(dCoord, dCoord, dCoord);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::BeforeCurrUnitArcB(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	CString strValue;  strValue.Format(_T("%g"), 0.0);
	if(m_pArcStartPEdit) m_pArcStartPEdit->SetWindowText(strCoord);
	if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);
	if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);
	if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strValue);
	if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strValue);
	//if(m_pArcInsideCheck) m_pArcRadiusEdit->SetCheck(TRUE);

	CurrRemoveDrawData();
	MakeCurrUnitArcB(dCoord, dCoord, dCoord);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::BeforeCurrUnitCircle(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	if(m_pCircleStartPEdit) m_pCircleStartPEdit->SetWindowText(strCoord);
	if(m_pCircleEndPEdit)   m_pCircleEndPEdit->SetWindowText(strCoord);
	if(m_pCircleRadiusEdit) m_pCircleRadiusEdit->SetWindowText(_T("0.0"));

	CurrRemoveDrawData();
	MakeCurrUnitCircle(dCoord, dCoord);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::BeforeCurrUnitPolyline(CPoint CurrP)
{	
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);	
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	if(m_pPolylineStartPEdit) m_pPolylineStartPEdit->SetWindowText(strCoord);
	if(m_pPolylinePassPEdit ) m_pPolylinePassPEdit->SetWindowText(strCoord);
	if(m_pPolylineEndPEdit  ) m_pPolylineEndPEdit->SetWindowText(strCoord);

	m_CurrPolylineRD.Init();

	CurrRemoveDrawData();
	MakeCurrUnitPolyline(m_CurrPolylineRD, dCoord);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::BeforeCurrUnitRect(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	if(m_pRectCorner1PEdit) m_pRectCorner1PEdit->SetWindowText(strCoord);
	if(m_pRectCorner2PEdit) m_pRectCorner2PEdit->SetWindowText(strCoord);

	CurrRemoveDrawData();
	MakeCurrUnitCircle(dCoord, dCoord);
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

// 작업시작 /////////////////////////////////
void CDrawEditBtn::StartCurrUnitSelect(CPoint CurrP)
{
	CPoint CurrP_Snap;
	double dCoord[2], dCoord_Snap[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	CurrP_Snap = CurrP;	
	dCoord_Snap[0] = dCoord[0];
	dCoord_Snap[1] = dCoord[1];
	ChangeSnapPoint(CurrP_Snap, dCoord_Snap);// Snap이 적용될 경우 좌표변환

	m_bReSelectDrawUnit = FALSE;
	//if(m_bUseMove)
	//{
		UINT SelectKey;
		if(m_DrawCtrl.GetSelectDrawUnitKey(CurrP_Snap, SelectKey))
		{
			m_bSelectMoveMode = TRUE; // 기존의 Item을 선택한경우 선택항목을 유지하며, 새로운 Item인 경우 선택항목을 갱신함			

			if(!m_DrawCtrl.IsSelected(SelectKey))
			{
				// 선택항목갱신				
				if(m_nShiftButtonState == 0)
				{
					nrUINT arConnectedKey;
					//m_DrawCtrl.GetConnectedLineKey(SelectKey,arConnectedKey,IOptionDB::Instance()->GetData()->m_RcDgnRbrGnrl.dOffsetAngToMergeLine);
					//m_DrawCtrl.GetConnectedLineKey(SelectKey,arConnectedKey,D_MERGE_LINE_ANG);
					arConnectedKey.SetSize(1);
					arConnectedKey[0] = SelectKey;

					m_DrawCtrl.RemoveAllSelectedKey();
					
					if(arConnectedKey.GetSize() == 0)
					{
						m_DrawCtrl.AddSelectedKey(SelectKey);
					}
					else
					{
						for(UINT i=0; i<arConnectedKey.GetSize(); i++)
						{
							m_DrawCtrl.AddSelectedKey(arConnectedKey[i]);
						}					
					}					
				}
				else 
				{
					nrUINT arConnectedKey;
					//m_DrawCtrl.GetConnectedLineKey(SelectKey,arConnectedKey,IOptionDB::Instance()->GetData()->m_RcDgnRbrGnrl.dOffsetAngToMergeLine);
					m_DrawCtrl.GetConnectedLineKey(SelectKey,arConnectedKey,D_MERGE_LINE_ANG);

					if(arConnectedKey.GetSize() == 0)
					{
						m_DrawCtrl.ReSelectedKey(SelectKey);
					}
					else
					{
						for(UINT i=0; i<arConnectedKey.GetSize(); i++)
						{
							m_DrawCtrl.ReSelectedKey(arConnectedKey[i]);
						}
					}
				}
			}
			else 
			{
				m_bReSelectDrawUnit = TRUE;
			}
		}
		else 
		{
			m_bSelectMoveMode = FALSE;// 선택항목 없음
		}
	//}
	//else 
	//{
	//	m_bSelectMoveMode = FALSE;// 선택항목 없음
	//}

	m_StartPoint     = m_bSelectMoveMode ? CurrP_Snap     : CurrP;	
	m_StartPointR[0] = m_bSelectMoveMode ? dCoord_Snap[0] : dCoord[0];
	m_StartPointR[1] = m_bSelectMoveMode ? dCoord_Snap[1] : dCoord[1];

	m_nButtonCount = 1;
}

void CDrawEditBtn::StartCurrUnitPoint(CPoint CurrP)
{ 
	//작업없음
}

void CDrawEditBtn::StartCurrUnitLine(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	m_StartPoint = CurrP;	
	m_StartPointR[0] = dCoord[0];
	m_StartPointR[1] = dCoord[1];

	m_nButtonCount = 1;

	CString strCoord = GetCoordText(m_StartPointR[0], m_StartPointR[1]);
	if(m_pLineStartPEdit) m_pLineStartPEdit->SetWindowText(strCoord);
	if(m_pLineEndPEdit)   m_pLineEndPEdit->SetWindowText(strCoord);		
}

void CDrawEditBtn::StartCurrUnitArc(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	m_StartPoint = CurrP;
	m_StartPointR[0] = dCoord[0];
	m_StartPointR[1] = dCoord[1];

	m_nButtonCount = 1;

	CString strCoord = GetCoordText(m_StartPointR[0], m_StartPointR[1]);
	CString strValue; 
	strValue.Format(_T("%g"), 0.0);
	if(m_pArcStartPEdit) m_pArcStartPEdit->SetWindowText(strCoord);
	if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);
	if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);	
	if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strValue);
	if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strValue);
	//if(m_pArcInsideCheck) m_pArcRadiusEdit->SetCheck(TRUE);
}

void CDrawEditBtn::StartCurrUnitArcB(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	m_StartPoint = CurrP;
	m_StartPointR[0] = dCoord[0];
	m_StartPointR[1] = dCoord[1];

	m_nButtonCount = 1;

	CString strCoord = GetCoordText(m_StartPointR[0], m_StartPointR[1]);
	CString strValue; 
	strValue.Format(_T("%g"), 0.0);
	if(m_pArcStartPEdit) m_pArcStartPEdit->SetWindowText(strCoord);
	if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);
	if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);	
	if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strValue);
	if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strValue);
	//if(m_pArcInsideCheck) m_pArcRadiusEdit->SetCheck(TRUE);
}

void CDrawEditBtn::StartCurrUnitCircle(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	m_StartPoint = CurrP;
	m_StartPointR[0] = dCoord[0];
	m_StartPointR[1] = dCoord[1];
	m_nButtonCount = 1;

	CString strCoord = GetCoordText(m_StartPointR[0], m_StartPointR[1]);
	if(m_pCircleStartPEdit) m_pCircleStartPEdit->SetWindowText(strCoord);
	if(m_pCircleEndPEdit)   m_pCircleEndPEdit->SetWindowText(strCoord);	
	if(m_pCircleRadiusEdit) m_pCircleRadiusEdit->SetWindowText(_T("0.0"));	
}

void CDrawEditBtn::StartCurrUnitPolyline(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	m_StartPoint = CurrP;	
	m_StartPointR[0] = dCoord[0];
	m_StartPointR[1] = dCoord[1];
	m_CurrPolylineRD.Init();
	T_POINT DgnPointD;  
	DgnPointD.x = dCoord[0];
	DgnPointD.y = dCoord[1];
	m_CurrPolylineRD.arPoint.Add(DgnPointD);

	m_nButtonCount = 1;

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	if(m_pPolylineStartPEdit) m_pPolylineStartPEdit->SetWindowText(strCoord);
	if(m_pPolylinePassPEdit ) m_pPolylinePassPEdit->SetWindowText(strCoord);
	if(m_pPolylineEndPEdit  ) m_pPolylineEndPEdit->SetWindowText(strCoord);
}
void CDrawEditBtn::StartCurrUnitRect(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	m_StartPoint = CurrP;
	m_StartPointR[0] = dCoord[0];
	m_StartPointR[1] = dCoord[1];
	m_nButtonCount = 1;

	CString strCoord = GetCoordText(m_StartPointR[0], m_StartPointR[1]);
	if(m_pRectCorner1PEdit) m_pRectCorner1PEdit->SetWindowText(strCoord);
	if(m_pRectCorner2PEdit) m_pRectCorner2PEdit->SetWindowText(strCoord);	
}

// 작업중 작업 /////////////////////////////////

void CDrawEditBtn::PassCurrUnitSelect(CPoint CurrP)
{
	//작업없음
}

void CDrawEditBtn::PassCurrUnitPoint(CPoint CurrP)
{	
	//작업없음
}

void CDrawEditBtn::PassCurrUnitLine(CPoint CurrP)
{	
	//작업없음
}

void CDrawEditBtn::PassCurrUnitArc(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	m_InterPoint = CurrP;
	m_InterPointR[0] = dCoord[0];
	m_InterPointR[1] = dCoord[1];
	m_nButtonCount = 2;

	CString strCoord = GetCoordText(m_InterPointR[0], m_InterPointR[1]);
	CString strValue; 
	strValue.Format(_T("%g"), 0.0);

	if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);
	if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);	
	if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strValue);
	if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strValue);
}

void CDrawEditBtn::PassCurrUnitArcB(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	m_InterPoint = CurrP;
	m_InterPointR[0] = dCoord[0];
	m_InterPointR[1] = dCoord[1];
	m_nButtonCount = 2;

	CString strCoord = GetCoordText(m_InterPointR[0], m_InterPointR[1]);	
	CString strValue;  
	strValue.Format(_T("%g"), 0.0);
	if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);
	if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);	
	if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strValue);
	if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strValue);
}

void CDrawEditBtn::PassCurrUnitCircle(CPoint CurrP)
{	
	//작업없음	
}

void CDrawEditBtn::PassCurrUnitPolyline(CPoint CurrP)
{	
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	if(m_nButtonCount >= 2 && m_CurrPolylineRD.arPoint.GetSize() >= 2)
	{
		// 시작점과 동일한지 여부 검토
		if(max(abs(m_StartPoint.x - CurrP.x), abs(m_StartPoint.y - CurrP.y)) <= (long)m_DrawCtrl.GetSnapDis())
		{
			CompleteCurrUnitPolyline(m_StartPoint); 
			return;
		}
		if(m_CurrPolylineRD.arPoint[0].x == dCoord[0] && m_CurrPolylineRD.arPoint[0].y == dCoord[1]) 
		{ 
			CompleteCurrUnitPolyline(m_StartPoint); 
			return; 
		}
	}

	m_InterPoint = CurrP;
	m_InterPointR[0] = dCoord[0];
	m_InterPointR[1] = dCoord[1];
	T_POINT DgnPointD; 
	DgnPointD.x = dCoord[0];  DgnPointD.y = dCoord[1];

	m_CurrPolylineRD.arPoint.Add(DgnPointD);
	m_nButtonCount++;

	CString strCoord = GetCoordText(m_InterPointR[0], m_InterPointR[1]);	
	if(m_pPolylinePassPEdit) m_pPolylinePassPEdit->SetWindowText(strCoord);
	if(m_pPolylineEndPEdit ) m_pPolylineEndPEdit->SetWindowText(strCoord);
}

void CDrawEditBtn::PassCurrUnitRect(CPoint CurrP)
{	
	//작업없음	
}

// 작업중 /////////////////////////////////
void CDrawEditBtn::InProgressCurrUnitSelect(CPoint CurrP)
{
	CPoint CurrP_Snap;
	double dCoord[2], dCoord_Snap[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	CurrP_Snap = CurrP;	
	dCoord_Snap[0] = dCoord[0]; 
	dCoord_Snap[1] = dCoord[1];
	ChangeSnapPoint(CurrP_Snap, dCoord_Snap);// Snap이 적용될 경우 좌표변환

	CurrRemoveDrawData();
	SelectRemoveDrawData();

	if(m_bUseMove)		MakeCurrUnitSelectMove(m_StartPoint, CurrP_Snap);
	else                MakeSelectRectGroup(m_StartPoint, CurrP);

	CurrConvertDrawData(FALSE);
	RedrawWindow();
}

void CDrawEditBtn::InProgressCurrUnitPoint(CPoint CurrP)
{	
	//작업없음	
}

void CDrawEditBtn::InProgressCurrUnitLine(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dEndP[2];	
	dStartP[0] = m_StartPointR[0];  
	dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord[0]; 
	dEndP[1]   = dCoord[1];

	CString strCoord = GetCoordText(dEndP[0], dEndP[1]);	
	if(::IsWindow(m_pLineEndPEdit->GetSafeHwnd())) 
	{
		m_pLineEndPEdit->SetWindowText(strCoord);	
	}

	CurrRemoveDrawData();
	MakeCurrUnitLine(dStartP, dEndP);		
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::InProgressCurrUnitArc(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dPassP[2], dEndP[2];

	if(m_nButtonCount == 0) return;

	if(m_nButtonCount == 1)		
	{
		dStartP[0] = m_StartPointR[0];  
		dStartP[1] = m_StartPointR[1];
		dPassP[0]  = dCoord[0]; 
		dPassP[1]  = dCoord[1];
		dEndP[0]   = dPassP[0];
		dEndP[1]   = dPassP[1];

		CString strCoord = GetCoordText(dEndP[0], dEndP[1]);	
		CString strValue;  
		strValue.Format(_T("%g"), 0.0);
		if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);
		if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);		
		if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strValue);
		if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strValue);
	}
	else if(m_nButtonCount == 2)		
	{
		dStartP[0] = m_StartPointR[0]; 
		dStartP[1] = m_StartPointR[1];
		dPassP[0]  = m_InterPointR[0]; 
		dPassP[1]  = m_InterPointR[1];
		dEndP[0]   = dCoord[0];
		dEndP[1]   = dCoord[1];

		double dCenterP[2];
		double dRadius, dStartAngle, dSweepAngle, dBulge;

		int nChk = CDrawFunc::mathArcForm3Point(dStartP, dPassP, dEndP, dCenterP, dRadius, dStartAngle, dSweepAngle, dBulge);
		if(nChk != 1)	dRadius = dBulge = 0.0;

		CString strCoord  = GetCoordText(dEndP[0], dEndP[1]);
		CString strRadius = GetValueText(dRadius);
		CString strBulge  = GetValueText(dBulge);
		BOOL    bInside   = (fabs(dSweepAngle) <= 180.0);

		if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);		
		if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strBulge);
		if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strRadius);
		if(m_pArcInsideCheck) m_pArcInsideCheck->SetCheck(bInside);
	}
	else return;

	CurrRemoveDrawData();
	MakeCurrUnitArc(dStartP, dPassP, dEndP);	
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::InProgressCurrUnitArcB(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dPassP[2], dEndP[2];
	if(m_nButtonCount == 0) return;
	if(m_nButtonCount == 1)		
	{
		dStartP[0] = m_StartPointR[0]; 
		dStartP[1] = m_StartPointR[1];
		dPassP[0]  = dCoord[0]; 
		dPassP[1]  = dCoord[1];
		dEndP[0]   = dPassP[0];
		dEndP[1]   = dPassP[1];

		CString strCoord = GetCoordText(dEndP[0], dEndP[1]);	
		CString strValue; 
		strValue.Format(_T("%g"), 0.0);
		if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);
		if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);		
		if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strValue);
		if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strValue);
	}
	else if(m_nButtonCount == 2)		
	{
		dStartP[0] = m_StartPointR[0]; 
		dStartP[1] = m_StartPointR[1];
		dEndP[0]   = m_InterPointR[0]; 
		dEndP[1]   = m_InterPointR[1];
		dPassP[0]  = dCoord[0]; 
		dPassP[1]  = dCoord[1];

		double dCenterP[2];
		double dRadius, dStartAngle, dSweepAngle, dBulge;
		int nChk = CDrawFunc::mathArcForm3Point(dStartP, dPassP, dEndP, dCenterP, dRadius, dStartAngle, dSweepAngle, dBulge);
		if(nChk != 1)	dRadius = dBulge = 0.0;

		CString strCoord  = GetCoordText(dPassP[0], dPassP[1]);
		CString strRadius = GetValueText(dRadius);
		CString strBulge  = GetValueText(dBulge);
		BOOL    bInside   = (fabs(dSweepAngle) <= 180.0);

		if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);		
		if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strBulge);
		if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strRadius);
		if(m_pArcInsideCheck) m_pArcInsideCheck->SetCheck(bInside);
	}
	else return;

	CurrRemoveDrawData();
	MakeCurrUnitArcB(dStartP, dEndP, dPassP);	
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::InProgressCurrUnitCircle(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0]; 
	dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord[0];  
	dEndP[1]   = dCoord[1];

	CString strCoord = GetCoordText(dEndP[0], dEndP[1]);	

	if(m_pCircleEndPEdit)  
	{
		m_pCircleEndPEdit->SetWindowText(strCoord);	
	}
	strCoord.Format(_T("%g"), sqrt((dEndP[0]-dStartP[0])*(dEndP[0]-dStartP[0]) + (dEndP[1]-dStartP[1])*(dEndP[1]-dStartP[1])));

	if(m_pCircleRadiusEdit) 
	{
		m_pCircleRadiusEdit->SetWindowText(strCoord);	
	}

	CurrRemoveDrawData();
	MakeCurrUnitCircle(dStartP, dEndP);	
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::InProgressCurrUnitPolyline(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);	
	if(m_pPolylineEndPEdit)  
	{
		m_pPolylineEndPEdit->SetWindowText(strCoord);	
	}

	CurrRemoveDrawData();
	MakeCurrUnitPolyline(m_CurrPolylineRD, dCoord);		
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}

void CDrawEditBtn::InProgressCurrUnitRect(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	// 확인필요
	double dStartP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0]; 
	dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord[0];  
	dEndP[1]   = dCoord[1];

	CurrRemoveDrawData();
	MakeCurrUnitRect(dStartP, dEndP);		
	CurrConvertDrawData(TRUE);
	RedrawWindow();
}


// 작업완료 /////////////////////////////////

void CDrawEditBtn::RunUpdateCompleteObjectData(UINT nFlags, double dVal1, double dVal2)
{	
}

void CDrawEditBtn::CompleteCurrUnitSelect(CPoint CurrP)
{
	CPoint CurrP_Snap;
	double dCoord[2], dCoord_Snap[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	CurrP_Snap = CurrP;	
	dCoord_Snap[0] = dCoord[0]; 
	dCoord_Snap[1] = dCoord[1];
	ChangeSnapPoint(CurrP_Snap, dCoord_Snap);// Snap이 적용될 경우 좌표변환

	
	double dStartP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0]; 
	dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord_Snap[0]; 
	dEndP[1]   = dCoord_Snap[1];

	if(dEndP[0] - dStartP[0] == 0.0 && dEndP[1] - dStartP[1] == 0.0)  // 하나만 찍은 경우
	{		
		UINT SelectKey;
		nrUINT naConnectedKey;
		if(m_DrawCtrl.GetSelectDrawUnitKey(CurrP_Snap, SelectKey))
		{
			nrUINT arSelectKey;
			arSelectKey.SetSize(1);
			arSelectKey[0] = SelectKey;
			//GetConnectedKey(arSelectKey, naConnectedKey);  // 연결되어 있는 외각선 혹은 Offset을 받아 옴	
			naConnectedKey.SetSize(1);
			naConnectedKey[0] = SelectKey;


			if(m_nShiftButtonState == 1)
			{
				if(m_bReSelectDrawUnit)
				{
					if(arSelectKey.GetSize() > naConnectedKey.GetSize())
					{
						// 선이 아닌경우는 여기로 들어온다.
						m_DrawCtrl.ReSelectedKey(SelectKey);
					}
					else
					{
						for(int i = 0 ; i < naConnectedKey.GetSize() ; i++)
						{
							m_DrawCtrl.ReSelectedKey(naConnectedKey[i]);
						}
					}
				}				
				RunUpdateCompleteObjectData(EN_DRAW_UPDATESELECT_SELECT, 0.0, 0.0);			
			}
			else 
			{
				if(m_bUseMove)
				{
					if(naConnectedKey.GetSize() > 0)
					{
						m_DrawCtrl.MoveDrawUnitR(naConnectedKey, dEndP[0] - dStartP[0], dEndP[1] - dStartP[1]);				
						OverlapChecking();
						RunUpdateCompleteObjectData(EN_DRAW_UPDATESELECT_MOVE, dEndP[0] - dStartP[0], dEndP[1] - dStartP[1]);
					}
					else
					{
						RunUpdateCompleteObjectData(EN_DRAW_UPDATESELECT_SELECT, 0.0, 0.0);
					}
				}
				else
				{  // 이미 선택된 것을 선택하면 그대로 있고, 선택된적 없는 녀석을 선택하면 선택을 해제 한 다음 그녀석만 선택한다. (DblClick했을때 선택 정보를 날리지 않기 위해서 이렇게 처리 함)										
					nrUINT naSelKey;
					m_DrawCtrl.GetSelectedKeyList(naSelKey);
					BOOL bOverlap = FALSE;
					for(int i = 0 ; i < naSelKey.GetSize() ; i++)
					{
						for (int j = 0 ; j < naConnectedKey.GetSize() ; j++)
						{
							if( naSelKey[i] == naConnectedKey[j] )
							{
								bOverlap = TRUE;
								break;
							}
						}
						if(bOverlap)
							break;

						if(arSelectKey.GetSize() > naConnectedKey.GetSize())
						{
							// 선이 아닌경우는 여기로 들어온다.
							for (int j = 0 ; j < arSelectKey.GetSize() ; j++)
							{
								if( naSelKey[i] == arSelectKey[j] )
								{
									bOverlap = TRUE;
									break;
								}
							}
							if(bOverlap)
								break;
						}
					}
					
					if(!bOverlap)
					{
						m_DrawCtrl.RemoveAllSelectedKey();
						if(arSelectKey.GetSize() > naConnectedKey.GetSize())
						{
							// 선이 아닌경우는 여기로 들어온다.
							m_DrawCtrl.AddSelectedKey(SelectKey);
						}
						else
						{
							m_DrawCtrl.AddSelectedKey(naConnectedKey);
						}
					}
					
					RunUpdateCompleteObjectData(EN_DRAW_UPDATESELECT_SELECT, 0.0, 0.0);
				}
			}
		}
		else
		{
			m_DrawCtrl.RemoveAllSelectedKey();
			RunUpdateCompleteObjectData(EN_DRAW_UPDATESELECT_SELECT, 0.0, 0.0);
		}
	}
	else
	{
		nrUINT arSelectKey;

		m_DrawCtrl.GetIncludeKeyList(m_StartPoint, CurrP, arSelectKey, TRUE);

		// Rebar가 선택 되었다면 우선순위가 Rebar가 높음..!
		nrUINT aRebarKeyList;
		for(int i = 0 ; i < arSelectKey.GetSize() ; i++)
		{
			UINT uiLinkKey = m_DrawCtrl.GetDrawClass(arSelectKey[i])->m_LinkKey;			
			if(uiLinkKey > 0)
			{
				aRebarKeyList.Add(arSelectKey[i]);
			}
		}

		nrUINT naConnectedKey;
		if(aRebarKeyList.GetSize() > 0)
		{			
			arSelectKey.Copy(aRebarKeyList);
		}
		else
		{
			GetConnectedKey(arSelectKey, naConnectedKey);  // 연결되어 있는 외각선 혹은 Offset을 받아 옴
		}

		if(arSelectKey.GetSize() > naConnectedKey.GetSize())
		{
			// 선이 아닌경우는 여기로 들어온다.
			if(m_nShiftButtonState == 0)
			{
				m_DrawCtrl.RemoveAllSelectedKey();
				m_DrawCtrl.AddSelectedKey(arSelectKey);
			}
			else 
			{		
				m_DrawCtrl.AddSelectedKey(arSelectKey);
			}			
		}
		else
		{
			if(m_nShiftButtonState == 0)
			{
				m_DrawCtrl.RemoveAllSelectedKey();
				m_DrawCtrl.AddSelectedKey(naConnectedKey);
			}
			else 
			{		
				m_DrawCtrl.AddSelectedKey(naConnectedKey);
			}
		}
		
		RunUpdateCompleteObjectData(EN_DRAW_UPDATESELECT_SELECT, 0.0, 0.0);
	}

	// Rebar와 선이 같이 선택되면 무조건 Rebar만 남긴다.
	nrUINT naSelKey;
	nrUINT naSelRebarKeyList;
	m_DrawCtrl.GetSelectedKeyList(naSelKey);
	BOOL bLineExist = FALSE;
	for(int i = 0 ; i < naSelKey.GetSize() ; i++)
	{
		UINT uiLinkKey = m_DrawCtrl.GetDrawClass(naSelKey[i])->m_LinkKey;			
		if(uiLinkKey > 0)
		{
			naSelRebarKeyList.Add(naSelKey[i]);
		}
		else
		{
			bLineExist = TRUE;
		}
	}

	if(naSelRebarKeyList.GetSize() > 0 && bLineExist)  // Rebar와 선이 같이 있는 경우
	{
		m_DrawCtrl.SetSelectedKeyList(naSelRebarKeyList);
	}	

	// CurrUnit제거
	m_nButtonCount = 0;
	CurrRemoveDrawData();
	SelectRemoveDrawData();
	RedrawWindow();	
}

void CDrawEditBtn::GetConnectedKey(nrUINT &arSelectKey, nrUINT &arConnectedKey)
{
	std::set<UINT> stSetConnKey;

	for(int i = 0 ; i < arSelectKey.GetSize() ; i++)
	{
		nrUINT arConnectedKey;
		//m_DrawCtrl.GetConnectedLineKey(arSelectKey[i], arConnectedKey, IOptionDB::Instance()->GetData()->m_RcDgnRbrGnrl.dOffsetAngToMergeLine);
		m_DrawCtrl.GetConnectedLineKey(arSelectKey[i], arConnectedKey, D_MERGE_LINE_ANG);
		for(int j = 0 ; j < arConnectedKey.GetSize() ; j++)
		{
			stSetConnKey.insert(arConnectedKey[j]);
		}		
	}

	arConnectedKey.RemoveAll();
	arConnectedKey.SetSize((int)stSetConnKey.size());
	int nCnt = 0;
	for(std::set<UINT>::iterator stSetItr = stSetConnKey.begin() ; stSetItr != stSetConnKey.end() ; stSetItr++ )
	{
		arConnectedKey[nCnt] = *stSetItr;
		nCnt++;
	}
}


void CDrawEditBtn::CompleteCurrUnitPoint(CPoint CurrP)
{	
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	CString strCoord = GetCoordText(dCoord[0], dCoord[1]);
	if(m_pPointEdit) m_pPointEdit->SetWindowText(strCoord);	
	
	// 만들어진 CurrUnit를 m_DrawCtrl에 올림
	MakeCurrUnitPoint(dCoord);
	if(m_CurrDrawClass_Point.m_DataR.List.GetSize() > 0)
	{
		m_DrawCtrl.AddDrawUnit(m_DefaultDrawFormatKey, m_CurrDrawClass_Point.m_DataR, TRUE, 0);		
		OverlapChecking();
	}

	// CurrUnit제거
	m_nButtonCount = 0;
	CurrRemoveDrawData();
	RedrawWindow();

	RunUpdateCompleteObjectData(EN_DRAW_UPDATECOMPLETE_POINT, dCoord[0], dCoord[1]);
}

void CDrawEditBtn::CompleteCurrUnitLine(CPoint CurrP)
{	
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0]; 
	dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord[0];
	dEndP[1]   = dCoord[1];

	CString strCoord = GetCoordText(dEndP[0], dEndP[1]);	
	if(m_pLineEndPEdit) m_pLineEndPEdit->SetWindowText(strCoord);	

	// 만들어진 CurrUnit를 m_DrawCtrl에 올림
	MakeCurrUnitLine(dStartP, dEndP);	
	if(m_CurrDrawClass_Line.m_DataR.List.GetSize() > 0)
	{
		m_DrawCtrl.AddDrawUnit(m_DefaultDrawFormatKey, m_CurrDrawClass_Line.m_DataR, TRUE, 0);			
		OverlapChecking();
	}

	// CurrUnit제거
	m_nButtonCount = 0;
	CurrRemoveDrawData();
	RedrawWindow();

	RunUpdateCompleteObjectData(EN_DRAW_UPDATECOMPLETE_LINE, dCoord[0], dCoord[1]);
}

void CDrawEditBtn::CompleteCurrUnitArc(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dPassP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0]; 
	dStartP[1] = m_StartPointR[1];
	dPassP[0]  = m_InterPointR[0]; 
	dPassP[1]  = m_InterPointR[1];
	dEndP[0]   = dCoord[0]; 
	dEndP[1]   = dCoord[1];

	double dCenterP[2];
	double dRadius, dStartAngle, dSweepAngle, dBulge;

	int nChk = CDrawFunc::mathArcForm3Point(dStartP, dPassP, dEndP, dCenterP, dRadius, dStartAngle, dSweepAngle, dBulge);
	if(nChk != 1) dRadius = dBulge = 0.0;

	CString strCoord  = GetCoordText(dEndP[0], dEndP[1]);
	CString strRadius = GetValueText(dRadius);
	CString strBulge  = GetValueText(dBulge);
	BOOL    bInside   = (fabs(dSweepAngle) <= 180.0);

	if(m_pArcEndPEdit)    m_pArcEndPEdit->SetWindowText(strCoord);		
	if(m_pArcBulgeEdit)   m_pArcBulgeEdit->SetWindowText(strBulge);
	if(m_pArcRadiusEdit)  m_pArcRadiusEdit->SetWindowText(strRadius);
	if(m_pArcInsideCheck) m_pArcInsideCheck->SetCheck(bInside);

	// 만들어진 CurrUnit를 m_DrawCtrl에 올림
	MakeCurrUnitArc(dStartP, dPassP, dEndP);
	if(m_CurrDrawClass_Arc.m_DataR.List.GetSize() > 0)
	{
		m_DrawCtrl.AddDrawUnit(m_DefaultDrawFormatKey, m_CurrDrawClass_Arc.m_DataR, TRUE, 0);			
		OverlapChecking();
	}

	// CurrUnit제거
	m_nButtonCount = 0;
	CurrRemoveDrawData();
	RedrawWindow();

	RunUpdateCompleteObjectData(EN_DRAW_UPDATECOMPLETE_ARC, dCoord[0], dCoord[1]);
}

void CDrawEditBtn::CompleteCurrUnitArcB(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dPassP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0]; 
	dStartP[1] = m_StartPointR[1];
	dEndP[0]   = m_InterPointR[0]; 
	dEndP[1]   = m_InterPointR[1];
	dPassP[0]  = dCoord[0]; 
	dPassP[1]  = dCoord[1];

	double dCenterP[2];
	double dRadius, dStartAngle, dSweepAngle, dBulge;
	int nChk = CDrawFunc::mathArcForm3Point(dStartP, dPassP, dEndP, dCenterP, dRadius, dStartAngle, dSweepAngle, dBulge);
	if(nChk != 1) dRadius = dBulge = 0.0;

	CString strCoord  = GetCoordText(dPassP[0], dPassP[1]);
	CString strRadius = GetValueText(dRadius);
	CString strBulge  = GetValueText(dBulge);
	BOOL    bInside   = (fabs(dSweepAngle) <= 180.0);

	if(m_pArcPassPEdit)  m_pArcPassPEdit->SetWindowText(strCoord);		
	if(m_pArcBulgeEdit)  m_pArcBulgeEdit->SetWindowText(strBulge);
	if(m_pArcRadiusEdit) m_pArcRadiusEdit->SetWindowText(strRadius);
	if(m_pArcInsideCheck) m_pArcInsideCheck->SetCheck(bInside);

	// 만들어진 CurrUnit를 m_DrawCtrl에 올림
	MakeCurrUnitArcB(dStartP, dEndP, dPassP);
	if(m_CurrDrawClass_Arc.m_DataR.List.GetSize() > 0)
	{
		m_DrawCtrl.AddDrawUnit(m_DefaultDrawFormatKey, m_CurrDrawClass_Arc.m_DataR, TRUE, 0);	
		OverlapChecking();
	}

	// CurrUnit제거
	m_nButtonCount = 0;
	CurrRemoveDrawData();
	RedrawWindow();

	RunUpdateCompleteObjectData(EN_DRAW_UPDATECOMPLETE_ARC_B, dCoord[0], dCoord[1]);
}

void CDrawEditBtn::CompleteCurrUnitCircle(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0]; 
	dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord[0]; 
	dEndP[1]   = dCoord[1];

	CString strCoord = GetCoordText(dEndP[0], dEndP[1]);	
	if(m_pCircleEndPEdit) 
	{
		m_pCircleEndPEdit->SetWindowText(strCoord);	
	}
	
	strCoord.Format(_T("%g"), sqrt((dEndP[0]-dStartP[0])*(dEndP[0]-dStartP[0]) + (dEndP[1]-dStartP[1])*(dEndP[1]-dStartP[1])));
	if(m_pCircleRadiusEdit) 
	{
		m_pCircleRadiusEdit->SetWindowText(strCoord);	
	}

	// 만들어진 CurrUnit를 m_DrawCtrl에 올림
	MakeCurrUnitCircle(dStartP, dEndP);	
	if(m_CurrDrawClass_Circle.m_DataR.List.GetSize() > 0)
	{
		m_DrawCtrl.AddDrawUnit(m_DefaultDrawFormatKey, m_CurrDrawClass_Circle.m_DataR, TRUE, 0);			
		OverlapChecking();
	}

	// CurrUnit제거
	m_nButtonCount = 0;
	CurrRemoveDrawData();
	RedrawWindow();

	RunUpdateCompleteObjectData(EN_DRAW_UPDATECOMPLETE_CIRCLE, dCoord[0], dCoord[1]);
}

void CDrawEditBtn::CompleteCurrUnitRect(CPoint CurrP)
{
	double dCoord[2];
	GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
	ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0]; 
	dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord[0]; 
	dEndP[1]   = dCoord[1];

	ASSERT(0);
	// 작업필요
//   CString strCoord = GetCoordText(dEndP[0], dEndP[1]);	
//   if(m_pLineEndPEdit) m_pLineEndPEdit->SetWindowText(strCoord);	
// 
//   // 만들어진 CurrUnit를 m_DrawCtrl에 올림
//   MakeCurrUnitLine(dStartP, dEndP);	
//   if(m_CurrDrawClass_Line.m_DataR.List.GetSize() > 0)
//   {
//     m_DrawCtrl.AddDrawUnit(m_DefaultDrawFormatKey, m_CurrDrawClass_Line.m_DataR, TRUE, 0);			
//     OverlapChecking();
//   }
// 
//   // CurrUnit제거
//   m_nButtonCount = 0;
//   CurrRemoveDrawData();
//   RedrawWindow();
// 
//   RunUpdateCompleteObjectData(EN_DRAW_UPDATECOMPLETE_LINE, dCoord[0], dCoord[1]);
}

void CDrawEditBtn::CompleteCurrUnitPolyline(CPoint CurrP, BOOL bIsCurrP)
{		
	double dCoord[2] = {0.0, 0.0};

	if(bIsCurrP)
	{	
		GetGenuineCoord(CurrP, dCoord[0], dCoord[1]);
		ChangeSnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

		CString strCoord = GetCoordText(dCoord[0], dCoord[1]);	

		if(m_pPolylineEndPEdit) 
		{
			m_pPolylineEndPEdit->SetWindowText(strCoord);	
		}

		MakeCurrUnitPolyline(m_CurrPolylineRD, dCoord, TRUE);
	}
	else 
	{
		m_CurrDrawClass_Polyline.m_Data.Init();
		m_CurrDrawClass_Polyline.m_DataR.Init();	

		int nSize = m_CurrPolylineRD.arPoint.GetSize();		
		if(nSize >= 2)
		{
			CString strCoord = GetCoordText(m_CurrPolylineRD.arPoint[nSize-2].x, m_CurrPolylineRD.arPoint[nSize-2].y);			
			
			if(m_pPolylinePassPEdit)   
			{
				m_pPolylinePassPEdit->SetWindowText(strCoord);	
			}
			
			strCoord = GetCoordText(m_CurrPolylineRD.arPoint[nSize-1].x, m_CurrPolylineRD.arPoint[nSize-1].y);			
			if(m_pPolylineEndPEdit)  
			{
				m_pPolylineEndPEdit->SetWindowText(strCoord);	
			}

			MakeCurrUnitPolyline(m_CurrPolylineRD, dCoord, FALSE);
		}	
	}

	if(m_CurrDrawClass_Polyline.m_DataR.List.GetSize() > 0)
	{
		m_DrawCtrl.AddDrawUnit(m_DefaultDrawFormatKey, m_CurrDrawClass_Polyline.m_DataR, TRUE, 0);			
		OverlapChecking();
	}

	// CurrUnit제거
	m_nButtonCount = 0;
	m_CurrPolylineRD.Init();
	CurrRemoveDrawData();
	RedrawWindow();

	RunUpdateCompleteObjectData(EN_DRAW_UPDATECOMPLETE_POLYLINE, dCoord[0], dCoord[1]);
}

//////////////////////////////////////////////////////////////////////////

void CDrawEditBtn::MakeCurrUnitPoint(double dStartP[2])
{
	m_CurrDrawClass_Point.m_DataR.List.SetSize(1);
	T_POINT PointR;
	PointR.x = dStartP[0]; 
	PointR.y = dStartP[1];
	m_CurrDrawClass_Point.m_DataR.List[0] = PointR;
}

void CDrawEditBtn::MakeCurrUnitLine(double dStartP[2], double dEndP[2])
{
	if(fabs(dStartP[0] - dEndP[0]) < m_dZero && fabs(dStartP[1] - dEndP[1]) < m_dZero) return;
	m_CurrDrawClass_Line.m_DataR.List.SetSize(1);
	T_LINE_R LineR;
	LineR.P1.x = dStartP[0]; 
	LineR.P1.y = dStartP[1];
	LineR.P2.x = dEndP[0];   
	LineR.P2.y = dEndP[1];
	m_CurrDrawClass_Line.m_DataR.List[0] = LineR;
}

void CDrawEditBtn::MakeCurrUnitLine(int no, double dStartP[2], double dEndP[2])
{
	if(fabs(dStartP[0] - dEndP[0]) < m_dZero && fabs(dStartP[1] - dEndP[1]) < m_dZero) return;
	if(m_CurrDrawClass_Line.m_DataR.List.GetSize() < no + 1)
		m_CurrDrawClass_Line.m_DataR.List.SetSize(no + 1);

	T_LINE_R LineR;
	LineR.P1.x = dStartP[0]; 
	LineR.P1.y = dStartP[1];
	LineR.P2.x = dEndP[0];   
	LineR.P2.y = dEndP[1];
	m_CurrDrawClass_Line.m_DataR.List[no] = LineR;
}

void CDrawEditBtn::MakeCurrUnitArc(double dStartP[2], double dPassP[2], double dEndP[2])
{	
	double CenterP[2];
	double dRadius, dStartAngle, dSweepAngle;

	int nChk = CMathFunc::mathArcForm3Point(dStartP, dPassP, dEndP, CenterP, dRadius, dStartAngle, dSweepAngle);

	if(nChk == 0)
	{
		if(fabs(dStartP[0] - dEndP[0]) < m_dZero && fabs(dStartP[1] - dEndP[1]) < m_dZero) return;
		m_CurrDrawClass_Line.m_DataR.List.SetSize(1);
		T_LINE_R LineR;
		LineR.P1.x = dStartP[0];  
		LineR.P1.y = dStartP[1];
		LineR.P2.x = dEndP[0];   
		LineR.P2.y = dEndP[1];
		m_CurrDrawClass_Line.m_DataR.List[0] = LineR;		
	}
	else if(nChk == 1)
	{
		m_CurrDrawClass_Arc.m_DataR.List.SetSize(1);
		T_ARC_R ArcR;
		ArcR.CenterP.x = CenterP[0]; 
		ArcR.CenterP.y = CenterP[1];
		ArcR.Radius = dRadius;
		ArcR.StartAngle = dStartAngle;
		ArcR.SweepAngle = dSweepAngle;
		m_CurrDrawClass_Arc.m_DataR.List[0] = ArcR;		
	}

	m_CurrDrawClass_Point.m_DataR.List.SetSize(3);
	T_POINT PointR;
	PointR.x = dStartP[0]; 
	PointR.y = dStartP[1];
	m_CurrDrawClass_Point.m_DataR.List[0] = PointR;
	PointR.x = dPassP[0];  
	PointR.y = dPassP[1];
	m_CurrDrawClass_Point.m_DataR.List[1] = PointR;
	PointR.x = dEndP[0];   
	PointR.y = dEndP[1];
	m_CurrDrawClass_Point.m_DataR.List[2] = PointR;
}

void CDrawEditBtn::MakeCurrUnitArcB(double dStartP[2], double dEndP[2], double dBulgeP[2])
{
	MakeCurrUnitArc(dStartP, dBulgeP, dEndP);
}

void CDrawEditBtn::MakeCurrUnitCircle(double dStartP[2], double dEndP[2])
{
	double dR = sqrt((dStartP[0] - dEndP[0]) * (dStartP[0] - dEndP[0]) + (dStartP[1] - dEndP[1]) * (dStartP[1] - dEndP[1]));
	//if(fabs(dR < m_dZero))	return;
	m_CurrDrawClass_Circle.m_DataR.List.SetSize(1);
	T_CIRCLE_R CircleR;
	CircleR.CenterP.x = dStartP[0];  
	CircleR.CenterP.y = dStartP[1];
	CircleR.Radius = dR;
	m_CurrDrawClass_Circle.m_DataR.List[0] = CircleR;
}

void CDrawEditBtn::MakeCurrUnitPolyline(T_POLYLINE_R& PolylineRD, double dEndP[2], BOOL bIsEndP)
{
	int nSize = (int)PolylineRD.arPoint.GetSize();
	if(nSize <= 0) return;

	m_CurrDrawClass_Polyline.m_Data.Init();
	m_CurrDrawClass_Polyline.m_DataR.Init();	
	m_CurrDrawClass_Polyline.m_DataR.List.SetSize(1);	
	if(bIsEndP)
	{
		T_POLYLINE_R PolylineRD_Sum;		
		PolylineRD_Sum.arPoint.SetSize(nSize+1);

		for(int i = 0; i < nSize ; i++) 
		{
			PolylineRD_Sum.arPoint[i] = PolylineRD.arPoint[i];
		}
		PolylineRD_Sum.arPoint[nSize].Init();
		PolylineRD_Sum.arPoint[nSize].x = dEndP[0]; 
		PolylineRD_Sum.arPoint[nSize].y = dEndP[1];
		m_CurrDrawClass_Polyline.m_DataR.List[0] = PolylineRD_Sum;
	}
	else 
	{
		m_CurrDrawClass_Polyline.m_DataR.List[0] = PolylineRD;
	}
}
void CDrawEditBtn::MakeCurrUnitRect(double dStartP[2], double dEndP[2])
{
	ASSERT(0);
	// 작업필요
//   double dR = sqrt((dStartP[0] - dEndP[0]) * (dStartP[0] - dEndP[0]) + (dStartP[1] - dEndP[1]) * (dStartP[1] - dEndP[1]));
//   //if(fabs(dR < m_dZero))	return;
//   m_CurrDrawClass_Circle.m_DataR.List.SetSize(1);
//   T_CIRCLE_R CircleR;
//   CircleR.CenterP.x = dStartP[0];  
//   CircleR.CenterP.y = dStartP[1];
//   CircleR.Radius = dR;
//   m_CurrDrawClass_Circle.m_DataR.List[0] = CircleR;
}

void CDrawEditBtn::MakeSnapGroup(CPoint snapPoint)
{
	m_SnapDrawGroup.List.SetSize(1);
	T_LINE snapRect;
	snapRect.P1.x = snapPoint.x - 3;
	snapRect.P1.y = snapPoint.y - 3;
	snapRect.P2.x = snapPoint.x + 3;
	snapRect.P2.y = snapPoint.y + 3;
	m_SnapDrawGroup.List[0] = snapRect;
}

void CDrawEditBtn::MakeHighLightGroup(nrLine &aHighlightLine)
{
	m_HighlightDrawGroup.List.SetSize(aHighlightLine.GetSize());

	for(int i = 0 ; i < m_HighlightDrawGroup.List.GetSize() ; i++)
	{
		m_HighlightDrawGroup.List[i] = aHighlightLine[i];
	}
}

void CDrawEditBtn::MakeSelectRectGroup(CPoint startPoint, CPoint endPoint)
{
	m_SelectAreaDrawGroup.List.SetSize(1);
	T_LINE selectRect;
	selectRect.P1 = startPoint;
	selectRect.P2 = endPoint;
	m_SelectAreaDrawGroup.List[0] = selectRect;
}

void CDrawEditBtn::MakeCurrUnitSelectMove(CPoint startPoint, CPoint endPoint)
{
	nrUINT arSelectKey;
	int nSize = m_DrawCtrl.GetSelectedKeyList(arSelectKey, TRUE);

	if(nSize == 0 || (startPoint.x == endPoint.x && startPoint.y == endPoint.y)) return;

	CDrawBase* pDrawClass = NULL;
	int i, j, nClassType;
	int nSizeR_Point = 0,			 nSize_Point = 0;
	int nSizeR_Line = 0,			 nSize_Line = 0;
	int nSizeR_ArrowLine = 0,		 nSize_ArrowLine = 0;
	int nSizeR_Circle = 0,			 nSize_Circle = 0;
	int nSizeR_Donut = 0,			 nSize_Donut = 0;
	int nSizeR_Ellipse = 0,		 nSize_Ellipse = 0;
	int nSizeR_Rect = 0,			 nSize_Rect = 0;
	int nSizeR_Arc = 0,			 nSize_Arc = 0;
	int nSizeR_Polyline = 0,		 nSize_Polyline = 0;
	int nSizeR_Polygon = 0,		 nSize_Polygon = 0;
	int nSizeR_ArrowPolyline = 0, nSize_ArrowPolyline = 0;
	int nSizeR_Text = 0,			 nSize_Text = 0;
	int nSizeR_Dimension = 0,		 nSize_Dimension = 0;
	int nSizeR_DimAngle = 0,		 nSize_DimAngle = 0;
	int nSizeR_DimLeaderline = 0, nSize_DimLeaderline = 0;

	for(i = 0; i < nSize ; i++)
	{
		pDrawClass = m_DrawCtrl.GetDrawClass(arSelectKey[i]);
		if(pDrawClass != NULL)
		{
			nClassType = pDrawClass->GetClassType();
			
			switch(nClassType)
			{
			case EN_DRCLASS_TYPE_POINT:
				nSize_Point  += (int)((CDrawPoint*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Point += (int)((CDrawPoint*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_LINE:
				nSize_Line  += (int)((CDrawLine*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Line += (int)((CDrawLine*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_ARROWLINE:
				nSize_ArrowLine  += (int)((CDrawArrowLine*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_ArrowLine += (int)((CDrawArrowLine*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_CIRCLE:
				nSize_Circle  += (int)((CDrawCircle*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Circle += (int)((CDrawCircle*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_DONUT:
				nSize_Donut  += (int)((CDrawDonut*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Donut += (int)((CDrawDonut*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_ELLIPSE:
				nSize_Ellipse  += (int)((CDrawEllipse*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Ellipse += (int)((CDrawEllipse*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_RECT:
				nSize_Rect  += (int)((CDrawRect*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Rect += (int)((CDrawRect*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_ARC:
				nSize_Arc  += (int)((CDrawArc*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Arc += (int)((CDrawArc*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_POLYLINE:
				nSize_Polyline  += (int)((CDrawPolyline*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Polyline += (int)((CDrawPolyline*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_POLYGON:
				nSize_Polygon  += (int)((CDrawPolygon*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Polygon += (int)((CDrawPolygon*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_ARROWPOLYLINE:
				nSize_ArrowPolyline  += (int)((CDrawArrowPolyline*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_ArrowPolyline += (int)((CDrawArrowPolyline*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_TEXT:
				nSize_Text  += (int)((CDrawText*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Text += (int)((CDrawText*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_DIMENSION:
				nSize_Dimension  += (int)((CDrawDimension*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_Dimension += (int)((CDrawDimension*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_DIMANGLE:
				nSize_DimAngle  += (int)((CDrawDimAngle*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_DimAngle += (int)((CDrawDimAngle*)pDrawClass)->m_DataR.List.GetSize();
				break;
			case EN_DRCLASS_TYPE_DIMLEADERLINE:
				nSize_DimLeaderline  += (int)((CDrawDimLeaderline*)pDrawClass)->m_Data.List.GetSize();
				nSizeR_DimLeaderline += (int)((CDrawDimLeaderline*)pDrawClass)->m_DataR.List.GetSize();
				break;
			}
		}
	}

	m_CurrDrawClass_Point.m_Data.List.SetSize(nSize_Point);
	m_CurrDrawClass_Line.m_Data.List.SetSize(nSize_Line);
	m_CurrDrawClass_ArrowLine.m_Data.List.SetSize(nSize_ArrowLine);
	m_CurrDrawClass_Circle.m_Data.List.SetSize(nSize_Circle);
	m_CurrDrawClass_Donut.m_Data.List.SetSize(nSize_Donut);
	m_CurrDrawClass_Ellipse.m_Data.List.SetSize(nSize_Ellipse);
	m_CurrDrawClass_Rect.m_Data.List.SetSize(nSize_Rect);
	m_CurrDrawClass_Arc.m_Data.List.SetSize(nSize_Arc);
	m_CurrDrawClass_Polyline.m_Data.List.SetSize(nSize_Polyline);
	m_CurrDrawClass_Polygon.m_Data.List.SetSize(nSize_Polygon);
	m_CurrDrawClass_ArrowPolyline.m_Data.List.SetSize(nSize_ArrowPolyline);
	m_CurrDrawClass_Text.m_Data.List.SetSize(nSize_Text);
	m_CurrDrawClass_Dimension.m_Data.List.SetSize(nSize_Dimension);
	m_CurrDrawClass_DimAngle.m_Data.List.SetSize(nSize_DimAngle);
	m_CurrDrawClass_DimLeaderline.m_Data.List.SetSize(nSize_DimLeaderline);

	m_CurrDrawClass_Point.m_DataR.List.SetSize(nSizeR_Point);
	m_CurrDrawClass_Line.m_DataR.List.SetSize(nSizeR_Line);
	m_CurrDrawClass_ArrowLine.m_DataR.List.SetSize(nSizeR_ArrowLine);
	m_CurrDrawClass_Circle.m_DataR.List.SetSize(nSizeR_Circle);
	m_CurrDrawClass_Donut.m_DataR.List.SetSize(nSizeR_Donut);
	m_CurrDrawClass_Ellipse.m_DataR.List.SetSize(nSizeR_Ellipse);
	m_CurrDrawClass_Rect.m_DataR.List.SetSize(nSizeR_Rect);
	m_CurrDrawClass_Arc.m_DataR.List.SetSize(nSizeR_Arc);
	m_CurrDrawClass_Polyline.m_DataR.List.SetSize(nSizeR_Polyline);
	m_CurrDrawClass_Polygon.m_DataR.List.SetSize(nSizeR_Polygon);
	m_CurrDrawClass_ArrowPolyline.m_DataR.List.SetSize(nSizeR_ArrowPolyline);
	m_CurrDrawClass_Text.m_DataR.List.SetSize(nSizeR_Text);
	m_CurrDrawClass_Dimension.m_DataR.List.SetSize(nSizeR_Dimension);
	m_CurrDrawClass_DimAngle.m_DataR.List.SetSize(nSizeR_DimAngle);
	m_CurrDrawClass_DimLeaderline.m_DataR.List.SetSize(nSizeR_DimLeaderline);

	int nListSize;

	int nCountR_Point = 0,				nCount_Point = 0;
	int nCountR_Line = 0,				nCount_Line = 0;
	int nCountR_ArrowLine = 0,		nCount_ArrowLine = 0;
	int nCountR_Circle = 0,			nCount_Circle = 0;
	int nCountR_Donut = 0,				nCount_Donut = 0;
	int nCountR_Ellipse = 0,			nCount_Ellipse = 0;
	int nCountR_Rect = 0,				nCount_Rect = 0;
	int nCountR_Arc = 0,				nCount_Arc = 0;
	int nCountR_Polyline = 0,			nCount_Polyline = 0;
	int nCountR_Polygon = 0,			nCount_Polygon = 0;
	int nCountR_ArrowPolyline = 0,	nCount_ArrowPolyline = 0;
	int nCountR_Text = 0,				nCount_Text = 0;
	int nCountR_Dimension = 0,		nCount_Dimension = 0;
	int nCountR_DimAngle = 0,			nCount_DimAngle = 0;
	int nCountR_DimLeaderline = 0,	nCount_DimLeaderline = 0;

	for(i = 0; i < nSize ; i++)
	{
		pDrawClass = m_DrawCtrl.GetDrawClass(arSelectKey[i]);
		if(pDrawClass != NULL)
		{			
			nClassType = pDrawClass->GetClassType();
			switch(nClassType)
			{
			case EN_DRCLASS_TYPE_POINT:
				nListSize = ((CDrawPoint*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{
					m_CurrDrawClass_Point.m_Data.List[nCount_Point] = ((CDrawPoint*)pDrawClass)->m_Data.List[j]; 
					nCount_Point++; 
				}
				nListSize = ((CDrawPoint*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Point.m_DataR.List[nCountR_Point] = ((CDrawPoint*)pDrawClass)->m_DataR.List[j]; 
					nCountR_Point++;
				}
				break;
			case EN_DRCLASS_TYPE_LINE:
				nListSize = ((CDrawLine*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{
					m_CurrDrawClass_Line.m_Data.List[nCount_Line] = ((CDrawLine*)pDrawClass)->m_Data.List[j];  
					nCount_Line++;
				}
				nListSize = ((CDrawLine*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{
					m_CurrDrawClass_Line.m_DataR.List[nCountR_Line] = ((CDrawLine*)pDrawClass)->m_DataR.List[j];
					nCountR_Line++;
				}
				break;
			case EN_DRCLASS_TYPE_ARROWLINE:
				nListSize = ((CDrawArrowLine*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{
					m_CurrDrawClass_ArrowLine.m_Data.List[nCount_ArrowLine] = ((CDrawArrowLine*)pDrawClass)->m_Data.List[j]; 
					nCount_ArrowLine++;
				}
				nListSize = ((CDrawArrowLine*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{
					m_CurrDrawClass_ArrowLine.m_DataR.List[nCountR_ArrowLine] = ((CDrawArrowLine*)pDrawClass)->m_DataR.List[j];  
					nCountR_ArrowLine++;
				}
				break;
			case EN_DRCLASS_TYPE_CIRCLE:
				nListSize = ((CDrawCircle*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Circle.m_Data.List[nCount_Circle] = ((CDrawCircle*)pDrawClass)->m_Data.List[j]; 
					nCount_Circle++; 
				}
				nListSize = ((CDrawCircle*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Circle.m_DataR.List[nCountR_Circle] = ((CDrawCircle*)pDrawClass)->m_DataR.List[j];
					nCountR_Circle++;
				}
				break;
			case EN_DRCLASS_TYPE_DONUT:
				nListSize = ((CDrawDonut*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{	
					m_CurrDrawClass_Donut.m_Data.List[nCount_Donut] = ((CDrawDonut*)pDrawClass)->m_Data.List[j]; 
					nCount_Donut++;
				}
				nListSize = ((CDrawDonut*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Donut.m_DataR.List[nCountR_Donut] = ((CDrawDonut*)pDrawClass)->m_DataR.List[j];  
					nCountR_Donut++;
				}
				break;
			case EN_DRCLASS_TYPE_ELLIPSE:
				nListSize = ((CDrawEllipse*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Ellipse.m_Data.List[nCount_Ellipse] = ((CDrawEllipse*)pDrawClass)->m_Data.List[j]; 
					nCount_Ellipse++;
				}
				nListSize = ((CDrawEllipse*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Ellipse.m_DataR.List[nCountR_Ellipse] = ((CDrawEllipse*)pDrawClass)->m_DataR.List[j]; 
					nCountR_Ellipse++;
				}
				break;
			case EN_DRCLASS_TYPE_RECT:
				nListSize = ((CDrawRect*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Rect.m_Data.List[nCount_Rect] = ((CDrawRect*)pDrawClass)->m_Data.List[j]; 
					nCount_Rect++; 
				}
				nListSize = ((CDrawRect*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Rect.m_DataR.List[nCountR_Rect] = ((CDrawRect*)pDrawClass)->m_DataR.List[j];
					nCountR_Rect++;
				}
				break;
			case EN_DRCLASS_TYPE_ARC:
				nListSize = ((CDrawArc*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Arc.m_Data.List[nCount_Arc] = ((CDrawArc*)pDrawClass)->m_Data.List[j]; 
					nCount_Arc++; 
				}
				nListSize = ((CDrawArc*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{
					m_CurrDrawClass_Arc.m_DataR.List[nCountR_Arc] = ((CDrawArc*)pDrawClass)->m_DataR.List[j]; 
					nCountR_Arc++;
				}
				break;
			case EN_DRCLASS_TYPE_POLYLINE:
				nListSize = ((CDrawPolyline*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Polyline.m_Data.List[nCount_Polyline] = ((CDrawPolyline*)pDrawClass)->m_Data.List[j]; 
					nCount_Polyline++; 
				}
				nListSize = ((CDrawPolyline*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Polyline.m_DataR.List[nCountR_Polyline] = ((CDrawPolyline*)pDrawClass)->m_DataR.List[j]; 
					nCountR_Polyline++;
				}
				break;
			case EN_DRCLASS_TYPE_POLYGON:
				nListSize = ((CDrawPolygon*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize ; j++)				
				{ 
					m_CurrDrawClass_Polygon.m_Data.List[nCount_Polygon] = ((CDrawPolygon*)pDrawClass)->m_Data.List[j]; 
					nCount_Polygon++;
				}
				nListSize = ((CDrawPolygon*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Polygon.m_DataR.List[nCountR_Polygon] = ((CDrawPolygon*)pDrawClass)->m_DataR.List[j];
					nCountR_Polygon++; 
				}
				break;
			case EN_DRCLASS_TYPE_ARROWPOLYLINE:
				nListSize = ((CDrawArrowPolyline*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_ArrowPolyline.m_Data.List[nCount_ArrowPolyline] = ((CDrawArrowPolyline*)pDrawClass)->m_Data.List[j];
					nCount_ArrowPolyline++;
				}
				nListSize = ((CDrawArrowPolyline*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_ArrowPolyline.m_DataR.List[nCountR_ArrowPolyline] = ((CDrawArrowPolyline*)pDrawClass)->m_DataR.List[j];  
					nCountR_ArrowPolyline++; 
				}
				break;
			case EN_DRCLASS_TYPE_TEXT:
				nListSize = ((CDrawText*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Text.m_Data.List[nCount_Text] = ((CDrawText*)pDrawClass)->m_Data.List[j]; 
					nCount_Text++;
				}
				nListSize = ((CDrawText*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Text.m_DataR.List[nCountR_Text] = ((CDrawText*)pDrawClass)->m_DataR.List[j]; 
					nCountR_Text++;
				}
				break;
			case EN_DRCLASS_TYPE_DIMENSION:
				nListSize = ((CDrawDimension*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{
					m_CurrDrawClass_Dimension.m_Data.List[nCount_Dimension] = ((CDrawDimension*)pDrawClass)->m_Data.List[j]; 
					nCount_Dimension++;
				}
				nListSize = ((CDrawDimension*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_Dimension.m_DataR.List[nCountR_Dimension] = ((CDrawDimension*)pDrawClass)->m_DataR.List[j]; 
					nCountR_Dimension++; 
				}
				break;
			case EN_DRCLASS_TYPE_DIMANGLE:
				nListSize = ((CDrawDimAngle*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize ; j++)				
				{ 
					m_CurrDrawClass_DimAngle.m_Data.List[nCount_DimAngle] = ((CDrawDimAngle*)pDrawClass)->m_Data.List[j]; 
					nCount_DimAngle++; 
				}
				nListSize = ((CDrawDimAngle*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_DimAngle.m_DataR.List[nCountR_DimAngle] = ((CDrawDimAngle*)pDrawClass)->m_DataR.List[j]; 
					nCountR_DimAngle++;
				}
				break;
			case EN_DRCLASS_TYPE_DIMLEADERLINE:
				nListSize = ((CDrawDimLeaderline*)pDrawClass)->m_Data.List.GetSize();
				for(j = 0; j < nListSize; j++)				
				{ 
					m_CurrDrawClass_DimLeaderline.m_Data.List[nCount_DimLeaderline] = ((CDrawDimLeaderline*)pDrawClass)->m_Data.List[j]; 
					nCount_DimLeaderline++;
				}
				nListSize = ((CDrawDimLeaderline*)pDrawClass)->m_DataR.List.GetSize();
				for(j = 0; j < nListSize ; j++)				
				{
					m_CurrDrawClass_DimLeaderline.m_DataR.List[nCountR_DimLeaderline] = ((CDrawDimLeaderline*)pDrawClass)->m_DataR.List[j]; 
					nCountR_DimLeaderline++;
				}
				break;
			}
		}
	}

	double dMoveVector[2];
	dMoveVector[0] = (double)(endPoint.x - startPoint.x);
	dMoveVector[1] = (double)(endPoint.y - startPoint.y);

	m_CurrDrawClass_Point.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Line.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_ArrowLine.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Circle.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Donut.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Ellipse.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Rect.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Arc.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Polyline.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Polygon.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_ArrowPolyline.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Text.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_Dimension.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_DimAngle.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
	m_CurrDrawClass_DimLeaderline.Move(TRUE, dMoveVector, &(m_DrawCtrl.m_PaperFormat));
}
