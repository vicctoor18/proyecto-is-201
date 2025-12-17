#include "StudentWindow.h"
#include "LoginWindow.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QTimer>

StudentWindow::StudentWindow(int studentId, QWidget *parent) : QWidget(parent), studentId(studentId), tutorId(-1) {
  setWindowTitle("Panel de Alumno");
  resize(1000, 800); // Tamaño de la ventana

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // Pestaña principal
  tabWidget = new QTabWidget(this);
  mainLayout->addWidget(tabWidget);

  // Inicializar y configurar las pestañas secundarias
  setupChatTab();
  setupAlertsTab();
  setupAppointmentsTab();

  // Añadir las pestañas al widget principal
  tabWidget->addTab(chatTab, "Chat");
  tabWidget->addTab(alertsTab, "Alertas");
  tabWidget->addTab(appointmentsTab, "Tutorias");

  // Cerrar sesion (Global)
  logoutButton = new QPushButton("Cerrar Sesion", this);
  mainLayout->addWidget(logoutButton);
  connect(logoutButton, &QPushButton::clicked, this, &StudentWindow::onLogoutClicked);

  // Cargar datos
  loadTutorInfo();
  loadAlerts();
  loadAppointments();

  // Refrescar datos chat (cada 2 segundos)
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &StudentWindow::refreshChat);
  connect(timer, &QTimer::timeout, this, &StudentWindow::refreshAlerts);
  connect(timer, &QTimer::timeout, this, &StudentWindow::refreshAppointments);
  timer->start(2000);
}

void StudentWindow::setupChatTab() {
  chatTab = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout(chatTab);

  // Etiqueta para mostrar la información del tutor asignado
  tutorInfoLabel = new QLabel("Cargando informacion del tutor...", chatTab);
  layout->addWidget(tutorInfoLabel);

  // Área de texto para mostrar el historial del chat (solo lectura)
  chatDisplay = new QTextEdit(chatTab);
  chatDisplay->setReadOnly(true);
  layout->addWidget(chatDisplay);

  // Área de entrada de mensajes y botón de enviar
  QHBoxLayout *inputLayout = new QHBoxLayout();
  messageInput = new QLineEdit(chatTab);
  sendButton = new QPushButton("Enviar", chatTab);
  inputLayout->addWidget(messageInput);
  inputLayout->addWidget(sendButton);
  layout->addLayout(inputLayout);

  // Conectar el botón de enviar con la función correspondiente
  connect(sendButton, &QPushButton::clicked, this, &StudentWindow::onSendMessage);
}

void StudentWindow::setupAlertsTab() {
  alertsTab = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout(alertsTab);

  // Etiqueta de título para la sección de alertas
  layout->addWidget(new QLabel("Alertas Recibidas:", alertsTab));

  // Lista para mostrar las alertas
  alertsList = new QListWidget(alertsTab);
  layout->addWidget(alertsList);

  // Botón para marcar una alerta seleccionada como leída
  markReadButton = new QPushButton("Marcar como Leida", alertsTab);
  layout->addWidget(markReadButton);

  // Conectar el botón con la función de marcar como leída
  connect(markReadButton, &QPushButton::clicked, this, &StudentWindow::onMarkAlertAsRead);
}

void StudentWindow::setupAppointmentsTab() {
  appointmentsTab = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout(appointmentsTab);

  // Formulario de Solicitud de Tutoría
  QGroupBox *requestGroup = new QGroupBox("Solicitar Tutoria");
  QFormLayout *formLayout = new QFormLayout(requestGroup);

  // Campos para fecha, hora y motivo
  dateEdit = new QDateEdit(QDate::currentDate(), appointmentsTab);
  dateEdit->setCalendarPopup(true);
  timeEdit = new QTimeEdit(QTime::currentTime(), appointmentsTab);
  reasonInput = new QLineEdit(appointmentsTab);
  requestButton = new QPushButton("Solicitar", appointmentsTab);

  formLayout->addRow("Fecha:", dateEdit);
  formLayout->addRow("Hora:", timeEdit);
  formLayout->addRow("Motivo:", reasonInput);
  formLayout->addWidget(requestButton);

  layout->addWidget(requestGroup);

  // Lista de Tutorías (Tabla)
  layout->addWidget(new QLabel("Mis Tutorias:", appointmentsTab));
  appointmentsTable = new QTableWidget(appointmentsTab);
  appointmentsTable->setColumnCount(5);
  appointmentsTable->setHorizontalHeaderLabels({"Fecha", "Hora", "Motivo", "Estado", "Notas Tutor"});
  appointmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  appointmentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  layout->addWidget(appointmentsTable);

  // Botón para cancelar la tutoría seleccionada
  cancelAppointmentButton = new QPushButton("Cancelar Tutoria Seleccionada", appointmentsTab);
  layout->addWidget(cancelAppointmentButton);

  // Conectar botones con sus funciones
  connect(requestButton, &QPushButton::clicked, this, &StudentWindow::onRequestAppointment);
  connect(cancelAppointmentButton, &QPushButton::clicked, this, &StudentWindow::onCancelAppointment);
}

