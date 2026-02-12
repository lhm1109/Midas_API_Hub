#ifndef __DB_ST_POST_READ_SPBR_H__
#define __DB_ST_POST_READ_SPBR_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

struct _COOR_SPBR_BN    // 업데이트된 좌표
{
	double dx;
	double dy;
	double dz;
	void ConvertOut(T_SBCO_D& Data)
	{
		Data.Initialize();
		Data.dx=dx;
		Data.dy=dy;
		Data.dz=dz;
	}
};
struct _CURL_SPBR_BN    // 케이블의 변형전 길이비
{
	double dElemId;
	double dCableForce;    // Cable Pretension Force
	double dCableLength;   // 케이블 길이
	double dHorzForce;     // Cable Horizontal Force // MNET:XXXX 20060918 JHKIM : 정진상 부장님 요구
	double dLengthRatio;   // 변형전 길이 비
	void ConvertOut(T_SBCR_D& Data)
	{
		Data.Initialize();
		Data.dCableForce=dCableForce;
		Data.dCableLength=dCableLength;
		Data.dHorzForce=dHorzForce;
		Data.dLengthRatio=dLengthRatio;
	}
};
struct _IFFN_SPBR_7DOF_BN    // 부재의 평형상태 절점력 및 부재력
{
	double dElemId;
	double dType;  // 1=Truss 2=Beam 3=Elastic Link 4=General Link
	double dNodeForce[2][7];
	double dElemForce[2][7];
	void Initialize()
	{
		memset(this, 0, sizeof(_IFFN_SPBR_7DOF_BN));
	}
	void ConvertOut(T_SBIF_D& Data)
	{
		Data.Initialize();
		Data.ElemK=(UINT)dElemId;
		Data.nType=(int)dType;
		for(int i=0; i<7; i++)
		{
			Data.dNodeForce[0][i]=dNodeForce[0][i];
			Data.dNodeForce[1][i]=dNodeForce[1][i];
			Data.dElemForce[0][i]=dElemForce[0][i];
			Data.dElemForce[1][i]=dElemForce[1][i];
		}
	}
};

struct _IFFN_SPBR_7DOF_BN_CONVERT    // 부재의 평형상태 절점력 및 부재력
{
	double dElemId;
	double dType;  // 1=Truss 2=Beam 3=Elastic Link 4=General Link
	double dNodeForce[2][6];
	double dNodeForceWaping_I;
	double dElemForceWaping_I;
	double dElemForce[2][6];
	double dNodeForceWaping_J;
	double dElemForceWaping_J;

	void ConvertOut(_IFFN_SPBR_7DOF_BN& Data)
	{
		Data.dElemId = dElemId;
		Data.dType = dType;
		for (int i = 0; i < 6; i++)
		{
			Data.dNodeForce[0][i] = dNodeForce[0][i];
			Data.dNodeForce[1][i] = dNodeForce[1][i];
			Data.dElemForce[0][i] = dElemForce[0][i];
			Data.dElemForce[1][i] = dElemForce[1][i];
		}
		Data.dNodeForce[0][6] = dNodeForceWaping_I;
		Data.dNodeForce[1][6] = dNodeForceWaping_J;

		Data.dElemForce[0][6] = dElemForceWaping_I;
		Data.dElemForce[1][6] = dElemForceWaping_J;
	}
};

struct _IFFN_SPBR_BN    // 부재의 평형상태 절점력 및 부재력
{
	double dElemId;
	double dType;  // 1=Truss 2=Beam 3=Elastic Link 4=General Link
	double dNodeForce[2][6];
	double dElemForce[2][6];
	void ConvertOut(T_SBIF_D& Data)
	{
		Data.Initialize();
		Data.ElemK=(UINT)dElemId;
		Data.nType=(int)dType;
		for(int i=0; i<6; i++)
		{
			Data.dNodeForce[0][i]=dNodeForce[0][i];
			Data.dNodeForce[1][i]=dNodeForce[1][i];
			Data.dElemForce[0][i]=dElemForce[0][i];
			Data.dElemForce[1][i]=dElemForce[1][i];
		}
	}
};

#pragma pack()

#endif