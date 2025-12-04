#include "StudentWindow.h"
#include "../database/DatabaseManager.h"
#include "LoginWindow.h"
#include <QTimer>

StudentWindow::StudentWindow(int studentId, QWidget *parent)
    : QWidget(parent), studentId(studentId), tutorId(-1) {
  setWindowTitle("Panel de Alumno");
  resize(800, 600); // Increased size

  QVBoxLayout *layout = new QVBoxLayout(this);

  tutorInfoLabel = new QLabel("Cargando informacion del tutor...", this);
  layout->addWidget(tutorInfoLabel);

  logoutButton = new QPushButton("Cerrar Sesion", this);
  layout->addWidget(logoutButton);
  connect(logoutButton, &QPushButton::clicked, this,
          &StudentWindow::onLogoutClicked);

  chatDisplay = new QTextEdit(this);
  chatDisplay->setReadOnly(true);
  layout->addWidget(chatDisplay);

  QHBoxLayout *inputLayout = new QHBoxLayout();
  messageInput = new QLineEdit(this);
  sendButton = new QPushButton("Enviar", this);
  inputLayout->addWidget(messageInput);
  inputLayout->addWidget(sendButton);
  layout->addLayout(inputLayout);

  connect(sendButton, &QPushButton::clicked, this,
          &StudentWindow::onSendMessage);

  loadTutorInfo();

  // Auto-refresh chat every 2 seconds
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &StudentWindow::refreshChat);
  timer->start(2000);
}

void StudentWindow::loadTutorInfo() {
  DatabaseManager &db = DatabaseManager::instance();
  tutorId = db.getAssignedTutor(studentId);

  if (tutorId == -1) {
    // Try to assign a tutor
    DatabaseManager::UserInfo userInfo = db.getUserInfo(studentId);
    if (db.assignTutor(studentId, userInfo.degree, userInfo.nationality)) {
      tutorId = db.getAssignedTutor(studentId);
    }
  }

  if (tutorId != -1) {
    DatabaseManager::UserInfo tutorInfo = db.getUserInfo(tutorId);
    tutorInfoLabel->setText("Tutor Asignado: " + tutorInfo.name + " (" +
                            tutorInfo.degree + ", " + tutorInfo.nationality +
                            ")");
    loadChat();
  } else {
    tutorInfoLabel->setText(
        "No se ha podido asignar un tutor. Contacte con administracion.");
    messageInput->setEnabled(false);
    sendButton->setEnabled(false);
  }
}

void StudentWindow::loadChat() {
  if (tutorId == -1)
    return;

  auto messages = DatabaseManager::instance().getMessages(studentId, tutorId);
  QString html;
  for (const auto &msg : messages) {
    QString color = (msg.senderId == studentId) ? "blue" : "green";
    QString sender = (msg.senderId == studentId) ? "Yo" : "Tutor";
    html += "<div style='color:" + color + "'><b>" + sender + ":</b> " +
            msg.content + "</div>";
  }
  chatDisplay->setHtml(html);
  chatDisplay->moveCursor(QTextCursor::End);
}

void StudentWindow::refreshChat() { loadChat(); }

void StudentWindow::onSendMessage() {
  QString content = messageInput->text();
  if (content.isEmpty() || tutorId == -1)
    return;

  if (DatabaseManager::instance().sendMessage(studentId, tutorId, content)) {
    messageInput->clear();
    loadChat();
  }
}

void StudentWindow::onLogoutClicked() {
  LoginWindow *loginWindow = new LoginWindow();
  loginWindow->show();
  this->close();
}
