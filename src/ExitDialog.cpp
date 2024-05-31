#include "ExitDialog.h"


ExitDialog::ExitDialog(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle("Unsaved changes");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 10);

    QHBoxLayout *imageAndLabelLayout = new QHBoxLayout();
    imageAndLabelLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    imageAndLabelLayout->setContentsMargins(5, 0, 0, 0);
    imageAndLabelLayout->setSpacing(10);

    QLabel *iconLabel = new QLabel(this);
    QPixmap iconPixmap = QApplication::style()->standardPixmap(QStyle::SP_MessageBoxWarning);
    iconLabel->setPixmap(iconPixmap);
    imageAndLabelLayout->addWidget(iconLabel);

    QLabel *messageLabel = new QLabel("Want to save your changes?", this);
    imageAndLabelLayout->addWidget(messageLabel);

    layout->addLayout(imageAndLabelLayout);
    layout->addSpacing(15);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *yesButton = new QPushButton("Yes", this);
    connect(yesButton, &QPushButton::clicked, this, &ExitDialog::accept);
    connect(this, &QDialog::rejected, yesButton, [yesButton]() {
        yesButton->setFocus();
    });
    buttonLayout->addWidget(yesButton);

    QPushButton *noButton = new QPushButton("No", this);
    connect(noButton, &QPushButton::clicked, this, &ExitDialog::reject);
    connect(noButton, &QPushButton::clicked, this, &ExitDialog::noClicked);
    buttonLayout->addWidget(noButton);

    QPushButton *cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &ExitDialog::reject);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);
}
