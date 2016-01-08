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

#ifndef __UIF_UI_TOUCH_CALIBRATION_UI_H__
#define __UIF_UI_TOUCH_CALIBRATION_UI_H__

#include "core-util/SharedPointer.h"

#include "UIFramework/UIView.h"
#include "UIFramework/UITextMonitorView.h"

#include "wrd-touch/AnalogSlider.h"
#include "wrd-touch/AnalogButton.h"

class TouchCalibrationView : public UIView
{
public:
    /**
     * @brief Constructor. UIView class for calibrating the wrd-touch sensor.
     */
    TouchCalibrationView();

    /**
     * @brief Destructor.
     */
    ~TouchCalibrationView();

    /**
     * @brief Fill frame buffer with the content of the UIView-object.
     *
     * @param canvas FrameBuffer-object wrapped in a SharedPointer.
     * @param xOffset Number of pixels the camera has been translated along the
     *        horizontal axis.
     * @param yOffset Number of pixels the camera has been translated along the
     *        vertical axis.
     * @return The time in milliseconds to when the object wants to be called
     *         again. This is a lower-bound, meaning calling the function sooner
     *         will only result in the same data being filled into the frame
     *         buffer again.
     */
    virtual uint32_t fillFrameBuffer(SharedPointer<FrameBuffer>& buffer, int16_t xOffset, int16_t yOffset);

    /**
     * @brief Activate action in object.
     *
     * @return UIView::Action-object wrapped in a SharedPointer.
     */
    virtual SharedPointer<UIView::Action> getAction();

private:
    /**
     * @brief Function for delaying calibration after getAction has been called.
     * @details The delay gives the user time to remove the hand from the touch
     *          sensor before calibration begins.
     */
    void calibrate();

    /**
     * @brief Callback function for when calibration is done.
     * @details This functio is responsible for disabling the pop-up screen.
     */
    void calibrateDone();

    /**
     * @brief Function for reading pad 0.
     * @return Analog reading.
     */
    int32_t getC0();

    /**
     * @brief Function for reading pad 1.
     * @return Analog reading.
     */
    int32_t getC1();

    /**
     * @brief Function for reading pad 2.
     * @return Analog reading.
     */
    int32_t getC2();

    /**
     * @brief Function for reading pad 3.
     * @return Analog reading.
     */
    int32_t getC3();

    /**
     * @brief Function for reading pad 4.
     * @return Analog reading.
     */
    int32_t getC4();

    /**
     * @brief Function for reading the slider position.
     * @return Slider position.
     */
    int32_t getPos();

    bool showAlert;
    bool calibrationInProgress;

    SharedPointer<UITextMonitorView<int32_t> > c0;
    SharedPointer<UITextMonitorView<int32_t> > c1;
    SharedPointer<UITextMonitorView<int32_t> > c2;
    SharedPointer<UITextMonitorView<int32_t> > c3;
    SharedPointer<UITextMonitorView<int32_t> > c4;
    SharedPointer<UITextMonitorView<int32_t> > c5;

    SharedPointer<UITextView> calibrationAlertCell;

    SharedPointer<AnalogSlider> slider;

    SharedPointer<AnalogButton> button0;
    SharedPointer<AnalogButton> button1;
    SharedPointer<AnalogButton> button2;
    SharedPointer<AnalogButton> button3;
    SharedPointer<AnalogButton> button4;

    minar::callback_handle_t callbackHandle;
};

#endif // __UIF_UI_TOUCH_CALIBRATION_UI_H__
