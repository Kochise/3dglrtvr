/*
 * Copyright (c) 2002-2004, xRhino, Inc.
 *
 * This library is free software; you can redistribute and/or modify
 * it under the terms of the dual license distributed with this source
 * package (a combined Trolltech QPL/GPL-compatible license)
 *
 * Under this dual license scheme, a separate license is available for use of 
 * this code with a proprietary (closed) system. For more information on the 
 * license, please see the LICENSE file distributed with this source package.
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <istream>
#include <ostream>
#include <iostream>
#include <GL/glut.h>
#include <glmath/glmath.h>
#include <glam/glam.h>
#include <GaeControlPoint.h>
#include <GaeCurveEditWidget.h>
#include <Gae3dCurveEditWidget.h>
#include <GaeSplineEditWidget.h>
#include <GaeAnimEditor.h>

float currentTValue = 0.0;
Gae3dCurveEditWidget rotationEditor;
Gae3dCurveEditWidget translationEditor;
Gae3dCurveEditWidget scaleEditor;
GmPixmapIcon sampleIcon;
GmAnimation* idleAnimation;

void gaeRenderPreview(GmTypes::RenderOptionType renderOptions)
{
   currentTValue += 0.001;
   if(currentTValue >= 1.0)
      currentTValue = 0.0;

   rotationEditor.getSplineRotation(currentTValue);
}

void gaeInitAnimations()
{
   GmAnimationSegment* animSegment = new GmAnimationSegment();
   GmTranslation* translation = new GmTranslation();
   GmRotation* rotation = new GmRotation();
   GmScale* scale = new GmScale();
   GlmBezierSpline3* ts = new GlmBezierSpline3(0.0, 0.0, 0.0,
                                               25.0, -50.0, 0.0,
                                               75.0, -50.0, 0.0,
                                               0.0, 0.0, 0.0);
   GlmBezierSpline3* rs = new GlmBezierSpline3(0.0, 0.0, 0.0,
                                               0.0, 0.0, 0.25,
                                               0.0, 0.0, 1.75,
                                               0.0, 0.0, 0.0);
   GlmBezierSpline3* ss = new GlmBezierSpline3(1.0, 1.0, 1.0,
                                               2.0, 2.0, 1.0,
                                               2.0, 2.0, 1.0,
                                               1.0, 1.0, 1.0);
   idleAnimation = new GmAnimation(GmTypes::AT_IDLE);
   
   translation->setTranslationSpline(ts);
   rotation->setRotationSpline(rs);
   scale->setScaleSpline(ss);
   animSegment->setLength(5.0);
   animSegment->setTranslation(translation);
   animSegment->setRotation(rotation);
   animSegment->setScale(scale);
   idleAnimation->addSegment(animSegment);
   sampleIcon.setEventAnimation(GmTypes::AT_IDLE, idleAnimation);
   
   idleAnimation->startAnimation();
}

void gaeInitPreviewWidgets()
{
   GmPushButton* b = new GmPushButton();
   
   gaeInitAnimations();
   
   b->setBaseColor(0.4, 0.4, 0.9, 0.9);
   b->setBorderWidth(0.1);
   b->setText("Preview");
   b->setTextJustification(GmTypes::JTH_CENTER,GmTypes::JTV_MIDDLE);
   //gWidgets.push_back(b);
   b->layoutInGLArea(0.0, -20.0, 40.0, 10.0, 0.0);

   sampleIcon.loadFromFile(GLAM_DATA_DIR "/animTestIcon.png");
   gWidgets.push_back(&sampleIcon);
   sampleIcon.layoutInGLArea(75.0, 75.0, 10.0, 10.0, 0.0);

   rotationEditor.setBaseColor(0.0, 0.8, 0.2, 1.0);
   gWidgets.push_back(&rotationEditor);
   rotationEditor.layoutInGLArea(0.0, 0.0, 50.0, 50.0, 0.0);

   translationEditor.setBaseColor(0.0, 0.0, 0.0, 0.0);
   translationEditor.layoutInGLVolume(0.0, 0.0, 0.0,
                                      500.0, 500.0, 0.0);

   rotationEditor.setBaseColor(0.0, 0.0, 0.0, 0.0);
   rotationEditor.layoutInGLVolume(0.0, 0.0, 0.0,
                                   500.0, 500.0, 0.0);

   scaleEditor.setBaseColor(0.0, 0.0, 0.0, 0.0);
   scaleEditor.layoutInGLVolume(0.0, 0.0, 0.0,
                                500.0, 500.0, 0.0);
   
   gWidgets.push_back(&rotationEditor);   
   gWidgets.push_back(&translationEditor);   
   gWidgets.push_back(&scaleEditor);   
}

void gaeRemovePreviewWidgets()
{
   //   GmEventDistributor* ed = GmEventDistributor::getInstance();
//  for(unsigned int i = 0; i < gCp.size(); i++)
   //      ed->unregisterWidget(gCp[i]);
   
   gCp.clear();
   gWidgets.clear();
}