void StudentWindow::loadTutorInfo() {
  DatabaseManager &db = DatabaseManager::instance();
  tutorId = db.getAssignedTutor(studentId);

  if (tutorId == -1) {
    // Intentar asignar un tutor si no tiene uno
    DatabaseManager::UserInfo userInfo = db.getUserInfo(studentId);
    if (db.assignTutor(studentId, userInfo.degree, userInfo.nationality)) {
      tutorId = db.getAssignedTutor(studentId);
    }
  }

  if (tutorId != -1) {
    // Si hay tutor asignado, mostrar su información y cargar el chat
    DatabaseManager::UserInfo tutorInfo = db.getUserInfo(tutorId);
    tutorInfoLabel->setText("Tutor Asignado: " + tutorInfo.name + " (" + tutorInfo.degree + ", " + tutorInfo.nationality + ")");
    loadChat();
  } else {
    // Si no se pudo asignar, mostrar mensaje de error y deshabilitar chat
    tutorInfoLabel->setText("No se ha podido asignar un tutor. Contacte con administracion.");
    messageInput->setEnabled(false);
    sendButton->setEnabled(false);
  }
}

void StudentWindow::loadChat() {
  if (tutorId == -1)
    return;

  // Obtener mensajes de la base de datos
  auto messages = DatabaseManager::instance().getMessages(studentId, tutorId);
  QString html;
  for (const auto &msg : messages) {
    // Diferenciar colores según el remitente (Alumno: azul, Tutor: verde)
    QString color = (msg.senderId == studentId) ? "blue" : "green";
    QString sender = (msg.senderId == studentId) ? "Yo" : "Tutor";
    html += "<div style='color:" + color + "'><b>" + sender + ":</b> " + msg.content + "</div>";
  }
  chatDisplay->setHtml(html);
  chatDisplay->moveCursor(QTextCursor::End);
}

void StudentWindow::loadAlerts() {
  auto alerts = DatabaseManager::instance().getAlerts(studentId);
  alertsList->clear();
  for (const auto &alert : alerts) {
    QString status = alert.isRead ? "[LEIDO]" : "[NUEVO]";
    QString text = QString("%1 %2 - %3: %4").arg(status).arg(alert.timestamp).arg(alert.senderName).arg(alert.content);
    QListWidgetItem *item = new QListWidgetItem(text, alertsList);
    item->setData(Qt::UserRole, alert.id);

    // Resaltar alertas no leídas en rojo y negrita
    if (!alert.isRead) {
      item->setForeground(Qt::red);
      QFont font = item->font();
      font.setBold(true);
      item->setFont(font);
    }
  }
}

