// Copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "dialogexport.h"
#include "ui_dialogexport.h"

DialogExport::DialogExport(QWidget *parent,QWinPDB *pWinPDB, QWinPDB::STATS *pStats) :
    QDialog(parent),
    ui(new Ui::DialogExport)
{
    ui->setupUi(this);

    this->pWinPDB=pWinPDB;
    this->pStats=pStats;

    ui->comboBoxSortType->addItem(tr("ID"),QWinPDB::ST_ID);
    ui->comboBoxSortType->addItem(tr("Name"),QWinPDB::ST_ID);
    ui->comboBoxSortType->addItem(tr("Dependencies"),QWinPDB::ST_ID);

    ui->comboBoxExportType->addItem(QString("C++"),QWinPDB::ET_CPLUSPLUS);

    ui->comboBoxFixOffsets->addItem(tr("No"),QWinPDB::FO_NO);
    ui->comboBoxFixOffsets->addItem(tr("Struct and Unions"),QWinPDB::FO_STRUCTSANDUNIONS);
    ui->comboBoxFixOffsets->addItem(tr("All"),QWinPDB::FO_ALL);

    QWinPDB::HANDLE_OPTIONS handleOptions=QWinPDB::getDefaultHandleOptions();

    ui->checkBoxShowComments->setChecked(handleOptions.bShowComments);
    ui->checkBoxFixTypes->setChecked(handleOptions.bFixTypes);
    ui->checkBoxAddAlignment->setChecked(handleOptions.bAddAlignment);

    {
        int nCount=ui->comboBoxFixOffsets->count();

        for(int i=0;i<nCount;i++)
        {
            if(ui->comboBoxFixOffsets->itemData(i).toUInt()==handleOptions.fixOffsets)
            {
                ui->comboBoxFixOffsets->setCurrentIndex(i);

                break;
            }
        }
    }
    {
        int nCount=ui->comboBoxSortType->count();

        for(int i=0;i<nCount;i++)
        {
            if(ui->comboBoxSortType->itemData(i).toUInt()==handleOptions.sortType)
            {
                ui->comboBoxSortType->setCurrentIndex(i);

                break;
            }
        }
    }
    {
        int nCount=ui->comboBoxExportType->count();

        for(int i=0;i<nCount;i++)
        {
            if(ui->comboBoxExportType->itemData(i).toUInt()==handleOptions.exportType)
            {
                ui->comboBoxExportType->setCurrentIndex(i);

                break;
            }
        }
    }
}

DialogExport::~DialogExport()
{
    delete ui;
}

QWinPDB::HANDLE_OPTIONS DialogExport::getHandleOptions()
{
    QWinPDB::HANDLE_OPTIONS result={};

    result.bShowComments=ui->checkBoxShowComments->isChecked();
    result.bFixTypes=ui->checkBoxFixTypes->isChecked();
    result.bAddAlignment=ui->checkBoxAddAlignment->isChecked();
    result.fixOffsets=(QWinPDB::FO)ui->comboBoxFixOffsets->currentData().toUInt();
    result.sortType=(QWinPDB::ST)ui->comboBoxSortType->currentData().toUInt();
    result.exportType=(QWinPDB::ET)ui->comboBoxExportType->currentData().toUInt();

    return result;
}

void DialogExport::on_pushButtonOK_clicked()
{
    // TODO
    QString sFileName=QFileDialog::getSaveFileName(this, tr("Save File..."),"export.h", tr("h-Files (*.h);;All Files (*)"));

    if(!sFileName.isEmpty())
    {
        QString sString; // TODO

        DialogProcess dp(this,pWinPDB,pStats,&sString,PDBProcess::TYPE_EXPORT);
        dp.exec(); // TODO Check return

        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadWrite))
        {
            file.resize(0);
            file.write(sString.toLatin1().data(),sString.length());
            file.close();
            QMessageBox::information(0, tr("Information"), QString("%1: %2").arg(tr("File saved")).arg(sFileName));
        }
        else
        {
            QMessageBox::critical(0, tr("Critical"),QString("%1: %2").arg(tr("Cannot save file")).arg(sFileName));
        }

        this->close();
    }
}

void DialogExport::on_pushButtonCancel_clicked()
{
    this->close();
}
