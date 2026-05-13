#include "mainwindow.h"
#include "globals.h"
#include "student.h"
#include "course.h"
#include "grades.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QFrame>
#include <QApplication>

static QString qs(const std::string &value)
{
    return QString::fromStdString(value);
}

static std::string ss(const QString &value)
{
    return value.trimmed().toStdString();
}

static bool onlyDigits(const QString &value)
{
    if (value.isEmpty()) return false;
    for (QChar ch : value)
        if (!ch.isDigit()) return false;
    return true;
}

static bool validTwoPartName(const QString &value)
{
    QStringList parts = value.simplified().split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 2) return false;

    for (const QString &part : parts)
        for (QChar ch : part)
            if (!ch.isLetter()) return false;

    return true;
}

static bool validProgramName(const QString &value)
{
    QString program = value.trimmed().toUpper();
    return program == "CSE" || program == "CCE" || program == "MCT";
}

static bool validStudentIdText(const QString &value)
{
    return onlyDigits(value.trimmed()) && value.trimmed().length() == 6;
}

static bool ageAtLeast17(const QString &dobText)
{
    QDate dob = QDate::fromString(dobText.trimmed(), "dd/MM/yyyy");
    if (!dob.isValid()) return false;
    return dob.daysTo(QDate::currentDate()) >= 17 * 365;
}

static QLabel *metricCard(const QString &title, const QString &value)
{
    QLabel *label = new QLabel("<b>" + title + "</b><br><span style='font-size:28px'>" + value + "</span>");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumHeight(95);
    label->setFrameShape(QFrame::StyledPanel);
    label->setStyleSheet("QLabel { border-radius: 12px; padding: 12px; background: #f5f5f5; } ");
    return label;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Student Information System");
    resize(1100, 720);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QLabel *title = new QLabel("Student Information System");
    title->setStyleSheet("font-size: 24px; font-weight: 700; margin: 4px;");

    statusLabel = new QLabel("Ready");
    statusLabel->setStyleSheet("padding: 8px; color: #555;");

    tabs = new QTabWidget;
    tabs->addTab(createDashboardTab(), "Dashboard");
    tabs->addTab(createStudentsTab(), "Students");
    tabs->addTab(createCoursesTab(), "Courses");
    tabs->addTab(createGradesTab(), "Grades");

    mainLayout->addWidget(title);
    mainLayout->addWidget(tabs);
    mainLayout->addWidget(statusLabel);
    setCentralWidget(central);

    refreshAll();
}

QWidget *MainWindow::createDashboardTab()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    QLabel *intro = new QLabel("Manage students, courses, registrations, grades, GPA, and transcripts from one interface.");
    intro->setWordWrap(true);
    intro->setStyleSheet("font-size: 15px; margin: 8px;");

    QHBoxLayout *cards = new QHBoxLayout;
    cards->addWidget(metricCard("Students", QString::number(studentCount)));
    cards->addWidget(metricCard("Courses", QString::number(courseCount)));
    cards->addWidget(metricCard("Grades", QString::number(gradeCount)));

    QPushButton *refreshBtn = new QPushButton("Refresh Data");
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshAll);

    layout->addWidget(intro);
    layout->addLayout(cards);
    layout->addWidget(refreshBtn, 0, Qt::AlignLeft);
    layout->addStretch();
    return page;
}

