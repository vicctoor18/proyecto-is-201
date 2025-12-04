#include "LoginWindow.h"
#include "../database/DatabaseManager.h"
#include "RegisterWindow.h"
#include "StudentWindow.h"
#include "TutorWindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Sistema Tutorizacion UCO - Login");
  resize(400, 300); // Increased size

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setSpacing(10);
  layout->setContentsMargins(20, 20, 20, 20);

  layout->addWidget(new QLabel("Usuario:"));
  usernameEdit = new QLineEdit(this);
  layout->addWidget(usernameEdit);

  layout->addWidget(new QLabel("Contrasena:"));
  passwordEdit = new QLineEdit(this);
  passwordEdit->setEchoMode(QLineEdit::Password);
  layout->addWidget(passwordEdit);

  loginButton = new QPushButton("Iniciar Sesion", this);
  layout->addWidget(loginButton);

  registerButton = new QPushButton("Registrarse", this);
  layout->addWidget(registerButton);

  statusLabel = new QLabel(this);
  statusLabel->setStyleSheet("color: red");
  layout->addWidget(statusLabel);

  connect(loginButton, &QPushButton::clicked, this,
          &LoginWindow::onLoginClicked);
  connect(registerButton, &QPushButton::clicked, this,
          &LoginWindow::onRegisterClicked);
}

void LoginWindow::onRegisterClicked() {
  RegisterWindow *registerWindow = new RegisterWindow();
  registerWindow->show();
}

void LoginWindow::onLoginClicked() {
  QString username = usernameEdit->text();
  QString password = passwordEdit->text();
  QString role;
  int userId;

  if (DatabaseManager::instance().login(username, password, role, userId)) {
    if (role == "alumno") {
      StudentWindow *studentWindow = new StudentWindow(userId);
      studentWindow->show();
      this->close();
    } else if (role == "tutor") {
      TutorWindow *tutorWindow = new TutorWindow(userId);
      tutorWindow->show();
      this->close();
    }
  } else {
    statusLabel->setText("Credenciales incorrectas");
  }
}
