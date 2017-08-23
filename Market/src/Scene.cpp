#include "Scene.h"

#include "Market.h"
#include "Factory.h"
#include "Person.h"
#include <iostream>


CScene::CScene()
{
    //ctor
    m_pMarket=NULL;
    m_nCycle=0;
    m_nPriceEstimationStep=0;
    m_bEstimatingPrices=false;
}

CScene::~CScene()
{
    //dtor
    delete m_pMarket;
    std::vector<CFactory*>::iterator itFact;
    for (itFact=m_vFactories.begin();itFact!=m_vFactories.end();itFact++)
    {
        delete (*itFact);
    }
    std::vector<CPerson*>::iterator itPerson;
    for (itPerson=m_vPersons.begin();itPerson!=m_vPersons.end();itPerson++)
    {
        delete (*itPerson);
    }
}

void CScene::CreateTestScene()
{
    CreateTestMarket();
    CreateTestFactories();
    CreateTestPersons();
}

void CScene::CreateTestPersons()
{
    std::cout << "CreateTestPersons started " << std::endl;
    //a person for each factory
    std::vector<CFactory*>::iterator itFact;
    for (itFact=m_vFactories.begin();itFact!=m_vFactories.end();itFact++)
    {
        CPerson* pPerson=new CPerson(this);

        (*itFact)->SetOwner(pPerson);
        pPerson->AddFactory(*itFact);
        pPerson->SetMarket(m_pMarket);
        m_vPersons.push_back(pPerson);

        std::cout << "Person id " <<pPerson->GetId() << std::endl;
        std::cout << "Factory id " <<(*itFact)->GetId() << std::endl;
    }

    std::cout << "CreateTestPersons finished " << std::endl;
}

void CScene::CreateTestMarket()
{
    std::cout << "CreateTestMarket started " << std::endl;
    m_pMarket = new CMarket(this);
}
void CScene::CreateTestFactories()
{
    std::cout << "CreateTestFactories started " << std::endl;
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {

        CFactory* pFactory= new CFactory(this);
        pFactory->Init();
        pFactory->SetMarket(m_pMarket);
        double dParamIncreaseInProductivity=0.1;
        pFactory->AddProductivity(static_cast<ePRODUCTS>(nProduct),dParamIncreaseInProductivity);
        m_vFactories.push_back(pFactory);

        std::cout << "Factory " << pFactory->GetId() << " created" << std::endl;
        std::cout << "productivity " << pFactory->GetProductivityInfoText() << std::endl;
    }
}


void CScene::MakeAStepInFactories()
{
    //std::cout << "Inside MakeAStep of Cycle " << this->m_nCycle << std::endl;
    //std::cout << "Number of persons " << m_vPersons.size()<<  std::endl;
    //std::cout << "Number of factories " << m_vFactories.size()<<  std::endl;
    std::vector<CFactory*>::iterator itFact;
    for (itFact=m_vFactories.begin();itFact!=m_vFactories.end();itFact++)
    {
        //std::cout << "Going to Produce in factory " << (*itFact)->GetId() <<  std::endl;
        (*itFact)->Produce();
        //std::cout << "Going to ManageStocks in factory " << (*itFact)->GetId() <<  std::endl;
        (*itFact)->ManageStock();
    }
}

void CScene::MakeAStepInPersons()
{
    std::vector<CPerson*>::iterator itPerson;
    for (itPerson=m_vPersons.begin();itPerson!=m_vPersons.end();itPerson++)
    {
        if (false==m_bEstimatingPrices) std::cout << "Stocks of person " << (*itPerson)->GetId() << " before collecting: " << (*itPerson)->GetStocksInfoText() << std::endl;
        //std::cout << "Going to CollectEarningsAndRegisterTheirValue in person " << (*itPerson)->GetId() <<  std::endl;
        (*itPerson)->CollectEarningsAndRegisterTheirValue();
        if (false==m_bEstimatingPrices) std::cout << "Stocks of person " << (*itPerson)->GetId() << " after collecting: " << (*itPerson)->GetStocksInfoText() << std::endl;
        //std::cout << "Going to Consume in person " << (*itPerson)->GetId() <<  std::endl;
        (*itPerson)->TransformToMoney();
        if (false==m_bEstimatingPrices) std::cout << "Stocks of person " << (*itPerson)->GetId() << " after transforming them to money: " << (*itPerson)->GetStocksInfoText() << std::endl;
        (*itPerson)->Consume();
        if (false==m_bEstimatingPrices) std::cout << "Stocks of person " << (*itPerson)->GetId() << " after consumption: " << (*itPerson)->GetStocksInfoText() << std::endl;
        if (false==m_bEstimatingPrices) std::cout << "Consumtion info of person " << (*itPerson)->GetId() << ": " <<(*itPerson)->GetConsumptionInfoText() << std::endl;

    }
}


