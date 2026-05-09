#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QTabWidget>

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void addStudent();
    void updateStudent();
    void deleteStudent();
    void clearStudentForm();
    void loadSelectedStudent(int row, int column);
    void searchStudents();

    void addCourse();
    void updateCourse();
    void deleteCourse();
    void clearCourseForm();
    void loadSelectedCourse(int row, int column);
    void registerCourse();

    void saveGrade();
    void deleteGrade();
    void calculateSelectedGpa();
    void generateTranscriptText();
    void loadSelectedGrade(int row, int column);

private:
    QTabWidget *tabs;
    QLabel *statusLabel;

    QLineEdit *studentNameEdit;
    QLineEdit *nationalIdEdit;
    QComboBox *genderCombo;
    QLineEdit *dobEdit;
    QLineEdit *phoneEdit;
    QLineEdit *programEdit;
    QSpinBox *levelSpin;
    QLineEdit *studentSearchEdit;
    QTableWidget *studentTable;

    QLineEdit *courseCodeEdit;
    QLineEdit *courseTitleEdit;
    QSpinBox *creditSpin;
    QLineEdit *registerStudentIdEdit;
    QLineEdit *registerCourseCodeEdit;
    QTableWidget *courseTable;

    QLineEdit *gradeStudentIdEdit;
    QLineEdit *gradeCourseCodeEdit;
    QDoubleSpinBox *midtermSpin;
    QDoubleSpinBox *finalSpin;
    QTableWidget *gradeTable;
    QTextEdit *transcriptBox;

    QWidget *createDashboardTab();
    QWidget *createStudentsTab();
    QWidget *createCoursesTab();
    QWidget *createGradesTab();

    void refreshAll();
    void refreshDashboard();
    void refreshStudents(const QString &filter = QString());
    void refreshCourses();
    void refreshGrades();
    void showStatus(const QString &message);

    int selectedStudentIndex() const;
    int selectedCourseIndex() const;
    int selectedGradeIndex() const;
    int findStudentById(int id) const;
    int findCourseByCode(const QString &code) const;
    bool studentFormIsValid(QString &error) const;
    bool courseFormIsValid(QString &error) const;
};
