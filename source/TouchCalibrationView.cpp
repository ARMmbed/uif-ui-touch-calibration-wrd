/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "uif-ui-touch-calibration-wrd/TouchCalibrationView.h"

#include "mbed-drivers/mbed.h"

#if 0
#include "swo/swo.h"
#define printf(...) { swoprintf(__VA_ARGS__); }
#else
#define printf(...)
#endif

#ifdef YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_TOUCH
#define SLIDER_1 YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_TOUCH_SLIDER_1
#define SLIDER_2 YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_TOUCH_SLIDER_2
#define SLIDER_3 YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_TOUCH_SLIDER_3
#define SLIDER_4 YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_TOUCH_SLIDER_4
#define SLIDER_BACK YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_TOUCH_SLIDER_BACK
#else
#error Platform not supported
#endif

TouchCalibrationView::TouchCalibrationView()
    :   UIView(),
        showAlert(false),
        calibrationInProgress(false),
        callbackHandle(NULL)
{
    c0 = SharedPointer<UITextMonitorView<int32_t> >(
            new UITextMonitorView<int32_t>(this, &TouchCalibrationView::getC0,
                                           "%4d", &Font_Dayndate, 100));

    c0->setVerticalAlignment(UIView::VALIGN_TOP);
    c0->setInverse(true);

    c1 = SharedPointer<UITextMonitorView<int32_t> >(
            new UITextMonitorView<int32_t>(this, &TouchCalibrationView::getC1,
                                           "%4d", &Font_Dayndate, 100));

    c1->setVerticalAlignment(UIView::VALIGN_TOP);
    c1->setInverse(true);

    c2 = SharedPointer<UITextMonitorView<int32_t> >(
            new UITextMonitorView<int32_t>(this, &TouchCalibrationView::getC2,
                                           "%4d", &Font_Dayndate, 100));

    c2->setVerticalAlignment(UIView::VALIGN_TOP);
    c2->setInverse(true);

    c3 = SharedPointer<UITextMonitorView<int32_t> >(
            new UITextMonitorView<int32_t>(this, &TouchCalibrationView::getC3,
                                           "%4d", &Font_Dayndate, 100));

    c3->setVerticalAlignment(UIView::VALIGN_TOP);
    c3->setInverse(true);

#if (YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_TOUCH_CHANNELS_IN_USE > 4)
    c4 = SharedPointer<UITextMonitorView<int32_t> >(
            new UITextMonitorView<int32_t>(this, &TouchCalibrationView::getC4,
                                           "%4d", &Font_Dayndate, 100));

    c4->setVerticalAlignment(UIView::VALIGN_TOP);
    c4->setInverse(true);
#endif

    c5 = SharedPointer<UITextMonitorView<int32_t> >(
            new UITextMonitorView<int32_t>(this, &TouchCalibrationView::getPos,
                                           "%4d", &Font_Dayndate, 100));

    c5->setVerticalAlignment(UIView::VALIGN_TOP);
    c5->setInverse(true);

    /* calibration alert cell */
    calibrationAlertCell = SharedPointer<UITextView>(new UITextView("Calibrating",
                                                     &Font_Menu));
    calibrationAlertCell->setInverse(true);
    calibrationAlertCell->setWidth(0);
    calibrationAlertCell->setHeight(0);

    /* enable slider channels */
    uint32_t sliderChannels[4] = {SLIDER_1, SLIDER_2, SLIDER_3, SLIDER_4};

    slider = SharedPointer<AnalogSlider>(new AnalogSlider(sliderChannels, 4));
    slider->resume();

    /* enable back channel */
    button0 = SharedPointer<AnalogButton>(new AnalogButton(SLIDER_1));
    button1 = SharedPointer<AnalogButton>(new AnalogButton(SLIDER_2));
    button2 = SharedPointer<AnalogButton>(new AnalogButton(SLIDER_3));
    button3 = SharedPointer<AnalogButton>(new AnalogButton(SLIDER_4));
    button4 = SharedPointer<AnalogButton>(new AnalogButton(SLIDER_BACK));
}

TouchCalibrationView::~TouchCalibrationView()
{
    if (callbackHandle)
    {
        minar::Scheduler::cancelCallback(callbackHandle);
    }

    if (calibrationInProgress)
    {
        slider->cancelCalibration();
    }
}

int32_t TouchCalibrationView::getC0()
{
    return button0->getValue();
}

int32_t TouchCalibrationView::getC1()
{
    return button1->getValue();
}

int32_t TouchCalibrationView::getC2()
{
    return button2->getValue();
}

int32_t TouchCalibrationView::getC3()
{
    return button3->getValue();
}

