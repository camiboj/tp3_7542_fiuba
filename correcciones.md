## Obligatorio
1- Falta un informe con diagramas de clase y secuencia explicando el trabajo realizado.

12- Tanto en la función main como toda función principal que ejecutes en un hilo deberían tener un bloque `try...catch`
que capture una referencia a `std::exception` y otro catch mas que atrape las elipsis. Revisá el apunte de clase

13- Es peligroso como estás eliminando los clientes terminados. Si eliminaste el nodo donde apunta `it` del vector, ¿A
donde va a apuntar `it++` ? Podés tener corrupción de memoria así. El método erase te devuelve un iterator que 
apunta al nodo posterior del eliminado. Tenés que pisar tu variable iterator para que no tengas corrupción de memoria. 
Notar que si la función te devuelve el nodo siguiente al eliminado en esa iteración no deberías incrementar el iterator
(porque sinó te habría un cliente que no estarías eliminando de estar finalizado). ***mail enviado***

16- Tenés race conditions. Están mal definidas tus funciones atómicas. ¿Las funciones atómicas son agregar, chequear,
    borrar , o deberían ser agregarSiNoExiste, borrarSiExiste ?. Revisar apuntes de clase y evaluar si existen otras
    critical sections. Pensar que pasaría (teniendo tu código así como está), si dos hilos quieren crear un certificado para el
    mismo subject. ¿Qué pasaría si un hilo quiere crear un certificado y al mismo tiempo otro cliente quiere revocar un
    certificado para ese subject?.    

## Puntos
8- Evitá usar la función atoi . Es una función que no chequea errores y es insegura. En C te diría que uses stol, en C++
recomiendo usar el operator>> .

9- Evitá que los constructores tengan lógica extra que no sea la de inicializar atributos.

11- Revisar la biblioteca iomanip para ver como manipular la salida de un número o un time_t .


MySocket ahora se llama Protocol (2)
Las impresiones de error de cliente son excepciones catcheadas (5)
Protocol ahora tien dos metodos nada mas, pero sobreescritos. Es mas linda la clase. (ninguna)
Server y client encerrados en un gran try catch que no deja escapar nada. (12)

socket conect whit client y connect with server lanzan errores 
