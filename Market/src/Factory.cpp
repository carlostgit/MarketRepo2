#include "Factory.h"
#include "Market.h"
#include "Scene.h"
#include <assert.h>
#include <iostream>
#include <sstream>

int CFactory::m_nIdCount = 0;
CFactory::CFactory(CScene* pScene):m_pScene(pScene)
{
    //ctor

    m_LogLastProducedProduct=GOLD;
    m_dLogLastProductionAmount=0.0;

    std::cout << "CFactory Ctor started " << std::endl;
    m_nIdCount++;
    m_nId=m_nIdCount;
    m_pOwnerPerson=0;
    Init();

    //write file
    std::string strFileName="FactoryDebug";
    std::stringstream strId;
    strId << m_nId;
    strFileName=strFileName+strId.str()+".txt";

    m_DebugFile.open (strFileName.c_str());
    m_DebugFile << "Writing in FactoryDebug file.\n";

    m_DebugFile << "------" << "\n";

}

CFactory::~CFactory()
{
    //dtor
}

void CFactory::Init()
{
    COwner::Init();
    assert(GetProductAmountsCopy().size()==NUM_PRODUCTS);
    m_vdProductivities.clear();
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdProductivities.push_back(0.1);
    }
}

void CFactory::AddProductivity(ePRODUCTS eProd, double dAmountOfProductivity)
{
    m_vdProductivities[eProd] = GetProductivity(eProd)+dAmountOfProductivity;
}

void CFactory::ManageStock()
{
    m_DebugFile << "Starting CFactory::ManageStock() \n";

    m_DebugFile << "ArePricesBeingEstimated "<< m_pScene->ArePricesBeingEstimated() << "\n";
    m_DebugFile << "Estimating Step "<< m_pScene->GetPriceEstimationStep() << "\n";
    m_DebugFile << "On step number "<< m_pScene->GetCycle() << "\n";
    //Simple rule to start with: sell all products except gold
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        ePRODUCTS eProd=static_cast<ePRODUCTS>(nProduct);
        SellProducts(eProd,GetAmount(eProd));
        m_DebugFile << "Selling product " << eProd << "\n";
    }
    m_DebugFile << "End of CFactory::ManageStock() \n";
}

void CFactory::SellProducts(ePRODUCTS eProdToSell, double dAmountToSell)
{
    m_DebugFile << "Trying to sell amount " << dAmountToSell << "\n";

    struct_Exchange strExchange = m_pMarket->SellToMarket(this, eProdToSell, GetAmount(eProdToSell) , GOLD);

    m_DebugFile << "Sold products " << strExchange.dAmountOwnerToMarket << "\n";

//    if(this->FrozenStocks())
//    {
//        this->CreateProducts(eProdToSell, strExchange.dAmountOwnerToMarket);
//        m_DebugFile << "FrozenStocks \n";
//
//    }
}

void CFactory::Produce()
{
    m_DebugFile << "CFactory::Produce()\n";
    ePRODUCTS eBestProduct=SelectProduct();
    m_DebugFile << "Product selected " << eBestProduct << "\n";
    m_LogLastProducedProduct=eBestProduct;
    Produce(eBestProduct);
    m_DebugFile << "End of CFactory::Produce()\n";
    m_DebugFile.flush();
}

void CFactory::Produce(ePRODUCTS eProd)
{
    double dAmountToProduce = GetProductivity(eProd);
    m_DebugFile << "Productivity " << dAmountToProduce << "\n";
    double dAmountReallyProduced = this->CreateProducts(eProd,dAmountToProduce);
    m_DebugFile << "AmountReallyProduced " << dAmountReallyProduced << "\n";
    m_dLogLastProductionAmount=dAmountReallyProduced;
}

ePRODUCTS CFactory::SelectProduct()
{
    double dBestGoldOutput=0;
    ePRODUCTS eBestProduct=GOLD;

    for (int nProduct=0;nProduct<NUM_PRODUCTS;nProduct++)
    {
        double dGoldOutput=CalculateGoldOutputOfProduct(static_cast<ePRODUCTS>(nProduct));
        if(dGoldOutput>dBestGoldOutput)
        {
            eBestProduct=static_cast<ePRODUCTS>(nProduct);
            dBestGoldOutput=dGoldOutput;
        }
    }
    return eBestProduct;
}

double CFactory::CalculateGoldOutputOfProduct(ePRODUCTS eProd)
{
    return m_pMarket->GetPrice(eProd)*this->GetProductivity(eProd);
}

std::string CFactory::GetProductivityInfoText()
{
    std::string strProductivityInfo;
    strProductivityInfo=strProductivityInfo+"\n";
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        double dProductProductivity = m_vdProductivities[nProduct];

        std::stringstream strStream;
        strStream.precision(2);
        strStream << std::fixed << dProductProductivity;
        std::string strProductProductivity = strStream.str();

        std::string strProductName = GetNameOfProduct(static_cast<ePRODUCTS>(nProduct));
        std::string strNewLine = "Product: " +strProductName+ ", \tProductivity: "+strProductProductivity+" \n";
        strProductivityInfo=strProductivityInfo+strNewLine;

    }
    //strStocksInfo=strStocksInfo+"/n";
    return strProductivityInfo;
}

std::string CFactory::GetLastProducedProductsInfo()
{
    std::string strProductionInfo;
    strProductionInfo=strProductionInfo+"\n";

    std::stringstream strStream;
    strStream.precision(2);
    strStream << std::fixed << m_dLogLastProductionAmount;
    std::string strLastProductionAmount = strStream.str();



    std::string strProductName = GetNameOfProduct(static_cast<ePRODUCTS>(m_LogLastProducedProduct));

    std::string strNewLine = "Product selected: " +strProductName+ ", \tProduced Amount: "+strLastProductionAmount+" \n";

    strProductionInfo=strProductionInfo+strNewLine;

    return strProductionInfo;
}
