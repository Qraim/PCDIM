//
// Created by qraim on 06/04/23.
//

#ifndef PCDIM_PCDIM_H
#define PCDIM_PCDIM_H


#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <tuple>
#include <iostream>
#include <QString>
#include <QKeyEvent>
#include <QDialog>
#include <QHeaderView>

class pcdim : public QWidget {
Q_OBJECT
public:

     pcdim(QWidget *parent = nullptr);
    ~pcdim();

private:

    void clearInput();
    void calculate();
    void clearAll();

    QGridLayout *mainLayout;
    QLabel *labels[5];
    QLineEdit *inputs[5];
    QPushButton *buttons[5];
    QPushButton *bottomButtons[2];
    QPushButton *diametersButton;


    float calculdebit();
    float calculvitesse();
    float calculdiametre();
    float calculperte();

    bool eventFilter(QObject *obj, QEvent *event);

    std::string gettableau(float inner_diameter);

    std::tuple<float, float, float> getMaterialProperties(const std::string &pipe_material);

    void showDiametersTable();
};


#endif //PCDIM_PCDIM_H
