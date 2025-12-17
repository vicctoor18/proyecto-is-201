#include "TutorWindow.h"
#include "LoginWindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QTimer>

TutorWindow::TutorWindow(int tutorId, QWidget *parent)
    : QWidget(parent), tutorId(tutorId), currentStudentId(-1),
      currentAppointmentId(-1) {
  setWindowTitle("Panel de Tutor");
  resize(1100, 800);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // Pestaña principal
  tabWidget = new QTabWidget(this);
  mainLayout->addWidget(tabWidget);

  // Configurar pestañas
  setupChatTab();
  setupAlertsTab();
  setupAppointmentsTab();

  tabWidget->addTab(chatTab, "Chat");
  tabWidget->addTab(alertsTab, "Alertas");
  tabWidget->addTab(appointmentsTab, "Tutorias");

  // Cerrar sesion (Global)
  logoutButton = new QPushButton("Cerrar Sesion", this);
  mainLayout->addWidget(logoutButton);
  connect(logoutButton, &QPushButton::clicked, this,
          &TutorWindow::onLogoutClicked);

  // Cargar datos
  loadStudents();
  loadAlertStudents();
  loadAppointments();

  // Refrescar chat y citas (cada 2 segundos)
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &TutorWindow::refreshChat);
  connect(timer, &QTimer::timeout, this, &TutorWindow::refreshAppointments);
  timer->start(2000);
}

void TutorWindow::setupChatTab() {
  chatTab = new QWidget();
  QHBoxLayout *layout = new QHBoxLayout(chatTab);

  // Izquierda: Lista de Alumnos
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(new QLabel("Mis Alumnos:"));
  studentList = new QListWidget(chatTab);
  leftLayout->addWidget(studentList);
  layout->addLayout(leftLayout, 1);

  // Derecha: Área de Chat
  QVBoxLayout *rightLayout = new QVBoxLayout();
  chatLabel = new QLabel("Seleccione un alumno para chatear", chatTab);
  rightLayout->addWidget(chatLabel);

  chatDisplay = new QTextEdit(chatTab);
  chatDisplay->setReadOnly(true);
  rightLayout->addWidget(chatDisplay);

  QHBoxLayout *inputLayout = new QHBoxLayout();
  messageInput = new QLineEdit(chatTab);
  sendButton = new QPushButton("Enviar", chatTab);
  inputLayout->addWidget(messageInput);
  inputLayout->addWidget(sendButton);
  rightLayout->addLayout(inputLayout);

  layout->addLayout(rightLayout, 2);

  connect(studentList, &QListWidget::itemClicked, this, &TutorWindow::onStudentSelected);
  connect(sendButton, &QPushButton::clicked, this, &TutorWindow::onSendMessage);
}

void TutorWindow::setupAlertsTab() {
  alertsTab = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout(alertsTab);

  // Selección de Alumnos
  QGroupBox *studentGroup = new QGroupBox("Seleccionar Destinatarios");
  QVBoxLayout *studentGroupLayout = new QVBoxLayout(studentGroup);

  selectAllCheckBox = new QCheckBox("Seleccionar Todos", alertsTab);
  studentGroupLayout->addWidget(selectAllCheckBox);

  alertStudentList = new QListWidget(alertsTab);
  studentGroupLayout->addWidget(alertStudentList);

  layout->addWidget(studentGroup);

  // Entrada de Mensaje
  QGroupBox *messageGroup = new QGroupBox("Mensaje de Alerta");
  QVBoxLayout *messageGroupLayout = new QVBoxLayout(messageGroup);

  alertMessageInput = new QTextEdit(alertsTab);
  messageGroupLayout->addWidget(alertMessageInput);

  sendAlertButton = new QPushButton("Enviar Alerta", alertsTab);
  messageGroupLayout->addWidget(sendAlertButton);

  layout->addWidget(messageGroup);

  connect(selectAllCheckBox, &QCheckBox::stateChanged, this, &TutorWindow::onSelectAllStudents);
  connect(sendAlertButton, &QPushButton::clicked, this, &TutorWindow::onSendAlert);
}

