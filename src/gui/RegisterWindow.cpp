#include "RegisterWindow.h"
#include "../database/DatabaseManager.h"
#include <QMessageBox>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Registro de Usuario");
  resize(400, 500); // Larger size

  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->addWidget(new QLabel("Usuario:"));
  usernameEdit = new QLineEdit(this);
  layout->addWidget(usernameEdit);

  layout->addWidget(new QLabel("Contrasena:"));
  passwordEdit = new QLineEdit(this);
  passwordEdit->setEchoMode(QLineEdit::Password);
  layout->addWidget(passwordEdit);

  layout->addWidget(new QLabel("Nombre Completo:"));
  nameEdit = new QLineEdit(this);
  layout->addWidget(nameEdit);

  layout->addWidget(new QLabel("Rol:"));
  roleCombo = new QComboBox(this);
  roleCombo->addItem("alumno");
  roleCombo->addItem("tutor");
  layout->addWidget(roleCombo);

  layout->addWidget(new QLabel("Grado/Carrera:"));
  degreeEdit = new QLineEdit(this);
  layout->addWidget(degreeEdit);

  layout->addWidget(new QLabel("Nacionalidad:"));
  nationalityEdit = new QLineEdit(this);
  layout->addWidget(nationalityEdit);

  registerButton = new QPushButton("Registrarse", this);
  layout->addWidget(registerButton);

  statusLabel = new QLabel(this);
  statusLabel->setStyleSheet("color: red");
  layout->addWidget(statusLabel);

  connect(registerButton, &QPushButton::clicked, this,
          &RegisterWindow::onRegisterClicked);
}

void RegisterWindow::onRegisterClicked() {
  QString username = usernameEdit->text();
  QString password = passwordEdit->text();
  QString name = nameEdit->text();
  QString role = roleCombo->currentText();
  QString degree = degreeEdit->text();
  QString nationality = nationalityEdit->text();

  if (username.isEmpty() || password.isEmpty() || name.isEmpty() ||
      degree.isEmpty() || nationality.isEmpty()) {
    statusLabel->setText("Todos los campos son obligatorios");
    return;
  }

  if (DatabaseManager::instance().registerUser(username, password, role, name,
                                               degree, nationality)) {
    QMessageBox::information(
        this, "Exito",
        "Usuario registrado correctamente. Ahora puede iniciar sesion.");
    this->close();
  } else {
    statusLabel->setText("Error al registrar. El usuario ya existe.");
  }
}