void CScene::MakeAStep()
{
    if (false==m_bEstimatingPrices)
        this->m_nCycle++;

    MakeAStepInFactories();
    MakeAStepInPersons();
}

void CScene::SaveStocksOfFactories()
{
    //Freeze stocks of owners
    std::vector<CFactory*>::iterator itFact;
    for (itFact=m_vFactories.begin();itFact!=m_vFactories.end();itFact++)
    {
//        std::cout << "Saving stocks of factory " << (*itFact)->GetId() << std::endl;
        (*itFact)->SaveStocks();
//        std::cout << "Stocks of factory being saved: " << (*itFact)->GetStocksInfoText() << std::endl;

    }
}

void CScene::SaveStocksOfPersons()
{
    std::vector<CPerson*>::iterator itPerson;
    for (itPerson=m_vPersons.begin();itPerson!=m_vPersons.end();itPerson++)
    {
//        std::cout << "Saving stocks of person " << (*itPerson)->GetId() << std::endl;
        (*itPerson)->SaveStocks();
//        std::cout << "Stocks of person being saved: " << (*itPerson)->GetStocksInfoText() << std::endl;
    }
}

void CScene::RecoverSavedStocksOfFactories()
{
    //Unfreeze stocks of owners
    std::vector<CFactory*>::iterator itFact;
    for (itFact=m_vFactories.begin();itFact!=m_vFactories.end();itFact++)
    {
//        std::cout << "Production info of factory " << (*itFact)->GetId() << ": " <<(*itFact)->GetLastProducedProductsInfo() << std::endl;

        (*itFact)->RecoverSavedStocks();
    }
}

void CScene::RecoverSavedStocksOfPersons()
{
    std::vector<CPerson*>::iterator itPerson;
    for (itPerson=m_vPersons.begin();itPerson!=m_vPersons.end();itPerson++)
    {
//        std::cout << "Consumtion info of person " << (*itPerson)->GetId() << ": " <<(*itPerson)->GetConsumptionInfoText() << std::endl;
        (*itPerson)->RecoverSavedStocks();
    }
}


void CScene::SimulateStepsToEstimatePrices()
{
    //m_pMarket->StimatePrices(); perhaps this is a better way

    std::cout << "Inside SimulateStepsToEstimatePrices of Cycle " << this->m_nCycle << std::endl;

    SaveStocksOfFactories();
    SaveStocksOfPersons();

//    std::cout << "Saving stocks of Market " << std::endl;
    m_pMarket->SaveStocks();
//    std::cout << "Stocks of Market being saved: " << m_pMarket->GetStocksInfoText() << std::endl;
    m_pMarket->RecoverStocksForPrices();
//    std::cout << "Stocks for prices of Market being recovered: " << m_pMarket->GetStocksInfoText() << std::endl;
//    std::cout << "Prices before estimation: " << m_pMarket->GetPricesInfoText() << std::endl;
    //Make steps to estimate
    m_nPriceEstimationStep=0;

    m_bEstimatingPrices=true;

    int nStepsToEstimatePrices=100;
    if (0==m_nCycle)
        nStepsToEstimatePrices=5000;

    for (int i=0;i<nStepsToEstimatePrices;i++)
    {
        std::vector<double> vPricesBeforeStep=m_pMarket->GetStocks();
        m_nPriceEstimationStep++;
        MakeAStep();
        std::vector<double> vPricesAfterStep=m_pMarket->GetStocks();
        double dChangeInPrices=COwner::DifferenceBetweenStocks(vPricesBeforeStep,vPricesAfterStep);
        std::cout << "ChangeInPrices: " << dChangeInPrices << " Cycle: " << m_nCycle << " EstiStep: " << i << std::endl;

        std::cout << m_pMarket->GetPricesInfoText() << std::endl;

        m_pMarket->SetStocksForPrices(m_pMarket->GetStocks()); //actualizo precios con los cambios en el stock
    }
    m_bEstimatingPrices=false;

    m_pMarket->SetStocksForPrices(m_pMarket->GetStocks());

    std::cout << "Prices after estimation: " << m_pMarket->GetPricesInfoText() << std::endl;
    m_pMarket->RecoverSavedStocks();//No sé si recuperar estos stocks

    RecoverSavedStocksOfFactories();
    RecoverSavedStocksOfPersons();

    std::cout << "End of SimulateStepsToEstimatePrices of Cycle " << this->m_nCycle << std::endl;
}