QWidget *MainWindow::createStudentsTab()
{
    QWidget *page = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(page);

    QGroupBox *formBox = new QGroupBox("Student Details");
    QFormLayout *form = new QFormLayout(formBox);

    studentNameEdit = new QLineEdit;
    nationalIdEdit = new QLineEdit;
    genderCombo = new QComboBox;
    genderCombo->addItems({"M", "F"});
    dobEdit = new QLineEdit;
    dobEdit->setPlaceholderText("DD/MM/YYYY");
    phoneEdit = new QLineEdit;
    programEdit = new QLineEdit;
    levelSpin = new QSpinBox;
    levelSpin->setRange(1, 4);

    form->addRow("Name", studentNameEdit);
    form->addRow("National ID", nationalIdEdit);
    form->addRow("Gender", genderCombo);
    form->addRow("DOB", dobEdit);
    form->addRow("Phone", phoneEdit);
    form->addRow("Program", programEdit);
    form->addRow("Level", levelSpin);

    QPushButton *addBtn = new QPushButton("Add Student");
    QPushButton *updateBtn = new QPushButton("Update Selected");
    QPushButton *deleteBtn = new QPushButton("Delete Selected");
    QPushButton *clearBtn = new QPushButton("Clear");

    form->addRow(addBtn, updateBtn);
    form->addRow(deleteBtn, clearBtn);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::updateStudent);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteStudent);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearStudentForm);

    QVBoxLayout *right = new QVBoxLayout;
    QHBoxLayout *searchLine = new QHBoxLayout;
    studentSearchEdit = new QLineEdit;
    studentSearchEdit->setPlaceholderText("Search by name or ID");
    QPushButton *searchBtn = new QPushButton("Search");
    QPushButton *showAllBtn = new QPushButton("Show All");
    searchLine->addWidget(studentSearchEdit);
    searchLine->addWidget(searchBtn);
    searchLine->addWidget(showAllBtn);

    studentTable = new QTableWidget;
    studentTable->setColumnCount(8);
    studentTable->setHorizontalHeaderLabels({"ID", "Name", "National ID", "Gender", "DOB", "Phone", "Program", "GPA"});
    studentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(studentTable, &QTableWidget::cellClicked, this, &MainWindow::loadSelectedStudent);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::searchStudents);
    connect(showAllBtn, &QPushButton::clicked, this, [this]() { studentSearchEdit->clear(); refreshStudents(); });

    right->addLayout(searchLine);
    right->addWidget(studentTable);

    layout->addWidget(formBox, 1);
    layout->addLayout(right, 3);
    return page;
}

QWidget *MainWindow::createCoursesTab()
{
    QWidget *page = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(page);

    QGroupBox *formBox = new QGroupBox("Course Details");
    QFormLayout *form = new QFormLayout(formBox);

    courseCodeEdit = new QLineEdit;
    courseCodeEdit->setPlaceholderText("Example: CSE101");
    courseTitleEdit = new QLineEdit;
    creditSpin = new QSpinBox;
    creditSpin->setRange(1, 4);

    form->addRow("Course Code", courseCodeEdit);
    form->addRow("Title", courseTitleEdit);
    form->addRow("Credit Hours", creditSpin);

    QPushButton *addBtn = new QPushButton("Add Course");
    QPushButton *updateBtn = new QPushButton("Update Selected");
    QPushButton *deleteBtn = new QPushButton("Delete Selected");
    QPushButton *clearBtn = new QPushButton("Clear");
    form->addRow(addBtn, updateBtn);
    form->addRow(deleteBtn, clearBtn);

    QGroupBox *registerBox = new QGroupBox("Register Course To Student");
    QFormLayout *registerForm = new QFormLayout(registerBox);
    registerStudentIdEdit = new QLineEdit;
    registerCourseCodeEdit = new QLineEdit;
    QPushButton *registerBtn = new QPushButton("Register Course");
    registerForm->addRow("Student ID", registerStudentIdEdit);
    registerForm->addRow("Course Code", registerCourseCodeEdit);
    registerForm->addRow(registerBtn);
    form->addRow(registerBox);

    courseTable = new QTableWidget;
    courseTable->setColumnCount(3);
    courseTable->setHorizontalHeaderLabels({"Code", "Title", "Credit Hours"});
    courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    courseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addCourse);
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::updateCourse);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteCourse);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearCourseForm);
    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::registerCourse);
    connect(courseTable, &QTableWidget::cellClicked, this, &MainWindow::loadSelectedCourse);

    layout->addWidget(formBox, 1);
    layout->addWidget(courseTable, 3);
    return page;
}

