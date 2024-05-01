varon(eduardo).
varon(francisco).
varon(luis).
varon(mario).
mujer(alicia).
mujer(veronica).
mujer(victoria).
mujer(beatriz).
padres(eduardo,francisco,victoria).
padres(alicia,francisco,victoria).
padres(luis,eduardo,veronica).
padres(beatriz,alicia,mario).
esposos(eduardo,veronica).
esposos(mario,alicia).
esposos(francisco,victoria).
hermana(Ella,X) :- mujer(Ella),padres(Ella,M,P),padres(X,M,P).
hermano(El,X) :- varon(El),padres(El,M,P), padres(X,M,P).
hijo(El,X) :- varon(El),padres(El,X,_).
hijo(El,X) :- varon(El),padres(El,_,X).
hija(Ella,X) :- mujer(Ella),padres(Ella,X,_).
hija(Ella,X) :- mujer(Ella),padres(Ella,_,X).

nieto(El, Y) :- varon(El), padres(El, X, _), (padres(X, Y, _) ; padres(X, _, Y)).
nieto(El, Y) :- varon(El), padres(El, _, X), (padres(X, Y, _) ; padres(X, _, Y)).

nieta(Ella, Y) :- mujer(Ella), padres(Ella, X, _), (padres(X, Y, _) ; padres(X, _, Y)).
nieta(Ella, Y) :- mujer(Ella), padres(Ella, _, X), (padres(X, Y, _) ; padres(X, _, Y)).

abuelo(El, X) :- varon(El), (padres(Y, El, _) ; padres(Y, _, El)), (padres(X, Y, _) ; padres(X, _, Y)).
abuela(Ella, X) :- mujer(Ella), (padres(Y, Ella, _) ; padres(Y, _, Ella)), (padres(X, Y, _) ; padres(X, _, Y)).

sobrino(El, X):- hijo(El, Y), (hermano(Y, X) ; hermana(Y, X)).
sobrina(Ella, X):- hija(Ella, Y), (hermano(Y, X) ; hermana(Y, X)).

cuñado(El, X):- esposos(El, Y), (hermano(Y, X) ; hermana(Y, X)).
cuñada(Ella, X):- esposos(Ella, Y), (hermano(Y, X) ; hermana(Y, X)).