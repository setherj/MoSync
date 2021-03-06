/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file CreateNotificationScreen.h
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Screen used for displaying push notification's values.
 */

#ifndef DISPLAYNOTIFICATIONSCREEN_H_
#define DISPLAYNOTIFICATIONSCREEN_H_

#define DEFAULT_SPACER_WIDTH 10

#include <NativeUI/Widgets.h>
#include <MAUtil/String.h>

using namespace NativeUI;
using namespace MAUtil;

// Forward declarations
namespace Notification
{
	class PushNotification;
}

/**
 * @brief Screen used for displaying push notification's values.
 */
class DisplayNotificationScreen:
	public Screen,
	public ButtonListener,
	public EditBoxListener
{
public:
	/**
	 * Constructor.
	 */
	DisplayNotificationScreen();

	/**
	 * Destructor.
	 */
	~DisplayNotificationScreen();

	/**
	 * This screen is notified when registration is done.
	 * @param registrationStatus True if registration succeed, false if
	 * it failed.
	 */
	void pushRegistrationDone(bool registrationStatus);

	/**
	 * This screen is notified that the registration was already completed.
	 */
	void pushRegistrationAlreadyCompleted();

	/**
	 * Displays notification's content on the screen.
	 */
	void pushNotificationReceived(
		Notification::PushNotification& pushNotification);

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Helper function to create list view item with
	 * specific label and edit/check box.
	 */
	ListViewItem* createListViewItem(
		const MAUtil::String& labelText, Widget* widget);

	/**
	 * Reset view's content.
	 * Set default values for widgets.
	 */
	virtual void resetView();

	/**
	 * Checks if the user input data is valid(e.g. numeric edit box values
	 * can be converted to integer).
	 * @return True if the data is valid, false otherwise.
	 */
	virtual bool isUserInputDataValid();

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

    /**
     * Creates an empty widget with a given width.
     * The ownership of the result is passed to the caller.
     * @return An empty widget.
     */
    virtual Widget* createSpacer(const int width = DEFAULT_SPACER_WIDTH);

    /**
     * Checks if a given string can be converted to integer.
     * @return True if string contains only characters between [0-9], false
     * otherwise.
     */
    virtual bool canStringBeConvertedToInteger(const MAUtil::String& string);

private:

    /**
     * Displays push notification's alert message.
     */
    Label* mMessageLabel;

    /**
     * Displays push notification's sound file name.
     * Only on iOS platform.
     */
    Label* mSoundFileNameLabel;

    /**
     * Displays push notification's icon badge number.
     * Only on iOS platform.
     */
    Label* mBadgeNumberLabel;
};

#endif /* DISPLAYNOTIFICATIONSCREEN_H_ */