QWidget *MainWindow::createGradesTab()
{
    QWidget *page = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(page);

    QGroupBox *formBox = new QGroupBox("Grade Details");
    QFormLayout *form = new QFormLayout(formBox);

    gradeStudentIdEdit = new QLineEdit;
    gradeCourseCodeEdit = new QLineEdit;
    midtermSpin = new QDoubleSpinBox;
    finalSpin = new QDoubleSpinBox;
    midtermSpin->setRange(0, 40);
    finalSpin->setRange(0, 60);
    midtermSpin->setDecimals(2);
    finalSpin->setDecimals(2);

    form->addRow("Student ID", gradeStudentIdEdit);
    form->addRow("Course Code", gradeCourseCodeEdit);
    form->addRow("Midterm", midtermSpin);
    form->addRow("Final", finalSpin);

    QPushButton *saveBtn = new QPushButton("Save Grade");
    QPushButton *deleteBtn = new QPushButton("Delete Selected Grade");
    QPushButton *gpaBtn = new QPushButton("Calculate GPA");
    QPushButton *transcriptBtn = new QPushButton("Generate Transcript");

    form->addRow(saveBtn);
    form->addRow(deleteBtn);
    form->addRow(gpaBtn);
    form->addRow(transcriptBtn);

    transcriptBox = new QTextEdit;
    transcriptBox->setReadOnly(true);
    transcriptBox->setPlaceholderText("Transcript will appear here.");
    form->addRow("Transcript", transcriptBox);

    gradeTable = new QTableWidget;
    gradeTable->setColumnCount(7);
    gradeTable->setHorizontalHeaderLabels({"Student ID", "Course", "Midterm", "Final", "Total", "Letter", "Points"});
    gradeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    gradeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    gradeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveGrade);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteGrade);
    connect(gpaBtn, &QPushButton::clicked, this, &MainWindow::calculateSelectedGpa);
    connect(transcriptBtn, &QPushButton::clicked, this, &MainWindow::generateTranscriptText);
    connect(gradeTable, &QTableWidget::cellClicked, this, &MainWindow::loadSelectedGrade);

    layout->addWidget(formBox, 1);
    layout->addWidget(gradeTable, 3);
    return page;
}

void MainWindow::refreshAll()
{
    refreshDashboard();
    refreshStudents();
    refreshCourses();
    refreshGrades();
}

void MainWindow::refreshDashboard()
{
    QWidget *oldDashboard = tabs->widget(0);
    tabs->removeTab(0);
    oldDashboard->deleteLater();
    tabs->insertTab(0, createDashboardTab(), "Dashboard");
}

void MainWindow::refreshStudents(const QString &filter)
{
    studentTable->setRowCount(0);
    QString f = filter.trimmed().toLower();

    for (int i = 0; i < studentCount; ++i)
    {
        QString id = QString::number(students[i].ID);
        QString name = qs(students[i].name);
        if (!f.isEmpty() && !name.toLower().contains(f) && !id.contains(f))
            continue;

        int row = studentTable->rowCount();
        studentTable->insertRow(row);
        studentTable->setItem(row, 0, new QTableWidgetItem(id));
        studentTable->setItem(row, 1, new QTableWidgetItem(name));
        studentTable->setItem(row, 2, new QTableWidgetItem(qs(students[i].nationalID)));
        studentTable->setItem(row, 3, new QTableWidgetItem(QString(students[i].gender)));
        studentTable->setItem(row, 4, new QTableWidgetItem(students[i].DOB));
        studentTable->setItem(row, 5, new QTableWidgetItem(qs(students[i].phone)));
        studentTable->setItem(row, 6, new QTableWidgetItem(qs(students[i].program)));
        studentTable->setItem(row, 7, new QTableWidgetItem(QString::number(students[i].GPA, 'f', 2)));
    }
}

void MainWindow::refreshCourses()
{
    courseTable->setRowCount(0);
    for (int i = 0; i < courseCount; ++i)
    {
        int row = courseTable->rowCount();
        courseTable->insertRow(row);
        courseTable->setItem(row, 0, new QTableWidgetItem(qs(courses[i].code)));
        courseTable->setItem(row, 1, new QTableWidgetItem(qs(courses[i].title)));
        courseTable->setItem(row, 2, new QTableWidgetItem(QString::number(courses[i].creditHours)));
    }
}

