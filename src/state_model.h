#ifndef STATE_MODEL_HPP_VP_2006_03_21
#define STATE_MODEL_HPP_VP_2006_03_21

#include "time_vis3.h"

#include <QString>
#include <QColor>

namespace vis4 {

/** �������� ��������� ��� ����� ������������. */
class State_model
{
public:
    /** ��������� ����� ���������. */
    common::Time begin;

    /** �������� ����� ���������. */
    common::Time end;

    /** State id. Link to state in Trace_model::states(). */
    int type;

    /** ����� ����������, � �������� ��������� ���������. */
    unsigned component;

    /** The color to be used when drawing it.  */
    QColor color;

    virtual ~State_model() {}
};

}
#endif
