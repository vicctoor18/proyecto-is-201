#include "LoginWindow.h"
#include "../database/DatabaseManager.h"
#include "RegisterWindow.h"
#include "StudentWindow.h"
#include "TutorWindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Sistema Tutorizacion UCO - Login");
  resize(400, 300); // Tamaño de la ventana

  QVBoxLayout *layout = new QVBoxLayout(this); // Layout vertical
  layout->setSpacing(10);                      // Espacio entre widgets
  layout->setContentsMargins(20, 20, 20, 20);  // Margen de la ventana

  layout->addWidget(new QLabel("Usuario:")); // Etiqueta de usuario
  usernameEdit = new QLineEdit(this);        // Editor nombre usuario
  layout->addWidget(usernameEdit);

  layout->addWidget(new QLabel("Contrasena:"));   // Etiqueta de contrasena
  passwordEdit = new QLineEdit(this);             // Editor contrasena
  passwordEdit->setEchoMode(QLineEdit::Password); // Oculta la contrasena
  layout->addWidget(passwordEdit);

  loginButton = new QPushButton("Iniciar Sesion", this); // Boton iniciar sesion
  layout->addWidget(loginButton);

  registerButton = new QPushButton("Registrarse", this); // Boton registrarse
  layout->addWidget(registerButton);

  statusLabel = new QLabel(this); // Etiqueta de estado
  statusLabel->setStyleSheet("color: red");
  layout->addWidget(statusLabel);

  connect(loginButton, &QPushButton::clicked, this,
          &LoginWindow::onLoginClicked);
  connect(registerButton, &QPushButton::clicked, this,
          &LoginWindow::onRegisterClicked);
}

void LoginWindow::onRegisterClicked() {
  RegisterWindow *registerWindow = new RegisterWindow();
  registerWindow->show(); // Muestra la ventana de registro
}

void LoginWindow::onLoginClicked() {
  QString username = usernameEdit->text();
  QString password = passwordEdit->text();
  QString role;
  int userId;

  if (DatabaseManager::instance().login(username, password, role, userId)) {
    if (role == "alumno") {
      StudentWindow *studentWindow = new StudentWindow(userId);
      studentWindow->show(); // Muestra la ventana de estudiante
      this->close();
    } else if (role == "tutor") {
      TutorWindow *tutorWindow = new TutorWindow(userId);
      tutorWindow->show(); // Muestra la ventana de tutor
      this->close();
    }
  } else {
    statusLabel->setText("Credenciales incorrectas");
  }
}
