#include "Person.h"
#include "Factory.h"
#include "Market.h"
#include "Scene.h"

#include <algorithm>
#include <string>
#include <sstream>
#include <assert.h>
#include <cstdlib>


int CPerson::m_nIdCount = 0;
CPerson::CPerson(CScene* pScene):m_pScene(pScene)
{
    //ctor

    std::cout << "CPerson Ctor started " << std::endl;
    m_nIdCount++;
    m_nId=m_nIdCount;
    Init();
//    std::cout << "After Init" << std::endl;
    m_dLastCollectedEarningsInGold=0.0;
    m_dLastConsumptionInGold=0.0;

    //write file
    //std::cout << "1" << std::endl;
    std::string strFileName="PersonDebug";
    //std::string strFileName2="pruba";
    //std::cout << "pruba?" << std::endl;
    //std::string strFileName="PersonDebug";
    //std::cout << "2" << std::endl;
    std::ostringstream strId;
    //std::cin.get();
    //std::cout << "3" << std::endl;

    //std::cin.get();
    strId << m_nId;
    //std::cin.get();
    //cout << strId << std::endl;
    //std::cout << "4" << std::endl;
    strFileName=strFileName+strId.str()+".txt";

//    std::cout << "Going to open " << strFileName << std::endl;
    m_DebugFile.open (strFileName.c_str());
//    std::cout << "After having tried to open " << strFileName << std::endl;


    m_DebugFile << "Writing in PersonDebug file.\n";

    m_DebugFile << "m_vdConsumptionMatrix:\n";

    m_DebugFile << "------" << "\n";

    for ( unsigned int nLevel=0;nLevel<m_vdConsumptionMatrix.size();nLevel++)
    {
        m_DebugFile << "\n\n";
        m_DebugFile << "Level: " << nLevel << ".\n";
        m_DebugFile << "------" << ".\n";
        for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
        {
            m_DebugFile << "Product: " << nProduct << ".\n";
            ePRODUCTS eProd = static_cast<ePRODUCTS>(nProduct);

            m_DebugFile << "Value: " << GetConsumptionValueForLevel(eProd,nLevel) << ".\n";

        }
    }

    m_DebugFile << "End of m_vdConsumptionMatrix\n";

    std::cout << "CPerson Ctor finished " << std::endl;

    m_DebugFile.flush();
}

CPerson::~CPerson()
{
    //dtor

    m_DebugFile.close();
}

