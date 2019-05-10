## Obligatorio
1- Falta un informe con diagramas de clase y secuencia explicando el trabajo realizado.

12- Tanto en la función main como toda función principal que ejecutes en un hilo deberían tener un bloque `try...catch`
que capture una referencia a `std::exception` y otro catch mas que atrape las elipsis. Revisá el apunte de clase


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
paso de:
```
for (; it != this->clients.end(); ++it) {
	Thread* client = *it;
    if (client->isDead()) {
        client->join(); 
        delete client;
        this->clients.erase(it);
    }
}
```
a:
```
while (it != this->clients.end()) {
	Thread* client = *it;
    if (client->isDead()) {
        client->join(); 
	    delete client;
	    this->clients.erase(it);
	}
    else {
        ++it;
    }
}
```



NO LOGRO PONER THROW AL RECIVE DEL SOCKET


## DIAGRAMA CLASE CLIENTE:
title Relationships -Client Class Diagram


class Protocol {
    - skt : Socket
    + receive(n : uint8_t&)
    + send(n : uint8_t&)
}

class Socket {
    - skt : int
    + connectWithServer(host, port)
}


class Client

class Certificate {
    - serial_number : uint32_t
    - subject : std::string
    - issuer : std::string
    - not_before: std::string
    - not_after: std::string
    - key : std::string

    + send(protocol : Protocol&)
    + receive(protocol : Protocol&)
    + toString() : std::string
}

class Processor {
    - protocol : Protocol&
    + run(...);
}


Client *-down- Processor

Processor o-left- Protocol
Protocol o-- Socket

NewProcessor --> Certificate
RevokeProcessor --> Certificate

Processor <|-- NewProcessor
Processor <|-- RevokeProcessor

## DIAGRAM SERVER 1
class Acceptor { 
    - skt : Socket
    - index : Index&
    - keep_talking : bool
    - clients : std::vector<Thread*>
}

Class Thread {
    + run()
    + stop()
    + isDead()
}

Class Revoke {
    - index : &Index
    - protocol : &Protocol 
}

Class New {
    - index : &Index
    - protocol : &Protocol 
}

Server *-- Acceptor
Acceptor *-- ClientProcessor
Thread <|-left- Acceptor
Thread <|-- ClientProcessor
ClientProcessor <|-- Revoke
ClientProcessor <|-- New



## DIAGRAMA SERVER 2
class Protocol {
    - skt : Socket
    + receive(n : uint8_t&)
    + send(n : uint8_t&)
}

class Socket {
    - skt : int
    + connectWithServer(host, port)
}



class Certificate {
    - serial_number : uint32_t
    - subject : std::string
    - issuer : std::string
    - not_before: std::string
    - not_after: std::string
    - key : std::string

    + send(protocol : Protocol&)
    + receive(protocol : Protocol&)
    + toString() : std::string
}

ClientProcessor --> Rsa
ClientProcessor --> Hash

class Index {
    - certificates : std::map<std::string, Key>
    - mutex : std::mutex
    - serial_number : uint32_t
    + write()
    + save(certificate : Certificate&)
    + putBack(certificate : Certificate&, client_key : Key&)
    + find(cartificate : Certificate&) : Key
    + erase(certificate : Certificate&);
}


Class Revoke {
    - index : &Index
    - protocol : &Protocol 
}

Class New {
    - index : &Index
    - protocol : &Protocol 
}

ClientProcessor <|-left- Revoke
ClientProcessor <|-right- New

ClientProcessor --> Index

ClientProcessor --> Certificate   


Protocol --o Revoke
Protocol --o New 

Protocol o-up- Socket