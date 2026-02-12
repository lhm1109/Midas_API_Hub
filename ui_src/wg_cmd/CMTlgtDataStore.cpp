
#include "stdafx.h"

#include "wg_cmd.h"
#include "CMTlgtDataStore.h"
#include "TEurocodeTrainTypeFunctor.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_QSort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CCMTlgtDataStore::CCMTlgtDataStore(CDBDoc* pDoc)
{
	m_pDoc = pDoc;
	m_mapThfc.clear();
}

CCMTlgtDataStore::~CCMTlgtDataStore()
{

}


void CCMTlgtDataStore::CalcDynLoad_GTS(TLGT_INFO& trainInfo, double dElemSize, double dArrivalTime,
									CArray <double, double>& arResTime, CArray <double, double>& arResValue,
									BOOL bPreview)
{
	// GTSNX에서 사용됐던 clacDynLoad 함수

	int i;

	double dVelo = trainInfo.dVelocity;
	dArrivalTime += trainInfo.dTime;
	double t1 = dElemSize / dVelo;
	double t2 = 2 * t1;

	CArray<double, double> arAccDist;
	CArray<double, double> arWheelLoad;
	arAccDist.SetSize(trainInfo.nNumWheel);
	arWheelLoad.SetSize(trainInfo.nNumWheel);

	BOOL bSymm = FALSE;
	if (trainInfo.arTlgtItem[0].dLength == 0.0)
	{
		bSymm = TRUE;
		arAccDist[0] = trainInfo.arTlgtItem[1].dLength;
	} // 대칭을 위함.
	else
	{
		arAccDist[0] = trainInfo.arTlgtItem[0].dLength;
	}

	arWheelLoad[0] = trainInfo.arTlgtItem[0].dForce;

	for (i = 1; i < trainInfo.nNumWheel; i++)
	{
		arAccDist[i] = arAccDist[i - 1] + trainInfo.arTlgtItem[i].dLength;
		arWheelLoad[i] = trainInfo.arTlgtItem[i].dForce;
	}

	int nSize = trainInfo.nNumWheel * 3;
	CArray<double, double> arArriveTime;
	CArray<double, double> arAppliedLoad;
	CArray<double, double> arSlope;
	CArray<double, double> arIntercept;
	CArray<double, double> arTime;

	CArray<double, double> arTimeLast;
	CArray<double, double> arLoad;

	arArriveTime.SetSize(nSize + 1);
	arAppliedLoad.SetSize(nSize + 1);
	arSlope.SetSize(nSize + 1);
	arIntercept.SetSize(nSize + 1);
	arTime.SetSize(nSize);

	arTimeLast.SetSize(nSize);
	arLoad.SetSize(nSize);

	double dAddTimeData = 0.0;
	for (i = 0; i < trainInfo.nNumWheel; i++)
	{
		if (bSymm && i == 0) dAddTimeData = (-1.0)*(arAccDist[i] / dVelo - t1);
		arArriveTime[3 * i] = arAccDist[i] / dVelo - t1 + dAddTimeData;
		arArriveTime[3 * i + 1] = arAccDist[i] / dVelo + dAddTimeData;
		arArriveTime[3 * i + 2] = arAccDist[i] / dVelo + t1 + dAddTimeData;

		arAppliedLoad[3 * i] = 0.;
		arAppliedLoad[3 * i + 1] = arWheelLoad[i];
		arAppliedLoad[3 * i + 2] = 0.;
	}

	double dEndTime = ceil(arArriveTime[nSize - 1]);

	arArriveTime[nSize] = dEndTime;
	arAppliedLoad[nSize] = 0.;

	arSlope[0] = 0.;
	arIntercept[0] = 0.;
	for (i = 1; i < nSize + 1; i++)
	{
		arSlope[i] = (arAppliedLoad[i] - arAppliedLoad[i - 1]) / (arArriveTime[i] - arArriveTime[i - 1]);
		arIntercept[i] = arAppliedLoad[i] - arSlope[i] * arArriveTime[i];
	}

	for (i = 1; i < nSize; i++)
	{
		arTime[i - 1] = (arArriveTime[i] > arArriveTime[i - 1]) ? arArriveTime[i] : ((arIntercept[i - 1] - arIntercept[i + 1]) / (arSlope[i + 1] - arSlope[i - 1]));
		arLoad[i - 1] = (fabs(arTime[i - 1] - arArriveTime[i]) < 1.e-12) ? arAppliedLoad[i] : (arSlope[i - 1] * arTime[i - 1] + arIntercept[i - 1]);
	}
	arTime[nSize - 1] = dEndTime;
	arLoad[nSize - 1] = 0.;

	for (i = 0; i < nSize - 1; i++)
	{
		arTimeLast[i] = (arTime[i + 1] < arTime[i]) ? arTime[i + 1] : arTime[i];
		arLoad[i] = (arTime[i + 1] < arTime[i]) ? arLoad[i + 1] : arLoad[i];
	}
	arTimeLast[nSize - 1] = dEndTime;

	for (i = nSize - 1; i >= 1; i--)
	{
		if (fabs(arTimeLast[i] - arTimeLast[i - 1]) < 1.e-12)
		{
			arTimeLast.RemoveAt(i);
			arLoad.RemoveAt(i);
		}
	}

	CArray<double, double> resTime, resValue;

	if (!bPreview) dArrivalTime = 0.0; // Time History Function 생성할 때는 도착시간 고려안함.

	if (dArrivalTime > 0)
	{
		resTime.Add(0.0);
		resValue.Add(0.0);

		resTime.Add(dArrivalTime - dArrivalTime * t1*0.00001);
		resValue.Add(0.0);
	}
	else
	{
		resTime.Add(0.0);
		resValue.Add(0.0);
	}

	double dScaleFactor = 1.;
	double dMaxValue = 0.;
	if (trainInfo.nScaling == 0) // Scale Factor
		dScaleFactor = trainInfo.dSacleFactor;
	else if (trainInfo.nScaling == 1) // Max Value
	{
		for (i = 0; i < arLoad.GetSize(); i++)
		{
			if (dMaxValue < fabs(arLoad[i]))
				dMaxValue = fabs(arLoad[i]);
		}

		if (dMaxValue > 0.0)
			dScaleFactor = trainInfo.dMaximumValue / dMaxValue;
	}

	for (i = 0; i < arTimeLast.GetSize(); i++)
	{
		resTime.Add(arTimeLast[i] + dArrivalTime);
		resValue.Add(arLoad[i] * dScaleFactor);
	}

	arResTime.Copy(resTime);
	arResValue.Copy(resValue);
}