void TutorWindow::setupAppointmentsTab() {
  appointmentsTab = new QWidget();
  QHBoxLayout *layout = new QHBoxLayout(appointmentsTab);

  // Izquierda: Lista de Solicitudes y Citas
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(new QLabel("Solicitudes y Citas:", appointmentsTab));
  appointmentsTable = new QTableWidget(appointmentsTab);
  appointmentsTable->setColumnCount(4);
  appointmentsTable->setHorizontalHeaderLabels({"Alumno", "Fecha", "Hora", "Estado"});
  appointmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  appointmentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  appointmentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  leftLayout->addWidget(appointmentsTable);
  layout->addLayout(leftLayout, 1);

  // Derecha: Detalles y Acciones
  appointmentDetailsGroup = new QGroupBox("Detalles de la Cita");
  appointmentDetailsGroup->setEnabled(false);
  QFormLayout *formLayout = new QFormLayout(appointmentDetailsGroup);

  appStudentName = new QLineEdit(appointmentsTab);
  appStudentName->setReadOnly(true);
  appDateEdit = new QDateEdit(appointmentsTab);
  appDateEdit->setCalendarPopup(true);
  appTimeEdit = new QTimeEdit(appointmentsTab);
  appReason = new QLineEdit(appointmentsTab);
  appReason->setReadOnly(true);
  appTutorNotes = new QTextEdit(appointmentsTab);
  appTutorNotes->setPlaceholderText("Notas para el alumno...");

  formLayout->addRow("Alumno:", appStudentName);
  formLayout->addRow("Fecha:", appDateEdit);
  formLayout->addRow("Hora:", appTimeEdit);
  formLayout->addRow("Motivo:", appReason);
  formLayout->addRow("Notas:", appTutorNotes);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  appAcceptButton = new QPushButton("Aceptar", appointmentsTab);
  appModifyButton = new QPushButton("Modificar", appointmentsTab);
  appStartButton = new QPushButton("Iniciar", appointmentsTab);
  appFinishButton = new QPushButton("Finalizar", appointmentsTab);

  buttonsLayout->addWidget(appAcceptButton);
  buttonsLayout->addWidget(appModifyButton);
  buttonsLayout->addWidget(appStartButton);
  buttonsLayout->addWidget(appFinishButton);

  formLayout->addRow(buttonsLayout);

  layout->addWidget(appointmentDetailsGroup, 1);

  connect(appointmentsTable, &QTableWidget::cellClicked, this,
          &TutorWindow::onAppointmentSelected);
  connect(appAcceptButton, &QPushButton::clicked, this,
          &TutorWindow::onAcceptAppointment);
  connect(appModifyButton, &QPushButton::clicked, this,
          &TutorWindow::onModifyAppointment);

  // Conexiones lambda para cambios de estado simples
  connect(appStartButton, &QPushButton::clicked, [this]() { onChangeStatus("IN_PROGRESS"); });
  connect(appFinishButton, &QPushButton::clicked, [this]() { onChangeStatus("FINISHED"); });
}

void TutorWindow::loadStudents() {
  students = DatabaseManager::instance().getStudentsForTutor(tutorId);
  studentList->clear();
  for (const auto &student : students) {
    QListWidgetItem *item = new QListWidgetItem( student.name + " (" + student.degree + ")", studentList);
    item->setData(Qt::UserRole, student.id);
  }
}

void TutorWindow::loadAlertStudents() {
  alertStudentList->clear();
  for (const auto &student : students) {
    QListWidgetItem *item = new QListWidgetItem(student.name + " (" + student.degree + ")", alertStudentList);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    item->setData(Qt::UserRole, student.id);
  }
}

