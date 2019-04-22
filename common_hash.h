#ifndef HASH_H_
#define HASH_H_
#include <stdint.h>
#include <string>

/*
* Recibe un texto que es cargado de a segmentos y lo hashea.
*/
class Hash {
    private:
        uint16_t count;
    
    public:
        Hash();
        ~Hash();
        
        /*
        * Recibe una cadena y actualiza el resultado de la función de hash.
        */
        void load(std::string str);

        /*
        * Devuelve el resultado (entero de 2 bytes) de hashear el texto 
        * completo.
        */
        uint16_t operator()();    
};

#endif
