#ifndef TUTOR_H
#define TUTOR_H

#include "UsuarioUCO.h"

class Tutor : public UsuarioUCO {
public:
  Tutor(int id, QString name, QString degree, QString nationality);
};

#endif // TUTOR_H