void CCMTlgtDataStore::CalcDynLoad(TLGT_INFO& trainInfo, double dLeftElemSize, double dRightElemSize, double dArrivalTime, CArray <double, double>& arResTime, CArray <double, double>& arResValue, double dVelocity, BOOL bPreview)
{
	// 개선된 CalcDynLoad

	double t1 = dLeftElemSize / dVelocity;
	double t2 = dRightElemSize / dVelocity;

	BOOL bStartNode = dLeftElemSize	== 0.0 ? TRUE : FALSE;
	BOOL bEndNode	= dRightElemSize == 0.0 ? TRUE : FALSE;
	BOOL bMidNode	= dLeftElemSize > 0.0 && dRightElemSize > 0.0 ? TRUE : FALSE;

	CArray<double, double> arAccDist; // 축하중 누적 거리
	CArray<double, double> arAccArrTime; // 축하중 도착 시간
	CArray<double, double> arWheelLoad;
	arAccDist.SetSize(trainInfo.nNumWheel);
	arAccArrTime.SetSize(trainInfo.nNumWheel);
	arWheelLoad.SetSize(trainInfo.nNumWheel);

	for (int i = 0; i < trainInfo.nNumWheel; i++)
	{
		if (i == 0)
			arAccDist[i] = trainInfo.arTlgtItem[i].dLength;
		else	
			arAccDist[i] = arAccDist[i - 1] + trainInfo.arTlgtItem[i].dLength;
		arWheelLoad[i] = trainInfo.arTlgtItem[i].dForce;
	}

	for (int i = 0; i < trainInfo.nNumWheel; i++)
	{
		arAccArrTime[i] = arAccDist[i] / dVelocity;
		arAccArrTime[i] = CMathFunc::mathRoundOff(arAccArrTime[i], 6);
	}
	

	CArray<TLFuncData, TLFuncData> arTlFuncData;
	arTlFuncData.SetSize(trainInfo.nNumWheel);

	// 삼각형 그래프 데이터 세팅
	// 첫번째, 마지막 절점에 대한 Time History Function을 만들 때 직각삼각형 함수가 생성되므로 같은 Time이 중복된다.
	// 그래서 아주작은 값을 더하거나 뺀다.
	double dDBL_EPSILON = 1.E-07;
	double dFirstArrivalTime = 0.0;
	double dTimeEpsilon = dDBL_EPSILON*10;

	// 실수 계산으로 인해 오차가 계속 발생한다...
	// x값, 기울기 값을 구할 때도 오차가 계속 생기기 때문에 y값을 0으로 기대했는데 -0이 나오는 경우도 있다...
	// 밑변 길이가 아주 조금차이나고 높이가 같다면 같은 기울기로 간주하는 알고리즘이 필요할듯.
	for (int i = 0; i < trainInfo.nNumWheel; i++)
	{
		if (bStartNode)
		{
			arTlFuncData[i].dArriveTime[0] = arAccArrTime[i] - dTimeEpsilon;
			arTlFuncData[i].dArriveTime[1] = arAccArrTime[i];
			arTlFuncData[i].dArriveTime[2] = arAccArrTime[i] + t2;
			dFirstArrivalTime = arAccArrTime[0];
		}
		if (bMidNode)
		{
			arTlFuncData[i].dArriveTime[0] = arAccArrTime[i] - t1;
			arTlFuncData[i].dArriveTime[1] = arAccArrTime[i];
			arTlFuncData[i].dArriveTime[2] = arAccArrTime[i] + t2;
			dFirstArrivalTime = arAccArrTime[0] - t1;
		}
		if (bEndNode)
		{
			arTlFuncData[i].dArriveTime[0] = arAccArrTime[i] - t1;
			arTlFuncData[i].dArriveTime[1] = arAccArrTime[i];
			arTlFuncData[i].dArriveTime[2] = arAccArrTime[i] + dTimeEpsilon;
			dFirstArrivalTime = arAccArrTime[0] - t1;
		}

		// 영향을 받는 순간부터가 arrival Time
		arTlFuncData[i].dArriveTime[0] -= dFirstArrivalTime;
		arTlFuncData[i].dArriveTime[1] -= dFirstArrivalTime;
		arTlFuncData[i].dArriveTime[2] -= dFirstArrivalTime;

		arTlFuncData[i].dAppliedLoad[0] = 0.;
		arTlFuncData[i].dAppliedLoad[1] = arWheelLoad[i];
		arTlFuncData[i].dAppliedLoad[2] = 0.;
	}

	// 직선 방정식 구하기
	for (int i = 0; i < trainInfo.nNumWheel; i++)
	{
		arTlFuncData[i].leftEQ.dSlope = (arTlFuncData[i].dAppliedLoad[1] - arTlFuncData[i].dAppliedLoad[0]) / (arTlFuncData[i].dArriveTime[1] - arTlFuncData[i].dArriveTime[0]);
		arTlFuncData[i].leftEQ.dIntercept = arTlFuncData[i].dAppliedLoad[1] - arTlFuncData[i].leftEQ.dSlope * arTlFuncData[i].dArriveTime[1];
		
		arTlFuncData[i].rightEQ.dSlope = (arTlFuncData[i].dAppliedLoad[2] - arTlFuncData[i].dAppliedLoad[1]) / (arTlFuncData[i].dArriveTime[2] - arTlFuncData[i].dArriveTime[1]);
		arTlFuncData[i].rightEQ.dIntercept = arTlFuncData[i].dAppliedLoad[2] - arTlFuncData[i].rightEQ.dSlope * arTlFuncData[i].dArriveTime[2];
		
		arTlFuncData[i].leftEQ.dRange[0] = arTlFuncData[i].dArriveTime[0];
		arTlFuncData[i].leftEQ.dRange[1] = arTlFuncData[i].dArriveTime[1];
		arTlFuncData[i].rightEQ.dRange[0] = arTlFuncData[i].dArriveTime[1];
		arTlFuncData[i].rightEQ.dRange[1] = arTlFuncData[i].dArriveTime[2];
	}
	
	CArray<double, double> arTime;
	CArray<double, double> arLoad;
	CArray<double, double> arTimeLast;

	// time값들 정렬
	for (int i = 0; i < trainInfo.nNumWheel; i++)
	{
		arTime.Add(arTlFuncData[i].dArriveTime[0]);
		arTime.Add(arTlFuncData[i].dArriveTime[1]);
		arTime.Add(arTlFuncData[i].dArriveTime[2]);
	}
	CQSort::QSortDbl(arTime.GetData(), arTime.GetSize());
	
	for (int i = 0; i < arTime.GetSize(); i++)
	{
		double dX = arTime[i];
		double dLoadSum = 0.0;

		int nFuncSize = arTlFuncData.GetSize();
		for (int j = 0; j < nFuncSize; j++)
		{
			if (bStartNode)
			{
				// 왼쪽 함수 범위에 들어와있으면 -> 무조건 0

				// 오른쪽 함수 범위에 들어와있으면 Load 값 추가
				if ((abs(dX - arTlFuncData[j].rightEQ.dRange[0]) <= dDBL_EPSILON) ||
					(dX > arTlFuncData[j].rightEQ.dRange[0] && dX < arTlFuncData[j].rightEQ.dRange[1]))
				{
					dLoadSum += arTlFuncData[j].rightEQ.dSlope * dX + arTlFuncData[j].rightEQ.dIntercept;
				}
			}

			if (bMidNode)
			{
				// 왼쪽 함수 범위에 들어와있으면 Load 값 추가
				if (dX > arTlFuncData[j].leftEQ.dRange[0] && dX < arTlFuncData[j].leftEQ.dRange[1])
				{
					dLoadSum += arTlFuncData[j].leftEQ.dSlope * dX + arTlFuncData[j].leftEQ.dIntercept;
				}

				// 오른쪽 함수 범위에 들어와있으면 Load 값 추가
				if ((abs(dX - arTlFuncData[j].rightEQ.dRange[0]) <= dDBL_EPSILON) ||
					(dX > arTlFuncData[j].rightEQ.dRange[0] && dX < arTlFuncData[j].rightEQ.dRange[1]))
				{
					dLoadSum += arTlFuncData[j].rightEQ.dSlope * dX + arTlFuncData[j].rightEQ.dIntercept;
				}
			}

			if (bEndNode)
			{
				// 왼쪽 함수 범위에 들어와있으면 Load 값 추가
				if ((abs(dX - arTlFuncData[j].leftEQ.dRange[1]) <= dDBL_EPSILON) ||
					(dX > arTlFuncData[j].leftEQ.dRange[0] && dX < arTlFuncData[j].leftEQ.dRange[1]))
				{
					dLoadSum += arTlFuncData[j].leftEQ.dSlope * dX + arTlFuncData[j].leftEQ.dIntercept;
				}

				// 오른쪽 함수 범위에 들어와있으면 -> 무조건 0
			}
		}
		arTimeLast.Add(dX);
		arLoad.Add(dLoadSum);
	}

	// Epsilon을 적용하면서 시작 Time이 음수가 될 수 있으므로 이동시켜준다.
	double dItpl = 0.0;
	BOOL bItpl = FALSE;
	if (arTimeLast[0] < 0.)
	{
		dItpl = -arTimeLast[0];
		bItpl = TRUE;
	}

	if (bPreview)
	{
		dArrivalTime += trainInfo.dTime;
		if (dArrivalTime > 0.0)
		{
			arResTime.Add(0.0);
			arResValue.Add(0.0);
		}
	}
	else
		dArrivalTime = 0.0; // Time History Function 생성할 때는 Arrival Time 과 start Time 고려안함.

	for (int i = 0; i < arTimeLast.GetSize(); i++)
	{
		double dTimeLast = arTimeLast[i] + dArrivalTime;
		arResTime.Add(bItpl ? dTimeLast + dItpl : dTimeLast);
		arResValue.Add(arLoad[i]);
	}

	if (bPreview)
	{
		arResTime.Add(ceil(arResTime[arResTime.GetSize() - 1] + 1.0));
		arResValue.Add(0.0);
	}
}

