#ifndef QSEARCHLINEEDIT_H
#define QSEARCHLINEEDIT_H

#include <QLineEdit>
#include <QPushButton>

class QSearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QSearchLineEdit(QWidget *parent = 0);
    void setButtonText(QString text);
    void setObjectName(const QString &name);

signals:
    void btnClicked();

public slots:

private:
    QPushButton* m_btnBrower;

    // QObject interface
};

#endif // QSEARCHLINEEDIT_H
