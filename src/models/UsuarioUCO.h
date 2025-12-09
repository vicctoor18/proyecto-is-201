#ifndef USUARIOUCO_H
#define USUARIOUCO_H

#include <QString> // Permite usar la clase QString para manejar cadenas de texto (nombres, grados, etc.)

class UsuarioUCO {
private:
  int id_;
  QString name_;
  QString degree_;
  QString nationality_;

public:
  UsuarioUCO(int id, QString name, QString degree, QString nationality);
  virtual ~UsuarioUCO() = default;

  inline int getId() const { return id_; }
  inline QString getName() const { return name_; }
  inline QString getDegree() const { return degree_; }
  inline QString getNationality() const { return nationality_; }
};

#endif // USUARIOUCO_H
