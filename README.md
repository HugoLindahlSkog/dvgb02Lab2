# dvgb02Lab2
Detta är en kort implementation av stop and wait protokollet. Det vill säga rdt3.0 
Reliable data transfer. Pålitiglig dataöverföring mellan två noder.
Laborationen utförs i en simulerade miljö där paket kan gå förlorade och bli korrupta, varav protokollet hanterar detta. 

använd make för kompilering
kör simulering med: 
./sim <antal_paket> <förlustsannolikhet> <korruptionssannolikhet> <meddelandeintervall> <trace_nivå>
tillexempel ./sim 20 0.2 0.2 10 2
