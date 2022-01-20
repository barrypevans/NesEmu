#pragma once

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "ppu2C02.h"

class PatternTableWidget : public QGraphicsView
{
    Q_OBJECT
public:
    PatternTableWidget();
    ~PatternTableWidget();

    void Render(Ppu2C02* pPpu);
    void UIRender();

    QGraphicsScene* scene;
    QPixmap* m_pFrameBuffer;
    QPixmap* m_pBackBuffer;
    QImage* m_pImage;
    QGraphicsPixmapItem* m_pPixmapItem;
    uint32_t* m_rawTable1;
    uint32_t* m_rawTable2;
};