void CPerson::Init()
{

    std::cout << "CPerson::Init started " << std::endl;
    COwner::Init();

//    int nParamConsumptionLevels=NUM_LEVELS_OF_CONSUMPTION;
//    m_vdConsumptionMatrix.resize(nParamConsumptionLevels);
//    for (int nLevel=0;nLevel<nParamConsumptionLevels;nLevel++)
//    {
//        m_vdConsumptionMatrix[nLevel].resize(NUM_PRODUCTS);
//    }


    m_vdConsumptionMatrix.resize(NUM_PRODUCTS);
    for (int nProduct=0;nProduct<NUM_PRODUCTS;nProduct++)
    {
        m_vdConsumptionMatrix[nProduct].resize(NUM_LEVELS_OF_CONSUMPTION);
    }


    std::cout << "Fill m_vdConsumptionMatrix started " << std::endl;

    m_vdConsumptionMatrix[GOLD  ][0]=0.01;
    m_vdConsumptionMatrix[LUMBER][0]=0.02;
    m_vdConsumptionMatrix[FOOD  ][0]=0.03;
    m_vdConsumptionMatrix[FABRIC][0]=0.02;

    m_vdConsumptionMatrix[GOLD  ][1]=0.01;
    m_vdConsumptionMatrix[LUMBER][1]=0.04;
    m_vdConsumptionMatrix[FOOD  ][1]=0.1;
    m_vdConsumptionMatrix[FABRIC][1]=0.04;

    m_vdConsumptionMatrix[GOLD  ][2]=0.1;
    m_vdConsumptionMatrix[LUMBER][2]=0.2;
    m_vdConsumptionMatrix[FOOD  ][2]=0.2;
    m_vdConsumptionMatrix[FABRIC][2]=0.2;

    m_vdConsumptionMatrix[GOLD  ][3]=0.3;
    m_vdConsumptionMatrix[LUMBER][3]=0.2;
    m_vdConsumptionMatrix[FOOD  ][3]=0.2;
    m_vdConsumptionMatrix[FABRIC][3]=0.3;

    m_vdConsumptionMatrix[GOLD  ][4]=0.5;
    m_vdConsumptionMatrix[LUMBER][4]=0.2;
    m_vdConsumptionMatrix[FOOD  ][4]=0.2;
    m_vdConsumptionMatrix[FABRIC][4]=0.4;

    m_vdConsumptionMatrix[GOLD  ][5]=0.6;
    m_vdConsumptionMatrix[LUMBER][5]=0.3;
    m_vdConsumptionMatrix[FOOD  ][5]=0.3;
    m_vdConsumptionMatrix[FABRIC][5]=0.6;

    std::cout << "CPerson::Init finished " << std::endl;

//    int nLevel=0;
//    vector< vector<double> >::iterator itCons:
//    for (itCons=m_vdConsumptionMatrix.begin();itCons!=m_vdConsumptionMatrix.end();itCons++)
//    {
//        for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
//        {
//            ePRODUCTS eProd=static_cast<ePRODUCTS>(nProduct);
//
//            //GOLD=0,
//            //LUMBER,
//            //FOOD,
//            //FABRIC,
//            double dValue=0.0;
//            if(0==nLevel)
//            {
//                if(GOLD==eProd)
//                    dValue=0.01;
//                else if (LUMBER==eProd)
//                    dVAlue=0.1;
//                else if (FOOD==eProd)
//                    dVAlue=0.1;
//                else if (FABRIC==eProd)
//                    dVAlue=0.05;
//            }
//            else if (1==nLevel)
//            {
//                if(GOLD==eProd)
//                    dValue=0.01;
//                else if (LUMBER==eProd)
//                    dVAlue=0.15;
//                else if (FOOD==eProd)
//                    dVAlue=0.15;
//                else if (FABRIC==eProd)
//                    dVAlue=0.15;
//            }
//            else if (2==nLevel)
//            {
//                if(GOLD==eProd)
//                    dValue=0.1;
//                else if (LUMBER==eProd)
//                    dVAlue=0.20;
//                else if (FOOD==eProd)
//                    dVAlue=0.20;
//                else if (FABRIC==eProd)
//                    dVAlue=0.20;
//            }
//            else if (3==nLevel)
//            {
//                if(GOLD==eProd)
//                    dValue=0.3;
//                else if (LUMBER==eProd)
//                    dVAlue=0.2;
//                else if (FOOD==eProd)
//                    dVAlue=0.2;
//                else if (FABRIC==eProd)
//                    dVAlue=0.3;
//            }
//
//        }
//
//        //
//        nLevel++;
//    }

}

double CPerson::CollectEarningsAndRegisterTheirValue()
{

    //std::cout << "Owned Factories: " << std::endl;
    m_DebugFile << "Owned Factories: " << ".\n";
    double dCollectedValueInGold=0.0;
    std::vector<CFactory*>::iterator itFact;
    for(itFact=m_vOwnedFactories.begin();itFact!=m_vOwnedFactories.end();itFact++)
    {
        //std::cout << "Owned Factory: " << (*itFact)->GetId() <<std::endl;
        m_DebugFile << "Owned Factory: " << (*itFact)->GetId() << ".\n";
        for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
        {
            ePRODUCTS eProduct = static_cast<ePRODUCTS>(nProduct);
            struct_Transaction strTrans = (*itFact)->MakeTransactionOfAll(this, eProduct);

            dCollectedValueInGold=strTrans.dAmount*m_pMarket->GetPrice(eProduct);
        }
    }

    m_dLastCollectedEarningsInGold = dCollectedValueInGold;

    return dCollectedValueInGold;

    //std::cout << "CollectEarningsAndRegisterItsValue finished " << std::endl;
}

long CPerson::CalculateLevelOfConsumption()
{
    //Dejando una cantidad en reserva, se intentará consumir tanto como se producía
    double dValueOfStock=this->GetValueOfAllProducts();
    double dParamMaxValueOfStock=1.0;

    double dConsumptionTarget=m_dLastCollectedEarningsInGold/2.0;
    if(dValueOfStock>dParamMaxValueOfStock)
    {
        dConsumptionTarget=m_dLastCollectedEarningsInGold+(dValueOfStock-dParamMaxValueOfStock);
    }

//    if(dConsumptionTarget>dValueOfStock)
//        dConsumptionTarget=dValueOfStock;

    unsigned int nLevel=0;
    //std::vector< std::vector<double> >::iterator itCons;
    for (nLevel=0;nLevel<m_vdConsumptionMatrix.size();nLevel++)
    {
        double dValueOfConsumptionInThisLevel=0.0;
        for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
        {
            ePRODUCTS eProd = static_cast<ePRODUCTS>(nProduct);
            dValueOfConsumptionInThisLevel=dValueOfConsumptionInThisLevel+GetConsumptionValueForLevel(eProd,nLevel)*m_pMarket->GetPrice(eProd);
        }
        if(dValueOfConsumptionInThisLevel>dConsumptionTarget)
            break;
    }

    //std::cout << "CalculateLevelOfConsumption finished " << std::endl;

    return nLevel;
}

