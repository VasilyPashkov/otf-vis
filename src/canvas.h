#ifndef CANVAS_HPP_VP_2006_03_20
#define CANVAS_HPP_VP_2006_03_20

#include "trace_model.h"
#include "trace_painter.h"

#include <QScrollArea>
#include <QPair>
#include <QTime>

#include <time_vis3.h>

class QKeyEvent;

namespace vis4 {

class State_model;

/** ����� ��� ������ � �������������� � �������.

    ���������� ��������� � ����� ��������� Trace_model � ���� ���������
    ���������, � ������������� ����������, ����������� ������������ �������� ����������� �
    ��������� ������������ � ��������� �������������� ����������� �������.
*/
class Canvas : public QScrollArea {

Q_OBJECT

public:

    Canvas(QWidget* parent);

    /** ������������� ������, ������������ � ������ ������, � ���������
       ������. ��� ���� ���������� ������, ���� ����� ������ ���� ������ ����.
       ���������� ������ modelChanged.
    */
    void setModel(Trace_model::Ptr model);
    Trace_model::Ptr & model() const;

    void setCursor(const QCursor& c);

    /** �� ���������� ����������, ���������� ����� ��������� ����� �����. */
    int nearest_lifeline(int y);

    /** �� ������ ���������� � �������, ���������� ���������� �����. */
    QPoint lifeline_point(int component, const common::Time& time);

    QPair<common::Time, common::Time> nearby_range(const common::Time& time);

    QRect boundingRect(int component,
                       const common::Time& min_time, const common::Time& max_time);

    /**
     * ��� �������, ������������ ��� ��������
     * nothingClicked -- ������ �����������
     * rootClicked -- ��� "���������" ����������
     * componentClicked -- ��� "��������" ����������
     * stateClicked -- ���������
     * lifelinesClicked -- ������� ����� ����� (�������� �������
     *    ��� �������� ����� ����������). ��������� ������� � ����
     *    ������ ������������� ����������� ������� �������.
     */
    enum clickTarget { nothingClicked = 1, componentClicked, stateClicked, lifelinesClicked };

    /** ��������� ����� ����������� �������. ��� �������� �������� ������ ����������
        �����. ��� ���������� ���������� ������ �������� ��������������, ������� ��
        ��������. */
    void addItem(class CanvasItem* item);

public slots:

    /** If true, uses maximally portable drawing mechanims, that
        don't rely on OS being decent, and not broken.  */
    void setPortableDrawing(bool);

signals:
    /** ������, ������������ ��� ��������� ������ ������� setModel. */
    void modelChanged(Trace_model::Ptr & new_model);

    /**
     * ������, ������������ ��� ������� �� �����-���� ������ �����.
     * ���� ������ �� ������ �������������� �������������. ������
     * �����, ��� ������ �������������� ��������������� ����� ������ Tool.
     * @param event  �������� ������� Qt
     * @param target ��� �������, ������������ ��� ��������
     * @param component ����� ���������� � ������� ����������
     * @param state ��������� �� ��������� ��� �������� (���� ����)
     * @param time �����, �������� ������������ ��������� �������
     * @param events_near ����, ��������� ���� �� ���������� � ������� �������
     */
    void mouseEvent(QEvent* event,
                    Canvas::clickTarget target,
                    int component,
                    State_model* state,
                    const common::Time& time,
                    bool events_near);

    /**
     * ������, ������������ ��� �������� ������.
     * ���� ������ �� ������ �������������� �������������. ������
     * �����, ��� ������ �������������� ��������������� ����� ������ Tool.
     * @param event  �������� ������� Qt
     * @param target ��� �������, ������������ ��� ��������
     * @param component ����� ���������� � ������� ����������
     * @param time �����, �������� ������������ ��������� �������
     */
    void mouseMoveEvent(QMouseEvent* event, Canvas::clickTarget target,
                        int component, const common::Time& time);

private slots:

    void timeSettingsChanged();

private: /* overloaded methods */

    void resizeEvent(QResizeEvent* event);
    void timerEvent(QTimerEvent * event);
    void closeEvent(QCloseEvent *event);
    void scrollContentsBy(int dx, int dy);

private: /* members */

    class Contents_widget* contents_;
    class Timeline* timeline_;

    /** Timer is used to prevent canvas redrawing while
        resizing is not finished. */
    int updateTimer;

    friend class MainWindow;
    friend class Contents_widget;

};

class Contents_widget : public QWidget {

    Q_OBJECT
public:

    Contents_widget(Canvas* parent);

    /** If false is true, always redraw, don't suppress redraw
        if the model seem unchanged.
     */
    void setModel(Trace_model::Ptr model, bool force = false);

    Trace_model::Ptr model() const;

    void scrolledBy(int dx, int dy);

    QRect boundingRect(int component,
                       const common::Time& min_time, const common::Time& max_time);

private: /* methods */

    QRect drawBaloon(QPainter* painter);

    void targetUnderCursor(
        const QPoint& pos, Canvas::clickTarget* target,
        int* component, State_model** state, bool* events_near);

private: /* QWidget overrides */

    /** Widget's events hook.
     * This is overloaded virtual function, that cathes QEvet::ToolTip event
     * and processes it.
     *
     * For more help see Qt documentation.
    */
    bool event(QEvent *event);

    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent * event);

    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private: /* members */

    Canvas* parent_;
    Trace_model::Ptr model_;
    std::auto_ptr<Trace_painter> trace_painter;
    std::auto_ptr<Trace_geometry> trace_geometry;

    QPaintDevice * paintBuffer;
    bool portable_drawing;

    int visir_position;
    QRect ballon;

    // FIXME: delete all items in destructor.
    std::vector<CanvasItem*> items;

private slots: /* support for background drawing */

    /** Periodically updates canvas and show current state of drawing. */
    void timerTick();

private:

    void doDrawing(bool start_in_background);

    bool pendingRedraw;

    /** Painter timer. Involves repaint for showing intermediate results of drawing. */
    QTimer * painter_timer;

    friend class Canvas;
};

}
#endif
