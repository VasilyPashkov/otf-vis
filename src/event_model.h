#ifndef EVENT_MODEL_HPP_VP_2006_03_21
#define EVENT_MODEL_HPP_VP_2006_03_21

#include "time_vis3.h"
#include <QString>

class QWidget;

namespace vis4 {


/** ������ ����� ��������� ������� ��� ����� ������������.
    ��������� �� Event_model ������������ ������� Trace_model::next_event.
*/
class Event_model
{
public:
    /** ����� ������������� �������. */
    common::Time time;

    /** ��� ������� (������). ������������ ������ ��� ������ ����
        ������� � ��������������� ����������.
    */
    QString kind;

    /** �����, ������������ ��� ������ �������. �������������� ������
       ������������ ��� ���������. */
    char letter;

    /** �������������� �����, ������������ ��� ������ �������. �������������� ������
    ������������ ��� ��������. �������� ������ �� �������� ����� ������� �������.
    ���� ����� ����, �� ������������. */
    char subletter;

    /** ������� ����� ������������ "�������" �������. */
    enum letter_position_t { left_top = 1, right_top, right_bottom, left_bottom}
    letter_position;

    /** ����� ����������, � ������� ��������� �������. */
    int component;

    /** Event priority, used to decide which letter to draw
        if letters from different event overlap.
    */
    unsigned priority;

    /** ���������� ������� �������� �������, ��������� �������� ���
        ������ �������. ������������ �������� �� ������ �������� �����. */
    virtual QString shortDescription() const { return kind; }

    /** ����� ����� ���� ���������� ��������������� ��������, ����� ������������
    ����������� ����� ������ ��������� ���������� � �������.

    ����� ������ ������� ������������ �������� ��� ������ ��������� ���������� � �������.
    ��� �������� ������ ���� ������ ������� parent. ����� ����� ���������� �����������
    � ����� � ��� �� parent ��� ������ ������ ������� � ������ ���������� ��������
    ����� ����������� �������� ��� ������� parent ��� �������. ����������� ����������
    �� ������� ����� ��������� �������� � ������ ������ ����� � ������ �� ��������.

    �������������, ��� ������� ������ ��� �� ��������� ������ � parent.
    �������������, ��� ������� ��� �������� ���������� �����, �� ����� ������� �����, ���
    ����������� ��������, �� ���������.
    */
    virtual void createDetailsWidget(QWidget* parent) {}

    /** ���������� true ���� createDetailsWidget �������������. */
    virtual bool customDetailsWidget() const { return false; }

    virtual ~Event_model() {}

    virtual bool operator==(const Event_model & other) const
    {
        // Very stupid implementation for vis_xml and vis_fake

        if (time != other.time) return false;
        if (kind != other.kind) return false;
        if (letter != other.letter) return false;
        if (subletter != other.subletter) return false;
        if (component != other.component) return false;

        return true;
    }

protected:
    /** ������� ������, ��������� ��� ������ ��������� ��������� ���������� � �������.
    �������������� ������ ����� ������������ ���� ����� � ����� ���������� createDetailsWidget,
    ���� � ��� ���� ��������� ���������� ��� ������, � ��� ���� �� ��������� ������ ��������
    �� �������������� ���� ����������.
    */
    QWidget* createTextDetailsWidget(const QString& text, const QWidget* parent);
};

}
#endif