double CPerson::GetConsumptionValueForLevel(ePRODUCTS eProd, int nLevel)
{
    assert(nLevel>=0);
    assert(nLevel<NUM_LEVELS_OF_CONSUMPTION);
    return m_vdConsumptionMatrix[eProd][nLevel];
}

void CPerson::BuyToConsumeAndRegisterConsumption()
{
    int nLevel=CalculateLevelOfConsumption();

    double dValueOfAllConsumedProduct=0.0;
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        ePRODUCTS eProd = static_cast<ePRODUCTS>(nProduct);

        double dValueOfNeededProductAmount=GetConsumptionValueForLevel(eProd,nLevel)*m_pMarket->GetPrice(eProd);

        //m_pMarket->BuyProducts(eProd,dValueOfNeededProductAmount);
        //SubtractProductAmount(GOLD,dValueOfNeededProductAmount);
        double dProductPrice = m_pMarket->GetPrice(eProd,GOLD);
        double dAmountOfProductToBuy = dValueOfNeededProductAmount/dProductPrice;
        struct_Exchange strExchange = m_pMarket->BuyFromMarket(this, eProd, dAmountOfProductToBuy, GOLD);

        dValueOfAllConsumedProduct=dValueOfAllConsumedProduct+strExchange.dAmountOwnerToMarket;

        //Consume the product
        this->ConsumeProducts(eProd,strExchange.dAmountMarketToOwner);

    }

    m_dLastConsumptionInGold=dValueOfAllConsumedProduct;
    m_nLastLevelOfConsumption=nLevel;

    m_DebugFile << "m_dLastConsumptionInGold: " << m_dLastConsumptionInGold << ".\n";
    m_DebugFile.flush();

    //std::cout << "BuyToConsumeAndRegisterConsumption finished " << std::endl;
}

void CPerson::Consume()
{
    BuyToConsumeAndRegisterConsumption();
}

void CPerson::AddFactory(CFactory* pFactory)
{
    std::vector<CFactory*>::iterator itFound = std::find(m_vOwnedFactories.begin(),m_vOwnedFactories.end(),pFactory);
    if(itFound==m_vOwnedFactories.end())
        m_vOwnedFactories.push_back(pFactory);
}

double CPerson::GetLastConsumptionInGold()
{
    return m_dLastConsumptionInGold;
}

std::string CPerson::GetConsumptionInfoText()
{
    std::string strStocksInfo;
    strStocksInfo=strStocksInfo+"\n";

//    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
//    {
//        int nLevelOfConsumption = m_nLastLevelOfConsumption;
//        double dConsumptionValue = this->GetConsumptionValueForLevel(static_cast<ePRODUCTS>(nProduct), nLevelOfConsumption);
//
//        std::stringstream strStream;
//        strStream.precision(2);
//        strStream << std::fixed << dConsumptionValue;
//        std::string strConsumptionValue = strStream.str();
//
//        std::string strProductName = GetNameOfProduct(static_cast<ePRODUCTS>(nProduct));
//        std::string strNewLine = "Product: " +strProductName+ ", \tConsumption: "+strConsumptionValue+" \n";
//        strStocksInfo=strStocksInfo+strNewLine;
//
//    }

    std::stringstream strStream;
    strStream.precision(2);
    strStream << std::fixed << GetLastConsumptionInGold();
    std::string strConsumption = strStream.str();

    std::string strNewLine = "Last consumption in gold: "+strConsumption+" \n";

    std::stringstream strStream2;
    strStream2.precision(2);
    strStream2 << std::fixed << m_nLastLevelOfConsumption;
    std::string strLevel = strStream2.str();

    std::string strNewLine2 = "Last level of consumption: "+strLevel+" \n";
    //strStocksInfo=strStocksInfo+"/n";
    return strStocksInfo=strStocksInfo+strNewLine+strNewLine2;
}

std::string CPerson::GetTotalConsumptionInfoText()
{
    std::string strStocksInfo;
    strStocksInfo=strStocksInfo+"\n";

    std::stringstream strStream;
    strStream.precision(2);
    strStream << std::fixed << GetLastConsumptionInGold();
    std::string strConsumption = strStream.str();

    strStocksInfo=strStocksInfo+strConsumption;
    return strStocksInfo;
}

void CPerson::TransformToMoney()
{
    //De momento transformo todo a GOLD
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        ePRODUCTS eProd = static_cast<ePRODUCTS>(nProduct);
        m_pMarket->SellToMarket(this,eProd,this->GetAmount(eProd));
    }
}