BOOL CCMTlgtDataStore::MakeTimeHistoryFunc(T_THFC_D& ThfcD, T_THFC_K& ThfcKey)
{
	// 모든 Time History Function을 순회하면서 같은 것이 있는지 찾는다.
	CArray<T_THFC_K, T_THFC_K> aThfcKeyList;
	m_pDoc->m_pAttrCtrl->GetThfcKeyList(aThfcKeyList);

	int nCount = aThfcKeyList.GetCount();

	for (int i = 0; i < nCount; i++)
	{
		T_THFC_D tempD;
		if (!(m_pDoc->m_pAttrCtrl->GetThfc(aThfcKeyList.GetData()[i], tempD)))
			return FALSE;
		AddThfcMapItem(aThfcKeyList.GetData()[i], tempD);
	}


	for (int i = 0; i < nCount; i++)
	{
		T_THFC_D tempD;
		if(!(m_pDoc->m_pAttrCtrl->GetThfc(aThfcKeyList.GetData()[i], tempD)))
			return FALSE;
		if (CompareTimeHistoryFunctionBase(ThfcD.arThisFuncData, tempD.arThisFuncData))
		{
			//같은 Time History Function 존재하면 Time History Function Key만 저장해둔다.
			ThfcKey = m_pDoc->m_pAttrCtrl->GetThfcKey(tempD.ThisFuncName);
			return TRUE;
		}
	}

	// Function이름 설정.
	CString strNewName;
	GetNewName(ThfcD.ThisFuncName, strNewName);
	ThfcD.ThisFuncName = strNewName;


	if (!(m_pDoc->m_pDataCtrl->AddThfc(ThfcD))) return FALSE;
	ThfcKey = m_pDoc->m_pAttrCtrl->GetThfcKey(ThfcD.ThisFuncName);
	return TRUE;
}