void MainWindow::refreshGrades()
{
    gradeTable->setRowCount(0);
    for (int i = 0; i < gradeCount; ++i)
    {
        int row = gradeTable->rowCount();
        gradeTable->insertRow(row);
        gradeTable->setItem(row, 0, new QTableWidgetItem(QString::number(grades[i].studentID)));
        gradeTable->setItem(row, 1, new QTableWidgetItem(qs(grades[i].courseCode)));
        gradeTable->setItem(row, 2, new QTableWidgetItem(QString::number(grades[i].midterm, 'f', 2)));
        gradeTable->setItem(row, 3, new QTableWidgetItem(QString::number(grades[i].finalExam, 'f', 2)));
        gradeTable->setItem(row, 4, new QTableWidgetItem(QString::number(grades[i].total, 'f', 2)));
        gradeTable->setItem(row, 5, new QTableWidgetItem(QString(grades[i].letterGrade)));
        gradeTable->setItem(row, 6, new QTableWidgetItem(QString::number(grades[i].points, 'f', 2)));
    }
}

void MainWindow::showStatus(const QString &message)
{
    statusLabel->setText(message);
}

int MainWindow::selectedStudentIndex() const
{
    int row = studentTable->currentRow();
    if (row < 0) return -1;
    int id = studentTable->item(row, 0)->text().toInt();
    return findStudentById(id);
}

int MainWindow::selectedCourseIndex() const
{
    int row = courseTable->currentRow();
    if (row < 0) return -1;
    return findCourseByCode(courseTable->item(row, 0)->text());
}

int MainWindow::selectedGradeIndex() const
{
    int row = gradeTable->currentRow();
    if (row < 0) return -1;
    int sid = gradeTable->item(row, 0)->text().toInt();
    QString code = gradeTable->item(row, 1)->text();
    for (int i = 0; i < gradeCount; ++i)
        if (grades[i].studentID == sid && QString::fromStdString(grades[i].courseCode) == code)
            return i;
    return -1;
}

int MainWindow::findStudentById(int id) const
{
    for (int i = 0; i < studentCount; ++i)
        if (students[i].ID == id) return i;
    return -1;
}

int MainWindow::findCourseByCode(const QString &code) const
{
    QString c = code.trimmed().toUpper();
    for (int i = 0; i < courseCount; ++i)
        if (QString::fromStdString(courses[i].code).toUpper() == c) return i;
    return -1;
}

bool MainWindow::studentFormIsValid(QString &error) const
{
    QString name = studentNameEdit->text().trimmed();
    QString nationalId = nationalIdEdit->text().trimmed();
    QString dobText = dobEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString program = programEdit->text().trimmed().toUpper();

    if (!validTwoPartName(name))
    {
        error = "Name must be exactly first and last name, letters only.";
        return false;
    }

    if (!onlyDigits(nationalId) || nationalId.length() != 14 || nationalId.startsWith('0'))
    {
        error = "National ID must be 14 digits and cannot start with 0.";
        return false;
    }

    if (dobText.size() != 10)
    {
        error = "DOB must be in DD/MM/YYYY format.";
        return false;
    }

    char dob[11];
    strncpy(dob, dobText.toStdString().c_str(), 10);
    dob[10] = '\0';

    if (!isValid(dob) || !ageAtLeast17(dobText))
    {
        error = "DOB is invalid or age is less than 17.";
        return false;
    }

    if (!onlyDigits(phone) || phone.length() != 11 || !phone.startsWith("01"))
    {
        error = "Phone must be 11 digits and start with 01.";
        return false;
    }

    if (!validProgramName(program))
    {
        error = "Program must be CSE, CCE, or MCT.";
        return false;
    }

    if (levelSpin->value() < 1 || levelSpin->value() > 4)
    {
        error = "Academic level must be from 1 to 4.";
        return false;
    }

    return true;
}

