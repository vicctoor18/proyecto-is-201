#ifndef TUTOR_H
#define TUTOR_H

#include "UsuarioUCO.h" // Importa la clase base UsuarioUCO para poder heredar de ella
#include <QString> // Necesario para usar tipos de texto en el constructor

class Tutor : public UsuarioUCO {
public:
  Tutor(int id, QString name, QString degree, QString nationality);
};

#endif
