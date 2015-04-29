#ifndef _STL_HEADS_H_
#define _STL_HEADS_H_
/*this file is using to include the head file of render class*/
#include "BaseRender.h"
#include "InterfaceRender.h"
#include "ZbufferRender.h"
#include "ScanRender.h"
#include "SectionRender.h"

typedef RenderAlgorithm::BaseRender BaseRender;
typedef RenderAlgorithm::InterfaceRender InterfaceRender;
typedef RenderAlgorithm::ZbufferRender ZbufferRender;
typedef RenderAlgorithm::point point3D;
typedef RenderAlgorithm::ScanRender ScanRender;
typedef RenderAlgorithm::SectionRender SectionRender;
#endif