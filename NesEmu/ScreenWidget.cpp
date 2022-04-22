#include "ScreenWidget.h"

ScreenWidget::ScreenWidget()
{
    m_pPixmapItem = new QGraphicsPixmapItem();
    m_pFrameBuffer = new QPixmap();
    m_pBackBuffer = new QPixmap();
    m_pImage = new QImage(256, 240, QImage::Format_RGB32);

    m_pPixmapItem->setPixmap(*m_pFrameBuffer);

    scene = new QGraphicsScene(this);
    scene->addItem(m_pPixmapItem);

    this->setScene(scene);
    m_rawTable1 = (uint32_t*)malloc(256 * 240 * 4);
}

ScreenWidget::~ScreenWidget()
{

}

void ScreenWidget::Render(Ppu2C02* pPpu)
{
    
    if (pPpu->m_frameCompleted)
    {
        m_pBackBuffer->convertFromImage(*m_pImage);

        QPixmap* pTemp = m_pBackBuffer;
        m_pBackBuffer = m_pFrameBuffer;
        m_pFrameBuffer = pTemp;

       /* pPpu->GetNameTable(m_rawTable1, 0, 0);
        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j < 240; j++)
            {
                m_pImage->setPixel(i, j, m_rawTable1[i + j * 256]);
            }
        }*/
    }

    if (pPpu->m_scanline < 0 || pPpu->m_scanline >=240 || (pPpu->m_cycle - 2) > 255 || (pPpu->m_cycle - 2) < 0) return;

    m_pImage->setPixel(pPpu->m_cycle-2, pPpu->m_scanline, pPpu->m_bufferedPixel); 


  
}

void ScreenWidget::UIRender()
{
    m_pPixmapItem->setPixmap(*m_pFrameBuffer);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