bool MainWindow::courseFormIsValid(QString &error) const
{
    QString code = courseCodeEdit->text().trimmed().toUpper();
    if (!isValidCourseCode(code.toStdString()))
    {
        error = "Course code must be 3 capital letters followed by 3 numbers, like CSE101.";
        return false;
    }

    if (courseTitleEdit->text().trimmed().isEmpty())
    {
        error = "Course title cannot be empty.";
        return false;
    }

    if (creditSpin->value() < 1 || creditSpin->value() > 4)
    {
        error = "Credit hours must be from 1 to 4.";
        return false;
    }

    return true;
}

void MainWindow::addStudent()
{
    QString error;
    if (!studentFormIsValid(error)) { QMessageBox::warning(this, "Check Student", error); return; }
    if (studentCount >= 1200) { QMessageBox::warning(this, "Limit", "Student storage is full."); return; }

    for (int i = 0; i < studentCount; ++i)
        if (QString::fromStdString(students[i].nationalID) == nationalIdEdit->text().trimmed())
        { QMessageBox::warning(this, "Duplicate", "National ID already exists."); return; }

    Student s;
    s.name = ss(studentNameEdit->text());
    s.nationalID = ss(nationalIdEdit->text());
    s.gender = genderCombo->currentText().toStdString()[0];
    strncpy(s.DOB, dobEdit->text().trimmed().toStdString().c_str(), 10);
    s.DOB[10] = '\0';
    s.phone = ss(phoneEdit->text());
    s.program = ss(programEdit->text().toUpper());
    s.level = levelSpin->value();
    s.GPA = 0.0f;
    s.registeredCourseCount = 0;

    int year = QDate::currentDate().year() - 2000;
    if (studentCount == 0)
        s.ID = year * 10000 + 1;
    else
        s.ID = students[studentCount - 1].ID + 1;

    students[studentCount++] = s;
    saveToFile(students, studentCount);
    clearStudentForm();
    refreshAll();
    showStatus("Student saved.");
}

void MainWindow::updateStudent()
{
    int isndex = selectedStudentIndex();
    if (index < 0) { QMessageBox::warning(this, "Select Student", "Select a student first."); return; }
    QString error;
    if (!studentFormIsValid(error)) { QMessageBox::warning(this, "Check Student", error); return; }

    for (int i = 0; i < studentCount; ++i)
        if (i != index && QString::fromStdString(students[i].nationalID) == nationalIdEdit->text().trimmed())
        { QMessageBox::warning(this, "Duplicate", "National ID already exists."); return; }

    students[index].name = ss(studentNameEdit->text());
    students[index].nationalID = ss(nationalIdEdit->text());
    students[index].gender = genderCombo->currentText().toStdString()[0];
    strncpy(students[index].DOB, dobEdit->text().trimmed().toStdString().c_str(), 10);
    students[index].DOB[10] = '\0';
    students[index].phone = ss(phoneEdit->text());
    students[index].program = ss(programEdit->text().toUpper());
    students[index].level = levelSpin->value();

    saveToFile(students, studentCount);
    refreshAll();
    showStatus("Student updated.");
}

void MainWindow::deleteStudent()
{
    int index = selectedStudentIndex();
    if (index < 0) { QMessageBox::warning(this, "Select Student", "Select a student first."); return; }
    if (QMessageBox::question(this, "Delete", "Delete selected student?") != QMessageBox::Yes) return;

    int id = students[index].ID;
    for (int i = index; i < studentCount - 1; ++i) students[i] = students[i + 1];
    --studentCount;

    for (int i = 0; i < gradeCount; )
    {
        if (grades[i].studentID == id)
        {
            for (int j = i; j < gradeCount - 1; ++j) grades[j] = grades[j + 1];
            --gradeCount;
        }
        else ++i;
    }

    saveToFile(students, studentCount);
    saveGradesToFile();
    clearStudentForm();
    refreshAll();
    showStatus("Student deleted.");
}

void MainWindow::clearStudentForm()
{
    studentNameEdit->clear();
    nationalIdEdit->clear();
    genderCombo->setCurrentIndex(0);
    dobEdit->clear();
    phoneEdit->clear();
    programEdit->clear();
    levelSpin->setValue(1);
}

