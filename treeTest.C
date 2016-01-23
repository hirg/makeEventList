void treeTest(  const TString fList = "temp.list",
                const TString fOutName = "fOut.root",
                Long64_t nEvents = 9999
                ) 
{

	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    gROOT->LoadMacro("makeEventList.C");
	loadSharedLibraries();

    TFile* fOut = new TFile(fOutName.Data(),"RECREATE");

    TH1D* hZdc = new TH1D("hZdc","hZdc", 50, 0.5, 5000.5);

	//----------------- Read data using StMuDstMaker ---------------//
	StChain* muChain = new StChain("StChain");
	StMuDstMaker* muMaker = new StMuDstMaker(0,0,"",fList.Data(),"st:MuDst.root", 100, "MuDst");

    muChain->Init();
    Float_t zdcCut = 1623;

    TEventList* eventList = makeEventList(muMaker->chain(), zdcCut);
    muMaker->SetEventList(eventList);
    Long64_t nList = eventList->GetN();
    if(nList < nEvents) {nEvents = nList;}

    for(Int_t i = 0; i <= (nEvents-1); i++)
    {
        muChain->Make();
        Float_t zdcWest = muMaker->muDst()->event()->zdcTriggerDetector().adc(0);
        Float_t zdcEast = muMaker->muDst()->event()->zdcTriggerDetector().adc(4);

        hZdc->Fill(zdcWest);
        hZdc->Fill(zdcEast);


    }

    muChain->Finish();
    fOut->Write();
    fOut->Close();

    delete muChain;
    delete eventList;

}
