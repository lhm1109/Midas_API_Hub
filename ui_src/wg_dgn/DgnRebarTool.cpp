#include "StdAfx.h"

#include "../wg_db/UnitCtrl.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/MatlDB.h"

#include "DgnRebarTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDgnRebarTool::CDgnRebarTool()
{
    m_pDoc = CDBDoc::GetDocPoint();
}

CDgnRebarTool::~CDgnRebarTool()
{
}

void CDgnRebarTool::CalcPlateAs(T_RPCM_D& RpcmD, double& dAsTop, double& dAsBot)
{
    dAsTop = 0.0;
    dAsBot = 0.0;

    T_UNIT_INDEX CurIndex;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
    m_pDoc->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);

    double dUnitLen;
    if ( CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT )
    {
        dUnitLen = M_InitValueCurUnit(1, N, FT, D_UNITSYS_BASE_LENGTH);
    }
    else
    {
        dUnitLen = M_InitValueCurUnit(1, N, M, D_UNITSYS_BASE_LENGTH);
    }

    const double dZero = 1.0E-7;
    for ( int i=0; i<RpcmD.nTopLayer; i++ )
    {
        // Top.
        double dAs1=0.0, dAs2=0.0;
        if ( RpcmD.strTopBarNa1[i]!=_T("") )
            dAs1 = m_pDoc->m_pMatlDB->Get_RebarArea(RpcmD.strTopBarNa1[i]);
        if ( RpcmD.strTopBarNa2[i]!=_T("") )
            dAs2 = m_pDoc->m_pMatlDB->Get_RebarArea(RpcmD.strTopBarNa2[i]);
        if ( dAs2<dZero ) dAs2 = dAs1;

        double dBarNum1;
        double dBarNum2;

        double dCTC;
        if ( RpcmD.bNum )
        {
            dCTC = dUnitLen / RpcmD.nTopBarNum[i];
        }
        else
        {
            dCTC = RpcmD.dTopBarCTC[i];
        }

        if ( dCTC == 0 )
        {
            dBarNum1 = 0;
        }
        else
        {
            dBarNum1 = dUnitLen / dCTC;
        }
        dBarNum1 = dBarNum1 / 2;
        dBarNum2 = dBarNum1;

        dAsTop += (dAs1*dBarNum1 + dAs2*dBarNum2);
    }
    for ( int i = 0; i < RpcmD.nBotLayer; i++ )
    {
        // Bot.
        double dAs1=0.0, dAs2=0.0;
        if ( RpcmD.strBotBarNa1[i]!=_T("") )
            dAs1 = m_pDoc->m_pMatlDB->Get_RebarArea(RpcmD.strBotBarNa1[i]);
        if ( RpcmD.strBotBarNa2[i]!=_T("") )
            dAs2 = m_pDoc->m_pMatlDB->Get_RebarArea(RpcmD.strBotBarNa2[i]);
        if ( dAs2<dZero ) dAs2 = dAs1;

        double dBarNum1;
        double dBarNum2;

        double dCTC;
        if ( RpcmD.bNum )
        {
            dCTC = dUnitLen / RpcmD.nBotBarNum[i];
        }
        else
        {
            dCTC = RpcmD.dBotBarCTC[i];
        }

        if ( dCTC == 0 )
        {
            dBarNum1 = 0;
        }
        else
        {
            dBarNum1 = dUnitLen / dCTC;
        }
        dBarNum1 = dBarNum1 / 2;
        dBarNum2 = dBarNum1;

        dAsBot += (dAs1*dBarNum1 + dAs2*dBarNum2);
    }
}