void MainWindow::loadSelectedStudent(int row, int)
{
    if (row < 0) return;
    int index = findStudentById(studentTable->item(row, 0)->text().toInt());
    if (index < 0) return;

    studentNameEdit->setText(qs(students[index].name));
    nationalIdEdit->setText(qs(students[index].nationalID));
    genderCombo->setCurrentText(QString(students[index].gender).toUpper());
    dobEdit->setText(students[index].DOB);
    phoneEdit->setText(qs(students[index].phone));
    programEdit->setText(qs(students[index].program));
    levelSpin->setValue(students[index].level);
}

void MainWindow::searchStudents()
{
    refreshStudents(studentSearchEdit->text());
}

void MainWindow::addCourse()
{
    QString error;
    if (!courseFormIsValid(error)) { QMessageBox::warning(this, "Check Course", error); return; }
    if (courseCount >= 200) { QMessageBox::warning(this, "Limit", "Course storage is full."); return; }
    if (findCourseByCode(courseCodeEdit->text()) >= 0) { QMessageBox::warning(this, "Duplicate", "Course code already exists."); return; }

    courses[courseCount].code = ss(courseCodeEdit->text().toUpper());
    courses[courseCount].title = ss(courseTitleEdit->text());
    courses[courseCount].creditHours = creditSpin->value();
    ++courseCount;

    saveCoursesToFile();
    clearCourseForm();
    refreshAll();
    showStatus("Course saved.");
}

void MainWindow::updateCourse()
{
    int index = selectedCourseIndex();
    if (index < 0) { QMessageBox::warning(this, "Select Course", "Select a course first."); return; }
    QString oldCode = QString::fromStdString(courses[index].code);
    QString error;
    if (!courseFormIsValid(error)) { QMessageBox::warning(this, "Check Course", error); return; }

    int duplicate = findCourseByCode(courseCodeEdit->text());
    if (duplicate >= 0 && duplicate != index) { QMessageBox::warning(this, "Duplicate", "Course code already exists."); return; }

    QString newCode = courseCodeEdit->text().trimmed().toUpper();
    courses[index].code = ss(newCode);
    courses[index].title = ss(courseTitleEdit->text());
    courses[index].creditHours = creditSpin->value();

    for (int i = 0; i < studentCount; ++i)
        for (int j = 0; j < students[i].registeredCourseCount; ++j)
            if (QString::fromStdString(students[i].registeredCourses[j]).toUpper() == oldCode.toUpper())
                students[i].registeredCourses[j] = newCode.toStdString();

    for (int i = 0; i < gradeCount; ++i)
        if (QString::fromStdString(grades[i].courseCode).toUpper() == oldCode.toUpper())
            grades[i].courseCode = newCode.toStdString();

    saveCoursesToFile();
    saveToFile(students, studentCount);
    saveGradesToFile();
    refreshAll();
    showStatus("Course updated.");
}

void MainWindow::deleteCourse()
{
    int index = selectedCourseIndex();
    if (index < 0) { QMessageBox::warning(this, "Select Course", "Select a course first."); return; }
    if (QMessageBox::question(this, "Delete", "Delete selected course?") != QMessageBox::Yes) return;

    QString code = QString::fromStdString(courses[index].code).toUpper();
    for (int i = index; i < courseCount - 1; ++i) courses[i] = courses[i + 1];
    --courseCount;

    for (int i = 0; i < studentCount; ++i)
    {
        for (int j = 0; j < students[i].registeredCourseCount; )
        {
            if (QString::fromStdString(students[i].registeredCourses[j]).toUpper() == code)
            {
                for (int k = j; k < students[i].registeredCourseCount - 1; ++k)
                    students[i].registeredCourses[k] = students[i].registeredCourses[k + 1];
                --students[i].registeredCourseCount;
            }
            else ++j;
        }
    }

    for (int i = 0; i < gradeCount; )
    {
        if (QString::fromStdString(grades[i].courseCode).toUpper() == code)
        {
            for (int j = i; j < gradeCount - 1; ++j) grades[j] = grades[j + 1];
            --gradeCount;
        }
        else ++i;
    }

    saveCoursesToFile();
    saveToFile(students, studentCount);
    saveGradesToFile();
    clearCourseForm();
    refreshAll();
    showStatus("Course deleted.");
}