void StudentWindow::loadAppointments() {
  auto appointments =DatabaseManager::instance().getAppointments(studentId, false);
  appointmentsTable->setRowCount(appointments.size());

  for (int i = 0; i < appointments.size(); ++i) {
    const auto &app = appointments[i];
    appointmentsTable->setItem(i, 0, new QTableWidgetItem(app.date));
    appointmentsTable->setItem(i, 1, new QTableWidgetItem(app.time));
    appointmentsTable->setItem(i, 2, new QTableWidgetItem(app.reason));

    // Configurar el estado de la tutoría con texto y colores
    QTableWidgetItem *statusItem = new QTableWidgetItem(app.status);
    if (app.status == "REQUESTED")
      statusItem->setText("ESPERANDO");
    else if (app.status == "SCHEDULED")
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

    // Resaltar si ha sido modificada por el tutor
    if (app.status == "MODIFIED") {
      statusItem->setForeground(Qt::magenta);
      statusItem->setToolTip(
          "El tutor ha modificado tu solicitud. Revisa la fecha/hora.");
    }

    // Guardar ID y Estado para la lógica de negocio (cancelación, etc.)
    appointmentsTable->item(i, 0)->setData(Qt::UserRole, app.id);
    appointmentsTable->item(i, 0)->setData(Qt::UserRole + 1, app.status);

    appointmentsTable->setItem(i, 3, statusItem);
    appointmentsTable->setItem(i, 4, new QTableWidgetItem(app.tutorNotes));
  }
}

void StudentWindow::refreshChat() { loadChat(); }

void StudentWindow::refreshAlerts() {
  if (alertsList->selectedItems().isEmpty()) {
    loadAlerts();
  }
}

void StudentWindow::refreshAppointments() { loadAppointments(); }

void StudentWindow::onSendMessage() {
  QString content = messageInput->text();
  if (content.isEmpty() || tutorId == -1)
    return;

  // Enviar mensaje a la base de datos y recargar el chat
  if (DatabaseManager::instance().sendMessage(studentId, tutorId, content)) {
    messageInput->clear();
    loadChat();
  }
}

void StudentWindow::onMarkAlertAsRead() {
  QList<QListWidgetItem *> selectedItems = alertsList->selectedItems();
  if (selectedItems.isEmpty()) {
    QMessageBox::warning(this, "Error", "Seleccione una alerta para marcar como leida.");
    return;
  }

  // Marcar todas las alertas seleccionadas como leídas
  for (auto item : selectedItems) {
    int alertId = item->data(Qt::UserRole).toInt();
    DatabaseManager::instance().markAlertAsRead(alertId);
  }
  loadAlerts();
}

void StudentWindow::onRequestAppointment() {
  // Validar que se haya introducido un motivo
  if (reasonInput->text().isEmpty()) {
    QMessageBox::warning(this, "Error", "Debe indicar un motivo.");
    return;
  }

  // Validar que la fecha sea posterior a hoy
  if (dateEdit->date() <= QDate::currentDate()) {
    QMessageBox::warning(this, "Error", "La fecha de la tutoria debe ser al menos con un dia de antelacion.");
    return;
  }

  // Enviar solicitud a la base de datos
  if (DatabaseManager::instance().requestAppointment(
          studentId, tutorId, dateEdit->date().toString("yyyy-MM-dd"),
          timeEdit->time().toString("HH:mm"), reasonInput->text())) {

    QMessageBox::information(this, "Exito", "Solicitud enviada correctamente.");
    reasonInput->clear();
    loadAppointments();
  } else {
    QMessageBox::critical(this, "Error", "No se pudo enviar la solicitud.");
  }
}

void StudentWindow::onCancelAppointment() {
  int currentRow = appointmentsTable->currentRow();
  if (currentRow == -1) {
    QMessageBox::warning(this, "Error", "Seleccione una tutoria para cancelar.");
    return;
  }

  // Obtener ID y estado de la cita seleccionada
  int appointmentId =
      appointmentsTable->item(currentRow, 0)->data(Qt::UserRole).toInt();
  QString status =
      appointmentsTable->item(currentRow, 0)->data(Qt::UserRole + 1).toString();

  // No permitir cancelar si ya está en curso o finalizada
  if (status == "IN_PROGRESS" || status == "FINISHED") {
    QMessageBox::warning(this, "Error", "No se puede cancelar una tutoria en curso o finalizada.");
    return;
  }

  if (status == "CANCELLED") {
    return; // Ya está cancelada
  }

  // Confirmar cancelación
  if (QMessageBox::question( this, "Confirmar", "Seguro que desea cancelar esta tutoria?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    DatabaseManager::instance().updateAppointmentStatus(appointmentId, "CANCELLED");
    loadAppointments();
  }
}

void StudentWindow::onLogoutClicked() {
  LoginWindow *loginWindow = new LoginWindow();
  loginWindow->show();
  this->close();
}
