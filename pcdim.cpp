//
// Created by qraim on 06/04/23.
//

#include "pcdim.h"
#include <cmath>
#include <QTableWidget>

pcdim::pcdim(QWidget *parent)
        : QWidget(parent),
          mainLayout(new QGridLayout(this))
{


    // Ajoutez les noms souhaités pour les labels dans ce tableau
    QString labelNames[] = {"Débit (m3/h)", "Diametre int (mm)", "Vitesse (m/s)", "Longueur (m)", "Perte (m)"};

    for (int i = 0; i < 5; ++i)
    {
        // Utilisez le tableau labelNames pour définir les noms des labels
        labels[i] = new QLabel(labelNames[i], this);
        mainLayout->addWidget(labels[i], i, 0);

        inputs[i] = new QLineEdit(this);
        mainLayout->addWidget(inputs[i], i, 1);
        inputs[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        buttons[i] = new QPushButton(QString("Effacer"));
        mainLayout->addWidget(buttons[i], i, 2);
        connect(buttons[i], &QPushButton::clicked, this, &pcdim::clearInput);
    }

    // Renommer les deux boutons
    bottomButtons[0] = new QPushButton("Calculer", this);
    mainLayout->addWidget(bottomButtons[0], 6, 0, 1, 3, Qt::AlignCenter);
    connect(bottomButtons[0], &QPushButton::clicked, this, &pcdim::calculate);

    bottomButtons[1] = new QPushButton("Effacer", this);
    mainLayout->addWidget(bottomButtons[1], 7, 0, 1, 3, Qt::AlignCenter);
    connect(bottomButtons[1], &QPushButton::clicked, this, &pcdim::clearAll);

    for (int i = 0; i < 5; ++i)
    {
        inputs[i]->installEventFilter(this);
    }

    diametersButton = new QPushButton("Diamètre", this);
    mainLayout->addWidget(diametersButton, 0, 3);
    connect(diametersButton, &QPushButton::clicked, this, &pcdim::showDiametersTable);


}


pcdim::~pcdim() = default;




void pcdim::clearInput()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    for (int i = 0; i < 5; ++i)
    {
        if (buttons[i] == button)
        {
            inputs[i]->clear();
            break;
        }
    }
}

void pcdim::clearAll()
{
    for (int i = 0; i < 5; ++i){
        inputs[i]->clear();
    }
}


float pcdim::calculdebit()
{
    float vitesse = inputs[2]->text().toFloat(); // Vitesse de QLineEdit
    float diametre = inputs[1]->text().toFloat(); // Diametre intérieur de QLineEdit

    float debit = (vitesse * M_PI * std::pow((diametre / 2), 2)) / 1000; // Debit m³/h
    return debit;
}

float pcdim::calculvitesse()
{
    float debit = inputs[0]->text().toFloat(); // Debit de QLineEdit
    float diametre = inputs[1]->text().toFloat(); // Diametre intérieur de QLineEdit

    float vitesse = (1000 * debit) / (M_PI * std::pow((diametre / 2), 2)); // Vitesse m/s
    return vitesse;
}

float pcdim::calculdiametre()
{
    float debit = inputs[0]->text().toFloat(); // Debit de QLineEdit
    float vitesse = inputs[2]->text().toFloat(); // Vitesse de QLineEdit

    float diametre = 2 * std::sqrt((1000 * debit) / (M_PI * vitesse)); // Diametre intérieur mm
    return diametre;
}