void MainWindow::clearCourseForm()
{
    courseCodeEdit->clear();
    courseTitleEdit->clear();
    creditSpin->setValue(1);
}

void MainWindow::loadSelectedCourse(int row, int)
{
    if (row < 0) return;
    int index = findCourseByCode(courseTable->item(row, 0)->text());
    if (index < 0) return;
    courseCodeEdit->setText(qs(courses[index].code));
    courseTitleEdit->setText(qs(courses[index].title));
    creditSpin->setValue(courses[index].creditHours);
    registerCourseCodeEdit->setText(qs(courses[index].code));
}

void MainWindow::registerCourse()
{
    QString studentIdText = registerStudentIdEdit->text().trimmed();
    QString courseCode = registerCourseCodeEdit->text().trimmed().toUpper();

    if (!validStudentIdText(studentIdText))
    {
        QMessageBox::warning(this, "Student", "Student ID must be 6 digits.");
        return;
    }

    if (!isValidCourseCode(courseCode.toStdString()))
    {
        QMessageBox::warning(this, "Course", "Course code format must be like CSE101.");
        return;
    }

    int studentIndex = findStudentById(studentIdText.toInt());
    int courseIndex = findCourseByCode(courseCode);
    if (studentIndex < 0) { QMessageBox::warning(this, "Student", "Student ID was not found."); return; }
    if (courseIndex < 0) { QMessageBox::warning(this, "Course", "Course code was not found."); return; }
    if (students[studentIndex].registeredCourseCount >= 10) { QMessageBox::warning(this, "Limit", "Student cannot register more than 10 courses."); return; }

    QString code = QString::fromStdString(courses[courseIndex].code).toUpper();
    for (int i = 0; i < students[studentIndex].registeredCourseCount; ++i)
        if (QString::fromStdString(students[studentIndex].registeredCourses[i]).toUpper() == code)
        { QMessageBox::information(this, "Already Registered", "Student is already registered in this course."); return; }

    students[studentIndex].registeredCourses[students[studentIndex].registeredCourseCount++] = code.toStdString();
    saveToFile(students, studentCount);
    refreshAll();
    showStatus("Course registered.");
}

void MainWindow::saveGrade()
{
    QString studentIdText = gradeStudentIdEdit->text().trimmed();
    QString courseCode = gradeCourseCodeEdit->text().trimmed().toUpper();

    if (!validStudentIdText(studentIdText))
    {
        QMessageBox::warning(this, "Student", "Student ID must be 6 digits.");
        return;
    }

    if (!isValidCourseCode(courseCode.toStdString()))
    {
        QMessageBox::warning(this, "Course", "Course code format must be like CSE101.");
        return;
    }

    int studentIndex = findStudentById(studentIdText.toInt());
    int courseIndex = findCourseByCode(courseCode);
    if (studentIndex < 0) { QMessageBox::warning(this, "Student", "Student ID does not exist."); return; }
    if (courseIndex < 0) { QMessageBox::warning(this, "Course", "Course code does not exist."); return; }

    QString code = QString::fromStdString(courses[courseIndex].code).toUpper();
    bool registered = false;
    for (int i = 0; i < students[studentIndex].registeredCourseCount; ++i)
        if (QString::fromStdString(students[studentIndex].registeredCourses[i]).toUpper() == code)
            registered = true;

    if (!registered)
    {
        QMessageBox::warning(this, "Registration", "This student is not registered in this course.");
        return;
    }

    int existing = -1;
    for (int i = 0; i < gradeCount; ++i)
        if (grades[i].studentID == students[studentIndex].ID && QString::fromStdString(grades[i].courseCode).toUpper() == code)
            existing = i;

    if (existing < 0)
    {
        if (gradeCount >= 5000) { QMessageBox::warning(this, "Limit", "Grade storage is full."); return; }
        existing = gradeCount++;
    }

    grades[existing].studentID = students[studentIndex].ID;
    grades[existing].courseCode = code.toStdString();
    grades[existing].midterm = midtermSpin->value();
    grades[existing].finalExam = finalSpin->value();
    grades[existing].total = grades[existing].midterm + grades[existing].finalExam;
    grades[existing].letterGrade = getLetterGrade(grades[existing].total);
    grades[existing].points = getGradePoints(grades[existing].letterGrade);

    saveGradesToFile();
    saveToFile(students, studentCount);
    refreshAll();
    showStatus("Grade saved.");
}

