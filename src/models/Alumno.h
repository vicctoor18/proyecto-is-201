#ifndef ALUMNO_H
#define ALUMNO_H

#include "UsuarioUCO.h" // Importa la clase base UsuarioUCO para poder heredar de ella
#include <QString> // Necesario para usar tipos de texto en el constructor

class Alumno : public UsuarioUCO {
public:
  Alumno(int id, QString name, QString degree, QString nationality);
};

#endif
