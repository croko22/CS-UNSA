soporta(Y, X) :- encima(X, Y).
encima(X, Y) :- arriba(X, Y), toca(X, Y).

arriba(taza, libro).
toca(taza, libro).