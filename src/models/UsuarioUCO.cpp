#include "UsuarioUCO.h"

UsuarioUCO::UsuarioUCO(int id, QString name, QString degree,
                       QString nationality)
    : id(id), name(name), degree(degree), nationality(nationality) {}

int UsuarioUCO::getId() const { return id; }
QString UsuarioUCO::getName() const { return name; }
QString UsuarioUCO::getDegree() const { return degree; }
QString UsuarioUCO::getNationality() const { return nationality; }
