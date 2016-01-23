#makeEventList
---
This repo contains a useful snippet of code for processing data files in the ROOT analysis framework. `makeEventList.C` gets passed a pointer to a TTree and a cut value, turns off all but one relevant TBranch, and adds all entries that pass the given cut to an event list, which the function returns. `treeTest.C` is just a wrapper macro that shows how to use `makeEventList()` in the context of the STAR analsysis library (StChain, StMuDstMaker, etc.)
