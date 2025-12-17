#include "../src/database/DatabaseManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <gtest/gtest.h>

class DatabaseTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Cerrar conexión DB para asegurar que se libera el bloqueo del archivo
    DatabaseManager::instance().close();

    // Limpiar base de datos existente para asegurar un estado fresco
    QString dataDir = QCoreApplication::applicationDirPath() + "/data";
    QString dbPath = dataDir + "/sistema_tutorizacion.db";
    QFile::remove(dbPath);

    // Inicializar DB
    DatabaseManager::instance().initDB();
  }

  void TearDown() override {
    // Opcional: Limpiar después de los tests
  }
};

// Test 2: Autenticación de Usuario (Login)
TEST_F(DatabaseTest, UserAuthentication) {
  QString role;
  int userId;

  // Login válido
  EXPECT_TRUE(
      DatabaseManager::instance().login("alberto", "1234", role, userId));
  EXPECT_EQ(role, "tutor");

  // Contraseña inválida
  EXPECT_FALSE(
      DatabaseManager::instance().login("alberto", "wrongpass", role, userId));

  // Usuario inexistente

  EXPECT_FALSE(
      DatabaseManager::instance().login("ghost", "1234", role, userId));
}

// Test 3: Lógica de Asignación de Tutor
TEST_F(DatabaseTest, TutorAssignment) {
  // Registrar un nuevo alumno que necesita tutor
  // "Informatica" y "Espana" deberían coincidir con tutores existentes (ej.
  // Alberto, Julian, etc.) Nota: initDB ya asigna tutores a los alumnos por
  // defecto.

  // Vamos a registrar un nuevo alumno manualmente para probar la asignación
  // Necesitamos asegurar un nombre de usuario único

  QString newStudent = "new_student_test";
  EXPECT_TRUE(DatabaseManager::instance().registerUser(
      newStudent, "pass", "alumno", "New Student", "Informatica", "Espana"));

  int studentId;
  QString role;
  DatabaseManager::instance().login(newStudent, "pass", role, studentId);

  int tutorId = DatabaseManager::instance().getAssignedTutor(studentId);
  EXPECT_NE(tutorId, -1);

  auto tutorInfo = DatabaseManager::instance().getUserInfo(tutorId);
  // Debería coincidir en grado o nacionalidad, o ser el mejor disponible

  bool matchDegree = (tutorInfo.degree == "Informatica");
  bool matchNationality = (tutorInfo.nationality == "Espana");
  EXPECT_TRUE(matchDegree || matchNationality);
}

// Test 4: Creación de Solicitud de Tutoría
TEST_F(DatabaseTest, AppointmentRequest) {
  QString role;
  int studentId;
  ASSERT_TRUE(
      DatabaseManager::instance().login("javier", "1234", role, studentId));

  int tutorId = DatabaseManager::instance().getAssignedTutor(studentId);
  ASSERT_NE(tutorId, -1);

  QString date = "2025-12-25";
  QString time = "10:00";
  QString reason = "Duda practica";

  EXPECT_TRUE(DatabaseManager::instance().requestAppointment(
      studentId, tutorId, date, time, reason));

  auto appointments =
      DatabaseManager::instance().getAppointments(studentId, false);
  bool found = false;
  for (const auto &app : appointments) {
    if (app.date == date && app.time == time && app.reason == reason &&
        app.status == "REQUESTED") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found);
}

// Test 5: Actualización de Estado de Tutoría
TEST_F(DatabaseTest, AppointmentStatusUpdate) {
  QString role;
  int studentId;
  ASSERT_TRUE(
      DatabaseManager::instance().login("javier", "1234", role, studentId));

  int tutorId = DatabaseManager::instance().getAssignedTutor(studentId);

  DatabaseManager::instance().requestAppointment(
      studentId, tutorId, "2025-12-30", "11:00", "Revision");
  auto appointments =
      DatabaseManager::instance().getAppointments(studentId, false);
  int appId = -1;
  for (const auto &app : appointments) {
    if (app.reason == "Revision") {
      appId = app.id;
      break;
    }
  }
  ASSERT_NE(appId, -1);

  // Actualizar estado a PROGRAMADA
  EXPECT_TRUE(
      DatabaseManager::instance().updateAppointmentStatus(appId, "SCHEDULED"));

  // Verificar

  auto updatedAppointments =
      DatabaseManager::instance().getAppointments(studentId, false);
  for (const auto &app : updatedAppointments) {
    if (app.id == appId) {
      EXPECT_EQ(app.status, "SCHEDULED");
      break;
    }
  }
}

// Test 6: Sistema de Mensajería
TEST_F(DatabaseTest, MessagingSystem) {
  QString role;
  int studentId;
  ASSERT_TRUE(
      DatabaseManager::instance().login("javier", "1234", role, studentId));

  int tutorId = DatabaseManager::instance().getAssignedTutor(studentId);

  QString msgContent = "Hola tutor";
  EXPECT_TRUE(
      DatabaseManager::instance().sendMessage(studentId, tutorId, msgContent));

  auto messages = DatabaseManager::instance().getMessages(studentId, tutorId);
  ASSERT_FALSE(messages.empty());
  EXPECT_EQ(messages.back().content, msgContent);
  EXPECT_EQ(messages.back().senderId, studentId);
}

// Test 7: Sistema de Alertas
TEST_F(DatabaseTest, AlertSystem) {
  QString tutorRole, studentRole;
  int tutorId, studentId;

  // Obtener IDs (Alberto es tutor, Javier es alumno)
  ASSERT_TRUE(
      DatabaseManager::instance().login("alberto", "1234", tutorRole, tutorId));
  ASSERT_TRUE(DatabaseManager::instance().login("javier", "1234", studentRole,
                                                studentId));

  QString content = "Alerta de prueba";

  // Enviar alerta
  EXPECT_TRUE(
      DatabaseManager::instance().sendAlert(tutorId, studentId, content));

  // Verificar recepción
  auto alerts = DatabaseManager::instance().getAlerts(studentId);
  ASSERT_FALSE(alerts.empty());
  // getAlerts ordena por timestamp DESC, así que la primera es la más reciente
  EXPECT_EQ(alerts.front().content, content);
  EXPECT_FALSE(alerts.front().isRead);

  // Marcar como leída
  int alertId = alerts.front().id;
  EXPECT_TRUE(DatabaseManager::instance().markAlertAsRead(alertId));

  // Verificar que está leída
  alerts = DatabaseManager::instance().getAlerts(studentId);
  EXPECT_TRUE(alerts.front().isRead);
}
