void treeTest(  const TString fList = "temp.list",
                const TString fOutName = "fOut.root",
                Long64_t nEvents = 999
                ) 
{

	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();

    TFile* fOut = new TFile(fOutName.Data(),"RECREATE");

    TH1D* hZdcTree = new TH1D("hZdcTree","hZdcTree", 50, 0.5, 5000.5);
    TH1D* hZdcMuMaker = new TH1D("hZdcMuMaker","hZdcMuMaker", 50, 0.5, 5000.5);

	//----------------- Read data using trees ---------------//

    TChain* theChain = new TChain("MuDst","MuDst");
    readFileList(theChain, fList.Data());
    Long64_t nEntries = theChain->GetEntries();
    if(nEntries < nEvents) {nEvents = nEntries;}
    TBranch* event = theChain->GetBranch("MuEvent");
    TLeaf* leaf = new TLeaf();

	TStopwatch* timer = new TStopwatch();
    for(Int_t i = 0; i <= (nEvents-1); i++)
    {
        event->GetEntry(i);
        leaf = theChain->GetLeaf("MuEvent.mZdcTriggerDetector.mAdc");
        hZdcTree->Fill(leaf->GetValue(0));
        hZdcTree->Fill(leaf->GetValue(4));

    }
    
	Double_t treeTime = timer->RealTime();

	//----------------- Read data using StMuDstMaker ---------------//
	StChain* chain = new StChain("StChain");
	StMuDstMaker* muMaker = new StMuDstMaker(0,0,"",fList.Data(),"st:MuDst.root", 100, "MuDst");

    chain->Init();

	timer->Start();
    for(Int_t i = 0; i <= (nEvents-1); i++)
    {
		chain->Clear();
		chain->Make(i); 

       hZdcMuMaker->Fill(muMaker->muDst()->event()->zdcTriggerDetector().adc(4));
       hZdcMuMaker->Fill(muMaker->muDst()->event()->zdcTriggerDetector().adc(0));

    }
	Double_t muMakerTime = timer->RealTime();

    cout << "Tree time: " << treeTime << "\tMuMaker time: " << muMakerTime << endl;

    chain->Finish();
    fOut->Write();
    fOut->Close();

}

void readFileList(TChain* chain, const TString fileList)
{

    if(fileList.Data()!="") 
    { 
        if(fileList.Contains(".root")) 
        { 
            chain->Add(fileList.Data());
        } else if(fileList.Contains(".list")) {
            fillChain(chain, fileList.Data());
        }
    }

}

void fillChain(TChain* chain, const TString fileList) {
    ifstream* inputStream = new ifstream;

    inputStream->open(fileList.Data());
    char* fileName = new char[256];

    while(inputStream->good())
    {
        inputStream->getline(fileName,256);
        TString fileString(fileName);

        // Check for spaces, take only first word of filename
        if(fileString.Contains(" "))
        {
            TString temp = fileString(0,fileString.First(" "));
            fileString = temp.Data();
        }

        if(fileString.Contains(".root"))
        {
            chain->Add(fileString.Data());
        }

    }

    delete fileName;
    delete inputStream;
}