int32_t TouchCalibrationView::getC4()
{
    return button4->getValue();
}

int32_t TouchCalibrationView::getPos()
{
    return slider->getLocation();
}

uint32_t TouchCalibrationView::fillFrameBuffer(SharedPointer<FrameBuffer>& canvas,
                                               int16_t xOffset,
                                               int16_t yOffset)
{
    (void) xOffset;
    (void) yOffset;
    unsigned int callInterval = ULONG_MAX;

    int32_t min;
    int32_t max;
    int32_t val;

    /* fill canvas with black */
    canvas->drawRectangle(0, canvas->getWidth(), 0, canvas->getHeight(), 0);

    /* pad 0 */
    SharedPointer<FrameBuffer> c0Canvas = canvas->getFrameBuffer(10,
                                                                 2,
                                                                 100,
                                                                 20);
    callInterval = c0->fillFrameBuffer(c0Canvas, 0, 0);

    min = button0->getMinValue();
    max = button0->getMaxValue();
    val = button0->getValue();
    canvas->drawRectangle(10, 110 * (val - min) / (max - min), 16, 20, 1);

    /* pad 1 */
    SharedPointer<FrameBuffer> c1Canvas = canvas->getFrameBuffer(10,
                                                                 22,
                                                                 100,
                                                                 20);
    c1->fillFrameBuffer(c1Canvas, 0, 0);

    min = button1->getMinValue();
    max = button1->getMaxValue();
    val = button1->getValue();
    canvas->drawRectangle(10, 110 * (val - min) / (max - min), 36, 40, 1);

    /* pad 2 */
    SharedPointer<FrameBuffer> c2Canvas = canvas->getFrameBuffer(10,
                                                                 42,
                                                                 100,
                                                                 20);
    c2->fillFrameBuffer(c2Canvas, 0, 0);

    min = button2->getMinValue();
    max = button2->getMaxValue();
    val = button2->getValue();
    canvas->drawRectangle(10, 110 * (val - min) / (max - min), 56, 60, 1);

    /* pad 3 */
    SharedPointer<FrameBuffer> c3Canvas = canvas->getFrameBuffer(10,
                                                                 62,
                                                                 100,
                                                                 20);
    c3->fillFrameBuffer(c3Canvas, 0, 0);

    min = button3->getMinValue();
    max = button3->getMaxValue();
    val = button3->getValue();
    canvas->drawRectangle(10, 110 * (val - min) / (max - min), 76, 80, 1);

#if (YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_TOUCH_CHANNELS_IN_USE > 4)
    /* pad 4 */
    SharedPointer<FrameBuffer> c4Canvas = canvas->getFrameBuffer(10,
                                                                 82,
                                                                 100,
                                                                 20);
    c4->fillFrameBuffer(c4Canvas, 0, 0);

    min = button4->getMinValue();
    max = button4->getMaxValue();
    val = button4->getValue();
    canvas->drawRectangle(10, 110 * (val - min) / (max - min), 96, 100, 1);
#endif

    SharedPointer<FrameBuffer> cposCanvas = canvas->getFrameBuffer(10,
                                                                   102,
                                                                   100,
                                                                   20);
    c5->fillFrameBuffer(cposCanvas, 0, 0);

    val = slider->getLocation();
    val = 128 * (val - 1000) / (4000 - 1000);
    canvas->drawRectangle(120, 128, val - 2, val + 2, 1);

    if (showAlert)
    {
        unsigned int yStart = (canvas->getHeight() / 2) - 18;

        SharedPointer<FrameBuffer> alertCanvas = canvas->getFrameBuffer(0,
                                                                        yStart,
                                                                        canvas->getWidth(),
                                                                        35);

        alertCanvas->drawRectangle(4, alertCanvas->getWidth() - 4,
                                   4, alertCanvas->getHeight() - 4, 0);

        calibrationAlertCell->fillFrameBuffer(alertCanvas, 0, 0);

//            callInterval = 1000;
    }

    return callInterval;
}

SharedPointer<UIView::Action> TouchCalibrationView::getAction()
{
    callbackHandle = minar::Scheduler::postCallback(this, &TouchCalibrationView::calibrate)
                        .delay(minar::milliseconds(500))
                        .getHandle();

    showAlert = true;

    return SharedPointer<UIView::Action>(new UIView::Action());
}

void TouchCalibrationView::calibrate()
{
    callbackHandle = NULL;

    calibrationInProgress = true;
    slider->calibrate(this, &TouchCalibrationView::calibrateDone);
}

void TouchCalibrationView::calibrateDone()
{
    showAlert = false;
    calibrationInProgress = false;

    printf("slider: calibrateDone\r\n");
}
