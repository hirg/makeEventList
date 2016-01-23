TEventList* makeEventList(TTree* tree, const Float_t zdcCut) 
{

	//----------------- Read data using trees ---------------//

	TStopwatch* timer = new TStopwatch();
    TEventList* eventList = new TEventList();
    Long64_t nEntries = tree->GetEntries(); 
    tree->SetBranchStatus("*",0);
    tree->SetBranchStatus("MuEvent.mZdcTriggerDetector.mAdc*",1);
	Double_t overheadTime = timer->RealTime();

	timer->Start();
    for(Int_t i = 0; i <= (nEntries-1); i++)
    {
        tree->GetEntry(i);
        TLeaf* muLeaf = tree->GetLeaf("MuEvent.mZdcTriggerDetector.mAdc");
        Float_t zdcWest = muLeaf->GetValue(0);
        Float_t zdcEast = muLeaf->GetValue(4);

        if( (zdcWest <= zdcCut) && (zdcEast <= zdcCut)) {eventList->Enter(i);}

    }
	Double_t eventLoopTime = timer->RealTime();

    cout << "\n\n***** makeEventList() Finished *****\n";
    cout << "Overhead time: " << overheadTime << "  Event loop time: " << eventLoopTime << endl;
    cout << "Processed " << nEntries << " entries. Added " << eventList->GetN() << " entries to the event list.\n\n";

    delete timer;

    return eventList;
}

