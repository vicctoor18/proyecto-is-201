#ifndef USUARIOUCO_H
#define USUARIOUCO_H

#include <QString>

class UsuarioUCO {
public:
  UsuarioUCO(int id, QString name, QString degree, QString nationality);
  virtual ~UsuarioUCO() = default;

  int getId() const;
  QString getName() const;
  QString getDegree() const;
  QString getNationality() const;

protected:
  int id;
  QString name;
  QString degree;
  QString nationality;
};

#endif // USUARIOUCO_H
