#ifndef CPERSON_H
#define CPERSON_H

#include "Defines.h"
#include "Owner.h"

#include <vector>
#include <iostream>
#include <fstream>


class CFactory;
class CMarket;
class CScene;

class CPerson: public COwner
{
    public:
        CPerson(CScene *pScene);
        virtual ~CPerson();

        long GetId(){return m_nId;}
        void Init();
        //void SetMarket(CMarket* pMarket){m_pMarket=pMarket;}
        void AddFactory(CFactory* pFactory);

        double CollectEarningsAndRegisterTheirValue();
        void Consume();

        double GetConsumptionValueForLevel(ePRODUCTS eProd, int nLevel);

        double GetLastConsumptionInGold();

        std::string GetConsumptionInfoText();
        std::string GetTotalConsumptionInfoText();

        void TransformToMoney();

    protected:

        long CalculateLevelOfConsumption();
        void BuyToConsumeAndRegisterConsumption();

    protected:

        CScene *m_pScene;
    //Consumer curves
    //    GOLD=0,
    //    LUMBER,
    //    FOOD,
    //    FABRIC,
    //    NUM_PRODUCTS
    //Consumption of each prod, for each level
        std::vector< std::vector<double> > m_vdConsumptionMatrix;

        std::vector<CFactory*> m_vOwnedFactories;

        //CWorkingPlace* m_pWorkingPlace

        //CMarket* m_pMarket;

        //std::vector<double> m_vdProductStocks;

        //Log
        double m_dLastCollectedEarningsInGold;
        double m_dLastConsumptionInGold;
        long m_nLastLevelOfConsumption;

        //Id
        static int m_nIdCount;
        int m_nId;

        //Debug
        std::ofstream m_DebugFile;

    private:


};

#endif // CPERSON_H
