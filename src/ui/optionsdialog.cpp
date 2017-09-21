#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    QDialogButtonBox *buttonBox = ui->buttonBox;
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->sliderThreshold, SIGNAL(valueChanged(int)), this,
            SLOT(valuesChanged(int)));
    connect(ui->spinboxThreshold, SIGNAL(valueChanged(int)), this,
            SLOT(valuesChanged(int)));
    connect(ui->buttonDetectStars, SIGNAL(released()), this,
            SLOT(handleButtonDetectStars()));

    QSettings settings("OpenSkyStacker", "OpenSkyStacker");
    int thresh = settings.value("StarDetector/thresholdCoeff", 20).toInt();
    valuesChanged(thresh);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::valuesChanged(int thresh)
{
    ui->sliderThreshold->blockSignals(true);
    ui->spinboxThreshold->blockSignals(true);
    ui->sliderThreshold->setValue(thresh);
    ui->spinboxThreshold->setValue(thresh);
    ui->sliderThreshold->blockSignals(false);
    ui->spinboxThreshold->blockSignals(false);

    SetThresh(thresh);
}

void OptionsDialog::setDetectedStars(int stars)
{
    QString label;
    if (stars < 0) {
        label = tr("Couldn't perform star detection. Make sure you have a reference image (displayed in bold).");
    } else {
        label = tr("Detected %n stars", "", stars);
    }
    ui->labelDetectStars->setText(label);
}

int OptionsDialog::GetThresh() const
{
    return thresh_;
}

void OptionsDialog::SetThresh(int value)
{
    thresh_ = value;
}

void OptionsDialog::handleButtonDetectStars()
{
    ui->labelDetectStars->setText(tr("Detecting stars..."));
    emit detectStars(GetThresh());
}
