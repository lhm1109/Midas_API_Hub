#pragma once
#include "..\wg_base\wg_base_ServiceProvider.h"

class CServiceAgencyDgn : public CServiceAgency
{
public:
	CServiceAgencyDgn();
	virtual ~CServiceAgencyDgn();

public:
	virtual int DoService(const int nServiceNo, void* pData);

private:
	void GetGenKfac(void* pData);
	void GetGenLeng(void* pData);
	int  ExportCRC();
	int  ImportCRC(void* pData);
	int  PrintExcelCRC(void* pData);
	int  PrintRT4PSC(void* pData);
	int  ExecDesignPSC_KSCE(void* pData);
	int  ExecDesignPSC_JTG(void* pData);
	int  ExecLoadRating(void* pData);
	int  PrintExcelLoadRating(void* pData);
	int  MakePlateGiderDesignData(void* pData);
	int  PrintPlateGiderDesignData(void* pData);
	int  PrintExcelPlateGider(void* pData);
	int  PrintExcelPlateGiderRT(void* pData);
	int  MakeOrthotropicDeckDesign(void* pData);
	int  PrintOrthotropicDeckDesign(void* pData);
	int  ExecuteRUSStlCompDesign(void* pData);
	int  ExecuteRUSStlCompPrintReport(void* pData);
	int  RCBeamDesign_JTG(void* pData);
	int  RCColmDesign_JTG(void* pData);
	int  RCBeamDesign_JTG_Railway(void* pData);
	int  RCColmDesign_JTG_Railway(void* pData);
	int  PSCBeamDesign_TB_Railway(void* pData);
	int  PSCColmDesign_TB_Railway(void* pData);
	int  GetGenSeisFactor(void* pData);
	int  GetPSCTendonStress(void* pData);
	int  PSCDesignAASHTO(void* pData);
	int  GetPSCCheckElementList(void* pData);
	int  SeismicDesign_JTG_D62_04(void* pData);
	int  GetDstl(void* pData);
	int  GetDcon(void* pData);
	int  GetPscd(void* pData);
	int  GetDsrc(void* pData);
	int  ExecBridgeSeismicEval(void* pData);
	int  PrintBridgeSeismicEval(void* pData);
	int  ExecBridgeSeismicEvalBearing(void* pData);
	int  PrintSeisEval1stCon(void* pData);
	int  PrintSeisEval1stStl(void* pData);
	int  PrintSeisEval1stMas(void* pData);
	int  PrintSeisEvalUphgCon(void* pData);
	int  PrintSeisEvalUphgStl(void* pData);
	int  PrintSeisEvalUphgMas(void* pData);
	int  ClearMPhiResultAll();
	int  ClearMPhiResultMemb(void* pData, bool bWall);
	int  ExecBridgeSeismicEval_JP(void* pData);
	int  PrintBridgeSeismicEval_JP(void* pData);
};
