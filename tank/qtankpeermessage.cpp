#include "qtankpeermessage.h"
#include "qcomponent.h"
#include "qtankpublic.h"

QTankPeerMessage::QTankPeerMessage(QObject *parent) :
    QObject(parent)
{
    m_Head = _PEER_HEAD;
    m_Size = m_Cmd = m_Sum = 0;
    m_Data.clear();;
    m_Tail = _PEER_TAIL;
}

void QTankPeerMessage::translate()
{
    m_Size = m_Data.length() + 0x0A;
    QByteArray qbaVerify;
    qbaVerify << m_Size << m_Cmd << m_Data;
    m_Sum = 0;
    // 校验码等错误 会导致服务器不回复消息
    // 如果不添加quint8 0x0112+0x0088=0x009a 单字节到二字节进位的位置看不懂
    // 带符号进位，需要调查，会引起移位上的某些问题。
    for(int i = 0; i < qbaVerify.length(); i++)
        m_Sum += quint8(qbaVerify.at(i));

}

bool QTankPeerMessage::checksum()
{
    QByteArray qbaVerify;
    qbaVerify << m_Size << m_Cmd << m_Data;
    quint16 sum = 0;
    for(int i = 0; i < qbaVerify.length(); i++)
        sum += quint8(qbaVerify.at(i));
    if(sum == m_Sum)
        return true;
    return false;
}

QDebug operator<<(QDebug dbg, const QTankPeerMessage &c)
{
    dbg.nospace() << "{" << hex << c.head() << "|" <<
                     hex << c.size() << "@@" << dec << c.size() << "|" <<
                     hex << c.cmd() << "|" <<
                     c.data().data() << "@" << dec << c.data().size() << "|" <<
                     hex << c.sum() << "|" <<
                     hex << c.tail() << "}";
    return dbg.space();
}

quint16 QTankPeerMessage::parseBlockSize(const QByteArray &netData)
{
    QByteArray l = netData.left(4);
    quint16 b0 = 0, b1 = 0;
    l >> b0 >> b1;
    return b1;
}

void QTankPeerMessage::parse(const QByteArray &netData)
{
    QByteArray l = netData;
    quint16 b0 = 0, b1 = 0, b2 = 0, b4 = 0, b5 = 0;
    QByteArray b3;
    l >> b0 >> b1 >> b2;
    b3.resize(b1-0x0A);
    l >> b3 >> b4 >> b5;
    setHead(b0);
    setSize(b1);
    setCmd(b2);
    setData(b3);
    setSum(b4);
    setTail(b5);
}

void QTankPeerMessage::pack(QByteArray &netData)
{
    netData << head();
    netData << size();
    netData << cmd();
    netData << data();
    netData << sum();
    netData << tail();
}


void QTankConnC51Struct::pack(QByteArray &l)
{
    setCmd(_PEER_CONC51);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankCmd2C51Ack::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
}


void QTankHeatStandardStruct::pack(QByteArray &l)
{
    //02	XX	XX	0000	0000	XXXX XXXX
    setCmd(_PEER_HEAT);
    QByteArray d;
    d << quint8(_HEAT_STANDARD) << uStage << uVessel <<
         quint16(0x0000) << quint16(0x0000) << wTempture << wHold;
    setData(d);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankPauseStruct::pack(QByteArray &l)
{
    setCmd(_PEER_PAUSE);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankStopStruct::pack(QByteArray &l)
{
    setCmd(_PEER_STOP);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankHeatPressStruct::pack(QByteArray &l)
{
    //03	XX	XX	0000	XXXX	0000 0000
    setCmd(_PEER_HEAT);
    QByteArray d;
    d << quint8(_HEAT_PRESS) << uStage << uVessel <<
         quint16(0x0000) << wPress << quint16(0x0000) << quint16(0x0000);
    setData(d);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankHeatRAMPStruct::pack(QByteArray &l)
{
    //01	XX	XX	XXXX	XXXX	XXXX XXXX
    setCmd(_PEER_HEAT);
    QByteArray d;
    d << quint8(_HEAT_RAMP) << uStage << uVessel <<
         wRamp << wPress << wTempture  << wHold;

    pline() << hex << wRamp;
    setData(d);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankHeatExtractStruct::pack(QByteArray &l)
{
    //04	XX	00	0000	0000	XXXX XXXX
    setCmd(_PEER_HEAT);
    QByteArray d;
    d << quint8(_HEAT_EXTRACT) << uStage << quint8(0x00) <<
         quint16(0x0000) << quint16(0x0000) << wTempture << wHold;
    setData(d);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankStatAck::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
    QByteArray d = data();
    d >> tmpe >> press >> stat;
}


void QTankStatStruct::pack(QByteArray &l)
{
    setCmd(_PEER_STAT);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankDebugStruct::pack(QByteArray &l)
{
    setCmd(_PEER_DEBUG);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankDebugAck::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
}


void QTankStirSetStruct::setSpeed(quint8 speed)
{
    d.clear();
    d << speed;
}

void QTankStirSetStruct::pack(QByteArray &l)
{
    setCmd(_PEER_STIRSET);
    setData(d);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankStirSetAck::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
}


void QTankCalibrateStruct::setParams(quint8 guangxian, quint8 hongwai, quint8 hongwai2, quint8 yali, quint8 optional)
{
    d.clear();
    d << guangxian
      << hongwai
      << hongwai2
      << yali << optional;

}

void QTankCalibrateStruct::pack(QByteArray &l)
{
    setCmd(_PEER_CALIBRATE);
    setData(d);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankCalibrateAck::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
    QByteArray d = data();
    d >> ramp1 >> ramp2 >> ramp3 >> press4;
}


quint16 QTankExceptionAck::status()
{
    return st;
}

void QTankExceptionAck::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
    QByteArray d = data();
    d >> st;
    pline() << st;
}


void QTankTurnLeftStruct::pack(QByteArray &l)
{
    setCmd(_PEER_LEFT);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankTurnRightStruct::pack(QByteArray &l)
{
    setCmd(_PEER_RIGHT);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankSurroundStruct::pack(QByteArray &l)
{
    setCmd(_PEER_SURROUND);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankStopRunStruct::pack(QByteArray &l)
{
    setCmd(_PEER_STOPRUN);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankTankNumStruct::pack(QByteArray &l)
{
}


void QTankTankNumAck::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
    QByteArray d = data();
    d >> m_num;
    pline() << m_num;
}


void QTankResetAck::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
    QByteArray d = data();
}


void QTankFactoryStruct::pack(QByteArray &l)
{
    setCmd(_PEER_FACTORY);
    translate();
    QTankPeerMessage::pack(l);
}


void QTankFactoryAck::parse(const QByteArray &l)
{
    QTankPeerMessage::parse(l);
    QByteArray d = data();
}

void QTankHeatHoldStruct::pack(QByteArray &l)
{
    //04	00	00	0000	0000	0000 XXXX
    setCmd(_PEER_HEAT);
    QByteArray d;
    d << quint8(_HEAT_HOLD) << quint8(0x00) << quint8(0x00) <<
         quint16(0x0000) << quint16(0x0000) << quint16(0x0000) << wHold;
    setData(d);
    translate();
    QTankPeerMessage::pack(l);
}
