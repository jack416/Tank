#include "qstageview.h"
#include "ui_qstageview.h"
#include "qcheaderview.h"
#include "qtankdefine.h"
#include "qtankpublic.h"

QStageView::QStageView(QWidget *parent) :
    QCTableView(parent),
    ui(new Ui::QStageView)
{
    ui->setupUi(this);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
#ifdef __MIPS_LINUX__
    setFocusPolicy(Qt::NoFocus);
#endif
    horizontalHeader()->setMinimumHeight(36);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    verticalHeader()->setHidden(true);
    resizeColumnsToContents();
    setShowGrid(false);

    dg = new QStageFormatTextDelegate(this);
    setItemDelegateForColumn(Stage_Timeramp, dg);
    dgHold = new QStageFormatTextDelegate(this);
    setItemDelegateForColumn(Stage_Hold, dgHold);
}

QStageView::~QStageView()
{
    delete ui;
}


void QStageFormatTextDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QString &text) const
{
    qint32 time = text.toInt();
    qint32 hour = time / 60;
    qint32 minute = time % 60;
    painter->save();
    pline() << quint32(option.state);
    if( 106753 == option.state || 98305 == option.state || 98561 == option.state)
    {
        painter->setPen(option.palette.color(QPalette::BrightText));
    }
    painter->drawText(rect, Qt::AlignCenter, QString("%1:%2")
                      .arg(hour, 2, 10, QLatin1Char('0'))
                      .arg(minute, 2, 10, QLatin1Char('0')));
    painter->restore();
}

void QStageView::mousePressEvent(QMouseEvent *)
{
    pline();
}


void QStageView::wheelEvent(QWheelEvent *)
{
    pline();
}

void QStageView::keyPressEvent(QKeyEvent *)
{
    pline();
}


void QStageView::mouseReleaseEvent(QMouseEvent *)
{
    pline();
}

void QStageView::mouseDoubleClickEvent(QMouseEvent *)
{
    pline();
}

void QStageView::mouseMoveEvent(QMouseEvent *)
{
    pline();
}

void QStageView::keyReleaseEvent(QKeyEvent *)
{
    pline();
}
