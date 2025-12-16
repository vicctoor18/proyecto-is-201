#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QLabel>      // Muestra texto o imagenes
#include <QLineEdit>   // Campo de entrada de texto de una linea
#include <QPushButton> // Boton de comando
#include <QVBoxLayout> // Organiza los widgets verticalmente
#include <QWidget> // Clase base para todos los objetos de interfaz de usuario

class LoginWindow : public QWidget {
Q_OBJECT // Hace que la clase sea interactiva (que cuando se pulsa el botón, se
         // registre el usuario)

    public : explicit LoginWindow(QWidget *parent = nullptr);

private slots: // Slots son funciones que se ejecutan cuando ocurre un evento
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
