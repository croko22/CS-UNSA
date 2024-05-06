/* Busqueda primero en profundida */
:- dynamic visitado/1.
:- dynamic vuelo/3.

/* preguntar el origen y el destino */
encontrar_ruta:-
    write("desde: "),
    read(A),
    write("a: "),read(B),
    ruta(A,B,D),
    write("distancia es "),
    write(D),
    nl,
    not(displayruta).

/*disponer para bien encontrar la ruta */
ruta(A,B,C):-
    es_vuelo(A,B,C).

/*o bien informar de fallo */
ruta(_,_,D):-!,
    write("no hay ruta o no"),
    nl,
    write("está dentro de la distancia especificada"),nl,
    D=0, purgar.

/* ver si hay conexion entre dos ciudades */
es_vuelo(T,T2,D):-
    vuelo(T,T2,D),
    añadir_a_ruta(T).
es_vuelo(T,T2,D):-
    vuelo(T,X,D2),
    \+ X = T2,
    añadir_a_ruta(T),
    es_vuelo(X,T2,D3),
    D is D2 + D3.
es_vuelo(T,_,D):-
    write("punto sin salida en "),
    write(T),
    nl, D=0, fail.
/* añadir a la lista de ciudades visitadas */
añadir_a_ruta(T):-
    not(visitado(T)),
    assert(visitado(T)),!.
añadir_a_ruta(_).
/* preparar otra ejecucion */
purgar:-
    retract(visitado(_)),
    fail,!.
/*mostrar las ciudades de la ruta */
displayruta:-
    write("la ruta es: "), nl,
    visitado(A),
    write(A),nl,
    fail,!.

/* programa*/
:- assert(vuelo(nueva_york,chicago,1000)),
assert(vuelo(chicago,denver,1000)),
assert(vuelo(nueva_york, toronto, 800)),
assert(vuelo(nueva_york, denver,1900)),
assert(vuelo(toronto, calgary,1500)),
assert(vuelo(toronto, los_angeles, 1800)),
assert(vuelo(toronto, chicago, 500)),
assert(vuelo(denver, urbana,1000)),
assert(vuelo(denver, houston, 1500)),
assert(vuelo(houston, los_angeles, 1500)),
assert(vuelo(denver, los_angeles, 1000)),

encontrar_ruta,nl, write("más? "), read(Q), Q = n.