std::string pcdim::gettableau(float inner_diameter) {
    std::string pipe_material = "";
    QString pipe_pressure = "";

    std::map<QString, std::map<QString, std::vector<std::pair<float, float>>>> materials = {
            {"PEBD", {{"6", {{16, 12}, {20, 15.4}, {25, 19.4}, {32, 24.8}}}}},
            {"PEHD", {
                      {"6", {{25, 21}, {32, 28}, {40, 35.2}, {50, 44}, {63, 55.4}, {75, 66}, {90, 79.2}}},
                             {"10", {{25, 20.4}, {32, 27.2}, {40, 34}, {50, 42.6}, {63, 53.6}, {75, 63.8}, {90, 76.6}, {110, 96.8}, {125, 110.2}, {140, 123.4}, {160, 141}, {200, 176.2}, {250, 220.2}, {315, 277.6}, {355, 312.8}, {400, 352.6}, {450, 396.6}, {500, 440.6}, {560, 493.6}, {630, 555.2}, {710, 625.8}}},
                             {"16", {{25, 19}, {32, 24.8}, {40, 31}, {50, 40.8}, {63, 51.4}, {75, 58.2}, {90, 73.6}, {110, 90}, {125, 102.2}, {140, 114.6}, {160, 130.8}, {200, 163.6}, {250, 204.6}, {315, 257.8}, {355, 290.6}, {400, 327.4}}},
                     }},
            {"PVC", {
                      {"8", {{25, 21.2}, {32, 28.4}, {40, 36.4}, {50, 46}}},
                             {"10", {{25, 25}, {32, 32}, {40, 40}, {50, 50}, {63, 63}, {75, 69}, {90, 83}, {110, 101.4}, {125, 116.2}, {140, 130.2}, {160, 148.8}, {200, 186.2}, {250, 232.6}, {315, 293.2}}},
                             {"16", {{90, 85}, {110, 105.2}, {125, 133.8}, {140, 153}, {160, 191.2}, {200, 239}, {250, 301.2}, {315, 293.2}}},
                     }},
    };
    for (const auto& material : materials) {
        for (const auto& pressure : material.second) {
            for (const auto& diameter : pressure.second) {
                if (diameter.second == inner_diameter) {
                    pipe_material = material.first.toStdString();
                    pipe_pressure = pressure.first;
                    return pipe_material;
                }
            }
        }
    }

    return "";
}

std::tuple<float, float, float> pcdim::getMaterialProperties(const std::string & pipe_material) {
    float k = 0;
    float a = 0;
    float b = 0;

    if (pipe_material == "PVC" || pipe_material == "PNHD" || pipe_material == "PNBD") {
        k = 831743.11;
        a = 1.75;
        b = -4.75;
    } else if (pipe_material == "Fonte") {
        k = 1458844.82;
        a = 1.83;
        b = -4.89;
    } else if (pipe_material == "Alu") {
        k = 1707785.38;
        a = 1.89;
        b = -4.87;
    }

    return std::make_tuple(k, a, b);
}
float pcdim::calculperte() {
    float debit_ls = inputs[0]->text().toFloat() / 3600; // Convert debit from m3/h to l/s
    float diametre = inputs[1]->text().toFloat(); // Convert diametre from mm to m

    std::string pipe_material = gettableau(diametre);


    std::tuple<float, float, float> material_properties = getMaterialProperties(pipe_material);


    float k = std::get<0>(material_properties);
    float a = std::get<1>(material_properties);
    float b = std::get<2>(material_properties);

    float longueur = inputs[3]->text().toFloat(); // Longueur in meters

    std::cout<<longueur<<" "<< k <<" "<< a << " "<< b<<" " << debit_ls << " " << diametre  <<std::endl;

    float perte = k * std::pow(debit_ls, a) * std::pow(diametre, b) * std::pow(longueur, 1.0);
    return perte;
}

void pcdim::calculate()
{
    bool debitEntered = !inputs[0]->text().isEmpty();
    bool diametreEntered = !inputs[1]->text().isEmpty();
    bool vitesseEntered = !inputs[2]->text().isEmpty();
    bool longueurEntered = !inputs[3]->text().isEmpty();

    if (debitEntered && diametreEntered && vitesseEntered && longueurEntered) {
        float perte = calculperte();
        std::cout<<perte<<std::endl;
        inputs[4]->setText(QString::number(perte));
        return;
    }

    if (debitEntered && diametreEntered) {
        float vitesse = calculvitesse();
        inputs[2]->setText(QString::number(vitesse));
        return;
    } else if (debitEntered && vitesseEntered) {
        float diametre = calculdiametre();
        inputs[1]->setText(QString::number(diametre));
        return;
    } else if (diametreEntered && vitesseEntered) {
        float debit = calculdebit();
        inputs[0]->setText(QString::number(debit));
        return;
    }


}


