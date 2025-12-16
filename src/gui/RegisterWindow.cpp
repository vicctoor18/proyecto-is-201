#include "RegisterWindow.h"
#include "../database/DatabaseManager.h"
#include <QMessageBox>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Registro de Usuario");
  resize(400, 500); // Tamaño de la ventana

  QVBoxLayout *layout = new QVBoxLayout(
      this); // Crea la ventana vertical para escribir todo el código

  layout->addWidget(new QLabel("Usuario:")); // Etiqueta de usuario
  usernameEdit = new QLineEdit(this);        // Editor de usuario
  layout->addWidget(usernameEdit);

  layout->addWidget(new QLabel("Contrasena:"));   // Etiqueta de contraseña
  passwordEdit = new QLineEdit(this);             // Editor de contraseña
  passwordEdit->setEchoMode(QLineEdit::Password); // Oculta constraseña
  layout->addWidget(passwordEdit);

  layout->addWidget(
      new QLabel("Nombre Completo:")); // Etiqueta del nombre de usuario
  nameEdit = new QLineEdit(this);      // Editor de nombre del usuario
  layout->addWidget(nameEdit);

  layout->addWidget(new QLabel("Rol:")); // Etiqueta de los roles
  roleCombo = new QComboBox(this);       // Selección de rol
  roleCombo->addItem("alumno");
  roleCombo->addItem("tutor");
  layout->addWidget(roleCombo);

  layout->addWidget(new QLabel("Grado/Carrera:")); // Etiqueta de grado
  degreeEdit = new QLineEdit(this);                // Editor de grado
  layout->addWidget(degreeEdit);

  layout->addWidget(new QLabel("Nacionalidad:")); // Etiqueta de nacionalidad
  nationalityEdit = new QLineEdit(this);          // Editor de nacionalidad
  layout->addWidget(nationalityEdit);

  registerButton = new QPushButton("Registrarse", this); // Etiqueta de registro
  layout->addWidget(registerButton); // Botón para registrarses

  statusLabel = new QLabel(this);
  statusLabel->setStyleSheet(
      "color: red"); // Estas 3 líneas generan el botón para crear el usuario
  layout->addWidget(statusLabel);

  connect(registerButton, &QPushButton::clicked, this,
          &RegisterWindow::onRegisterClicked); // Conecta el botón con la
                                               // función onRegisterClicked
}

void RegisterWindow::onRegisterClicked() { // Función que pasa la información
                                           // escrita a un usuario
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
    QMessageBox::information(this, "Exito",
                             "Usuario registrado correctamente. Ahora puede "
                             "iniciar sesion."); // Si se ha creado con éxito
    this->close();
  } else {
    statusLabel->setText(
        "Error al registrar. El usuario ya existe."); // En caso de que las
                                                      // credenciales ya estén
                                                      // registradas
  }
}
