#pragma once

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "ppu2C02.h"

class ScreenWidget : public QGraphicsView
{
    Q_OBJECT
public:
    ScreenWidget();
    ~ScreenWidget();

    void Render(Ppu2C02* pPpu);
    void UIRender();

    QGraphicsScene* scene;
    QPixmap* m_pFrameBuffer;
    QPixmap* m_pBackBuffer;
    QImage* m_pImage;
    QGraphicsPixmapItem* m_pPixmapItem;
};