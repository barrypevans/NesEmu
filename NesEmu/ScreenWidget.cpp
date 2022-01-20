#include "ScreenWidget.h"

ScreenWidget::ScreenWidget()
{
    m_pPixmapItem = new QGraphicsPixmapItem();
    m_pFrameBuffer = new QPixmap();
    m_pBackBuffer = new QPixmap();
    m_pImage = new QImage(341, 261, QImage::Format_RGB32);

    m_pPixmapItem->setPixmap(*m_pFrameBuffer);

    scene = new QGraphicsScene(this);
    scene->addItem(m_pPixmapItem);

    this->setScene(scene);
}

ScreenWidget::~ScreenWidget()
{

}

void ScreenWidget::Render(Ppu2C02* pPpu)
{
    m_pImage->setPixel(pPpu->m_cycle, pPpu->m_scanline, rand());

    if (pPpu->m_frameCompleted)
    {
        m_pBackBuffer->convertFromImage(*m_pImage);

        QPixmap* pTemp = m_pBackBuffer;
        m_pBackBuffer = m_pFrameBuffer;
        m_pFrameBuffer = pTemp;
    }
}

void ScreenWidget::UIRender()
{
    m_pPixmapItem->setPixmap(*m_pFrameBuffer);
}