void TutorWindow::loadAppointments() {
  auto appointments =
      DatabaseManager::instance().getAppointments(tutorId, true);
  appointmentsTable->setRowCount(appointments.size());

  for (int i = 0; i < appointments.size(); ++i) {
    const auto &app = appointments[i];
    appointmentsTable->setItem(i, 0, new QTableWidgetItem(app.studentName));
    appointmentsTable->setItem(i, 1, new QTableWidgetItem(app.date));
    appointmentsTable->setItem(i, 2, new QTableWidgetItem(app.time));

    QTableWidgetItem *statusItem = new QTableWidgetItem(app.status);
    if (app.status == "REQUESTED") {
      statusItem->setText("SOLICITADA");
      statusItem->setForeground(Qt::blue);
      statusItem->setFont(QFont("Arial", 10, QFont::Bold));
    } else if (app.status == "SCHEDULED")
      statusItem->setText("PROGRAMADA");
    else if (app.status == "MODIFIED")
      statusItem->setText("MODIFICADA");
    else if (app.status == "IN_PROGRESS")
      statusItem->setText("EN CURSO");
    else if (app.status == "FINISHED")
      statusItem->setText("FINALIZADA");
    else if (app.status == "CANCELLED") {
      statusItem->setText("CANCELADA");
      statusItem->setForeground(Qt::gray);
    }

    appointmentsTable->setItem(i, 3, statusItem);

    // Guardar ID y otros datos en el primer item para recuperarlos luego
    appointmentsTable->item(i, 0)->setData(Qt::UserRole, app.id);
    appointmentsTable->item(i, 0)->setData(Qt::UserRole + 1, app.reason);
    appointmentsTable->item(i, 0)->setData(Qt::UserRole + 2, app.tutorNotes);
  }
}

void TutorWindow::onAppointmentSelected(int row, int column) {
  currentAppointmentId =
      appointmentsTable->item(row, 0)->data(Qt::UserRole).toInt();
  QString studentName = appointmentsTable->item(row, 0)->text();
  QString date = appointmentsTable->item(row, 1)->text();
  QString time = appointmentsTable->item(row, 2)->text();
  QString reason =
      appointmentsTable->item(row, 0)->data(Qt::UserRole + 1).toString();
  QString notes =
      appointmentsTable->item(row, 0)->data(Qt::UserRole + 2).toString();

  // Obtener estado actual
  QString statusText = appointmentsTable->item(row, 3)->text();
  QString status;
  if (statusText == "SOLICITADA")
    status = "REQUESTED";
  else if (statusText == "PROGRAMADA")
    status = "SCHEDULED";
  else if (statusText == "MODIFICADA")
    status = "MODIFIED";
  else if (statusText == "EN CURSO")
    status = "IN_PROGRESS";
  else if (statusText == "FINALIZADA")
    status = "FINISHED";
  else if (statusText == "CANCELADA")
    status = "CANCELLED";

  appointmentDetailsGroup->setEnabled(true);
  appStudentName->setText(studentName);
  appDateEdit->setDate(QDate::fromString(date, "yyyy-MM-dd"));
  appTimeEdit->setTime(QTime::fromString(time, "HH:mm"));
  appReason->setText(reason);
  appTutorNotes->setText(notes);

  updateAppointmentButtons(status);
}

void TutorWindow::updateAppointmentButtons(const QString &status) {
  appAcceptButton->setVisible(false);
  appModifyButton->setVisible(false);
  appStartButton->setVisible(false);
  appFinishButton->setVisible(false);

  if (status == "REQUESTED" || status == "MODIFIED") {
    appAcceptButton->setVisible(true);
    appModifyButton->setVisible(true);
  } else if (status == "SCHEDULED") {
    appModifyButton->setVisible(true);
    appStartButton->setVisible(true);
  } else if (status == "IN_PROGRESS") {
    appFinishButton->setVisible(true);
  }
}

void TutorWindow::onAcceptAppointment() {
  if (currentAppointmentId == -1)
    return;

  if (appDateEdit->date() <= QDate::currentDate()) {
    QMessageBox::warning(
        this, "Error", "La fecha de la tutoria debe ser al menos con un dia de antelacion.");
    return;
  }

  DatabaseManager::instance().updateAppointment(
      currentAppointmentId, appDateEdit->date().toString("yyyy-MM-dd"),
      appTimeEdit->time().toString("HH:mm"), "SCHEDULED",
      appTutorNotes->toPlainText());

  QMessageBox::information(this, "Exito", "Tutoria aceptada y programada.");
  loadAppointments();
  appointmentDetailsGroup->setEnabled(false);
}

