#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QLabel> // Widget para mostrar texto estático (etiquetas como "Usuario:")
#include <QLineEdit> // Widget para campos de entrada de texto de una línea (usuario y contraseña)
#include <QPushButton> // Widget para botones clicables ("Iniciar Sesión")
#include <QVBoxLayout> // Gestor de diseño que organiza los elementos verticalmente
#include <QWidget>     // Clase base de todos los objetos de la interfaz gráfica

class LoginWindow : public QWidget {
  Q_OBJECT

public:
  explicit LoginWindow(QWidget *parent = nullptr);

private slots:
  void onLoginClicked();
  void onRegisterClicked();

private:
  QLineEdit *usernameEdit;
  QLineEdit *passwordEdit;
  QPushButton *loginButton;
  QPushButton *registerButton;
  QLabel *statusLabel;
};

#endif // LOGINWINDOW_H
