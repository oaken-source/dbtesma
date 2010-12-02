#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

/** public ******************************************************************************************************/

    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        /** qt generated mainwindow constructor - added initialisation of instance vars **/
        ui->setupUi(this);

        // set default values
        setWorkingFlag(false);
        _resultTableTypeFlag = ENUM::SINGLE_TABLE_COLS_ROWS;
        _resultFileTypeFlag = ENUM::CSV;
        _dg = new DataGenerator();

    #ifdef _WIN32
        ui->lineEdit->setText("C:\\out\\");
    #else
        ui->lineEdit->setText("/home/");
    #endif // _WIN32

        // ui setup
        ui->comboBox_2->setVisible(false);
    }

    MainWindow::~MainWindow()
    {
        /** qt generated mainwindow destructor  **/
        delete ui;
    }

/** private *****************************************************************************************************/

    void MainWindow::triggerCancelButton()
    {
        /** if there is currently a creation process running cancel it, if not - terminate app **/
        // dispatch on state
        switch(_workingFlag)
        {
        case true:
            this->setWorkingFlag(false);
            break;
        case false:
            quitApplication();
            break;
        }
    }

    void MainWindow::setWorkingFlag(bool state)
    {
        /** switch between busy and idle state - switching to idle while still working will cancel all calculations
            params:
              - state: busy state to be set
        **/
        switch(state)
        {
        case true:
            ui->pushButton_2->setText("Cancel");
            ui->pushButton->setEnabled(false);
            break;
        case false:
            ui->pushButton_2->setText("Close");
            ui->pushButton->setEnabled(true);
            break;
        }
        _workingFlag = state;
    }

    void MainWindow::setProgress(int progress)
    {
        /** update progressbar in the main ui **/
        ui->progressBar->setValue((progress == FINISHED)?ui->progressBar->maximum():progress);
        qApp->processEvents();
    }

    void MainWindow::setFileTypeFlag()
    {
        /** dispatch on state of file type dropdown, save to instance var **/
        int index = ui->comboBox_3->currentIndex();
        switch(index)
        {
        case 0:
            _resultFileTypeFlag = ENUM::CSV;
            break;
        case 1:
            _resultFileTypeFlag = ENUM::DUMP;
            break;
        }
    }

    void MainWindow::startDataGeneration()
    {
        /** fetch user input and begin calculation **/
        setProgress(0);

        // get parameters from UI
        QString outDirName = ui->lineEdit->text();
        std::vector<unsigned int> args;
        args.push_back(ui->lineEdit_input_0->text().toUInt());
        args.push_back(ui->lineEdit_input_1->text().toUInt());
        args.push_back(ui->comboBox_2->currentIndex());
        args.push_back(ui->horizontalSlider->value());
        args.push_back(ui->horizontalSlider_2->value());
        args.push_back(ui->horizontalSlider_3->value());
        setFileTypeFlag();

        if(!args[0] > 0 || !args[1] > 0)
        {
            QMessageBox::information(NULL,"DB Generator","input is not valid. Aborting.");
            setWorkingFlag(false);
            return;
        }

        // start DataGenerator
        if(!_dg->startUp(outDirName, args, _resultTableTypeFlag, _resultFileTypeFlag))
        {
            QMessageBox::information(NULL,"DB Generator","an Error occured. Maybe the given Directory is not valid?");
            setWorkingFlag(false);
            return;
        }
        _dg->writeHeader();

        int progress;
        while(true)
        {
            if(!_workingFlag)
            {
                QMessageBox::information(NULL,"DB Generator","generation aborted");
                break;
            }

            progress = _dg->writeData();
            setProgress(progress);

            if(progress == FINISHED)
            {
                QMessageBox::information(NULL,"DB Generator","generation finished");
                break;
            }
        }
        _dg->tearDown();
    }

    void MainWindow::changeUiPropertiesForIndex(int index)
    {
        /** on change of dropdown, change the ui to fit the new table type and save to instance var
            params:
              - index: index of option in dropdown menu - match on state
        **/
        // dispatch on result type
        switch(index)
        {
        case 0:
            ui->lineEdit_input_1->setGeometry(110,40,91,20);
            ui->comboBox_2->setVisible(false);
            ui->label_input_1->setText("number of rows:");
            _resultTableTypeFlag = ENUM::SINGLE_TABLE_COLS_ROWS;
            break;
        case 1:
            ui->lineEdit_input_1->setGeometry(110,40,51,20);
            ui->comboBox_2->setVisible(true);
            ui->label_input_1->setText("size of outfile:");
            _resultTableTypeFlag = ENUM::SINGLE_TABLE_COLS_SIZE;
            break;
        }
    }

    void MainWindow::ensureSliderPositionsSum(int value, int index)
    {
        /** ensure that sum of all slider values equals 99
            params:
              - value: new value fom selected slider
              - index: number of selected slider
        **/
        QSlider* sliders[] = {ui->horizontalSlider, ui->horizontalSlider_2};
        sliders[index%2]->setValue(99-value);
    }

    void MainWindow::quitApplication()
    {
        /** make an educated guess **/
        this->close();
    }

    void MainWindow::triggerStartDataGeneration()
    {
        /** start generation, if not already running set working flag before and after calculation **/
        if(!_workingFlag)
        {
             setWorkingFlag(true);

            startDataGeneration();
            // wait for data generation to finish

            setWorkingFlag(false);
        }
        else
        {
            QMessageBox::information(NULL,"DB Generator","generation already in progress");
        }
    }

    void MainWindow::showAppInformation()
    {
        /** display general information about this app **/
        QString text = "a project by Andreas Henning\n\nVersion ";
        text.append(VERSION_NUMBER);
        QMessageBox::information(NULL,"About DB Generator",text);
    }

/** slots *******************************************************************************************************/

    void MainWindow::on_action_Exit_triggered()
    {
        quitApplication();
    }

    void MainWindow::on_action_Generate_triggered()
    {
        triggerStartDataGeneration();
    }

    void MainWindow::on_pushButton_clicked()
    {
        triggerStartDataGeneration();
    }

    void MainWindow::on_pushButton_2_clicked()
    {
        triggerCancelButton();
    }

    void MainWindow::on_comboBox_currentIndexChanged(int index)
    {
        changeUiPropertiesForIndex(index);
    }

    void MainWindow::on_horizontalSlider_valueChanged(int value)
    {
        ensureSliderPositionsSum(value, 1);
    }

    void MainWindow::on_horizontalSlider_2_valueChanged(int value)
    {
        ensureSliderPositionsSum(value, 2);
    }

    void MainWindow::on_action_About_triggered()
    {
        showAppInformation();
    }
