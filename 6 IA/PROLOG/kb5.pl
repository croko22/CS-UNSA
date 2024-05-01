tran(eins,one). 
tran(zwei,two). 
tran(drei,three). 
tran(vier,four). 
tran(fuenf,five). 
tran(sechs,six). 
tran(sieben,seven). 
tran(acht,eight). 
tran(neun,nine). 

listtran([],[]).
listtran([G|T1],[E|T2]) :- tran(G,E), listtran(T1,T2).