void TutorWindow::onModifyAppointment() {
  if (currentAppointmentId == -1)
    return;

  if (appDateEdit->date() <= QDate::currentDate()) {
    QMessageBox::warning(this, "Error", "La fecha de la tutoria debe ser al menos con un dia de antelacion.");
    return;
  }

  DatabaseManager::instance().updateAppointment(
      currentAppointmentId, appDateEdit->date().toString("yyyy-MM-dd"),
      appTimeEdit->time().toString("HH:mm"), "MODIFIED",
      appTutorNotes->toPlainText());

  QMessageBox::information(this, "Exito", "Tutoria modificada. El alumno sera notificado.");
  loadAppointments();
  appointmentDetailsGroup->setEnabled(false);
}

void TutorWindow::onChangeStatus(const QString &newStatus) {
  if (currentAppointmentId == -1)
    return;
  DatabaseManager::instance().updateAppointment(
      currentAppointmentId, appDateEdit->date().toString("yyyy-MM-dd"),
      appTimeEdit->time().toString("HH:mm"), newStatus,
      appTutorNotes->toPlainText());

  loadAppointments();
  appointmentDetailsGroup->setEnabled(false);
}

void TutorWindow::refreshAppointments() {
  // Solo refrescar si no se está editando una cita
  if (!appointmentDetailsGroup->isEnabled()) {
    loadAppointments();
  }
}

void TutorWindow::onSelectAllStudents(int state) {
  Qt::CheckState checkState =
      (state == Qt::Checked) ? Qt::Checked : Qt::Unchecked;
  for (int i = 0; i < alertStudentList->count(); ++i) {
    alertStudentList->item(i)->setCheckState(checkState);
  }
}

void TutorWindow::onSendAlert() {
  QString content = alertMessageInput->toPlainText();
  if (content.isEmpty()) {
    QMessageBox::warning(this, "Error", "El mensaje de alerta no puede estar vacio.");
    return;
  }

  std::vector<int> receiverIds;
  for (int i = 0; i < alertStudentList->count(); ++i) {
    QListWidgetItem *item = alertStudentList->item(i);
    if (item->checkState() == Qt::Checked) {
      receiverIds.push_back(item->data(Qt::UserRole).toInt());
    }
  }

  if (receiverIds.empty()) {
    QMessageBox::warning(this, "Error", "Debe seleccionar al menos un alumno.");
    return;
  }

  int successCount = 0;
  for (int receiverId : receiverIds) {
    if (DatabaseManager::instance().sendAlert(tutorId, receiverId, content)) {
      successCount++;
    }
  }

  QMessageBox::information(
      this, "Exito", QString("Alerta enviada a %1 alumnos.").arg(successCount));
  alertMessageInput->clear();
  // Reiniciar selección
  selectAllCheckBox->setChecked(false);
  for (int i = 0; i < alertStudentList->count(); ++i) {
    alertStudentList->item(i)->setCheckState(Qt::Unchecked);
  }
}

void TutorWindow::onStudentSelected(QListWidgetItem *item) {
  currentStudentId = item->data(Qt::UserRole).toInt();
  chatLabel->setText("Chat con: " + item->text());
  loadChat();
}

void TutorWindow::loadChat() {
  if (currentStudentId == -1)
    return;

  auto messages = DatabaseManager::instance().getMessages(tutorId, currentStudentId);
  QString html;
  for (const auto &msg : messages) {
    QString color = (msg.senderId == tutorId) ? "blue" : "green";
    QString sender = (msg.senderId == tutorId) ? "Yo" : "Alumno";
    html += "<div style='color:" + color + "'><b>" + sender + ":</b> " + msg.content + "</div>";
  }
  chatDisplay->setHtml(html);
  chatDisplay->moveCursor(QTextCursor::End);
}

void TutorWindow::refreshChat() { loadChat(); }

void TutorWindow::onSendMessage() {
  QString content = messageInput->text();
  if (content.isEmpty() || currentStudentId == -1)
    return;

  if (DatabaseManager::instance().sendMessage(tutorId, currentStudentId, content)) {
    messageInput->clear();
    loadChat();
  }
}

void TutorWindow::onLogoutClicked() {
  LoginWindow *loginWindow = new LoginWindow();
  loginWindow->show();
  this->close();
}
