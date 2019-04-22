
#include <iostream>
#include <string>
#define ARGUMENT_ERROR_MSSG "Error: argumentos invalidos.\n"
#define ARGUMENT_ERROR 0
#define USER_ERROR_MSSG "Error: usuario no registrado.\n"
#define USER_ERROR 0
#define CERTIFICATE_ERROR_MSSG "Error: ya existe un certificado.\n"
#define CERTIFICATE_ERROR 0
#define HASH_ERROR_MSSG "Error: los hashes no coinciden.\n"
#define HASH_ERROR 0

/*
* new
*   ./client <ip/hostname> <puerto/servicio> new <claves clientes> 
*           <pública servidor> <información certificado>
* 
* <información certificado> 
*   Federico Manuel Gomez Peter         <---subject
*   Mar 28 21:33:04 2019                MMM DD HH:mm:SS YYYY
*   May 27 21:33:04 2019                MMM DD HH:mm:SS YYYY
*/

/*
* revoke
*   ./client <ip/hostname> <puerto/servicio> revoke <claves clientes> 
*           <pública servidor> <certificado>
*
* ME PERTURBA QUE LA PRUEBA SEA :
*   revoke "Federico Manuel Gomez Peter.cert" client0.keys server.pubkeys
*
*/

/* 
* <claves clientes>
*   <exp_publico> <exp_privado> <modulo>
*           => clave publ = (exp_publico, modulo)
*           => clave priv = (exp_privado, modulo)
*
* <pública servidor>
*   <exp_publico> <modulo>
*
* Los distintos campos pueden estar separados por 1 o más espacios, únicamente
* se garantiza que estos se encuentran en una misma línea.
*
*
*
* ENVIAR:
* 
*  Comando:     - 1 byte sin signo
*
*  Subject:     - Longitud: 4 bytes big endian sin signo.
*               - String sin ‘\0’
*
*  Módulo:      - 2 bytes en big endian sin signo
* 
*  Exponente:   - 1 byte
*
*  Fechas:      - Longitud: 4 bytes big endian sin signo.
*               - String sin ‘\0’
*
*
* ANSWER: ​
*  0 -> subject ya tiene un certificado vigente => CERTIFICATE_ERROR_MSSG
*  1 ​-> El certificado creado por el servidor.
*    -> El hash del certificado, encriptado con la clave privada del servidor y
*       luego encriptado con la clave pública del cliente.
*
* IF (TODO OK) donde TODO OK = recibir 1 + datos
*   1- Desencriptar el  hash, primero usando su  propia clave  privada, y luego
*      usando la clave pública del servidor. 
*   2- Calcula el hash del certificado recibido 
*   3- compara si este cálculo coincide con el del que recibió del servidor. 
*       3.T- Guarda el certificado en un archivo
*            Notifica al servidor su correcta recepción (enviar uint8_t 0)
*            Termina su ejecución normalmente.
*       3.F- Notifica al servidor la falla (enviar uint8_t 1)​ , 
*            Cierra su ejecución escribiendo en pantalla HASH_ERROR_MSSG
*/

/*
* revoke
*   ./client <ip/hostname> <puerto/servicio> revoke <claves clientes> 
*           <pública servidor> <certificado>
*
*
* huella del certificado -> como un entero de 4 bytes sin signo en big endian
*
*
* El certificado se debe enviar pasando los valores de los distintos atributos 
* en el orden que aparecen en el archivo (primero se envía el valor del número
* de serie, luego el  subject, siguiendo  por el valor del issuer, la fecha de 
* inicio, la fecha de finalización, el módulo del cliente y luego el exponente)
*/

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cout << ARGUMENT_ERROR_MSSG;
        return ARGUMENT_ERROR;
    }

    std::string modo1("new");
    std::string modo2("revoke");
    std::string mode = std::string(argv[3]);
    if (mode != modo1 && mode != modo2) {
        std::cout << ARGUMENT_ERROR_MSSG;
        return ARGUMENT_ERROR;
    }

    /*char* host = argv[1];
    char* port = argv[2];
    char* client_key_filename = argv[4];
    char* server_key_filename = argv[5];
    char* certificate = argv[6];*/
    
    return 0;
}