BOOL CCMTlgtDataStore::MakeTimeHistoryFunc(CArray<T_THFC_D, T_THFC_D&>& aThfcD, CArray<T_THFC_K, T_THFC_K>& aThfcKey)
{
	// 모든 Time History Function을 순회하면서 같은 것이 있는지 찾는다.
	CArray<T_THFC_K, T_THFC_K> aThfcKeyList;

	CArray<BOOL, BOOL> aExist; aExist.SetSize(aThfcKey.GetSize());
	CArray<T_THFC_D, T_THFC_D&> aThfcDTemp; aThfcDTemp.RemoveAll();
	for (int i = 0; i < aThfcD.GetCount(); i++)
	{
		aThfcKeyList.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetThfcKeyList(aThfcKeyList);

		int nCount = aThfcKeyList.GetCount();
		for (int j = 0; j < nCount; j++)
		{
			T_THFC_D tempD;
			if (!(m_pDoc->m_pAttrCtrl->GetThfc(aThfcKeyList.GetData()[j], tempD)))
				return FALSE;
			AddThfcMapItem(aThfcKeyList.GetData()[j], tempD);
		}

		BOOL bExist = FALSE;
		for (int j = 0; j < nCount; j++)
		{
			T_THFC_D tempD;
			if (!(m_pDoc->m_pAttrCtrl->GetThfc(aThfcKeyList.GetData()[j], tempD)))
				return FALSE;
			if (CompareTimeHistoryFunctionBase(aThfcD[i].arThisFuncData, tempD.arThisFuncData))
			{
				//같은 Time History Function 존재하면 Time History Function Key만 저장해둔다.
				aThfcKey[i] = m_pDoc->m_pAttrCtrl->GetThfcKey(tempD.ThisFuncName);
				bExist = TRUE;
				break;
			}
		}

		// Function이름 설정.
		CString strNewName;
		GetNewName(aThfcD[i].ThisFuncName, strNewName);
		aThfcD[i].ThisFuncName = strNewName;

		aExist.SetAt(i, bExist);
		if (!bExist) aThfcDTemp.Add(aThfcD[i]);
	}

	if(aThfcDTemp.GetSize()>0)
	{
		if (!(m_pDoc->m_pDataCtrl->AddThfc(aThfcDTemp))) return FALSE;
	}
	for (int i = 0; i < aThfcD.GetCount(); i++)
	{
		if(aExist[i]) continue;
		aThfcKey[i] = m_pDoc->m_pAttrCtrl->GetThfcKey(aThfcD[i].ThisFuncName);
	}

	return TRUE;
}

void CCMTlgtDataStore::GetNewName(const CString& strPrefix, CString& newName)
{
	UINT suffixIdx;

	std::map<CString, UINT>::iterator itrStr = m_mapNameSuffix.find(strPrefix);

	if (itrStr == m_mapNameSuffix.end())
		suffixIdx = 1;
	else
		suffixIdx = itrStr->second;

	BOOL bExistTemplate;
	BOOL bExistDB;
	while (TRUE)
	{
		newName.Format(_T("%s_f%03d"), strPrefix, suffixIdx++);
		bExistTemplate = IsExistTimeHistoryFunc(newName);
		bExistDB = m_pDoc->m_pAttrCtrl->GetThfcKey(newName);
		if (bExistDB == FALSE && bExistTemplate == FALSE)
			break;
	}

	m_mapNameSuffix[strPrefix] = suffixIdx;
}

BOOL CCMTlgtDataStore::IsExistTimeHistoryFunc(CString& strName)
{
	std::vector<UINT> thfcKeyList; thfcKeyList.clear();
	std::map<UINT, T_THFC_D>::iterator itr;
	for (itr = m_mapThfc.begin(); itr != m_mapThfc.end(); itr++)
	{
		int nKey = itr->first;
		thfcKeyList.push_back(nKey);
	}
	int nSize = (int)thfcKeyList.size();

	T_THFC_D ThfcD;
	for (int i = 0; i < nSize; i++)
	{
		itr = m_mapThfc.find(thfcKeyList[i]);
		if (itr != m_mapThfc.end())
		{
			//존재함
			ThfcD = itr->second;
			if (ThfcD.ThisFuncName.CompareNoCase(strName) == 0) return TRUE;
		}
	}
	return FALSE;
}

void CCMTlgtDataStore::AddThfcMapItem(T_THFC_K& ThfcKey, T_THFC_D& ThfcD)
{
	const UINT unKey = ThfcKey;
	m_mapThfc.insert(std::pair<const UINT, T_THFC_D>(unKey, ThfcD));
}

