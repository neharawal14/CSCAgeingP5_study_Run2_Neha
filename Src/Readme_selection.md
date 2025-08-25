# Major points in  code : 

1. Applying the condition that atleast 5 hits are needed to make a segment 
  old code line 351, and 378  
```if((Int_t)(*fcscSegments_recHitRecord_endcap)[i].size() >= minhitpersegment )
 ```
2. Segment should be single segment in the chamber
old code line 390  , 459
```          if(m_nsegments_chamber[key_chmb]==1) {
```
the condition in line 422, to have single segment chamber
```  if(m_Single_cscSegments_recHitRecordX.size() > 0) {/
```

3. Major change - to count both the muons in the CSC  
old code line 432 removed 
```     if(trackne == 1) { // use events with single muon track
```
4. Count only those layers with single hit in a layer   
