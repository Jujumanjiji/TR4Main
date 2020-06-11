#pragma once

class W2V_MATRIX
{
public:
    void SetupMatrix_Ptr();
    void SetupMatrix_CosAndSin(PHD_3DPOS* viewpos);
    void Matrix00();
    void Matrix01();
    void Matrix02();
    void Matrix03();
    void Matrix10();
    void Matrix11();
    void Matrix12();
    void Matrix13();
    void Matrix20();
    void Matrix21();
    void Matrix22();
    void Matrix23();
    void SetupPoisonEffect();
    void AspectCorrection(int mXX);

    short poison;
private:
    PHD_MATRIX* nptr, *mptr;
    PHD_3DPOS* viewpos;
    PHD_VECTOR s, c;
    PHD_VECTOR pos;
    short angx, angy, angz;
    short power;
};

extern W2V_MATRIX w2v;
