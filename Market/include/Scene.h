#ifndef CSCENE_H
#define CSCENE_H

#include "Defines.h"
#include <vector>

class CMarket;
class CFactory;
class CPerson;

class CScene
{
    public:
        CScene();
        virtual ~CScene();

        void CreateTestScene();

        void MakeAStep();
        void SimulateStepsToEstimatePrices();
        CMarket* GetMarket(){return m_pMarket;};
        std::vector<CFactory*> GetFactories(){return m_vFactories;}

        long GetCycle() {return m_nCycle;}
        void SetCycle(long nCycle) {m_nCycle=nCycle;}
        long GetPriceEstimationStep(){return m_nPriceEstimationStep;}
        bool ArePricesBeingEstimated(){return m_bEstimatingPrices;}

    protected:

        void CreateTestMarket();
        void CreateTestFactories();
        void CreateTestPersons();

        void MakeAStepInFactories();
        void MakeAStepInPersons();
        void RecoverSavedStocksOfFactories();
        void RecoverSavedStocksOfPersons();

        void SaveStocksOfFactories();
        void SaveStocksOfPersons();

    protected:
        CMarket* m_pMarket;
        std::vector<CFactory*> m_vFactories;
        std::vector<CPerson*> m_vPersons;

        long m_nCycle;
        long m_nPriceEstimationStep;
        bool m_bEstimatingPrices;

    private:
};

#endif // CSCENE_H
