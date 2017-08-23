#ifndef CFACTORY_H
#define CFACTORY_H
#include <vector>
#include "Defines.h"
#include "Owner.h"
#include <fstream>

class CMarket;
class CPerson;
class CScene;

class CFactory: public COwner
{
    public:
        CFactory(CScene* pScene);
        virtual ~CFactory();

        long GetId(){return m_nId;}
        void Init();

        void AddProductivity(ePRODUCTS eProd, double dAmountOfProductivity);
        //void SetMarket(CMarket *pMarket){m_pMarket=pMarket;};
        void ManageStock();
        void Produce();


        //Log methods
        double GetLogLastProductionAmount(){return m_dLogLastProductionAmount;}
        ePRODUCTS GetLogLastProducedProduct(){return m_LogLastProducedProduct;}

        double GetProductivity(ePRODUCTS eProd){return m_vdProductivities[eProd];}
        double GetStock(ePRODUCTS eProd){return GetAmount(eProd);}
        void SetOwner(CPerson* pPerson){m_pOwnerPerson=pPerson;}

        std::string GetProductivityInfoText();
        //double SubtractAllProduct(ePRODUCTS eProd);
        std::string GetLastProducedProductsInfo();
    protected:
        void Produce(ePRODUCTS eProd);
        double CalculateGoldOutputOfProduct(ePRODUCTS eProd);
        ePRODUCTS SelectProduct();
        void SellProducts(ePRODUCTS eProdToSell, double nAmountToSell);

        //void AddProductAmount(ePRODUCTS eProd, double nAmount);
        //double SubtractProductAmount(ePRODUCTS eProd, double nAmount);



    protected:

        CScene* m_pScene;
        //CMarket* m_pMarket;
        CPerson* m_pOwnerPerson;

        std::vector<double> m_vdProductivities;
        //std::vector<double> m_vdProductStocks;



        //Id
        static int m_nIdCount;
        int m_nId;

        //Log variables
        double m_dLogLastProductionAmount;
        ePRODUCTS m_LogLastProducedProduct;

        //Debug
        std::ofstream m_DebugFile;


    private:
};

#endif // CFACTORY_H