bool pcdim::eventFilter(QObject *obj, QEvent *event)
{
    for (int i = 0; i < 5; ++i)
    {
        if (obj == inputs[i] && event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                if (keyEvent->modifiers() == Qt::ShiftModifier)
                {
                    // Shift + Entrer: Calculer
                    calculate();
                    return true;
                }
                else if (keyEvent->modifiers() == Qt::ControlModifier && i > 0)
                {
                    // Control : Champ précédent
                    inputs[i - 1]->setFocus();
                    return true;
                }
                else if (i < 4)
                {
                    // Enter: Champ suivant
                    inputs[i + 1]->setFocus();
                    return true;
                }
                else if (i == 4)
                {
                    // Si tout est remplis sauf perte calculer
                    bool allFilled = true;
                    for (int j = 0; j < 4; ++j)
                    {
                        if (inputs[j]->text().isEmpty())
                        {
                            allFilled = false;
                            break;
                        }
                    }
                    if (allFilled)
                    {
                        calculate();
                        return true;
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void pcdim::showDiametersTable()
{
    QDialog *diametersDialog = new QDialog(this);
    diametersDialog->setWindowTitle("Tableau des diamètres");
    QVBoxLayout *dialogLayout = new QVBoxLayout(diametersDialog);

    QTabWidget *tabWidget = new QTabWidget(diametersDialog);

    // Set dialog size to a percentage of the screen size
    const int dialogWidthPercentage = 80;
    const int dialogHeightPercentage = 80;
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int screenWidth = screenGeometry.width() * dialogWidthPercentage / 100;
    int screenHeight = screenGeometry.height() * dialogHeightPercentage / 100;
    diametersDialog->resize(screenWidth, screenHeight);


    std::map<QString, std::map<QString, std::vector<std::pair<float, float>>>> materials = {
            {"PEBD", {{"6", {{16, 12}, {20, 15.4}, {25, 19.4}, {32, 24.8}}}}},
            {"PEHD", {
                      {"6", {{25, 21}, {32, 28}, {40, 35.2}, {50, 44}, {63, 55.4}, {75, 66}, {90, 79.2}}},
                             {"10", {{25, 20.4}, {32, 27.2}, {40, 34}, {50, 42.6}, {63, 53.6}, {75, 63.8}, {90, 76.6}, {110, 96.8}, {125, 110.2}, {140, 123.4}, {160, 141}, {200, 176.2}, {250, 220.2}, {315, 277.6}, {355, 312.8}, {400, 352.6}, {450, 396.6}, {500, 440.6}, {560, 493.6}, {630, 555.2}, {710, 625.8}}},
                             {"16", {{25, 19}, {32, 24.8}, {40, 31}, {50, 40.8}, {63, 51.4}, {75, 58.2}, {90, 73.6}, {110, 90}, {125, 102.2}, {140, 114.6}, {160, 130.8}, {200, 163.6}, {250, 204.6}, {315, 257.8}, {355, 290.6}, {400, 327.4}}},
                     }},
            {"PVC", {
                      {"8", {{25, 21.2}, {32, 28.4}, {40, 36.4}, {50, 46}}},
                             {"10", {{25, 25}, {32, 32}, {40, 40}, {50, 50}, {63, 63}, {75, 69}, {90, 83}, {110, 101.4}, {125, 116.2}, {140, 130.2}, {160, 148.8}, {200, 186.2}, {250, 232.6}, {315, 293.2}}},
                             {"16", {{90, 85}, {110, 105.2}, {125, 133.8}, {140, 153}, {160, 191.2}, {200, 239}, {250, 301.2}, {315, 293.2}}},
                     }},
    };

    for (const auto& material : materials) {
        QTableWidget *tableWidget = new QTableWidget(diametersDialog);

        // On tri les pressions
        std::vector<QString> sortedPressures;
        for (const auto& pressure : material.second) {
            sortedPressures.push_back(pressure.first);
        }
        std::sort(sortedPressures.begin(), sortedPressures.end(),
                  [](const QString &a, const QString &b) { return a.toFloat() < b.toFloat(); });

        tableWidget->setColumnCount(sortedPressures.size() + 1);
        tableWidget->setRowCount(0);

        QStringList headerLabels;
        headerLabels << "Pression";
        for (const auto& pressure : sortedPressures) {
            headerLabels << pressure;
        }
        tableWidget->setHorizontalHeaderLabels(headerLabels);
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        int maxRows = 0;
        for (const auto& pressure : material.second) {
            maxRows = std::max(maxRows, static_cast<int>(pressure.second.size()));
        }

        tableWidget->setRowCount(maxRows);

        int col = 1;
        for (const auto& pressureStr : sortedPressures) {
            auto& pressureData = material.second.at(pressureStr);
            int row = 0;
            for (const auto& diameter : pressureData) {
                QTableWidgetItem *item = new QTableWidgetItem(QString("(%1, %2)").arg(diameter.second).arg(diameter.first));
                tableWidget->setItem(row, col, item);
                row++;
            }
            col++;
        }

        tabWidget->addTab(tableWidget, material.first);
    }

    dialogLayout->addWidget(tabWidget);
    diametersDialog->setLayout(dialogLayout);
    diametersDialog->exec();
}


