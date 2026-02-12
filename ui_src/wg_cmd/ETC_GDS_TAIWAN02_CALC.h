#pragma once

struct stETC_GDS_TAIWAN02_CALC
{
    double dSDS, dSD1, dSMS, dSM1;
    double dSDS_1, dSD1_1;      // Near Fault Zone
    double dTd0, dTd0_1, dTm0;  // Trans. Period
    // PushOver Spectral Data
    double dBs, dB1;            // Defined by Damping
    double dTg_0, dTg_1, dTg_2; // 감쇠적용 후 교차점.

    void Init()
    {
        dSDS = 0.0, dSD1 = 0.0, dSMS = 0.0, dSM1 = 0.0;
        dSDS_1 = 0.0, dSD1_1 = 0.0;
        dTd0 = 0.0, dTd0_1 = 0.0, dTm0 = 0.0;

        dBs = 0.0, dB1 = 0.0;
        dTg_0 = 0.0, dTg_1 = 0.0, dTg_2 = 0.0;
    }
    stETC_GDS_TAIWAN02_CALC() { Init(); }
};