void MainWindow::deleteGrade()
{
    int index = selectedGradeIndex();
    if (index < 0) { QMessageBox::warning(this, "Select Grade", "Select a grade first."); return; }
    if (QMessageBox::question(this, "Delete", "Delete selected grade?") != QMessageBox::Yes) return;

    for (int i = index; i < gradeCount - 1; ++i) grades[i] = grades[i + 1];
    --gradeCount;
    saveGradesToFile();
    refreshAll();
    showStatus("Grade deleted.");
}

void MainWindow::calculateSelectedGpa()
{
    QString studentIdText = gradeStudentIdEdit->text().trimmed();
    if (!validStudentIdText(studentIdText)) { QMessageBox::warning(this, "Student", "Student ID must be 6 digits."); return; }
    int id = studentIdText.toInt();
    int studentIndex = findStudentById(id);
    if (studentIndex < 0) { QMessageBox::warning(this, "Student", "Enter a valid student ID."); return; }

    float totalPoints = 0;
    int totalCredits = 0;
    for (int i = 0; i < gradeCount; ++i)
    {
        if (grades[i].studentID == id)
        {
            int ci = findCourseByCode(qs(grades[i].courseCode));
            if (ci >= 0)
            {
                totalPoints += grades[i].points * courses[ci].creditHours;
                totalCredits += courses[ci].creditHours;
            }
        }
    }

    if (totalCredits == 0) { QMessageBox::information(this, "GPA", "No graded courses found for this student."); return; }
    students[studentIndex].GPA = totalPoints / totalCredits;
    saveToFile(students, studentCount);
    refreshAll();
    QMessageBox::information(this, "GPA", "GPA: " + QString::number(students[studentIndex].GPA, 'f', 2));
}

void MainWindow::generateTranscriptText()
{
    QString studentIdText = gradeStudentIdEdit->text().trimmed();
    if (!validStudentIdText(studentIdText)) { QMessageBox::warning(this, "Student", "Student ID must be 6 digits."); return; }
    int id = studentIdText.toInt();
    int studentIndex = findStudentById(id);
    if (studentIndex < 0) { QMessageBox::warning(this, "Student", "Enter a valid student ID."); return; }

    QString text;
    text += "Transcript\n";
    text += "==============================\n";
    text += "Student: " + qs(students[studentIndex].name) + "\n";
    text += "ID: " + QString::number(students[studentIndex].ID) + "\n";
    text += "Program: " + qs(students[studentIndex].program) + "\n";
    text += "GPA: " + QString::number(students[studentIndex].GPA, 'f', 2) + "\n\n";

    for (int i = 0; i < gradeCount; ++i)
    {
        if (grades[i].studentID == id)
        {
            int ci = findCourseByCode(qs(grades[i].courseCode));
            text += qs(grades[i].courseCode) + " - ";
            text += (ci >= 0 ? qs(courses[ci].title) : "Unknown Course");
            text += " | Total: " + QString::number(grades[i].total, 'f', 2);
            text += " | Grade: " + QString(grades[i].letterGrade) + "\n";
        }
    }

    transcriptBox->setPlainText(text);
}

void MainWindow::loadSelectedGrade(int row, int)
{
    if (row < 0) return;
    gradeStudentIdEdit->setText(gradeTable->item(row, 0)->text());
    gradeCourseCodeEdit->setText(gradeTable->item(row, 1)->text());
    midtermSpin->setValue(gradeTable->item(row, 2)->text().toDouble());
    finalSpin->setValue(gradeTable->item(row, 3)->text().toDouble());
}
