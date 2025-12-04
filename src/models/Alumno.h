#ifndef ALUMNO_H
#define ALUMNO_H

#include "UsuarioUCO.h"

class Alumno : public UsuarioUCO {
public:
  Alumno(int id, QString name, QString degree, QString nationality);
};

#endif // ALUMNO_H
