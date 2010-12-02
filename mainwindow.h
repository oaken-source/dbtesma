#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datagenerator.h"
#include "MISC/enums.h"
#include "MISC/macros.h"

#include <QMainWindow>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    /**
      main Qt interface class
      tasks:
        - handle user-interface, user-input
        - start data generation
    **/

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void setWorkingFlag(bool);
    void setProgress(int);
    void setFileTypeFlag();

    void startDataGeneration();

    void changeUiPropertiesForIndex(int);
    void triggerStartDataGeneration();
    void ensureSliderPositionsSum(int, int);
    void showAppInformation();
    void triggerCancelButton();
    void quitApplication();

    /** saves busy / non-busy **/
    bool _workingFlag;
    /** saves table generation type **/
    ENUM::RESULT_TABLE_TYPES _resultTableTypeFlag;
    /** saves output file type **/
    ENUM::RESULT_FILE_TYPES _resultFileTypeFlag;

    /** instance of DataGenerator **/
    DataGenerator *_dg;

private slots:
    void on_action_About_triggered();
    void on_action_Generate_triggered();
    void on_action_Exit_triggered();
    void on_horizontalSlider_2_valueChanged(int);
    void on_horizontalSlider_valueChanged(int);
    void on_comboBox_currentIndexChanged(int);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H

