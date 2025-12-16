
#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include "../database/DatabaseManager.h"
#include <QDateEdit>    // Widget para editar fechas
#include <QLabel>       // Etiqueta de texto
#include <QLineEdit>    // Campo de texto de una linea
#include <QListWidget>  // Lista de elementos seleccionables
#include <QMessageBox>  // Ventana emergente para mensajes
#include <QPushButton>  // Boton pulsable
#include <QTabWidget>   // Widget para organizar contenido en pestañas
#include <QTableWidget> // Tabla para mostrar datos
#include <QTextEdit>    // Campo de texto multilineas
#include <QTimeEdit>    // Widget para editar horas
#include <QVBoxLayout>  // Layout vertical
#include <QWidget>      // Clase base para todos los widgets

class StudentWindow : public QWidget {
  Q_OBJECT

public:
  explicit StudentWindow(int studentId, QWidget *parent = nullptr);

private slots:
  // Slots para chat
  void onSendMessage();
  void refreshChat();
  void onLogoutClicked();

  // Slots para alertas
  void onMarkAlertAsRead();
  void refreshAlerts();

  // Slots para tutorias
  void onRequestAppointment();
  void refreshAppointments();
  void onCancelAppointment();

private:
  int studentId;
  int tutorId;

  QTabWidget *tabWidget;

  // Widgets para chat
  QWidget *chatTab;
  QLabel *tutorInfoLabel;
  QTextEdit *chatDisplay;
  QLineEdit *messageInput;
  QPushButton *sendButton;
  QPushButton *logoutButton;

  // Widgets para las alertas
  QWidget *alertsTab;
  QListWidget *alertsList;
  QPushButton *markReadButton;

  // Widgets para las tutorias
  QWidget *appointmentsTab;
  QDateEdit *dateEdit;
  QTimeEdit *timeEdit;
  QLineEdit *reasonInput;
  QPushButton *requestButton;
  QTableWidget *appointmentsTable;
  QPushButton *cancelAppointmentButton;

  void setupChatTab();
  void setupAlertsTab();
  void setupAppointmentsTab();
  void loadTutorInfo();
  void loadChat();
  void loadAlerts();
  void loadAppointments();
};

#endif // STUDENTWINDOW_H
