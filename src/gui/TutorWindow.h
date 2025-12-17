#ifndef TUTORWINDOW_H
#define TUTORWINDOW_H

#include "../database/DatabaseManager.h"
#include <QCheckBox>    // Casilla de verificacion
#include <QDateEdit>    // Widget para editar fechas
#include <QFormLayout>  // Layout para formularios
#include <QGroupBox>    // Caja de grupo con titulo
#include <QHBoxLayout>  // Layout horizontal
#include <QLabel>       // Etiqueta de texto
#include <QLineEdit>    // Campo de texto de una linea
#include <QListWidget>  // Lista de elementos seleccionables
#include <QPushButton>  // Boton pulsable
#include <QTabWidget>   // Widget para organizar contenido en pestañas
#include <QTableWidget> // Tabla para mostrar datos
#include <QTextEdit>    // Campo de texto multilineas
#include <QTimeEdit>    // Widget para editar horas
#include <QVBoxLayout>  // Layout vertical
#include <QWidget>      // Clase base para todos los widgets
#include <vector>

class TutorWindow : public QWidget {
  Q_OBJECT

public:
  explicit TutorWindow(int tutorId, QWidget *parent = nullptr);

private slots:
  void onStudentSelected(QListWidgetItem *item);
  void onSendMessage();
  void refreshChat();
  void onLogoutClicked();

  // Slots para alertas
  void onSendAlert();
  void onSelectAllStudents(int state);

  // Slots para tutorias
  void onAppointmentSelected(int row, int column);
  void onAcceptAppointment();
  void onModifyAppointment();
  void onChangeStatus(const QString &newStatus);
  void refreshAppointments();

private:
  int tutorId;
  int currentStudentId;

  QTabWidget *tabWidget;

  // Widgets para Chat
  QWidget *chatTab;
  QListWidget *studentList;
  QTextEdit *chatDisplay;
  QLineEdit *messageInput;
  QPushButton *sendButton;
  QPushButton *logoutButton;
  QLabel *chatLabel;

  // Widgets para Alertas
  QWidget *alertsTab;
  QListWidget *alertStudentList; // Lista con checkboxes
  QCheckBox *selectAllCheckBox;
  QTextEdit *alertMessageInput;
  QPushButton *sendAlertButton;

  // Widgets para Tutorías
  QWidget *appointmentsTab;
  QTableWidget *appointmentsTable;
  QGroupBox *appointmentDetailsGroup;
  QLineEdit *appStudentName;
  QDateEdit *appDateEdit;
  QTimeEdit *appTimeEdit;
  QLineEdit *appReason;
  QTextEdit *appTutorNotes;
  QPushButton *appAcceptButton;
  QPushButton *appModifyButton;
  QPushButton *appStartButton;
  QPushButton *appFinishButton;
  int currentAppointmentId;

  std::vector<DatabaseManager::UserInfo> students;

  void setupChatTab();
  void setupAlertsTab();
  void setupAppointmentsTab();
  void loadStudents();
  void loadChat();
  void loadAlertStudents(); // Cargar estudiantes en la lista de alertas
  void loadAppointments();
  void updateAppointmentButtons(const QString &status);
};

#endif // TUTORWINDOW_H