BOOL CCMTlgtDataStore::SaveToFile(TLGT_INFO& rData)
{
	CFileDialog dlg(FALSE, _T(".wzd"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES, _T("WZD Files(*.wzd)|*.wzd||"));
	CString csFileName;
	if (dlg.DoModal() == IDOK) csFileName = dlg.GetPathName();
	else return FALSE;

	CFileRW FileRW;
	TLGT_InfoRW DataRW;
	DataRW.m_data = rData;
	DataRW.m_data.ConvertUnitIn();  // 중립 단위계로 저장

	BOOL bResult = FileRW.Save(csFileName, &DataRW);
	if (bResult) GSaveHistoryFormatNF(_LS(IDS_SW_GRIL_MSG313));
	return bResult;
}

BOOL CCMTlgtDataStore::ReadFromFile(TLGT_INFO& rData)
{
	CFileDialog dlg(TRUE, _T(".wzd"), NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES, _T("WZD Files(*.wzd)|*.wzd||"));
	CString csFileName;
	if (dlg.DoModal() == IDOK) csFileName = dlg.GetPathName();
	else return FALSE;

	CFileRW FileRW;
	TLGT_InfoRW DataRW;
	if (!FileRW.Load(csFileName, &DataRW)) return FALSE;
	if (DataRW.m_bAdded == FALSE)
	{
		//     AfxMessageBox(_LS(IDS_WG_SW_GRIL_Data_Not_Exist));
		return FALSE; // 데이타 없는 경우
	}

	DataRW.m_data.ConvertUnitOut(); // 현재 단위계로 변환
	rData = DataRW.m_data;

	GSaveHistoryFormatNF(_LS(IDS_SW_GRIL_MSG314));
	return TRUE;
}

BOOL CCMTlgtDataStore::CompareTimeHistoryFunctionBase(T_THFC_DT& arThfcBase1, T_THFC_DT& arThfcBase2)
{
	// Time History Function을 생성할 때 중복되는지 확인하는 코드
	// 중복이면 TRUE 리턴

	double dTol = (M_InitValueCurUnit(0.01, N, MM, D_UNITSYS_BASE_LENGTH)); // 0.1mm
	double dTolTime = 0.00001;

	int nCount1, nCount2;
	nCount1 = arThfcBase1.GetCount();
	nCount2 = arThfcBase2.GetCount();

	if (nCount1 != nCount2)
		return FALSE;

	for (int i = 0; i < nCount1; i++)
	{
		if (fabs( arThfcBase1.GetData()[i].dblTime - arThfcBase2.GetData()[i].dblTime) > dTolTime)
			return FALSE;
		if (fabs(arThfcBase1.GetData()[i].dblValue - arThfcBase2.GetData()[i].dblValue) > dTol)
			return FALSE;
	}
	return TRUE;
}

// Dynamic Nodal Load를 추가할 때 THFC도 매핑
void CCMTlgtDataStore::GetDefaultData(int nCode, int nType, int nVehcleNum, ARR_TLGT_ITEM& arUserItem, ARR_TLGT_ITEM& arItem)
{
	int nCount;
	arItem.RemoveAll();

	if (nCode == TRAINCODE_KOREA)
	{
		switch (nType)
		{
		case TRAINTYPE_KOREA_MKH_D2:
			{
				double dLength[] = { 0, 1.854, 1.854, 8.789, 1.854, 1.854, 4.582, 1.854, 1.854, 8.789,
					1.854, 1.854, 5.094,   2.3,  13.6,   2.3,   5.3,   2.3,  13.6,   2.3,
					5.3,   2.3,  13.6,   2.3,   5.3,   2.3,  13.6,   2.3,   5.3,   2.3,
					13.6,   2.3,   5.3,   2.3,  13.6,   2.3,   5.3,   2.3,  13.6,   2.3 };
				double dForce[] = { 220, 220, 220, 220, 220, 220, 220, 220, 220, 220,
					220, 220, 120, 120, 120, 120, 120, 120, 120, 120,
					120, 120, 120, 120, 120, 120, 120, 120, 120, 120,
					120, 120, 120, 120, 120, 120, 120, 120, 120, 120 };

				nCount = 40;
				arItem.SetSize(nCount);
				for (long i = 0; i < nCount; ++i)
				{
					arItem.GetData()[i].dLength = dLength[i];
					arItem.GetData()[i].dForce = dForce[i] * 1000;
				}
			}
			break;
		case TRAINTYPE_KOREA_PMC_8:
			{
				double dLength[] = { 0, 2.5, 12.7, 2.5, 4.985, 2.6, 13.3, 2.6, 4.4, 2.6,
					13.3, 2.6, 4.4, 2.6, 13.3, 2.6, 4.4, 2.6, 13.3, 2.6,
					4.4, 2.6, 13.3, 2.6, 4.4, 2.6, 13.3, 2.6, 4.985, 2.5,
					12.7, 2.5 };
				double dForce[] = { 220, 220, 180, 180, 110, 110, 110, 110, 110, 110,
					110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
					110, 110, 110, 110, 110, 110, 110, 110, 180, 180,
					220, 220 };
				nCount = 32;
				arItem.SetSize(nCount);
				for (long i = 0; i < nCount; ++i)
				{
					arItem.GetData()[i].dLength = dLength[i];
					arItem.GetData()[i].dForce = dForce[i] * 1000;
				}
			}
			break;
		case TRAINTYPE_KOREA_KTX_20:
			{
				double dLength[] = { 0, 3, 11, 3, 3.275, 3, 15.7, 3, 15.7, 3,
					15.7, 3, 15.7, 3, 15.7, 3, 15.7, 3, 15.7, 3,
					15.7, 3, 15.7, 3, 15.7, 3, 15.7, 3, 15.7, 3,
					15.7, 3, 15.7, 3, 15.7, 3, 15.7, 3, 15.7, 3,
					15.7, 3, 3.275, 3, 11, 3 };
				nCount = 46;
				arItem.SetSize(nCount);
				for (long i = 0; i < nCount; ++i)
				{
					arItem.GetData()[i].dLength = dLength[i];
					arItem.GetData()[i].dForce = 170. * 1000;
				}
			}
			break;
		case TRAINTYPE_KOREA_EL18_6:
			{
				double dLength[] = { 0, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1,
					10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1,
					3.5, 2.1, 10.3, 2.1 };
				nCount = 24;
				arItem.SetSize(nCount);
				for (long i = 0; i < nCount; ++i)
				{
					arItem.GetData()[i].dLength = dLength[i];
					arItem.GetData()[i].dForce = 180. * 1000;
				}
			}
			break;
		case TRAINTYPE_KOREA_EL18_8:
			{
				double dLength[] = { 0, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1,
					10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1,
					3.5, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1,
					10.3, 2.1 };
				nCount = 32;
				arItem.SetSize(nCount);
				for (long i = 0; i < nCount; ++i)
				{
					arItem.GetData()[i].dLength = dLength[i];
					arItem.GetData()[i].dForce = 180. * 1000;
				}
			}
			break;
		case TRAINTYPE_KOREA_EL18_10:
			{
				double dLength[] = { 0, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1,
					10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1,
					3.5, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1,
					10.3, 2.1, 3.5, 2.1, 10.3, 2.1, 3.5, 2.1, 10.3, 2.1 };
				nCount = 40;
				arItem.SetSize(nCount);
				for (long i = 0; i < nCount; ++i)
				{
					arItem.GetData()[i].dLength = dLength[i];
					arItem.GetData()[i].dForce = 180. * 1000;
				}
			}
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	else if (nCode == TRAINCODE_UK)
	{
		switch (nType)
		{
		case TRAINTYPE_UK_ATC_SINGLE_AXLE:
		{
			double dLength[] = { 0, 2.8, 8.2, 2.8, 4.5, 13.14, 13.1, 13.1, 13.1, 13.1,
				13.1, 13.1, 13.1, 13.1, 13.1, 13.1, 13.14, 4.5, 2.8, 8.2,
				2.8 };

			double dForce[] = { 177, 177, 177, 177, 111, 215, 209, 216, 222, 222,
				214, 214, 222, 222, 222, 228, 117, 177, 177, 177,
				177 };

			nCount = 21;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		case TRAINTYPE_UK_ATC_DOUBLE_DECK_11:
		{
			double dLength[] = { 0, 3, 7, 3, 3.2, 3, 14.3, 3, 14.3, 3,
				14.3, 3, 14.3, 3, 14.3, 3, 14.3, 3, 14.3, 3,
				14.3, 3, 14.3, 3, 3.2, 3, 7, 3};

			double dForce[] = { 181, 181, 181, 181, 202, 202, 175, 175, 189, 189, 
				208, 208, 198, 198, 182, 182, 188, 188, 188, 188, 
				187, 187, 219, 219, 181, 181, 181, 181 };

			nCount = 28;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		case TRAINTYPE_UK_ATC_DOUBLE_DECK_10:
		{
			double dLength[] = { 0, 3, 11, 3, 3.2, 3, 15.7, 3, 15.7, 3,
				15.7, 3, 15.7, 3, 15.7, 3, 15.7, 3, 15.7, 3,
				15.7, 3, 3.2, 3, 11, 3 };

			double dForce[] = { 184, 184, 184, 184, 184.32009, 184.32009, 196, 196, 209, 209,
				212, 212, 210, 210, 205, 205, 205, 205, 197, 197,
				221.907105, 221.907105, 184, 184, 184, 184 };

			nCount = 26;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		case TRAINTYPE_UK_ATC_DISTRIBUTED:
		{
			double dLength[] = { 0, 3, 14.1, 3, 14.3, 3, 14.3, 3, 14.3, 3,
				14.3, 3, 14.3, 3, 14.3, 3, 14.3, 3, 14.3, 3,
				14.3, 3, 14.1, 3 };

			double dForce[] = { 114, 114, 210, 210, 214, 214, 207, 207, 225, 225,
				230, 230, 215, 215, 213, 213, 215, 215, 225, 225,
				217, 217, 116.5324995, 116.5324995 };

			nCount = 24;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		case TRAINTYPE_UK_CVN_CURRENT:
		{
			double dLength[] = { 0, 2.7, 14.7, 2.7, 4.8, 2.7, 14.7, 2.7, 4.8, 2.7,
				14.7, 2.7, 4.8, 2.7, 14.7, 2.7, 4.8, 2.7, 14.7, 2.7,
				4.8, 2.7, 14.7, 2.7, 4.8, 2.7, 14.7, 2.7, 4.8, 2.7,
				14.7, 2.7 };

			double dForce[] = { 173, 173, 173, 173, 170, 170, 170, 170, 184, 184,
				184, 184, 163, 163, 163, 163, 167, 167, 167, 167,
				194, 194, 194, 194, 179, 179, 179, 179, 175, 175,
				175, 175 };

			nCount = 32;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		case TRAINTYPE_UK_CVN_OMNI:
		{
			double dLength[] = { 0, 2.7, 14.7, 2.7, 4.8, 2.7, 14.7, 2.7, 4.8, 2.7,
				14.7, 2.7, 4.8, 2.7, 14.7, 2.7, 4.8, 2.7, 14.7, 2.7,
				4.8, 2.7, 14.7, 2.7, 4.8, 2.7, 14.7, 2.7, 4.8, 2.7,
				14.7, 2.7 };

			double dForce[] = { 177, 177, 177, 177, 173, 173, 173, 173, 183, 183,
				183, 183, 165, 165, 165, 165, 164, 164, 164, 164,
				193, 193, 193, 193, 176, 176, 176, 176, 177, 177,
				177, 177 };

			nCount = 32;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		case TRAINTYPE_UK_CVN_FUTURE:
		{
			double dLength[] = { 0, 2.5, 14.85, 2.5, 5.2, 2.5, 14.85, 2.5, 5.2, 2.5,
				14.85, 2.5, 5.2, 2.5, 14.85, 2.5, 5.2, 2.5, 14.85, 2.5,
				5.2, 2.5, 14.85, 2.5, 5.2, 2.5, 14.85, 2.5, 5.2, 2.5,
				14.85, 2.5 };

			double dForce[] = { 148, 148, 148, 148, 168, 168, 168, 168, 164, 164,
				164, 164, 153, 153, 153, 153, 167, 167, 167, 167,
				164, 164, 164, 164, 176, 176, 176, 176, 151, 151,
				151, 151 };

			nCount = 32;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		case TRAINTYPE_UK_CVN_OMNI_28M:
		{
			double dLength[] = { 0, 2.7, 16.05, 2.7, 6.6, 2.7, 16.05, 2.7, 6.6, 2.7,
				16.05, 2.7, 6.6, 2.7, 16.05, 2.7, 6.6, 2.7, 16.05, 2.7,
				6.6, 2.7, 16.05, 2.7, 6.6, 2.7, 16.05, 2.7 };

			double dForce[] = { 191, 191, 191, 191, 208, 208, 208, 208, 205, 205,
				205, 205, 198, 198, 198, 198, 208, 208, 208, 208,
				210, 210, 210, 210, 191, 191, 191, 191 };

			nCount = 28;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		case TRAINTYPE_UK_CVN_SEGMENTED_28M:
		{
			double dLength[] = { 0, 2.7, 16.05, 2.7, 6.6, 2.7, 16.05, 2.7, 6.6, 2.7,
				16.05, 2.7, 6.6, 2.7, 16.05, 2.7, 6.6, 2.7, 16.05, 2.7,
				6.6, 2.7, 16.05, 2.7, 6.6, 2.7, 16.05, 2.7 };

			double dForce[] = { 188, 188, 188, 188, 201, 201, 201, 201, 206, 206,
				206, 206, 199, 199, 199, 199, 209, 209, 209, 209,
				211, 211, 211, 211, 192, 192, 192, 192 };

			nCount = 28;
			arItem.SetSize(nCount);
			for (long i = 0; i < nCount; ++i)
			{
				arItem.GetData()[i].dLength = dLength[i];
				arItem.GetData()[i].dForce = dForce[i] * 1000;
			}
		}
		break;
		default:
			ASSERT(0);
			break;
		}
	}
    else if (nCode == TRAINCODE_EUROCODE)
    {
        typedef std::pair<double, double> Paird;
        typedef std::vector<Paird> vPaird;

        vPaird vLF;
        vLF.clear();

        //TypeB는 예외적인 Option이 있어서 GetDataEUTrainTypeB 함수를 통해야 합니다.
        const auto& TrainType = static_cast<TRAIN_TYPE>(nType);
        TEurocodeTrainTypeFunctor TypeEU;
        if ( !TypeEU.GetTypeA(TrainType, vLF) )
        {
            ASSERT(0); return;
        }

        const auto& nCount = static_cast<int>(vLF.size());
        arItem.SetSize(nCount);
        for ( int nIdx = 0; nIdx < nCount; ++nIdx )
        {
            arItem[nIdx].dLength = vLF[nIdx].first;
            arItem[nIdx].dForce  = vLF[nIdx].second * 1000;
        }
    }
	else if (nCode == TRAINCODE_JAPANRAIL)
	{
		switch (nType)
		{
		case TRAINCODE_JAPANRAIL_N16:
			{
				nCount = 4;
				double dLength[] = {   2.8,   2.2,   6.3,   2.2};
				double dForce[]  = { 160.0, 160.0, 160.0, 160.0};	

				arItem.SetSize(nCount * nVehcleNum);

				int nIdx = 0;
				for (int nV = 0; nV < nVehcleNum; nV++)
				{
					for (int i = 0; i < nCount; i++)
					{
						arItem.GetData()[nIdx].dLength = (nV==0 && i==0)? 0.0 : dLength[i];
						arItem.GetData()[nIdx].dForce  = dForce[i] * 1000;
						nIdx++;
					}
				}

			}
			break;
		case TRAINCODE_JAPANRAIL_P16:
			{
				nCount = 4;
				double dLength[] = {   2.8,   2.2,  12.8,   2.2};
				double dForce[]  = { 160.0, 160.0, 160.0, 160.0};	

				arItem.SetSize(nCount * nVehcleNum);

				int nIdx = 0;
				for (int nV = 0; nV < nVehcleNum; nV++)
				{
					for (int i = 0; i < nCount; i++)
					{
						arItem.GetData()[nIdx].dLength = (nV==0 && i==0)? 0.0 : dLength[i];
						arItem.GetData()[nIdx].dForce  = dForce[i] * 1000;
						nIdx++;
					}
				}
			}
			break;
		case TRAINCODE_JAPANRAIL_P17:
			{
				nCount = 4;
				double dLength[] = {   3.5,   2.2,  12.1,   2.2};
				double dForce[]  = { 170.0, 170.0, 170.0, 170.0};	

				arItem.SetSize(nCount * nVehcleNum);

				int nIdx = 0;
				for (int nV = 0; nV < nVehcleNum; nV++)
				{
					for (int i = 0; i < nCount; i++)
					{
						arItem.GetData()[nIdx].dLength = (nV==0 && i==0)? 0.0 : dLength[i];
						arItem.GetData()[nIdx].dForce  = dForce[i] * 1000;
						nIdx++;
					}
				}
			}
			break;
		case TRAINCODE_JAPANRAIL_M18:
			{
				nCount = 4;
				double dLength[] = {   4.1,   2.1,  11.7,   2.1};
				double dForce[]  = { 180.0, 180.0, 180.0, 180.0};	

				arItem.SetSize(nCount * nVehcleNum);

				int nIdx = 0;
				for (int nV = 0; nV < nVehcleNum; nV++)
				{
					for (int i = 0; i < nCount; i++)
					{
						arItem.GetData()[nIdx].dLength = (nV==0 && i==0)? 0.0 : dLength[i];
						arItem.GetData()[nIdx].dForce  = dForce[i] * 1000;
						nIdx++;
					}
				}
			}
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	else if (nCode == TRAINCODE_USER)
	{
		switch (nType)
		{
		case TRAINTYPE_USER:
			{
				nCount = arUserItem.GetSize();
				arItem.SetSize(nCount);
				for (long i = 0; i < nCount; ++i)
				{
					arItem.GetData()[i].dLength = arUserItem.GetData()[i].dLength;
					arItem.GetData()[i].dForce = arUserItem.GetData()[i].dForce;
				}
			}
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	else
		ASSERT(0);

	if(nCode != TRAINCODE_USER)
	{
		long lSize = arItem.GetSize();
		for(long i = 0; i < lSize; ++i)
		{
			arItem.GetData()[i].dLength = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthOut(arItem.GetData()[i].dLength);
			arItem.GetData()[i].dForce = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitForceOut(arItem.GetData()[i].dForce);
		}
	}
}

bool CCMTlgtDataStore::GetDataEUTrainTypeB(UINT unForceNumber, 
                                           double dSpace, 
                                           ARR_TLGT_ITEM& arItem) const
{
    typedef std::pair<double, double> Paird;
    typedef std::vector<Paird> vPaird;

    arItem.RemoveAll();

    const auto bZero1 = unForceNumber == 0U;
    const auto bZero2 = fabs(dSpace) < 1.0E-07; //Civil에서는 부동소수점 비교를 어찌하죠?
    if ( bZero1 || bZero2 )
    {
        ASSERT(0); return false;
    }

    vPaird vLF;
    vLF.clear();

    TEurocodeTrainTypeFunctor TypeEU;
    if ( !TypeEU.GetTypeB(unForceNumber, dSpace, vLF) )
    {
        ASSERT(0); return false;
    }

    auto* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;

    const auto& nCount = static_cast<int>(vLF.size());
    arItem.SetSize(nCount);
    for ( int nIdx = 0; nIdx < nCount; ++nIdx )
    {
        const auto& dRawLength = vLF[nIdx].first;
        const auto& dRawForce = vLF[nIdx].second * 1000.0;

        arItem[nIdx].dLength = pUnitCtrl->ConvertUnitLengthOut(dRawLength);
        arItem[nIdx].dForce  = pUnitCtrl->ConvertUnitForceOut(dRawForce);
    }

    return true;
}

double CCMTlgtDataStore::GetNodeDist(const T_NODE_D& Node1, const T_NODE_D& Node2)
{
	return sqrt(pow(Node1.x - Node2.x, 2) + pow(Node1.y - Node2.y, 2) + pow(Node1.z - Node2.z, 2));
}

// Save할 때 호출
void TLGT_InfoRW::GetNextData(POSITION& rPos, DB_TDataRW* pDataRW)
{
	TDataRW_TLGT_INFO* pData = (TDataRW_TLGT_INFO*)pDataRW;
	pData->m_data = m_data;
	rPos = 0;
}

// Load할 때 호출
BOOL TLGT_InfoRW::AddToDB(UINT nVer, DB_TDataRW* pDataRW)
{
	// 이전 버전이면 최신 버전으로 데이터 변환
	if (nVer == D_PGM_VER(9, 5, 5, 0))
	{
		TDataRW_TLGT_INFO* pData = (TDataRW_TLGT_INFO*)pDataRW;
		m_data = pData->m_data;
	}
	else if (nVer == D_PGM_VER(9, 0, 0, 0))
	{
		TDataRW_TLGT_INFO_900* pData = (TDataRW_TLGT_INFO_900*)pDataRW;
		if (!pData->ConvertToLatest(m_data))
		{
			m_bAdded = FALSE;
			return FALSE;
		}
	}
	else { ASSERT(0); return FALSE; }

	m_bAdded = TRUE;

	return TRUE;
}

DB_TDataRW* TLGT_InfoRW::CreateData(UINT nVer)
{
	if (nVer == D_PGM_VER(9, 5, 5, 0))
	{
		return new TDataRW_TLGT_INFO;
	}
	else if (nVer == D_PGM_VER(9, 0, 0, 0))
	{
		return new TDataRW_TLGT_INFO_900;
	}
	else ASSERT(0);
	return 0;
}

UINT TDataRW_TLGT_INFO::GetSize()
{
	int nSize = sizeof(TLGT_INFO_RW);
	nSize += static_cast<int>(m_data.arKeyNode.GetSize()) * sizeof(T_NODE_K);
	nSize += static_cast<int>(m_data.strName.GetLength()) * sizeof(TCHAR);

	for (int i = 0; i < m_data.arTlgtItem.GetSize(); i++)
	{
		nSize += sizeof(TLGT_ITEM);
	}

	return nSize;
}

BOOL TDataRW_TLGT_INFO::Stream2Data(const void* pStream, int nMaxSize)
{
	TLGT_INFO_RW RW;
	int nStart = 0;
	return RW.S2D(m_data, (unsigned char*)pStream, nStart, nMaxSize);
}

BOOL TDataRW_TLGT_INFO::Data2Stream(void* pStream, int nMaxSize)
{
	TLGT_INFO_RW RW;
	int nStart = 0;
	return RW.D2S(m_data, (unsigned char*)pStream, nStart, nMaxSize);
}


BOOL TDataRW_TLGT_INFO_900::Stream2Data(const void* pStream, int nMaxSize)
{
	TLGT_INFO_900_RW RW;
	int nStart = 0;
	return RW.S2D(m_data, (unsigned char*)pStream, nStart, nMaxSize);
}

BOOL TDataRW_TLGT_INFO_900::ConvertToLatest(TLGT_INFO& rData)
{
	return m_data.ConvertToLatest(rData);
}
