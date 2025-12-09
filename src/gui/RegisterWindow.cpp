#include "RegisterWindow.h"
#include "../database/DatabaseManager.h"
#include <QMessageBox>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Registro de Usuario");
  resize(400, 500); // Larger size

  QVBoxLayout *layout = new QVBoxLayout(this); //Crea la ventana vertical para escribir todo el código

  layout->addWidget(new QLabel("Usuario:"));
  usernameEdit = new QLineEdit(this);
  layout->addWidget(usernameEdit);

  layout->addWidget(new QLabel("Contrasena:")); //Crea la etiqueta de cada caja de texto
  passwordEdit = new QLineEdit(this); //Crea lo que es la caja de texto (PERO NO SE MUESTRA) y se guarda en passwordEdit
  passwordEdit->setEchoMode(QLineEdit::Password); //Muestra puntitos en vez de el texto
  layout->addWidget(passwordEdit); //Hace que aparezca la caja de texto físicamente en el layout

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
  statusLabel->setStyleSheet("color: red"); //Estas 3 líneas generan el botón para crear el usuario
  layout->addWidget(statusLabel);

  connect(registerButton, &QPushButton::clicked, this,
          &RegisterWindow::onRegisterClicked); //Conecta el botón con la función onRegisterClicked
}

void RegisterWindow::onRegisterClicked() { //Función que pasa la información escrita a un usuario
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
        "Usuario registrado correctamente. Ahora puede iniciar sesion."); //Si se ha creado con éxito
    this->close();
  } else {
    statusLabel->setText("Error al registrar. El usuario ya existe."); //En caso de que las credenciales ya estén registradas
  }
}
