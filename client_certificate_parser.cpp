#include <fstream>
#include "client_certificate_parser.h"
#include "common_key.h"

CertificateParser::CertificateParser(std::string& filename, Protocol& skt) {
    std::ifstream file;
    file.open(filename);
    std::string line;
    int count = 0;
    int pos;
    int len;
    std::string module;
    std::string exp;
    while (std::getline(file, line, '\n')) {
        if (!file.eof()) {
            std::string aux = line + '\n';
            this->hash.load(aux);
        } else {
            this->hash.load(line);
        }
        pos = line.find(':');
        line = line.c_str();
        len = line.length();
        if (pos + 2 > len) { //certificate:\0
            ++count;
            continue;
        }
        line = line.substr(pos + 2,len);
        if (count == 1) {
            len = line.find(' ');
            uint32_t n = (uint32_t) std::stoi(line.substr(0, len));
            skt.sendNumber(&n);
        } else if ((count == 2) | (count == 5) | (count == 6)) {
            skt.sendAll(line);
        } else if (count == 8) {
            len = line.find(' ');
            module = line.substr(0, len);
        } else if (count == 9) {
            len = line.find(' ');
            exp = line.substr(0, len);
        }        
        ++count;
    }
    Key key(exp, module);
    key.send(skt);
}

/*std::ifstream file(filename);
std::string key;
char aux;
// certificate:
std::getline(file, key);
// subject:
std::getline(file, key, ':');
// el espacio después de los dos puntos
file >> aux;
// valor
std::getline(file, this->subject);
// issuer:
std::getline(file, key, ':');
file >> aux;
std::getline(file, this->issuer);
... // se repite para los strings
//modulus:
std::getline(file, key, ':');
//el operador >> trimea los espacios al buscar numeros
file >> this->modulus;*/

uint32_t CertificateParser::hashCertificate() {
    return this->hash();
}
